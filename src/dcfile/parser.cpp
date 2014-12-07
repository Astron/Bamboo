// Filename: parser.cpp
#include "parser.h"
#include <cstring>
#include <array>
#include <sstream> // @TODO(Kevin): Benchmark stringstreams and replace for better performance
using namespace std;

#include "../dcfile/lexer.h"
#include "../module/module"
namespace bamboo
{


const array<Type **, 14> token_to_primitive = {
    &Type::Byte, &Type::Char,
    &Type::Int8, &Type::Int16, &Type::Int32, &Type::Int64,
    &Type::Uint8, &Type::Uint16, &Type::Uint32, &Type::Uint64,
    &Type::Float32, &Type::Float64,
    &Type::String, &Type::Blob
};

const array<NumericType, 10> token_to_numtype = {
    Numeric_Int8, Numeric_Int16, Numeric_Int32, Numeric_Int64,
    Numeric_Uint8, Numeric_Uint16, Numeric_Uint32, Numeric_Uint64,
    Numeric_Float32, Numeric_Float64
};

static Type *to_primitive(TokenType type) { return *token_to_primitive[type - Token_Byte]; }
static NumericType to_numeric_type(TokenType type) { return token_to_numtype[type - Token_Int8]; }

static void unexpected_token(Parser *parser, const char *expected);
static void unexpected_token(Parser *parser, const char *expected, const char *info);
static void add_warning(Parser *parser, const LineInfo& where, const char *what);
static void add_error(Parser *parser, const LineInfo& where, const char *what);
static void add_error(Parser *parser, const LineInfo& where, const char *what, const char *info);
static void eat_token(Parser *parser);
static void eat_unary(Parser *parser);
static void eat_until_end_of_statement_or_line(Parser *parser, const LineInfo& start);
static void eat_until_end_of_block_or_topdecl(Parser *parser);

struct MoleculeDefn
{
    string name;
    unordered_map<string, LineInfo> fields;
};
static MoleculeDefn *parse_molecule(Parser *parser);
static Value *parse_numeric_value(Parser *parser, const Numeric *numeric);
static Value *parse_string_value(Parser *parser, const Array *array);
static Value *parse_blob_value(Parser *parser, const Array *array);
static Value *parse_array_value(Parser *parser, const Array *array);
static Value *parse_struct_value(Parser *parser, const Struct *struct_);
static Value *parse_method_value(Parser *parser, const Method *method);


Parser::Parser(Lexer *lex) : lexer(lex) {}
Parser::~Parser()
{
    prev_token.destroy();
    curr_token.destroy();
    next_token.destroy();
}

void Parser::start()
{
    if(lexer == nullptr) {
        curr_token.type = Token_Eof;
    } else {
        // Scan the first two tokens into curr_token and next_token
        eat_token(this);
        eat_token(this);
    }
}

Module *Parser::parse_module(Module *mod) {
    module = mod;
    return parse_module();
}

Module *Parser::parse_module()
{
    if(module == nullptr) {
        module = new Module();
        parser_owns_module = true;
    }

    while(curr_token.type != Token_Eof && curr_token.type != Token_NotAToken) {
        switch(curr_token.type) {
        case Token_Struct:
            {
                Struct *struct_ = parse_struct();
                if(struct_ == nullptr) {
                    eat_until_end_of_block_or_topdecl(this);
                } else {
                    module->add_struct(struct_);
                    mask_next_error = false;
                }
            }
            break;
        case Token_Class:
            {
                Class *class_ = parse_class();
                if(class_ == nullptr) {
                    eat_until_end_of_block_or_topdecl(this);
                } else {
                    module->add_class(class_);
                    mask_next_error = false;
                }
            }
            break;
        case Token_Typedef:
            {
                bool ok = parse_typedef_into_module(module);
                if(!ok) {
                    eat_until_end_of_statement_or_line(this, curr_token.line);
                }
            }
            break;
        case Token_Keyword:
            {
                bool ok = parse_keywords_into_module(module);
                if(!ok) {
                    eat_until_end_of_statement_or_line(this, curr_token.line);
                }
            }
            break;
        case Token_Import:
        case Token_ImportFrom:
            {
                Import *import = parse_import();
                if(import == nullptr) {
                    eat_until_end_of_statement_or_line(this, curr_token.line);
                } else {
                    module->add_import(import);
                    mask_next_error = false;
                }
            }
            break;
        case ';':
            eat_token(this);
            break;
        default:
            if(curr_token.is_operator()) {
                stringstream error;
                error << "Unexpected " << format_tokentype(curr_token.type)
                      << " at top level";
                add_error(this, curr_token.line, error.str().c_str());
                mask_next_error = true;
            } else if(curr_token.is_delimiter()) {
                stringstream error;
                error << "Unexpected delimiter " << format_tokentype(curr_token.type)
                      << " at top level";
                add_error(this, curr_token.line, error.str().c_str());
                mask_next_error = true;
            } else if(curr_token.is_type()) {
                stringstream error;
                error << "Unexpected " << format_tokentype(curr_token.type) << " at top level";
                add_error(this, curr_token.line, error.str().c_str());
                mask_next_error = true;
            } else if(curr_token.is_literal()) {
                stringstream error;
                error << "Unexpected " << format_tokentype(curr_token.type) << " at top level";
                add_error(this, curr_token.line, error.str().c_str());
                mask_next_error = true;
            } else if(curr_token.is_special()) {
                stringstream error;
                error << "Unexpected " << format_tokentype(curr_token.type) << " at top level";
                add_error(this, curr_token.line, error.str().c_str());
                mask_next_error = true;
            } else {
                stringstream error;
                error << "Stray " << format_tokentype(curr_token.type) << " in .dc module";
                add_error(this, curr_token.line, error.str().c_str());
                mask_next_error = true;
            }
            break;
        }
    }

    if(num_errors > 0) {
        if(parser_owns_module) { delete module; }
        module = nullptr;
    }

    return module;
}

// if(Error) { Recover: Scan to end of '}' or next top-level keyword. }
Struct *Parser::parse_struct()
{
    bool error_occured = false;
    LineInfo start = curr_token.line;
    if(curr_token.type != Token_Struct) {
        unexpected_token(this, "keyword \"struct\"", "Attempting to parse struct definition");
        return nullptr;
    }
    eat_token(this);

    string struct_name;
    if(curr_token.type == '{') {
        add_error(this, curr_token.line, "Missing struct name before '{'");
        error_occured = true;
    } else if(curr_token.type != Token_Identifier) {
        add_error(this, curr_token.line, "Expecting struct name after keyword \"struct\"");
        mask_next_error = true;
        return nullptr;
    } else {
        struct_name = curr_token.value.text;
        eat_token(this);
    }

    // As a special case, lets check to see if a list of base classes follows the identifier
    if(curr_token.type == ':') {
        add_error(this, curr_token.line,
                  "Unexpected ':', expecting '{'",
                  "Structs cannot inherit from other types");
        error_occured = true;
        eat_token(this);
        while(true) {
            if(curr_token.type == ';' ||
               curr_token.type == Token_Identifier) {
                eat_token(this);
            } else {
                break;
            }
        }
    }

    // Parse struct body
    if(curr_token.type != '{') {
        stringstream info;
        info << "Missing struct body for \"struct " << struct_name << '"';
        unexpected_token(this, "'{'", info.str().c_str());
        mask_next_error = true;
        return nullptr;
    }
    eat_token(this);

    vector<Field *> fields;
    unordered_map<string, LineInfo> field_names;
    while(curr_token.type != '}' && curr_token.type != Token_Eof) {
        LineInfo field_start = curr_token.line;
        Field *field = parse_struct_field();
        if(field == nullptr) {
            error_occured = true;
            if(curr_token.type == Token_Eof) { break; }
            if(curr_token.type == '}') { break; }

            eat_until_end_of_statement_or_line(this, field_start);
            mask_next_error = true;
        } else {
            fields.push_back(field);
            mask_next_error = false;

            if(!field->name().empty()) {
                if(field_names.find(field->name()) != field_names.end()) {
                    stringstream error, info;
                    LineInfo prev = field_names[field->name()];
                    error << "A field with the name \"" << field->name()
                          << "\" was already defined in \"struct " << struct_name << '\"';
                    info << "Previous definition at line " << prev.num << ", column " << prev.col;

                    add_error(this, field_start, error.str().c_str(), info.str().c_str());
                    error_occured = true;
                } else {
                    field_names.emplace(field->name(), field_start);
                }
            }
        }

        // Consume any extra semi-colons before the next field
        while(curr_token.type == ';') { eat_token(this); }
    }
    if(curr_token.type == Token_Eof) {
        add_error(this, start, "Missing closing brace '}' at end of struct definition");
        for(Field *field : fields) {
            delete field;
        }
        return nullptr;
    }
    eat_token(this);

    if(curr_token.type == ';') {
        eat_token(this);
    } else {
        add_error(this, start, "Missing ';' after struct definition");
        error_occured = true;
    }

    if(error_occured) {
        for(Field *field : fields) {
            delete field;
        }
        return nullptr;
    }

    // Actually construct the struct.  If anything fails here, it is
    // an internal parser error and needs to be caught up above.
    Struct *struct_ = new Struct(struct_name);
    for(Field *field : fields) { struct_->add_field(field); }
    return struct_;
}

// if(Error) { Recover: Scan to end of '}' or next top-level keyword. }
Class *Parser::parse_class()
{
    bool error_occured = false;
    LineInfo start = curr_token.line;
    if(curr_token.type != Token_Class) {
        unexpected_token(this, "keyword \"class\"", "Attempting to parse class definition");
        return nullptr;
    }
    eat_token(this);

    string class_name;
    if(curr_token.type == ':') {
        add_error(this, curr_token.line,
                  "Unexpected ':', expecting identifier",
                  "Missing class name before list of base classes");
        error_occured = true;
    } else if(curr_token.type == '{') {
        add_error(this, curr_token.line, "Missing class name before '{'");
        error_occured = true;
    } else if(curr_token.type != Token_Identifier) {
        add_error(this, curr_token.line, "Expecting class name after keyword \"class\"");
        mask_next_error = true;
        return nullptr;
    } else {
        class_name = curr_token.value.text;
        eat_token(this);
    }

    // Parse class parents
    vector<Class *> parents; // Track parents in list so we don't modify a parent if parsing fields
    if(curr_token.type == Token_Identifier) {
        add_error(this, curr_token.line,
                  "Unexpected identifier, expecting ':' or '{'",
                  "Must use ':' before listing base classes");
        mask_next_error = true;
        return nullptr;
    } else if(curr_token.type == ':') {
        eat_token(this);
        if(curr_token.type == '{') {
            add_error(this, curr_token.line, "No base classes listed after ':'");
            error_occured = true;
        } else {
            while(true) {
                if(curr_token.type == ',') {
                    add_error(this, curr_token.line, "Missing base class name before ','");
                    error_occured = true;
                    eat_token(this);
                    continue;
                } else if(curr_token.type == '{') {
                    add_error(this, curr_token.line, "Missing base class name before '{'");
                    error_occured = true;
                    break;
                } else if(curr_token.type != Token_Identifier) {
                    unexpected_token(this, "identifier");
                    return nullptr;
                }

                const char *name = nullptr;
                Type *base_type = nullptr;
                Struct *base_struct = nullptr;
                Class *base_class = nullptr;

                // If the parser doesn't have a module, all other class names are undeclared
                if(module == nullptr) {
                    stringstream error;
                    error << "Type \"class " << name << "\" has not been declared";
                    add_error(this, curr_token.line, error.str().c_str());
                    mask_next_error = true;
                    error_occured = true;
                    goto next_parent;
                }

                name = curr_token.value.text;
                base_type = module->type_by_name(name);
                if(base_type != nullptr) {
                    stringstream error;
                    error << "Type \"class " << name << "\" has not been declared";
                    add_error(this, curr_token.line, error.str().c_str());
                    error_occured = true;
                    goto next_parent;
                }

                base_struct = base_type->as_struct();
                if(base_struct == nullptr) {
                    stringstream error;
                    error << "Class cannot inherit from non-class type \"" << name << '"';
                    add_error(this, curr_token.line, error.str().c_str());
                    error_occured = true;
                    goto next_parent;
                }

                base_class = base_struct->as_class();
                if(base_class == nullptr) {
                    stringstream error;
                    error << "Class cannot inherit from \"struct " << name << '"';
                    add_error(this, curr_token.line, error.str().c_str(),
                              "Base class must be a class (a struct cannot be used as a base class)");
                    error_occured = true;
                    goto next_parent;
                }

                // Everything went ok!
                parents.push_back(base_class);

            next_parent:
                eat_token(this); // Eat the identifier
                if(curr_token.type == ',') {
                    eat_token(this); // Eat the comma
                } else {
                    break;
                }
            }
        }
    }

    // Parse Class Body
    if(curr_token.type != '{') {
        stringstream info;
        info << "Missing class body for \"class " << class_name << "\"";
        unexpected_token(this, "'{'", info.str().c_str());
        mask_next_error = true;
        return nullptr;
    }
    eat_token(this);

    // @TODO(Kevin): Potential for a lot of cleanup/simplification if we can
    // just update the fields after the fact to be in the correct order.
    //
    // We keep track of a lot of state here because we don't want to add our parents
    // (and their fields) to the class until we know parsing will be successful.
    vector<string> fields;
    unordered_map<string, LineInfo> field_definitions;
    unordered_map<string, Field *> atomics;
    unordered_map<string, MoleculeDefn *> molecules;
    while(curr_token.type != '}' && curr_token.type != Token_Eof) {
        LineInfo field_start = curr_token.line;

        // Try parsing as molecular
        if(curr_token.type == Token_Identifier && next_token.type == ':') {
            MoleculeDefn *molecule = parse_molecule(this);
            if(molecule == nullptr) {
                error_occured = true;
                if(curr_token.type == Token_Eof) { break; }
                if(curr_token.type == '}') { break; }

                eat_until_end_of_statement_or_line(this, field_start);
                mask_next_error = true;
            } else if(molecule->name.empty()) {
                add_error(this, field_start, "Fields in classes must have names");
                error_occured = true;
                delete molecule;
            } else if(field_definitions.find(molecule->name) != field_definitions.end()) {
                stringstream error, info;
                LineInfo prev = field_definitions[molecule->name];
                error << "A field with the name \"" << molecule->name
                      << "\" was already defined in \"class " << class_name << '\"';
                info << "Previous definition at line " << prev.num << ", column " << prev.col;

                add_error(this, field_start, error.str().c_str(), info.str().c_str());
                error_occured = true;
                delete molecule;
            } else {
                mask_next_error = false;

                fields.push_back(molecule->name);
                field_definitions.emplace(molecule->name, field_start);
                molecules.emplace(molecule->name, molecule);
            }
            continue;
        }

        // Try parsing as function or variable
        Field *field = parse_class_field(nullptr);
        if(field == nullptr) {
            error_occured = true;
            if(curr_token.type == Token_Eof) { break; }
            if(curr_token.type == '}') { break; }

            eat_until_end_of_statement_or_line(this, field_start);
            mask_next_error = true;
        } else if(field->name().empty()) {
                add_error(this, field_start, "Fields in classes must have names");
                error_occured = true;
        } else if(field_definitions.find(field->name()) != field_definitions.end()) {
                stringstream error, info;
                LineInfo prev = field_definitions[field->name()];
                error << "A field with the name \"" << field->name()
                      << "\" was already defined in \"class " << class_name << '\"';
                info << "Previous definition at line " << prev.num << ", column " << prev.col;

                add_error(this, field_start, error.str().c_str(), info.str().c_str());
                error_occured = true;
        } else {
            mask_next_error = false;

            fields.push_back(field->name());
            field_definitions.emplace(field->name(), field_start);
            atomics.emplace(field->name(), field);
        }

        // Consume any extra semi-colons before the next field
        while(curr_token.type == ';') { eat_token(this); }
    }

    if(curr_token.type == Token_Eof) {
        add_error(this, start, "Missing closing brace '}' at end of class definition");
        for(auto it : atomics) { delete it.second; }
        for(auto it : molecules) { delete it.second; }
        return nullptr;
    }
    eat_token(this);

    if(curr_token.type == ';') {
        eat_token(this);
        // Semicolons are a good point to begin unmasking errors, and this
        // one is likely to cbe ours because it is after a closing brace
        mask_next_error = false;
    } else {
        add_error(this, start, "Missing ';' after struct definition");
        for(auto it : atomics) { delete it.second; }
        for(auto it : molecules) { delete it.second; }
        return nullptr;
    }

    // Actually construct the class
    Class *class_ = new Class(class_name);
    for(Class *parent : parents) { class_->add_parent(parent); }
    for(string field : fields) {
        if(atomics.find(field) != atomics.end()) {
            class_->add_field(atomics[field]);
            continue;
        }

        // FIXME(Kevin): I'm breaking the "keep module in consistent-state" rule
        // with molecular fields so I can get basic parsing implemented and testable;
        // fix as soon as the parser can accept the .dc files that are currently in-use
        MoleculeDefn *defn = molecules[field];
        MolecularField *molecule = new MolecularField(defn->name);
        for(auto atom_decl : defn->fields) {
            const string& atom_name = atom_decl.first;
            const LineInfo& atom_line = atom_decl.second;

            Field *atom = class_->field_by_name(atom_name);
            if(atom == nullptr) {
                error_occured = true;

                stringstream error;
                error << "No field with name \"" << atom_name << "\" defined in class";
                add_error(this, atom_line, error.str().c_str());
            } else if(atom->as_molecular() != nullptr) {
                error_occured = true;
                add_error(this, atom_line, "Molecular field cannot use a molecular as an atom");
            } else {
                bool ok = molecule->add_atomic(atom);
                if(!ok) {
                    error_occured = true;

                    stringstream error;
                    error << "Atomic field \"" << atom_name
                          << "\" has incompatible keywords with previous atoms";
                    add_error(this, atom_line, error.str().c_str());
                }
            }
        }
    }

    if(error_occured) {
        // @FIXME(Kevin): This leaves dangling pointers in any parents!
        delete class_;
        return nullptr;
    }

    return class_;
}

Import *Parser::parse_import()
{
    bool has_symbols;
    LineInfo start = curr_token.line;
    if(curr_token.type == Token_Import) {
        has_symbols = false;
    } else if(curr_token.type == Token_ImportFrom) {
        has_symbols = true;
    } else {
        unexpected_token(this, "keyword \"import\" or \"from\"", "Attempting to parse import");
        return nullptr;;
    }
    eat_token(this);

    if(curr_token.type != Token_Identifier && curr_token.type != '.') {
        if(has_symbols) {
            add_error(this, start, "Missing module name after \"from\"");
        } else {
            add_error(this, start, "Missing module name after \"import\"");
        }
        mask_next_error = true;
        return nullptr;
    }

    string module_path;
    while(true) {
        if(curr_token.type == Token_Identifier) {
            module_path += curr_token.value.text;
        } else if(curr_token.type == '.') {
            module_path += '.';
        } else {
            break;
        }
        eat_token(this);
    }

    if(!has_symbols) { return new Import(module_path); }
    if(curr_token.type != Token_Import) {
        add_error(this, start, "Missing \"import\" after \"from <module>\" in import");
        mask_next_error = true;
        return nullptr;
    }
    eat_token(this);

    if(curr_token.type != Token_Identifier) {
        unexpected_token(this, "symbol name", "Attempting to parse list of imported symbols");
        mask_next_error = true;
        return nullptr;
    }

    int last_line = curr_token.line.num;
    bool error_occured = false;
    string curr_symbol;
    Import *import = new Import(module_path);
    while(true) {
        if(curr_token.type == Token_Identifier) {
            curr_symbol += curr_token.value.text;
        } else if(curr_token.type == '/') {
            curr_symbol += '/';
        } else if(curr_token.type == ',') {
            if(curr_symbol.empty()) {
                error_occured = true;
                add_error(this, curr_token.line, "Missing symbol name before ','");
            } else {
                import->symbols.push_back(curr_symbol);
                curr_symbol.clear();
            }
        } else if(curr_token.type == ';') {
            if(curr_symbol.empty()) {
                error_occured = true;
                add_error(this, curr_token.line, "Missing symbol name before ';'");
            } else {
                import->symbols.push_back(curr_symbol);
                curr_symbol.clear();
            }
            break;
        } else if(curr_token.line.num > last_line) {
            if(!curr_symbol.empty()) {
                import->symbols.push_back(curr_symbol);
                curr_symbol.clear();
            }
            break;
        } else {
            if(curr_symbol.empty()) {
                unexpected_token(this, "symbol name", "Attempting to parse list of imported symbols");
                mask_next_error = true;
                return nullptr;
            } else {
                unexpected_token(this, "',' or ';'", "Attempting to parse list of imported symbols");
                mask_next_error = true;
                return nullptr;                
            }
        }
        last_line = curr_token.line.num;
        eat_token(this);
    }

    if(error_occured) {
        delete import;
        return nullptr;
    }

    return import;
}

bool Parser::parse_typedef_into_module(Module *module)
{
    if(curr_token.type != Token_Typedef) {
        unexpected_token(this, "keyword \"typedef\"", "Attempting to parse typedef declaration");
        return false;
    }
    eat_token(this);

    // Parse the existing type that we want to alias
    bool caller_owns_return;
    Type *type = parse_type_expr(caller_owns_return);
    if(type == nullptr) { return false; }

    // Look for the new type name as an identifier
    if(curr_token.type == ';') {
        add_error(this, curr_token.line, "Missing new type name before ';' in typedef");
        if(caller_owns_return) { delete type; }
        return false;
    } else if(curr_token.type != Token_Identifier) {
        unexpected_token(this, "identifier", "Attempting to parse typedef");
        if(caller_owns_return) { delete type; }
        return false;
    }

    string name = curr_token.value.text;
    if(module->type_by_name(name) != nullptr) {
        stringstream error;
        error << "Type \"" << name << "\" has already been declared";
        add_error(this, curr_token.line, error.str().c_str());
        if(caller_owns_return) { delete type; }
        return false;
    }
    eat_token(this);

    if(curr_token.type != ';') {
        add_error(this, prev_token.line, "Missing ';' after typedef declaration");
        if(caller_owns_return) { delete type; }
        return false;
    }

    Type *type_alias = new TypeAlias(name, type, caller_owns_return);
    module->add_typedef(name, type_alias, true);
    return true;
}

bool Parser::parse_keywords_into_module(Module *module)
{
    if(curr_token.type != Token_Keyword) {
        unexpected_token(this, "keyword \"keyword\"", "Attempting to parse keyword declaration");
        return false;
    }
    eat_token(this);

    if(curr_token.type != Token_Identifier) {
        unexpected_token(this, "identifier",
                         "Attempting to parse keyword declaration"
                         "Declaration must include at least one word.");
        mask_next_error = true;
        return false;
    }

    while(curr_token.type == Token_Identifier) {
        module->add_keyword(curr_token.value.text);
        eat_token(this);
    }
    return true;

// @NOTE(Kevin): Eventually I want to separate keywords with commas, and end the keyword list
// with a semicolon.  We may consider storing keywords in groups if we want to be able to reproduce
// a particular file.

/*
    bool error_occured = false;

    while(true) {
        if(curr_token.type != Token_Identifier) {
            module->add_keyword(curr_token.value.text);
        } else {
            unexpected_token(this, "identifier", "Attempting to parse keyword declaration");
            mask_next_error = true;
            return false;
        }
        eat_token(this);

        while(curr_token.type == Token_Identifier) {
            error_occured = true;
            add_error(this, curr_token.line, "Missing ',' between declared keywords");
            eat_token(this);
        }

        if(curr_token.type == ';') {
            break;
        } else if(curr_token.type != ',') {
            unexpected_token(this, "',' or ';'", "Attempting to parse keyword declaration");
            mask_next_error = true;
            return false;
        }
        eat_token(this); // Eat ','
    }

    if(error_occured) { return false; }
    eat_token(this); // Eat ';'
    return true;
*/
}

// if(Error) { Recover: Scan to end of line or first ';' }
Field *Parser::parse_struct_field()
{
    if(curr_token.type == Token_Identifier) {
        if(module == nullptr || module->type_by_name(curr_token.value.text) == nullptr) {
            if(next_token.type == '(') {
                // We think this is supposed to look like a method
                stringstream error;
                error << "Cannot find type \"" << curr_token.value.text << "\"";
                add_error(this, curr_token.line, error.str().c_str(),
                          "Field looks like a method, but methods cannot be defined in a struct");
                return nullptr;
            } else if(next_token.type == ':') {
                // We think this is supposed to look like a molecular field
                stringstream error;
                error << "Cannot find type \"" << curr_token.value.text << "\"";
                add_error(this, curr_token.line, error.str().c_str(),
                          "Field looks like a molecular, but molecular cannot be defined in a struct");
                return nullptr;
            }
        }
    } else if(!curr_token.is_type()) {
        unexpected_token(this, "a type name", "Attempting to parse fields in a struct");
        return nullptr;
    }

    // Get field type
    bool caller_owns_return;
    Type *type = parse_type_expr(caller_owns_return);
    if(type == nullptr) { return nullptr; }

    // A struct field can have a name
    string field_name;
    if(curr_token.type == Token_Identifier) {
        field_name = curr_token.value.text;
        eat_token(this);
    }

    // A struct field can have a default value
    Value *value = nullptr;
    if(curr_token.type == '=') {
        eat_token(this);
        value = parse_value_expr(type);
        if(value == nullptr) {
            if(caller_owns_return) { delete type; }
            return nullptr;
        }
    }

    // As a special case, lets check for extra identifiers on this line
    // that look like keywords and give a helpful error message
    bool extra_identifiers = false;
    LineInfo decl_end = prev_token.line;
    while(curr_token.type == Token_Identifier &&
          curr_token.line.num <= decl_end.num) {
        extra_identifiers = true;
        eat_token(this);
    }

    if(extra_identifiers) {
        add_error(this, curr_token.line,
                  "Unexpected identifier after field definition",
                  "Struct fields cannot be annotated with keywords");
        if(caller_owns_return) { delete type; }
        if(value != nullptr) { delete value; }
        return nullptr;
    }


    if(curr_token.type != ';') {
        stringstream error;
        error << "Unexpected " << format_tokentype(curr_token.type) << ", expecting ';'";
        if(value != nullptr) {
            error << ", or operator '='";
        } else if(field_name.empty()) {
            error << ", or identifier";
        }
        add_error(this, curr_token.line, error.str().c_str(),
                  "Attempting to parse fields in a struct");
        if(caller_owns_return) { delete type; }
        if(value != nullptr) { delete value; }
        mask_next_error = true;
        return nullptr;
    }

    // Actually construct the field.  If anything fails here, it is
    // an internal parser error and needs to be caught up above.
    Field *field = nullptr;
    if(field_name.empty()) {
        field = new Field(type, caller_owns_return);
    } else {
        field = new Field(field_name, type, caller_owns_return);
    }

    if(value != nullptr) {
        field->set_default_value(value);
        delete value;
    }

    return field;
}

enum ClassFieldParseMode {
    ClassParseMode_None,
    ClassParseMode_Field,
    ClassParseMode_Method,
    ClassParseMode_Molecular
};

// if(Error) { Recover: Scan to end of line or first ';' }
Field *Parser::parse_class_field(Class *class_)
{
    LineInfo start = curr_token.line;
    ClassFieldParseMode parse_mode;
    if(curr_token.type == Token_Identifier) {
        if(module != nullptr && module->type_by_name(curr_token.value.text) != nullptr) {
            parse_mode = ClassParseMode_Field;
        } else if(next_token.type == '(') {
            parse_mode = ClassParseMode_Method;
        } else if(next_token.type == ':') {
            parse_mode = ClassParseMode_Molecular;
        } else {
            stringstream error;
            error << "Cannot find type \"" << curr_token.value.text << "\"";
            add_error(this, curr_token.line, error.str().c_str(), "Attempting to parse class field");
            return nullptr;
        }
    } else if(!curr_token.is_type()) {
        unexpected_token(this, "identifier or type name", "Attempting to parse class field");
        return nullptr;
    } else {
        parse_mode = ClassParseMode_Field;
    }

    Field *field = nullptr;
    switch(parse_mode) {
    case ClassParseMode_Field:
        {
            // Get field type
            bool field_owns_type;
            Type *type = parse_type_expr(field_owns_type);
            if(type == nullptr) { return nullptr; }

            // Class fields must have a name
            if(curr_token.type != Token_Identifier) {
                add_error(this, start, "Missing field name after type; class fields must be named");
                return nullptr;
            }
            string field_name = curr_token.value.text;
            eat_token(this);

            // Class fields can have default values
            Value *value = nullptr;
            if(curr_token.type == '=') {
                eat_token(this);
                value = parse_value_expr(type);
                if(value == nullptr) {
                    if(field_owns_type) { delete type; }
                    return nullptr;
                }
            }

            // Actually construct the field
            field = new Field(field_name, type, field_owns_type);
            if(value != nullptr) {
                field->set_default_value(value);
                delete value;
            }

            // Class fields can have keywords
            bool ok = parse_keywords_for_field(field);
            if(!ok) {
                delete field;
                return nullptr;
            }
        }
        break;
    case ClassParseMode_Method:
        {
            string method_name = curr_token.value.text;
            eat_token(this); // Eat identifier
            eat_token(this); // Eat '('

            // Parse function
            Method *method = new Method();
            while(curr_token.type != ')') {
                LineInfo param_start = curr_token.line;

                bool param_owns_type;
                Type *type = parse_type_expr(param_owns_type);
                if(type == nullptr) {
                    delete method;
                    return nullptr;
                } else if(type->subtype() == Subtype_Struct && type->as_struct()->as_class()) {
                    add_error(this, param_start, "Parameter type cannot be a class");
                    if(param_owns_type) { delete type; }
                    delete method;
                    return nullptr;
                }

                string param_name; // = curr_token.value.text;
                if(curr_token.type == Token_Identifier) {
                    param_name = curr_token.value.text;
                    if(method->param_by_name(param_name) != nullptr) {
                        stringstream error;
                        error << "Duplicate parameter name \"" << param_name
                              << "\" in method definition";
                        add_error(this, curr_token.line, error.str().c_str());
                        if(param_owns_type) { delete type; }
                        return nullptr;
                    }
                    eat_token(this);
                }

                Value *value = nullptr;
                if(curr_token.type == '=') {
                    value = parse_value_expr(type);
                    if(value == nullptr) {
                        if(param_owns_type) { delete type; }
                        return nullptr;
                    }
                }

                if(curr_token.type == ',') {
                    eat_token(this);
                } else if(curr_token.type != ')') {
                    unexpected_token(this, "',' or ')'", "Attempting to parse method parameters");
                }

                Parameter *param;
                param = method->add_param(param_name, type, param_owns_type);
                if(value != nullptr) { param->set_default_value(value); }
            }
            eat_token(this); // Eat right-paren ')'

            // @NOTE(Kevin): I want to remove support for assigning a default value to methods
            // Parameters already support having a default value, so we should rely on that tool
            Value *value = nullptr;
            if(curr_token.type == '=') {
                value = parse_value_expr(method);
                if(value == nullptr) {
                    delete method;
                    return nullptr;
                }
            }

            // Actually construct the field
            field = new Field(method_name, method, true);
            if(value != nullptr) { field->set_default_value(value); }

            // Class methods can have keywords
            bool ok = parse_keywords_for_field(field);
            if(!ok) {
                delete field;
                return nullptr;
            }

            return field;
        }
        break;
    case ClassParseMode_Molecular:
        {
            if(class_ == nullptr) {
                add_error(this, curr_token.line,
                          "Unable to parse molecular field outside the scope of a class");
                return nullptr;
            }

            // @TODO(Kevin): Factored into parse_molecular(Parser *), which could probably
            // be reused here;  Maybe some of the other parse_class cleanup will help inform this
            MolecularField *molecular = new MolecularField(curr_token.value.text);
            eat_token(this); // Eat identifier
            eat_token(this); // Eat ':'

            bool error_occured = true;
            while(curr_token.type != ';') {
                if(curr_token.type == ',') {
                    add_error(this, curr_token.line,
                              "Missing atomic field before ',' in molecular field definition");
                    error_occured = true;
                    eat_token(this);
                    continue;
                } else if(curr_token.type != Token_Identifier) {
                    unexpected_token(this, "identifier", "Attempting to parse molecular field");
                    mask_next_error = true;
                    delete molecular;
                    return nullptr;
                }

                const char *field_name = curr_token.value.text;
                Field *field = class_->field_by_name(curr_token.value.text);
                if(field == nullptr) {
                    error_occured = true;

                    stringstream error;
                    error << "No field with name \"" << field_name << "\" defined in class";
                    add_error(this, curr_token.line, error.str().c_str());
                } else if(field->as_molecular() != nullptr) {
                    error_occured = true;
                    add_error(this, curr_token.line, "Molecular field cannot use a molecular as an atom");
                } else {
                    bool ok = molecular->add_atomic(field);
                    if(!ok) {
                        error_occured = true;

                        stringstream error;
                        error << "Atomic field \"" << field_name
                              << "\" has incompatible keywords with previous atoms";
                        add_error(this, curr_token.line, error.str().c_str());
                    }
                }
                eat_token(this);

                if(curr_token.type == ',') {
                    eat_token(this);
                }
            }

            if(error_occured) {
                delete molecular;
                return nullptr;
            }

            field = molecular;
        }
        break;
    default:
        add_error(this, curr_token.line, "Internal parser error: Unexpected parse_mode for field");
        return nullptr;
    }

    // Handle semicolon at end of field
    if(curr_token.type != ';') {
        unexpected_token(this, "';'", "Attempting to parse a class field");
        delete field;
        return nullptr;
    }
    eat_token(this);

    return field;
}

MoleculeDefn *parse_molecule(Parser *parser)
{
    MoleculeDefn *molecule = new MoleculeDefn();
    molecule->name = parser->curr_token.value.text;
    eat_token(parser); // Eat identifier
    eat_token(parser); // Eat ':'

    bool error_occured = false;
    while(parser->curr_token.type != ';') {
        if(parser->curr_token.type == ',') {
            error_occured = true;
            add_error(parser, parser->curr_token.line,
                      "Missing atomic field before ',' in molecular field definition");
            eat_token(parser);
            continue;
        } else if(parser->curr_token.type != Token_Identifier) {
            unexpected_token(parser, "identifier", "Attempting to parse molecular field");
            parser->mask_next_error = true;

            delete molecule;
            return nullptr;
        }

        molecule->fields.emplace(parser->curr_token.value.text, parser->curr_token.line);
        eat_token(parser);

        if(parser->curr_token.type == ',') {
            eat_token(parser);
        }
    }

    if(error_occured) {
        delete molecule;
        return nullptr;
    }

    return molecule;
}

bool Parser::parse_keywords_for_field(Field *field)
{
    bool error_occured = false;
    while(curr_token.type == Token_Identifier) {
        if(!module->has_keyword(curr_token.value.text)) {
            stringstream error;
            error << "Keyword \"" << curr_token.value.text << "\" not declared before use";
            add_warning(this, curr_token.line, error.str().c_str());

            // Declare the keyword so we only show this once per keyword
            if(module != nullptr && parser_owns_module) {
                module->add_keyword(curr_token.value.text);
            }
        }

        const char * keyword = curr_token.value.text;
        bool added = field->add_keyword(keyword);
        if(!added) {
            stringstream error;
            error << "Duplicate keyword \"" << keyword << " for field \"" << field->name() << '"';
            add_error(this, curr_token.line, error.str().c_str());
            error_occured = true;
        }
        eat_token(this);
    }

    return !error_occured;
}

Type *Parser::parse_type_expr(bool& caller_owns_return)
{
    Type* type = nullptr;
    if(curr_token.type == Token_Identifier) {
        if(module != nullptr) {
            type = module->type_by_name(curr_token.value.text);
        }

        if(type == nullptr) {
            stringstream error;
            error << "Type \"" << curr_token.value.text << "\" has not been declared";
            add_error(this, curr_token.line, error.str().c_str());
            return nullptr;
        }

        caller_owns_return = false;
    } else if(curr_token.is_type()) {
        bool is_primitive = (next_token.type != '(' &&
                             next_token.type != '/' &&
                             next_token.type != '%');
        if(is_primitive) {
            type = to_primitive(curr_token.type);
            caller_owns_return = false;
        } else if(curr_token.type == Token_String) {
            // NOTE(Kevin): Types with Subtype string shouldn't need to be modified below
            type = Type::String;
            caller_owns_return = false;
        } else if(curr_token.type == Token_Blob) {
            // NOTE(Kevin): Types with Subtype blob shouldn't need to be modified below
            type = Type::Blob;
            caller_owns_return = false;
        } else if(curr_token.type == Token_Byte) {
            type = new TypeAlias("byte", new Numeric(Numeric_Uint8), true);
            caller_owns_return = true;
        } else if(curr_token.type == Token_Char) {
            type = new TypeAlias("char", new Numeric(Numeric_Uint8), true);
            caller_owns_return = true;
        } else {
            type = new Numeric(to_numeric_type(curr_token.type));
            caller_owns_return = true;
        }
    } else {
        unexpected_token(this, "a type name");
        return nullptr;
    }
    eat_token(this);

    bool declared_array = false;
    bool declared_divisor = false;
    bool continue_expr = true;
    while(continue_expr) {
        switch(curr_token.type) {
        case '/':
            {
                Numeric *num = type->as_numeric();
                if(num == nullptr) {
                    stringstream error;
                    error << "Type \"" << type->to_string() << " cannot have a divisor";
                    add_error(this, curr_token.line, error.str().c_str());
                } else if(declared_divisor || num->divisor() != 1) {
                    stringstream error;
                    error << "Type \"" << type->to_string() << " already has a divisor";
                    add_error(this, curr_token.line, error.str().c_str());
                }
                declared_divisor = true;
                eat_token(this);

                if(curr_token.type == '-') {
                    add_error(this, curr_token.line,
                              "Unexpected unary operator '-' before divisor",
                              "Divisor must be unsigned integer");
                    eat_token(this);
                } else if(curr_token.type == '+') {
                    add_warning(this, curr_token.line,
                                "'+' ignored before divisor (divisor is treated as unsigned)");
                    eat_token(this);
                }

                unsigned int divisor = 1;
                if(curr_token.type == Token_Real) {
                    divisor = (unsigned int)curr_token.value.real;
                    if((double)divisor != curr_token.value.real) {
                        add_warning(this, curr_token.line,
                             "Truncation occured using real number as integer divisor");
                    }
                } else if(curr_token.type != Token_Integer) {
                    divisor = (unsigned int)curr_token.value.integer;
                    if((uint64_t)divisor != curr_token.value.integer) {
                        add_warning(this, curr_token.line,
                                    "Integer overflow occured converting divisor to 32-bit integer");
                    }
                } else {
                    stringstream error;
                    error << "Unexpected " << format_tokentype(curr_token.type)
                          << ", expecting integer";
                    add_error(this, curr_token.line, error.str().c_str(),
                              "Attempting to parse divisor for type");
                    if(caller_owns_return) { delete type; }
                    return nullptr;
                }

                if(divisor == 0) {
                    add_error(this, curr_token.line, "Divisor for numeric type cannot be 0");
                } else if(num != nullptr) {
                    bool ok = num->set_divisor(divisor);
                    if(!ok) {
                        stringstream error;
                        error << "Invalid divisor for type \"" << type->to_string() << '"';
                        add_error(this, curr_token.line, error.str().c_str());
                    }
                }
                eat_token(this);
            }
            break;
        case '%':
            {
                Numeric *num = type->as_numeric();
                if(num == nullptr) {
                    stringstream error;
                    error << "Type \"" << type->to_string() << "\" cannot have a modulus";
                    add_error(this, curr_token.line, error.str().c_str());
                } else if(num->has_modulus()) {
                    stringstream error;
                    error << "Type \"" << type->to_string() << "\" already has a modulus";
                    add_error(this, curr_token.line, error.str().c_str());
                }
                eat_token(this);

                eat_unary(this);
                double modulus = 0.0;
                bool is_negative = prev_token.type == '-';
                if(curr_token.type == Token_Real) {
                    modulus = curr_token.value.real;
                } else if(curr_token.type == Token_Integer) {
                    modulus = (double)curr_token.value.integer;
                } else {
                    unexpected_token(this, "number", "Attempting to parse modulus for type");
                    if(caller_owns_return) { delete type; }
                    return nullptr;
                }
                eat_token(this);

                if(modulus == 0.0) {
                    add_error(this, curr_token.line, "Modulus for numeric type cannot be 0");
                } else if(num != nullptr) {
                    if(is_negative) { modulus = -modulus; }
                    bool ok = num->set_modulus(modulus);
                    if(!ok) {
                        stringstream error;
                        error << "Invalid modulus for type \"" << type->to_string() << '"';
                        add_error(this, curr_token.line, error.str().c_str());
                    }
                }
            }
            break;
        case '(':
            {
                if(type->subtype() == Subtype_String || type->subtype() == Subtype_Blob) {
                    Array *ary = type->as_array();
                    if(ary->has_range()) {
                        stringstream error;
                        error << "Type \"" << type->to_string() << "\" already has a range";
                        add_error(this, curr_token.line, error.str().c_str());
                    }
                } else if(type->as_numeric() != nullptr) {
                    Numeric *num = type->as_numeric();
                    if(num->has_range()) {
                        stringstream error;
                        error << "Type \"" << type->to_string() << "\" already has a range";
                        add_error(this, curr_token.line, error.str().c_str());
                    }
                } else if(type->as_array()) {
                    if(declared_array) {
                        add_error(this, curr_token.line,
                                  "Unexpected numeric range for array type",
                                  "Numeric range must be declared before array specifier");
                    } else if(type->as_aliased()) {
                        add_error(this, curr_token.line,
                                  "Unexpected numeric range for array type",
                                  "Typedef was declared as an array");
                    } else {
                        add_error(this, curr_token.line, "Unexpected numeric range for array type");
                    }
                } else {
                    stringstream error;
                    error << "Type \"" << type->to_string() << "\" cannot have a range";
                    add_error(this, curr_token.line, error.str().c_str());
                }

                NumericRange range = parse_range_expr();
                if(range.is_nan()) {
                    // Pass the error upstream
                    if(caller_owns_return) { delete type; }
                    return nullptr;
                } else if(type->subtype() == Subtype_String) {
                    if(range.type != Number_Unsigned) {
                        add_error(this, curr_token.line,
                                  "Range for string-like type must be unsigned integer");
                    }

                    // @FIXME(Kevin): We should be able to tell the difference
                    // between a `string(0,20)` and a `char[0,20]`
                    // but this doesn't make that distinction yet
                    type = new Array(Type::Char, false, range);
                    caller_owns_return = true;
                } else if(type->subtype() == Subtype_Blob) {
                    if(range.type != Number_Unsigned) {
                        add_error(this, curr_token.line,
                                  "Range for blob-like type must be unsigned integer");
                    }

                    // @FIXME(Kevin): We should be able to tell the difference
                    // between a `blob(0,20)` and a `byte[0,20]`
                    // but this doesn't make that distinction yet
                    type = new Array(Type::Byte, false, range);
                    caller_owns_return = true;
                } else if(type->as_numeric()) {
                    Numeric *num = type->as_numeric();
                    if(!num->set_range(range)) {
                        stringstream error;
                        error << "Invalid range for type \"" << type->to_string() << "\"";
                        add_error(this, curr_token.line, error.str().c_str());
                    }
                }
            }
            break;
        case '[':
            {
                declared_array = true;
                if(next_token.type == ']') {
                    type = new Array(type, caller_owns_return);
                    caller_owns_return = true;

                    eat_token(this); // Eat '['
                    eat_token(this); // Eat ']'
                    break;
                }

                NumericRange range = parse_array_expr();
                if(range.is_nan()) {
                    // Pass the error upstream
                    if(caller_owns_return) { delete type; }
                    return nullptr;
                } else {
                    type = new Array(type, caller_owns_return, range);
                    caller_owns_return = true;
                }
            }
            break;
        default:
            continue_expr = false;
            break;
        }
    }

    return type;
}

NumericRange Parser::parse_range_expr()
{
    LineInfo start = curr_token.line;
    if(curr_token.type != '(') {
        unexpected_token(this, "'('", "Attempting to parse range for numeric, string, or blob");
        return NumericRange();
    }
    eat_token(this);

    Number lhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        if(prev_token.type == '-') {
            lhs = -(int64_t)curr_token.value.integer;
        } else if(prev_token.type == '+') {
            lhs = (int64_t)curr_token.value.integer;
        } else {
            lhs = (uint64_t)curr_token.value.integer;
        }
    } else if(curr_token.type == Token_Real) {
        if(prev_token.type == '-') {
            lhs = -curr_token.value.real;
        } else if(prev_token.type == '+') {
            lhs = (int64_t)curr_token.value.real;
        } else {
            lhs = curr_token.value.real;
        }
    } else if(curr_token.type == ')') {
        add_error(this, curr_token.line, "Missing size before ')' in range");
        mask_next_error = true;
        return NumericRange();
    } else {
        unexpected_token(this, "number", "Attempting to parse first value in range for numeric, string, or blob");
        mask_next_error = true;
        return NumericRange();
    }

