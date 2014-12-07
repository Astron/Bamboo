// Filename: parser.cpp
#include "parser.h"
#include "lexer.h"
using namespace std;
using namespace bamboo::ast;
namespace bamboo {


DcFile *parse_dcfile(Lexer *lexer)
{
    Parser parser;
    parser.lexer = lexer;
    parser.start();
    return parser.parse_dcfile();
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

DcFile *Parser::parse_dcfile() {
    DcFile *dcfile = new DcFile();
    dcfile->where = curr_token.line;
    while(curr_token.type != Token_Eof && curr_token.type != Token_NotAToken) {
        Expression *expr = nullptr;
        switch(curr_token.type) {
        case Token_Keyword:
            expr = (Expression *)parse_keyword_decl();
            break;
        case Token_Typedef:
            expr = (Expression *)parse_typedef_decl();
            break;
        case Token_ImportFrom:
            expr = (Expression *)parse_import_decl();
            break;
        case Token_Struct:
            expr = (Expression *)parse_struct_defn();
            break;
        case Token_Class:
            expr = (Expression *)parse_class_defn();
            break;
        case ';':
            continue;
        default:
            if(curr_token.is_stray()) {
                format_error("Stray %s in dcfile", format_tokentype(curr_token.type));
            } else {
                error_expecting("top-level declaration");
            }
        }

        if(expr == nullptr) {
            delete dcfile;
            return nullptr;
        } else {
            dcfile.declarations.push_back(expr);
        }
    }

    return dcfile;
}

KeywordDecl *Parser::parse_keyword_decl()
{
    LineInfo expr_start = curr_token.line;
    if(curr_token.type != Token_Keyword) {
        error_expecting("keyword \"keyword\"", "Attempting to parse keyword declaration");
        return nullptr;
    }
    eat_token();

    // @NOTE(Kevin): Eventually I want to separate keywords with commas,
    // and end the keyword list with a semicolon.

    KeywordDecl *decl = new KeywordDecl();
    decl->where = expr_start;
    while(curr_token.type == Token_Identifier) {
        decl->keywords.push_back(curr_token.value.text);
        eat_token();
    }
    return decl;
}

TypedefDecl *Parser::parse_typedef_decl()
{
    LineInfo expr_start = curr_token.line;
    if(curr_token.type != TokenTypedef) {
        error_expecting("keyword \"typedef\"", "At the start of a typedef declaration");
        return nullptr;
    }
    eat_token();

    TypeExpr *type = parse_type_expr();
    if(type == nullptr) { return nullptr; }

    TypedefDecl *decl = new TypedefDecl();
    decl->base_type = type;
    if(curr_token.type == ';') {
        eat_token(this);
        return decl;
    } else if(curr_token.type != Token_Identifier) {
        error_expecting("identifier", "After type in typedef declaration");
        delete type;
        return nullptr;
    }

    decl->type_name = curr_token.value.text;
    eat_token();

    if(curr_token.type != ';') {
        error_at(expr_start, "Missing ';' after typedef declaration");
        delete decl;
        return nullptr;
    }
    eat_token();

    return decl;
}

ImportDecl *parse_import_decl()
{
    LineInfo expr_start = curr_token.line;
    if(curr_token.type != Token_From) {
        error_expecting("keyword \"from\"", "At the start of an import declaration");
        return nullptr;
    }
    eat_token();

    if(curr_token.type != Token_Identifier) {
        error_at(expr_start, "Missing module name after \"from\"");
        return nullptr;
    }

    string module_name;
    while(true) {
        if(curr_token.type == Token_Identifier) {
            module_name += curr_token.value.text;
        } else if(curr_token.type == '.') {
            module_name += '.';
        } else if(curr_token.type == '/') {
            module_name += '/';
        } else {
            break;
        }
        eat_token();
    }

    if(curr_token.type != Token_Import) {
        error_expecting("import", "Missing \"import\" after \"from <module>\" in import declaration");
        return nullptr;
    }
    eat_token();

    if(curr_token.type == '*') {
        eat_token();

        ImportDecl *decl = new ImportDecl();
        decl->where = expr_start;
        decl->modules.push_back(module_name);
        decl->all_symbols = true;
        return decl;
    } else if(curr_token.type != Token_Identifier) {
        error_at(prev_token.line, "Missing symbol names after \"import\" in import declaration");
        return nullptr;
    }

    ImportDecl *decl = new ImportDecl();
    decl->where = expr_start;
    decl->modules.push_back(module_name);

    string curr_symbol;
    int last_line = curr_token.line.num;
    while(true) {
        if(curr_token.type == Token_Identifier) {
            base_symbol += curr_token.value.text;
        } else if(curr_token.type == '/') {
            if(base_symbol.empty()) {
                curr_symbol += '/';
            }
        } else if(curr_token.type == ',') {
            if(curr_symbol.empty()) {
                warning("Missing symbol name before ','");
            } else {
                decl->symbol_names.push_back(curr_symbol);
                curr_symbol.clear();
            }
        } else if(curr_token.type == ';') {
            if(curr_symbol.empty()) {
                warning("Missing symbol name before ';'");
            } else {
                decl->symbol_names.push_back(curr_symbol);
                curr_symbol.clear();
            }
            break;
        } else if(curr_token.line.num > last_line) {
            if(!curr_symbol.empty()) {
                decl->symbol_names.push_back(curr_symbol);
                curr_symbol.clear();
            }
            break;
        } else {
            if(curr_symbol.empty()) {
                error_expecting("symbol name", "In list of imported symbols");
                delete decl;
                return nullptr;
            } else {
                error_expecting("',' or ';'", "In list of imported symbols");
                delete decl;
                return nullptr;                
            }
        }
        last_line = curr_token.line.num;
        eat_token();
    }

    return decl;
}

StructDefn *parse_struct_defn()
{
    LineInfo expr_start = curr_token.line;
    if(curr_token.type != Token_Struct) {
        error_expecting("keyword \"struct\"", "At beginning of struct definition");
        return nullptr;
    }
    eat_token();

    string struct_name;
    if(curr_token.type == Token_Identifier) {
        struct_name = curr_token.value.text;
        eat_token();
    }

    if(curr_token.type == ':') {
        error_expecting("'{'", "Structs cannot inherit from other types");
        return nullptr;
    } else if(curr_token.type == ';') {
        error_formatted("Missing body for \"struct %s\"", struct_name.c_str());
        return nullptr;
    } else if(curr_token.type != '{') {
        if(struct_name.empty()) {
            error_expecting("struct name", "After keyword \"struct\" in struct definition");
            return nullptr;
        } else {
            error_expecting("'{'", "After struct name in struct definition");
            return nullptr;
        }
    }
    eat_token();

    StructDefn *defn = new StructDefn();
    defn->where = expr_start;
    defn->name = struct_name;

    while(curr_token.type != '}') {
        if(curr_token.type == Token_Eof) {
            error("Missing closing brace '}' at end of struct definition");
            delete defn;
            return nullptr;
        }

        FieldExpr *field_expr = parse_field_expr();
        if(field_expr == nullptr) {
            return nullptr;
        } else {
            defn->fields.push_back(field_expr);
        }

        // Consume any extra semi-colons before the next field
        while(curr_token.type == ';') { eat_token(this); }
    }
    eat_token(); // Eat '}'

    return defn;
}

ClassDefn *Parser::parse_class_defn()
{
    LineInfo expr_start = curr_token.line;
    if(curr_token.type != Token_Struct) {
        error_expecting("keyword \"struct\"", "At beginning of class definition");
        return nullptr;
    }
    eat_token();

    string class_name;
    if(curr_token.type == Token_Identifier) {
        class_name = curr_token.value.text;
        eat_token();
    }

    ClassDefn *defn = new ClassDefn();
    defn->where = expr_start;
    defn->name = class_name;

    if(curr_token.type == ':') {
        eat_token();
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

TokenType Parser::eat_token()
{
    if(curr_token.type == Token_Eof) {
        error("Tried to parse past EOF");
    } else {
        prev_token.destroy();
        prev_token = curr_token;
        curr_token = next_token;
        next_token = lexer->scan_token();
    }
}

UnaryType Parser::eat_unary()
{
    if(curr_token.type == '+') {
        return Unary_Plus;
    } else if(curr_token.type == '-') {
        return Unary_Minus;
    } else {
        return Unary_None;
    }
}


} // close namespace bamboo
