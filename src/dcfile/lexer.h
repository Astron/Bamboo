// Filename: lexer.h
#pragma once
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
namespace bamboo
{


struct Lexer;
struct Token;
struct LineInfo;

struct LineInfo
{
    int num = 0;
    int col = 0;
    char *text_pos = nullptr;
};

struct Lexer
{
    /* Lexer(FILE *file); */
    Lexer(char *buf, size_t bufsize);

    Token scan_token();

    /* FILE *input_file = nullptr; */
    char *input_buf = nullptr;
    char *input_pos = nullptr;
    size_t input_size = 0;

    LineInfo curr_line;
};

enum TokenType
{
    Token_Eof = -1,

    // 0 to 255 are raw-ascii tokens

    Token_Identifier = 256,

    // Keywords
    Token_Class,
    Token_Struct,
    Token_Typedef,
    Token_Keyword,
    Token_From,
    Token_Import,

    // Types
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

    // Values
    Token_Character,
    Token_Integer,
    Token_Real,
    Token_Text,
    Token_Hexstring
};

struct Token
{
    TokenType type;
    LineInfo line;

    unsigned int size = 0;
    union {
        uint8_t *data = nullptr;
        char *text;

        double real;
        uint64_t integer;
        char character;
    } value;

    // Token is Movable but not copyable
    Token() = default;
    Token(Token&& other) = default;
    Token& operator=(Token&& other) = default;

/*
    Token(const Token& other);
    Token& operator=(const Token other);
*/

    ~Token()
    {
        if(type == Token_Text || type == Token_Identifier) {
            if(value.text != nullptr) free(value.text);
        } else if(type == Token_Hexstring) {
            if(value.data != nullptr) free(value.data);
        }
    }
};


} // close namespace bamboo