    LineInfo lhs_line = curr_token.line;
    eat_token(this);

    // Range can be ( A ) or ( A , B )
    if(curr_token.type == ')') {
        eat_token(this);
        return NumericRange(lhs);
    } else if(curr_token.type == '+' || curr_token.type == '-') {
        // As a special cass, lets print a nice error method if it looks like math
        if(next_token.type == Token_Integer || next_token.type == Token_Real) {
            if(next_token.line.col - curr_token.line.col > 1) {
                add_error(this, curr_token.line,
                          "Unexpected integer, expecting ',' or ')'",
                          "Addition and subtraction are not supported");
                mask_next_error = true;
                return NumericRange();
            }
        }

        unexpected_token(this, "',' or ')'", "Attempting to parse range for numeric, string, or blob");
        mask_next_error = true;
        return NumericRange();
    } else if(curr_token.type == Token_Integer || curr_token.type == Token_Real) {
        if(next_token.type == ')') {
            eat_token(this); // Eat number
            eat_token(this); // Eat ')'
        } else {
            mask_next_error = true;
        }

        add_error(this, curr_token.line,
                  "Unexpected integer, expecting ',' or ')'",
                  "Must separate numbers in numeric range with ','");
        return NumericRange();
    } else if(curr_token.type != ',') {
        unexpected_token(this, "',' or ')'", "Attempting to parse range for numeric, string, or blob");
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);

