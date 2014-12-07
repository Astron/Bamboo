// Filename: parser.h
#pragma once
#include "../dcfile/ast.h"
namespace bamboo
{


struct Lexer;

bool load_module(const char *filename, Module *module);
bool parse_module(const char *buf, size_t bufsize, Module *module);
Module *load_module(const char *filename);
Module *parse_module(const char *buf, size_t bufsize);

ast::DcFile *parse_dcfile(Lexer *);

struct Parser
{
    Lexer *lexer = nullptr;

    void start();

    ast::DcFile *parse_dcfile();
    ast::KeywordDecl *parse_keyword_decl();
    ast::TypedefDecl *parse_typedef_decl();
    ast::ImportDecl *parse_import_decl();
    ast::StructDefn *parse_struct_defn();
    ast::ClassDefn *parse_class_defn();
    ast::FieldExpr *parse_field_expr();
    ast::FieldDefn *parse_field_defn();
    ast::MethodDefn *parse_method_defn();
    ast::MoleculeDefn *parse_molecule_defn();
    ast::TypeExpr *parse_type_expr();
    ast::RangeExpr *parse_range_expr();
    ast::ValueExpr *parse_value_expr();

    TokenType eat_token();
    UnaryType eat_unary();

    void error(const char *error);
    void error_at(LineInfo line, const char *error);
    void error_expecting(const char *expected);
    void error_expecting(const char *expected, const char *info);
    void error_formatted(const char *fmt, ...);

    char error_msg[255];
    Token prev_token;
    Token curr_token;
    Token next_token;
};


} // close namespace bamboo
