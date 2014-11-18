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

struct Parser
{
    Parser(Lexer *lexer);
    Parser(Lexer *lexer, Module *module);
    void start();

    Module *parse_module();
    Struct *parse_struct();
    Class *parse_class();
    Import *parse_import();
    const char *parse_keyword_decl();

    Field *parse_struct_field();
    Field *parse_class_field();

    Type *parse_typedef(bool& caller_owns_return);
    Type *parse_type_expr(bool& caller_owns_return);
    NumericRange parse_range_expr();
    NumericRange parse_array_expr();

    Value *parse_value_expr(Type *type);

    Lexer *lexer = nullptr;
    Module *module = nullptr;
    bool parser_owns_module = false;

    /* Parser-specific internals -- use with caution */
    bool mask_next_error = false;
    int num_errors = 0;
    int num_warnings = 0;
    Token curr_token = Token(Token_NotAToken);
    Token next_token = Token(Token_NotAToken);
    Token prev_token = Token(Token_NotAToken);
};


} // close namespace bamboo