    Number rhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        if(prev_token.type == '-') {
            rhs = -(int64_t)curr_token.value.integer;
        } else if(prev_token.type == '+') {
            rhs = (int64_t)curr_token.value.integer;
        } else {
            rhs = (uint64_t)curr_token.value.integer;
        }
    } else if(curr_token.type == Token_Real) {
        if(prev_token.type == '-') {
            rhs = -curr_token.value.real;
        } else if(prev_token.type == '+') {
            rhs = (int64_t)curr_token.value.real;
        } else {
            rhs = curr_token.value.real;
        }
    } else if(curr_token.type == ')') {
        add_error(this, curr_token.line, "Missing max value before ')' in range");
        eat_token(this);
        return NumericRange();
    } else {
        unexpected_token(this, "number", "Attempting to parse fixed-size/min value in numeric range");
        mask_next_error = true;
        return NumericRange();
    }

    LineInfo rhs_line = curr_token.line;
    eat_token(this);

    // Close expression
    if(curr_token.type != ')') {
        unexpected_token(this, "')'", "Looking for closing paren to numeric range");
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);;

    // Actually build the numeric range
    if(lhs.type == Number_Floating || rhs.type == Number_Floating) {
        if((double)lhs > (double)rhs) {
            add_error(this, start, "Min value of range greater than max");
            return NumericRange();
        } else {
            return NumericRange((double)lhs, (double)rhs);
        }
    } else if(lhs.type == Number_Signed || rhs.type == Number_Signed) {
        // Check left integer overflow
        if(lhs.type != Number_Signed) {
            int64_t lhs_s64 = (int64_t)lhs;
            if(lhs.uinteger > 0 && !(lhs_s64 > 0)) {
                add_warning(this, lhs_line,
                            "Unsigned integer overflow in conversion to signed range");
            }
        }

        // Check right integer overflow
        if(rhs.type != Number_Signed) {
            int64_t rhs_s64 = (int64_t)rhs;
            if(rhs.uinteger > 0 && !(rhs_s64 > 0)) {
                add_warning(this, rhs_line,
                            "Unsigned integer overflow in conversion to signed range");
            }
        }

        if((int64_t)lhs > (int64_t)rhs) {
            add_error(this, start, "Min value of range greater than max");
            return NumericRange();
        } else {
            return NumericRange((int64_t)lhs, (int64_t)rhs);
        }
    } else {
        if(lhs.uinteger > rhs.uinteger) {
            add_error(this, start, "Min value of range greater than max");
            return NumericRange();
        } else {
            return NumericRange(lhs.uinteger, rhs.uinteger);
        }
    }
}

