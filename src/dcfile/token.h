// Filename: token.h
#pragma once
#include <cstdio>
#include <cstdint>
#include <cstdlib>
namespace bamboo {


struct LineInfo
{
    int num = 0, col = 0;
    const char *text_pos = nullptr;
};

enum UnaryType {
    Unary_None = 0,
    Unary_Plus,
    Unary_Minus,
};

enum TokenType
{
    Token_NotAToken = -2,
    Token_Eof = -1,

    // Operators
    Token_AssignDefault = '=',
    Token_ComposeFrom = ':',
    Token_MemberOf = '.',
    Token_Positive = '+',
    Token_Negative = '-',
    Token_Divisor = '/',
    Token_Modulus = '%',

    // Delimiters
    Token_EndStatement = ';',
    Token_Sequence = ',',
    Token_CurlyLeft = '{',
    Token_CurlyRight = '}',
    Token_ParenLeft = '(',
    Token_ParenRight = ')',
    Token_SquareLeft = '[',
    Token_SquareRight = ']',

    // 0 to 255 are raw-ascii tokens

    Token_Identifier = 256,

    // Keywords
    Token_Class,
    Token_Struct,
    Token_Typedef,
    Token_Keyword,
    Token_Import,
    Token_ImportFrom,

    // Types
    Token_Byte,
    Token_Char,
    Token_Int8,
    Token_Int16,
    Token_Int32,
    Token_Int64,
    Token_Uint8,
    Token_Uint16,
    Token_Uint32,
    Token_Uint64,
    Token_Float32,
    Token_Float64,
    Token_String,
    Token_Blob,

    // Literals
    Token_Character,
    Token_Integer,
    Token_Real,
    Token_Text,
    Token_Hexstring
};

const char *format_tokentype(TokenType); // implemented in lexer.cpp

struct Token
{
    TokenType type = Token_NotAToken;
    LineInfo line;

    unsigned int size = 0;
    union {
        uint8_t *data = nullptr;
        char *text;

        double real;
        uint64_t integer;
        char character;
    } value;

    void destroy()
    {
        if(type == Token_Text || type == Token_Identifier) {
            if(value.text != nullptr) {
                free(value.text);
                value.text = nullptr;
            }
        } else if(type == Token_Hexstring) {
            if(value.data != nullptr) {
                free(value.data);
                value.data = nullptr;
            }
        }
    }

    bool is_special()
    {
        return type == Token_NotAToken || type == Token_Eof || type == Token_Identifier;
    }

    bool is_keyword()
    {
        return Token_Class <= type && type <= Token_Import;
    }

    bool is_type()
    {
        return Token_Char <= type && type <= Token_Blob;
    }

    bool is_literal()
    {
        return Token_Character <= type && type <= Token_Hexstring;
    }

    bool is_operator()
    {
        return type == (TokenType)'+'
            || type == (TokenType)'-'
            || type == (TokenType)'='
            || type == (TokenType)'.'
            || type == (TokenType)'/'
            || type == (TokenType)':';
    }

    bool is_delimiter()
    {
        return type == (TokenType)';'
            || type == (TokenType)','
            || type == (TokenType)'{'
            || type == (TokenType)'}'
            || type == (TokenType)'('
            || type == (TokenType)')'
            || type == (TokenType)'['
            || type == (TokenType)']';
    }

    bool is_stray()
    {
        return 0 <= type && type <= 255
            && !is_operator()
            && !is_delimiter();
    }
};


} // close namespace bamboo
