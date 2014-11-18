// Filename: parser.cpp
#include "parser.h"
#include <cstring>
#include <array>
#include <sstream> // @FUTURE: Benchmark uses of this for performance
using namespace std;

#include "../dcfile/lexer.h"
#include "../module/module"
namespace bamboo
{


static const array<Type *, 13> token_to_primitive = {
    Type::Char, Type::Int8, Type::Int16, Type::Int32, Type::Int64,
    Type::Uint8, Type::Uint16, Type::Uint32, Type::Uint64,
    Type::Float32, Type::Float64, Type::String, Type::Blob
};

static const array<Subtype, 13> token_to_subtype = {
    kTypeChar, kTypeInt8, kTypeInt16, kTypeInt32, kTypeInt64,
    kTypeUint8, kTypeUint16, kTypeUint32, kTypeUint64,
    kTypeFloat32, kTypeFloat64, kTypeString, kTypeBlob
};

static Type *to_primitive(TokenType type) { return token_to_primitive[type - Token_Char]; }
static Subtype to_subtype(TokenType type) { return token_to_subtype[type - Token_Char]; }

static void add_warning(Parser *parser, const LineInfo& where, const char *what);
static void add_error(Parser *parser, const LineInfo& where, const char *what);
static void add_error(Parser *parser, const LineInfo& where, const char *what, const char *info);
static void eat_token(Parser *parser);
static void eat_until_end_of_statement_or_line(Parser *parser, const LineInfo& start);
static void eat_until_end_of_block_or_topdecl(Parser *parser);
static void eat_unary(Parser *parser);

Parser::Parser(Lexer *lexer) : lexer(lexer) {}
Parser::Parser(Lexer *lexer, Module *module) : lexer(lexer), module(module) {}

void Parser::start()
{
    eat_token(this);
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
                    module->add_struct(struct_, true);
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
                    module->add_class(class_, true);
                    mask_next_error = false;
                }
            }
            break;
        case Token_Typedef:
            {
                bool caller_owns_return;
                Type *type = parse_typedef(caller_owns_return);
                if(type == nullptr) {
                    eat_until_end_of_statement_or_line(this, curr_token.line);
                } else {
                    module->add_typedef(type->alias(), type, caller_owns_return);
                }
            }
            break;
        case Token_Keyword:
            {
                const char *keyword = parse_keyword_decl();
                if(keyword == nullptr) {
                    eat_until_end_of_statement_or_line(this, curr_token.line);
                } else {
                    module->add_keyword(keyword);
                }
            }
            break;
        case Token_Import:
        case Token_ImportFrom:
            {

            }
            parse_import();
            break;
        case ';':
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
        stringstream error;
        error << "Unexpected " << format_tokentype(curr_token.type)
              << ", expecting keyword \"struct\"";
        add_error(this, curr_token.line, error.str().c_str(),
                  "Attempting to parse struct definition");
        return nullptr;
    }
    eat_token(this);

    string struct_name;
    if(curr_token.type == (TokenType)'{') {
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
    if(curr_token.type == (TokenType)':') {
        add_error(this, curr_token.line,
                  "Unexpected ':', expecting '{'",
                  "Structs cannot inherit from other types");
        error_occured = true;
        eat_token(this);
        while(true) {
            if(curr_token.type == (TokenType)';' ||
               curr_token.type == Token_Identifier) {
                eat_token(this);
            } else {
                break;
            }
        }
    }

    // Parse struct body
    if(curr_token.type != (TokenType)'{') {
        stringstream error, info;
        error << "Unexpected " << format_tokentype(curr_token.type) << ", expecting '{'";
        info << "Missing struct body for \"struct " << struct_name << '"';
        add_error(this, curr_token.line, error.str().c_str(), info.str().c_str());
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
            if(curr_token.type == (TokenType)'}') { break; }

            eat_until_end_of_statement_or_line(this, start);
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
        while(curr_token.type == (TokenType)';') { eat_token(this); }
    }
    if(curr_token.type == Token_Eof) {
        add_error(this, start, "Missing closing brace '}' at end of struct definition");
        for(Field *field : fields) {
            delete field;
        }
        return nullptr;
    }
    eat_token(this);

    if(curr_token.type == (TokenType)';') {
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
    for(Field *field : fields) { struct_->register_field(unique_ptr<Field>(field)); }
    return struct_;
}

// if(Error) { Recover: Scan to end of '}' or next top-level keyword. }
Class *Parser::parse_class()
{
    bool error_occured = false;
    LineInfo start = curr_token.line;
    if(curr_token.type != Token_Class) {
        stringstream error;
        error << "Unexpected " << format_tokentype(curr_token.type)
              << ", expecting keyword \"class\"";
        add_error(this, curr_token.line, error.str().c_str(),
                  "Attempting to parse class definition");
        return nullptr;
    }
    eat_token(this);

    string class_name;
    if(curr_token.type == (TokenType)':') {
        add_error(this, curr_token.line,
                  "Unexpected ':', expecting identifier",
                  "Missing class name before list of base classes");
        error_occured = true;
    } else if(curr_token.type == (TokenType)'{') {
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
    vector<Class *> parents;
    if(curr_token.type == Token_Identifier) {
        add_error(this, curr_token.line,
                  "Unexpected identifier, expecting ':' or '{'",
                  "Must use ':' before listing base classes");
        mask_next_error = true;
        return nullptr;
    } else if(curr_token.type == (TokenType)':') {
        eat_token(this);
        if(curr_token.type == (TokenType)'{') {
            add_error(this, curr_token.line, "No base classes listed after ':'");
            error_occured = true;
        } else {
            while(true) {
                if(curr_token.type == (TokenType)',') {
                    add_error(this, curr_token.line, "Missing base class name before ','");
                    error_occured = true;
                    eat_token(this);
                    continue;
                } else if(curr_token.type == (TokenType)'{') {
                    add_error(this, curr_token.line, "Missing base class name before '{'");
                    error_occured = true;
                    break;
                } else if(curr_token.type != Token_Identifier) {
                    stringstream error;
                    error << "Unexpected " << format_tokentype(curr_token.type)
                          << ", expecting identifier";
                    add_error(this, curr_token.line, error.str().c_str());
                    return nullptr;
                }

                const char *name = nullptr;
                Type *base_type = nullptr;
                Struct *base_struct = nullptr;
                Class *base_class = nullptr;

                // If the parser doesn't have a module, all other class names are undeclared
                if(module != nullptr) {
                    stringstream error;
                    error << "Type \"class " << name << "\" has not been declared";
                    add_error(this, curr_token.line, error.str().c_str());
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
                if(curr_token.type == (TokenType)',') {
                    eat_token(this); // Eat the comma
                } else {
                    break;
                }
            }
        }
    }

    // Parse Class Body
    if(curr_token.type != (TokenType)'{') {
        stringstream error, info;
        error << "Unexpected " << format_tokentype(curr_token.type) << ", expecting '{'";
        info << "Missing class body for \"class " << class_name << "\"";
        add_error(this, curr_token.line, error.str().c_str(), info.str().c_str());
        mask_next_error = true;
        return nullptr;
    }
    eat_token(this);

    vector<Field *> fields;
    unordered_map<string, LineInfo> field_names;
    while(curr_token.type != '}' && curr_token.type != Token_Eof) {
        LineInfo field_start = curr_token.line;
        Field *field = parse_class_field();
        if(field == nullptr) {
            error_occured = true;
            if(curr_token.type == Token_Eof) { break; }
            if(curr_token.type == (TokenType)'}') { break; }

            eat_until_end_of_statement_or_line(this, start);
            mask_next_error = true;
        } else {
            fields.push_back(field);
            mask_next_error = false;

            if(field->name().empty()) {
                add_error(this, field_start, "Fields in classes must have names");
                error_occured = true;
            } else if(field_names.find(field->name()) != field_names.end()) {
                stringstream error, info;
                LineInfo prev = field_names[field->name()];
                error << "A field with the name \"" << field->name()
                      << "\" was already defined in \"class " << class_name << '\"';
                info << "Previous definition at line " << prev.num << ", column " << prev.col;

                add_error(this, field_start, error.str().c_str(), info.str().c_str());
                error_occured = true;
            } else {
                field_names.emplace(field->name(), field_start);
            }
        }

        // Consume any extra semi-colons before the next field
        while(curr_token.type == (TokenType)';') { eat_token(this); }
    }
    if(curr_token.type == Token_Eof) {
        add_error(this, start, "Missing closing brace '}' at end of class definition");
        for(Field *field : fields) {
            delete field;
        }
        return nullptr;
    }
    eat_token(this);

    if(curr_token.type == (TokenType)';') {
        eat_token(this);
        // Semicolons are a good point to begin unmasking errors, and this
        // one is likely to be ours because it is after a closing brace
        mask_next_error = false;
    } else {
        add_error(this, start, "Missing ';' after struct definition");
        for(Field *field : fields) {
            delete field;
        }
        return nullptr;
    }

    if(error_occured) {
        for(Field *field : fields) {
            delete field;
        }
        return nullptr;
    }

    // Actually construct the class.  If anything fails here, it is
    // an internal parser error and needs to be caught up above.
    Class *class_ = new Class(class_name);
    for(Class *parent : parents) { class_->add_parent(parent); }
    for(Field *field : fields) { class_->register_field(unique_ptr<Field>(field)); }
    return class_;
}

Import *parse_import()
{
    // TODO: Implement
}

bool parse_keyword_decl()
{
    // TODO: Implement
}

// if(Error) { Recover: Scan to end of line or first ';' }
Field *Parser::parse_struct_field()
{
    if(curr_token.type == Token_Identifier) {
        if(module->type_by_name(curr_token.value.text) == nullptr) {
            if(next_token.type == (TokenType)'(') {
                // We think this is supposed to look like a method
                stringstream error;
                error << "Cannot find type \"" << curr_token.value.text << "\"";
                add_error(this, curr_token.line, error.str().c_str(),
                          "Field looks like a method, but methods cannot be defined in a struct");
                return nullptr;
            } else if(next_token.type == (TokenType)':') {
                // We think this is supposed to look like a molecular field
                stringstream error;
                error << "Cannot find type \"" << curr_token.value.text << "\"";
                add_error(this, curr_token.line, error.str().c_str(),
                          "Field looks like a molecular, but molecular cannot be defined in a struct");
                return nullptr;
            }
        }
    } else if(!curr_token.is_type()) {
        stringstream error;
        error << "Unexpected " << format_tokentype(curr_token.type) << ", expecting a type name";
        add_error(this, curr_token.line, error.str().c_str(),
                  "Attempting to parse fields in a struct");
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
    if(curr_token.type == (TokenType)'=') {
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


    if(curr_token.type != (TokenType)';') {
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
    }

    return field;
}

Field *parse_class_field()
{
    // TODO: Implement
}

Type *Parser::parse_type_expr(bool& caller_owns_return)
{
    Type* type;
    if(curr_token.type == Token_Identifier) {
        type = module->type_by_name(curr_token.value.text);
        if(type == nullptr) {
            return nullptr;
        }

        caller_owns_return = false;
    } else if(curr_token.is_type()) {
        bool is_primitive = (next_token.type != (TokenType)'(' &&
                             next_token.type != (TokenType)'/' &&
                             next_token.type != (TokenType)'%');
        if(is_primitive) {
            type = to_primitive(curr_token.type);
            caller_owns_return = false;
        } else if(curr_token.type == Token_String) {
            type = Type::String;
            caller_owns_return = false;
        } else if(curr_token.type == Token_Blob) {
            type = Type::Blob;
            caller_owns_return = false;
        } else {
            type = new Numeric(to_subtype(curr_token.type));
            caller_owns_return = true;
        }
    } else {
        stringstream error;
        error << "Unexpected " << format_tokentype(curr_token.type) << ", expecting a type name";
        add_error(this, curr_token.line, error.str().c_str());
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

                if(curr_token.type == (TokenType)'-') {
                    add_error(this, curr_token.line,
                              "Unexpected unary operator '-' before divisor",
                              "Divisor must be unsigned integer");
                    eat_token(this);
                } else if(curr_token.type == (TokenType)'+') {
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
                bool is_negative = prev_token.type == (TokenType)'-';
                if(curr_token.type == Token_Real) {
                    modulus = curr_token.value.real;
                } else if(curr_token.type != Token_Integer) {
                    modulus = (double)curr_token.value.integer;
                } else {
                    stringstream error;
                    error << "Unexpected " << format_tokentype(curr_token.type)
                          << ", expecting number";
                    add_error(this, curr_token.line, error.str().c_str(),
                              "Attempting to parse modulus for type");
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
                if(type->subtype() == kTypeString || type->subtype() == kTypeBlob) {
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
                    } else if(type->has_alias()) {
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
                } else if(type->subtype() == kTypeString) {
                    if(range.type != Number::kUint) {
                        add_error(this, curr_token.line,
                                  "Range for string-like type must be unsigned integer");
                    }

                    type = new Array(Type::Char, false, range);
                    caller_owns_return = true;
                } else if(type->subtype() == kTypeBlob) {
                    if(range.type != Number::kUint) {
                        add_error(this, curr_token.line,
                                  "Range for blob-like type must be unsigned integer");
                    }

                    type = new Array(Type::Uint8, false, range);
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
                if(next_token.type == (TokenType)']') {
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
    if(curr_token.type != '(') {
        stringstream error;
        error << "Unexpected " << format_tokentype(curr_token.type) << ", expecting '('";
        add_error(this, curr_token.line, error.str().c_str(),
                  "Attempting to parse range for numeric, string, or blob");
        return NumericRange();
    }
    eat_token(this);

    Number lhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        if(prev_token.type == (TokenType)'-') {
            lhs = -(int64_t)curr_token.value.integer;
        } else if(prev_token.type == (TokenType)'+') {
            lhs = (int64_t)curr_token.value.integer;
        } else {
            lhs = (uint64_t)curr_token.value.integer;
        }
    } else if(curr_token.type == Token_Real) {
        if(prev_token.type == (TokenType)'-') {
            lhs = -curr_token.value.real;
        } else if(prev_token.type == (TokenType)'+') {
            lhs = (int64_t)curr_token.value.real;
        } else {
            lhs = curr_token.value.real;
        }
    } else if(curr_token.type == (TokenType)')') {
        // TODO: Custom error
        mask_next_error = true;
        return NumericRange();
    } else {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }

    LineInfo lhs_line = curr_token.line;
    eat_token(this);

    // Range can be ( A ) or ( A , B )
    if(curr_token.type == (TokenType)')') {
        eat_token(this);
        return NumericRange(lhs);
    } else if(curr_token.type == Token_Integer || curr_token.type == Token_Real) {
        // TODO: Custom error
        mask_next_error = true;
        return NumericRange();
    } else if(curr_token.type != ',') {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);

    Number rhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        if(prev_token.type == (TokenType)'-') {
            rhs = -(int64_t)curr_token.value.integer;
        } else if(prev_token.type == (TokenType)'+') {
            rhs = (int64_t)curr_token.value.integer;
        } else {
            rhs = (uint64_t)curr_token.value.integer;
        }
    } else if(curr_token.type == Token_Real) {
        if(prev_token.type == (TokenType)'-') {
            rhs = -curr_token.value.real;
        } else if(prev_token.type == (TokenType)'+') {
            rhs = (int64_t)curr_token.value.real;
        } else {
            rhs = curr_token.value.real;
        }
    } else if(curr_token.type == (TokenType)')') {
        // TODO: Custom error
        mask_next_error = true;
        return NumericRange();
    } else {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }

    LineInfo rhs_line = curr_token.line;
    eat_token(this);

    // Close expression
    if(curr_token.type != ')') {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);;

    // Actually build the numeric range
    // TODO: Generate error if (lhs > rhs)
    if(lhs.type == Number::kFloat || rhs.type == Number::kFloat) {
        return NumericRange((double)lhs, (double)rhs);
    } else if(lhs.type == Number::kInt || rhs.type == Number::kInt) {
        // Check left integer overflow
        if(lhs.type != Number::kInt) {
            int64_t lhs_s64 = (int64_t)lhs;
            if(lhs.uinteger > 0 && !(lhs_s64 > 0)) {
                add_warning(this, lhs_line,
                            "Unsigned integer overflow in conversion to signed range");
            }
        }

        // Check right integer overflow
        if(rhs.type != Number::kInt) {
            int64_t rhs_s64 = (int64_t)rhs;
            if(rhs.uinteger > 0 && !(rhs_s64 > 0)) {
                add_warning(this, rhs_line,
                            "Unsigned integer overflow in conversion to signed range");
            }
        }

        return NumericRange((int64_t)lhs, (int64_t)rhs);
    } else {
        return NumericRange(lhs.uinteger, rhs.uinteger);
    }
}

NumericRange Parser::parse_array_expr()
{
    if(curr_token.type != '[') {
        stringstream error;
        error << "Unexpected " << format_tokentype(curr_token.type) << ", expecting '['";
        add_error(this, curr_token.line, error.str().c_str(),
                  "Attempting to parse array size/range");
        return NumericRange();
    }
    eat_token(this);

    Number lhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        lhs = (uint64_t)curr_token.value.integer;
    } else if(curr_token.type == Token_Real) {
        lhs = Number(); // NaN signals error
        return NumericRange();
    } else if(curr_token.type == (TokenType)']') {
        // TODO: Custom error
        mask_next_error = true;
        return NumericRange();
    } else {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }

    if(prev_token.type == (TokenType)'-' ||
       prev_token.type == (TokenType)'+')
    {
        // TODO: ErrorButContinue
        lhs = Number(); // NaN signals error
    }
    eat_token(this);

    // Range can be ( A ) or ( A , B )
    if(curr_token.type == (TokenType)']') {
        eat_token(this);
        return NumericRange(lhs);
    } else if(curr_token.type == Token_Integer || curr_token.type == Token_Real) {
        // TODO: Custom error
        mask_next_error = true;
        return NumericRange();
    } else if(curr_token.type != ',') {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);

    Number rhs;
    eat_unary(this);
    if(curr_token.type == Token_Integer) {
        rhs = (uint64_t)curr_token.value.integer;
    } else if(curr_token.type == Token_Real) {
        // TODO: Custom error
        mask_next_error = true;
        return NumericRange();
    } else if(curr_token.type == (TokenType)']') {
        // TODO: Custom error
        mask_next_error = true;
        return NumericRange();
    } else {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }

    if(prev_token.type == (TokenType)'-' ||
       prev_token.type == (TokenType)'+')
    {
        // TODO: ErrorButContinue
        rhs = Number(); // NaN signals error
    }
    eat_token(this);

    if(curr_token.type != ']') {
        // TODO: Generic error
        mask_next_error = true;
        return NumericRange();
    }
    eat_token(this);;

    // Actually build the array range
    // TODO: Generate error if (lhs > rhs)
    if(lhs.type == Number::kNaN || rhs.type == Number::kNaN) {
        return NumericRange();
    } else {
        return NumericRange(lhs.uinteger, rhs.uinteger);
    }
}

Value *Parser::parse_value_expr(Type *type)
{
    // TODO: Implement
}

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

static void eat_token(Parser *parser)
{
    if(parser->curr_token.type == Token_Eof) {
        add_error(parser, parser->curr_token.line, "Internal-Parser-Error: Tried to eat EOF");
    } else {
        parser->prev_token = move(parser->curr_token);
        parser->curr_token = move(parser->next_token);
        parser->next_token = parser->lexer->scan_token();
    }
}

static void eat_unary(Parser *parser)
{
    if(parser->curr_token.type == (TokenType)'-' ||
       parser->curr_token.type == (TokenType)'+')
    {
        eat_token(parser);
    }
}

static void eat_until_end_of_statement_or_line(Parser *parser, const LineInfo& start)
{
    while(true) {
        if(parser->curr_token.type == (TokenType)';') { break; }
        if(parser->curr_token.type == Token_Eof) { break; }
        if(parser->curr_token.type == Token_NotAToken) { break; }
        if(parser->curr_token.line.num > start.num) { break; }
        eat_token(parser);
    }
}

static void eat_until_end_of_block_or_topdecl(Parser *parser)
{
    while(true) {
        if(parser->curr_token.type == (TokenType)'}') { break; }
        if(parser->curr_token.is_keyword()) { break; }
        if(parser->curr_token.type == Token_Eof) { break; }
        if(parser->curr_token.type == Token_NotAToken) { break; }
        eat_token(parser);
    }
}


} // close namespace bamboo