NumericRange Parser::parse_array_expr()
{
    LineInfo start = curr_token.line;
    if(curr_token.type != '[') {
        unexpected_token(this, "'['", "Attempting to parse sized array expression");
        return NumericRange();
    }
    eat_token(this);

    Number lhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        lhs = (uint64_t)curr_token.value.integer;
    } else if(curr_token.type == Token_Real) {
        if(next_token.type == ']') {
            add_error(this, curr_token.line, "Array size must be integer");
        } else {
            add_error(this, curr_token.line, "Array min must be integer");
        }
        lhs = Number(); // NaN signals error
    } else if(curr_token.type == ']') {
        add_error(this, curr_token.line, "Missing size before closing ']' in sized array");
        mask_next_error = true;
        return NumericRange();
    } else {
        unexpected_token(this, "number", "Attempting to parse array expression");
        mask_next_error = true;
        return NumericRange();
    }

    if(prev_token.type == '-' ||
       prev_token.type == '+')
    {
        if(next_token.type == ']') {
            add_error(this, curr_token.line, "Array size must be unsigned");
        } else {
            add_error(this, curr_token.line, "Array min must be unsigned");
        }
        lhs = Number(); // NaN signals error
    }
    eat_token(this);

    // Array expression can be [ A ] or [ A , B ]
    if(curr_token.type == ']') {
        eat_token(this);
        return NumericRange(lhs);
    } else if(curr_token.type == Token_Integer || curr_token.type == Token_Real) {
        if(next_token.type == ']') {
            eat_token(this); // Eat number
            eat_token(this); // Eat ']'
        } else {
            mask_next_error = true;
        }

        add_error(this, curr_token.line,
                  "Unexpected integer, expecting ',' or ']'",
                  "Must separate numbers in array expression with ','");
        return NumericRange();
    } else if(curr_token.type != ',') {
        unexpected_token(this, "',' or ']'", "Attempting to parse array expression");
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);

    Number rhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        rhs = (uint64_t)curr_token.value.integer;
    } else if(curr_token.type == Token_Real) {
        add_error(this, curr_token.line, "Array max must be integer");
        rhs = Number(); // NaN signals error
    } else if(curr_token.type == ']') {
        add_error(this, curr_token.line, "Missing max value before ']' in array range");
        eat_token(this);
        return NumericRange();
    } else {
        unexpected_token(this, "unsigned integer", "Attempting to parse array expression");
        mask_next_error = true;
        return NumericRange();
    }

    if(prev_token.type == '-' ||
       prev_token.type == '+')
    {
        add_error(this, curr_token.line, "Array max must be unsigned");
        rhs = Number(); // NaN signals error
    }
    eat_token(this);

    if(curr_token.type != ']') {
        unexpected_token(this, "']'", "Looking for closing bracket to array range");
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);;

    // Actually build the array range
    if(lhs.type == Number_NotANumber || rhs.type == Number_NotANumber) {
        return NumericRange();
    } else if(lhs.uinteger > rhs.uinteger) {
        add_error(this, start, "Min size of array range greater than max");
        return NumericRange();
    } else {
        return NumericRange(lhs.uinteger, rhs.uinteger);
    }
}

