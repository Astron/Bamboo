// Filename: ast.h
#pragma once
#include <string>
#include <vector>
#include <list>
#include "../bits/numbers.h"
#include "../dcfile/token.h"
namespace bamboo {
namespace ast {


// @TODO(Kevin): Add destructors/cleanup

struct Module;
struct KeywordDecl;
struct TypedefDecl;
struct ImportDecl;
struct StructDefn;
struct ClassDefn;
struct FieldExpr;
struct FieldDefn;
struct MethodDefn;
struct MoleculeDefn;
struct TypeExpr;
struct RangeExpr;
struct ValueExpr;
struct NumberValue;
struct StringValue;
struct HexstringValue;
struct ArrayValue;
struct StructValue;
struct MethodValue;

enum ExpressionType {
    Expr_Empty = 0,
    Expr_DcFile,
    Expr_KeywordDecl,
    Expr_TypedefDecl,
    Expr_ImportDecl,
    Expr_StructDefn,
    Expr_ClassDefn,
    Expr_FieldExpr,
    Expr_TypeExpr,
    Expr_RangeExpr,
    Expr_ValueExpr,
};

enum FieldType { 
    Field_None = 0,
    Field_Field,
    Field_Method,
    Field_Molecule,
};

enum ValueType {
    Value_None = 0,
    Value_Number,
    Value_String,
    Value_Hexstring,
    Value_Array,
    Value_Struct,
    Value_Method,
};

struct Expression
{
    Expression(ExpressionType type) : expr_type(type) {}
    virtual ~Expression() {}
    ExpressionType expr_type;
    LineInfo where;
};

struct RangeExpr : public Expression
{
    RangeExpr() : Expression(Expr_RangeExpr) {}
    virtual ~RangeExpr() {}
    UnaryType min_prefix;
    UnaryType max_prefix;
    Number min;
    Number max;
};

struct TypeExpr : public Expression
{
    TypeExpr() : Expression(Expr_TypeExpr) {}
    virtual ~TypeExpr() {}
    std::string type_name;
    unsigned int divisor;
    double modulus;
    RangeExpr simple_range;
    RangeExpr array_range;
};

struct ValueExpr : public Expression
{
    ValueExpr(ValueType type) : Expression(Expr_ValueExpr), value_type(type) {}
    virtual ~ValueExpr() {}
    ValueType value_type;
};

struct NumberValue : public ValueExpr
{
    NumberValue() : ValueExpr(Value_Number) {}
    virtual ~NumberValue() {}
    UnaryType unary_prefix;
    Number number;
};

struct StringValue : public ValueExpr
{
    StringValue() : ValueExpr(Value_String) {}
    virtual ~StringValue() {}
    std::string string;
};

struct HexstringValue : public ValueExpr
{
    HexstringValue() : ValueExpr(Value_Hexstring) {}
    virtual ~HexstringValue() {}
    std::vector<uint8_t> hexstring;
};

struct ArrayValue : public ValueExpr
{
    ArrayValue() : ValueExpr(Value_Array) {}
    virtual ~ArrayValue() { for(auto val : elements) { delete val; } }
    std::list<ValueExpr *> elements;
};

struct StructValue : public ValueExpr
{
    StructValue() : ValueExpr(Value_Struct) {}
    virtual ~StructValue() { for(auto val : fields) { delete val; } }
    std::list<ValueExpr *> fields;
};

struct MethodValue : public ValueExpr
{
    MethodValue() : ValueExpr(Value_Method) {}
    virtual ~MethodValue() { for(auto val : arguments) { delete val; } }
    std::list<ValueExpr *> arguments;
};

struct FieldExpr : public Expression
{
    FieldExpr(FieldType type) : Expression(Expr_FieldExpr), field_type(type) {}
    virtual ~FieldExpr() {}
    FieldType field_type;
    std::string name;
    std::list<std::string> keywords;
};

struct FieldDefn : public FieldExpr
{
    FieldDefn() : FieldExpr(Field_Field) {}
    virtual ~FieldDefn()
    {
        if(value_type) { delete value_type; }
        if(default_value) { delete default_value; }
    }

    TypeExpr *value_type = nullptr;
    ValueExpr *default_value = nullptr;
};

struct MethodDefn : public FieldExpr
{
    MethodDefn() : FieldExpr(Field_Method) {}
    virtual ~MethodDefn() { for(auto param : parameters) { delete param; } }
    std::list<Parameter *> parameters;
};

struct MoleculeDefn : public FieldExpr
{
    MoleculeDefn() : FieldExpr(Field_Molecule) {}
    virtual ~MoleculeDefn() {}
    std::list<std::string> atomics;
};

struct KeywordDecl : public Expression
{
    KeywordDecl() : Expression(Expr_KeywordDecl) {}
    virtual ~KeywordDecl() {}
    std::list<std::string> keywords;
};

struct TypedefDecl : public Expression
{
    TypedefDecl() : Expression(Expr_TypedefDecl) {}
    virtual ~TypedefDecl() { if(base_type) { delete base_type; } }
    TypeExpr *base_type = nullptr;
    std::string type_name;
};

struct ImportDecl : public Expression
{
    ImportDecl() : Expression(Expr_ImportDecl) {}
    virtual ~ImportDecl() {}
    bool import_all = false;
    std::string module_name;
    std::list<std::string> symbol_names;
};

struct StructDefn : public Expression
{
    StructDefn() : Expression(Expr_StructDefn) {}
    virtual ~StructDefn() { for(auto fld : fields) { delete fld; } }
    std::string name;
    std::list<FieldExpr *> fields;
};

struct ClassDefn : public Expression
{
    ClassDefn() : Expression(Expr_ClassDefn) {}
    virtual ~ClassDefn() { for(auto fld : fields) { delete fld; } }
    std::string name;
    std::list<std::string> parents;
    std::list<FieldExpr *> fields;
};

struct DcFile : public Expression
{
    DcFile() : Expression(Expr_DcFile) {}
    virtual ~DcFile()
    {
        if(input) { delete input; }
        for(auto decl : declarations) { delete decl; }
    }

    char *input = nullptr;
    std::list<Expression *> declarations;
};


} // close namespace ast
} // close namespace bamboo
