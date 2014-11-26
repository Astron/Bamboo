// Filename: parser.h
#pragma once
#include "../dcfile/token.h"
namespace bamboo
{


struct Lexer;

struct NumericRange;
struct Import;
class Module;
class Type;
class Struct;
class Class;
class Field;
class Method;
class Parameter;
class Value;


bool load_module(const char *filename, Module *module);
bool parse_module(const char *buf, size_t bufsize, Module *module);
Module *load_module(const char *filename);
Module *parse_module(const char *buf, size_t bufsize);

// A Parser consumes tokens from the Lexer and attempts to parse any
// requested production, printing errors out as they occur.
//
// When parsing an entire module, an existing module may be provided and new
// items will be added to the existing module.
//
// Currently, this parser is guaranteed to leave a user-provided module
// in a consistent state when an error occurs; although some keywords,
// complete classes and structs, typedefs, or imports may have been added.
struct Parser
{
    Parser() = default;
    Parser(Lexer *lexer);
    ~Parser();
    void start();

    Lexer *lexer = nullptr;

    Module *parse_module();
    Module *parse_module(Module *module);
    Struct *parse_struct();
    Class *parse_class();
    Import *parse_import();
    bool parse_keywords_into_module(Module *module);
    bool parse_typedef_into_module(Module *module);

    Field *parse_struct_field();
    Field *parse_class_field(Class *);
    bool parse_keywords_for_field(Field *field);

    Type *parse_typedef(bool& caller_owns_return);
    Type *parse_type_expr(bool& caller_owns_return);
    NumericRange parse_range_expr();
    NumericRange parse_array_expr();

    Value *parse_value_expr(const Type *type);

    // Parser internals -- use with caution
    Module *module = nullptr;
    bool parser_owns_module = false;
    int num_errors = 0;
    int num_warnings = 0;
    Token curr_token;
    Token next_token;
    Token prev_token;

    // TODO(Kevin): Maybe have the individual parse methods return a parser result with
    // results for "success", "error but finished expression", "error and could not recover"
    // so that we can avoid masking valid errors and produce fewer meaningless errors.
    bool mask_next_error = false;
};


} // close namespace bamboo