Value *Parser::parse_value_expr(const Type *type)
{
    Value *value = nullptr;
    switch(type->subtype()) {
    case Subtype_Numeric:
        value = parse_numeric_value(this, type->as_numeric());
        break;
    case Subtype_String:
        value = parse_string_value(this, type->as_array());
        break;
    case Subtype_Blob:
        value = parse_blob_value(this, type->as_array());
        break;
    case Subtype_Array:
        value = parse_array_value(this, type->as_array());
        break;
    case Subtype_Struct:
        value = parse_struct_value(this, type->as_struct());
        break;
    case Subtype_Method:
        value = parse_method_value(this, type->as_method());
        break;
    case Subtype_None:
        break;
    }

    return value;
}

Value *parse_numeric_value(Parser *parser, const Numeric *numeric)
{
    bool is_negative = false;
    Value *value = nullptr;

    // @TODO(Kevin): Add warnings for overflow/underflow/etc

    if(parser->curr_token.type == '-') {
        if(numeric->is_unsigned()) {
            add_error(parser, parser->curr_token.line, "Unexpected '-' for unsigned integer");
            return nullptr;
        }

        is_negative = true;
        eat_token(parser);
    } else if(parser->curr_token.type == '+') {
        if(numeric->is_unsigned()) {
            add_warning(parser, parser->curr_token.line, "'+'' ignored for unsigned integer");
        }

        eat_token(parser);
    }

    Token token = parser->curr_token;
    switch(token.type) {
    case Token_Character:
        if(parser->prev_token.type == '-' || parser->prev_token.type == '+') {
            add_warning(parser, parser->prev_token.line, "Unary +/- ignored before character literal");
        }

        value = new Value(numeric);
        if(numeric->is_floating()) {
            value->m_numeric = (double)token.value.character;
        } else if(numeric->is_signed()) {
            value->m_numeric = (int64_t)token.value.character;
        } else {
            value->m_numeric = (uint64_t)token.value.character;
        }
        break;
    case Token_Integer:
        {
            value = new Value(numeric);
            if(numeric->is_floating()) {
                if(is_negative) {
                    value->m_numeric = -(double)token.value.integer;
                } else {
                    value->m_numeric = (double)token.value.integer;
                }
            } else if(numeric->is_signed()) {
                if(is_negative) {
                    value->m_numeric = -(int64_t)token.value.integer;
                } else {
                    value->m_numeric = (int64_t)token.value.integer;
                }
            } else  {
                value->m_numeric = token.value.integer;
            }
        }
        break;
    case Token_Real:
        {
            value = new Value(numeric);
            if(numeric->is_floating()) {

                if(is_negative) {
                    value->m_numeric = -token.value.real;
                } else {
                    value->m_numeric = token.value.real;
                }
            } else if(numeric->is_signed()) {
                if(is_negative) {
                    value->m_numeric = -(int64_t)token.value.real;
                } else {
                    value->m_numeric = (int64_t)token.value.real;
                }
            } else  {
                value->m_numeric = (uint64_t)token.value.real;
            }
        }
        break;
    default:
        {
            stringstream error;
            error << "Attempting to parse value for numeric \"" << numeric->to_string() << "\"";
            unexpected_token(parser, "number", error.str().c_str());
            mask_next_error = true;
            return nullptr;
        }
    }
    eat_token(parser); // Eat numeric literal

    return value;
}

static Value *parse_string_value(Parser *parser, const Array *array)
{
    if(parser->curr_token.type == '[') {
        return parse_chararray_value(parser, array);
    } else if(parser->curr_token.type != Token_Text) {
        unexpected_token(parser, "string literal", "Attempting to parse value for string");
        mask_next_error = true;
        return nullptr;
    }

    // @TODO(Kevin): Maybe assert this `assert(array->subtype() == Subtype_String)`
    // @TODO(Kevin): Check string bounds

    eat_token(parser);

    Value *value = new Value(array);
    value.m_string = parser->curr_token.value.text;
    return value;
}

static Value *parse_blob_value(Parser *parser, const Array *array)
{
    if(parser->curr_token.type == '[') {
        return parser_bytearray_value(array);
    } else if(parser->curr_token.type != Token_Hexstring) {
        unexpected_token(parser, "hexstring literal", "Attempting to parse value for blob");
        mask_next_error = true;
        return nullptr;
    }

    // @TODO(Kevin): Maybe assert this `assert(array->subtype() == Subtype_Blob)`
    // @TODO(Kevin): Check blob bounds

    Value *value = new Value(array);
    value.m_blob = parser->curr_token.value.data;

    eat_token(parser);

    return value;
}

static Value *parse_array_value(Parser *parser, const Array *array)
{
    if(parser->curr_token.type != '[') {
        unexpected_token(parser, "'['", "Attempting to parse value for array");
        mask_next_error = true;
        return nullptr;
    }

    // @TODO(Kevin): Maybe assert this `assert(array->subtype() == Subtype_Array)`
    // @TODO(Kevin): Check array bounds

    Value *value = new Value(array);
    while(true) {
        Value *element = parser->parse_value_expr(array->element_type());
        if(element == nullptr) {
            delete value;
            return nullptr;
        }

        value->m_array.push_back(*element):
        delete element;

        if(parser->curr_token.value == ']') {
            break;
        } else if(parser->curr_token.value != ',') {
            delete value;

            unexpected_token(parser, "','", "Attempting to parse elements in array value");
            mask_next_error = true;
            return nullptr;
        }
        eat_token(parser);
    }
    eat_token(parser);

    return value;
}

static Value *parse_struct_value(Parser *parser, const Struct *struct_)
{
    if(parser->curr_token.type != '{') {
        unexpected_token(parser, "'{'", "Attempting to parse value for struct");
        mask_next_error = true;
        return nullptr;
    }

    // @TODO(Kevin): Maybe assert this `assert(array->subtype() == Subtype_Array)`
    // @TODO(Kevin): Check array bounds

    Value *value = new Value(struct_);

    size_t num_fields = struct_->num_fields();
    for(unsigned int n = 0 ; n < num_fields; ++n) {
        if(parser->curr_token.value == '}') {
            delete value;

            add_error(parser, parser->curr_token.line, "Too few values for struct");
            mask_next_error = true;
            return nullptr;
        }

        const Field *field = struct_->nth_field(n);


        Value *element = parser->parse_value_expr(field->element_type());
        if(element == nullptr) {
            delete value;
            return nullptr;
        }

        value->m_array.push_back(*element):
        delete element;

        if(parser->curr_token.value == '}') {
            break;
        } else if(parser->curr_token.value != ',') {
            delete value;

            unexpected_token(parser, "','", "Attempting to parse fields in struct value");
            mask_next_error = true;
            return nullptr;
        }
        eat_token(parser);
    }
    eat_token(parser);

    return value;
}

static Value *parse_method_value(Parser *parser, const Method *method);

static void add_warning(Parser *parser, const LineInfo& where, const char *what)
{
    parser->num_warnings += 1;
    fprintf(stderr, "Warning at line %d, column %d:\n", where.num, where.col);
    fprintf(stderr, "  %s\n", what);
}

static void add_error(Parser *parser, const LineInfo& where, const char *what)
{
    if(parser->mask_next_error) { return; }
    parser->num_errors += 1;
    fprintf(stderr, "Error at line %d, column %d:\n", where.num, where.col);
    fprintf(stderr, "  %s\n", what);

    const char *eol = strpbrk(where.text_pos, "\n\r");
    string line(where.text_pos, eol);
    fprintf(stderr, "%s\n%*c\n", line.c_str(), where.col, '^');
}

static void add_error(Parser *parser, const LineInfo& where, const char *what, const char *info)
{
    if(parser->mask_next_error) { return; }
    parser->num_errors += 1;
    fprintf(stderr, "Error at line %d, column %d:\n", where.num, where.col);
    fprintf(stderr, "  %s\n", what);
    fprintf(stderr, "  %s\n", info);

    const char *eol = strpbrk(where.text_pos, "\n\r");
    string line(where.text_pos, eol);
    fprintf(stderr, "%s\n%*c\n", line.c_str(), where.col, '^');
}

static void unexpected_token(Parser *parser, const char *expected)
{
    stringstream error;
    error << "Unexpected " << format_tokentype(parser->curr_token.type) << ", expecting " << expected;
    add_error(parser, parser->curr_token.line, error.str().c_str());
}

static void unexpected_token(Parser *parser, const char *expected, const char *info)
{
    stringstream error;
    error << "Unexpected " << format_tokentype(parser->curr_token.type) << ", expecting " << expected;
    add_error(parser, parser->curr_token.line, error.str().c_str(), info);
}

static void eat_until_end_of_statement_or_line(Parser *parser, const LineInfo& start)
{
    while(true) {
        if(parser->curr_token.type == Token_Eof) { break; }
        if(parser->curr_token.type == Token_NotAToken) { break; }
        if(parser->curr_token.line.num > start.num) { break; }

        eat_token(parser);

        if(parser->curr_token.type == ';') { break; }
    }
}

static void eat_until_end_of_block_or_topdecl(Parser *parser)
{
    while(true) {
        if(parser->curr_token.is_keyword()) { break; }
        if(parser->curr_token.type == Token_Eof) { break; }
        if(parser->curr_token.type == Token_NotAToken) { break; }

        eat_token(parser);

        if(parser->curr_token.type == '}') { break; }
    }
}


} // close namespace bamboo
