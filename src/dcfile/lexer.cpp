// Filename: lexer.cpp
#include "lexer.h"
#include <cctype>
#include <cstring>
#include <array>
#include <string>
#include <vector>
using namespace std;
namespace bamboo
{


static const array<string, 19> keywords = {
    "keyword", "struct", "class", "typedef", "from", "import",
    "char", "int8", "int16", "int32", "int64",
    "uint8", "uint16", "uint32", "uint64",
    "float32", "float64", "string", "blob"
};

static const array<TokenType, 19> keyword_types = {
    Token_Keyword, Token_Struct, Token_Class, Token_Typedef, Token_From, Token_Import,
    Token_Char, Token_Int8, Token_Int16, Token_Int32, Token_Int64,
    Token_Uint8, Token_Uint16, Token_Uint32, Token_Uint64,
    Token_Float32, Token_Float64, Token_String, Token_Blob
};

static char next_char(Lexer *lexer);
static char peek_char(Lexer *lexer);
static void eat_char(Lexer *lexer);
static void set_token_data(Token& token, const vector<uint8_t>& data);
static void set_token_text(Token& token, const string& text);
static string scan_quoted_text(Lexer *lexer);

Lexer::Lexer(char *buf, size_t bufsize) :
    input_buf(buf),
    input_pos(buf),
    input_size(bufsize)
{
    curr_line.text_pos = input_pos;
}

Token Lexer::scan_token()
{
    char c = peek_char(this);
    while(isspace(c)) { c = next_char(this); }

    LineInfo where = this->curr_line;

    if(isalpha(c)) {
        // Scan identifier

        std::string name;
        char c = peek_char(this);
        while(isalnum(c) || c == '_') {
            name += c;
            c = next_char(this);
        }

        for(unsigned int i = 0; i < keywords.size(); ++i) {
            if(name == keywords[i]) {
                Token token;
                token.line = where;
                token.type = keyword_types[i];
                set_token_text(token, name);;
                return token;
            }
        }

        Token token;
        token.line = where;
        token.type = Token_Identifier;
        set_token_text(token, name);;
        return token;
    } else if(isdigit(c) || c == '.') {
        // Scan number literal

        char *start = input_pos;

        bool has_decimal_point = false;
        bool has_zero_prefix = false;
        bool has_hex_prefix = false;
        bool has_bin_prefix = false;
        bool has_exponent = false;

        if(c == '.') {
            has_decimal_point = true;
        } else if(c == '0') {
            has_zero_prefix = true;
            c = next_char(this);

            if(c == 'x' || c == 'X') {
                has_hex_prefix = true;
                c = next_char(this);
            } else if(c == 'b' || c == 'B') {
                has_bin_prefix = true;
                c = next_char(this);
            }
        }

        // Read the entire number first to produce good error messages and eat junk characters.
        while(c != EOF) {
            if(c == '.') {
                if(has_decimal_point) { /* TODO: Error: Too many decimal points in number */ }
                else if(has_bin_prefix) { /* TODO: Error: Invalid prefix "0b" for float literal */ }
                has_decimal_point = true;
            } else if(c == 'e' || c == 'E') {
                if(has_exponent) { /* TODO: Error: Too many exponents in number */ }
                else if(!has_bin_prefix) { /* TODO: Error: Invalid prefix "0b" for float literal */ }
                has_exponent = true;

                c = next_char(this);
                if(c == '-' || c == '+') { c = next_char(this); }
                continue;
            } else if(has_hex_prefix) {
                if(!isalnum(c)) { break; }
                if(!isxdigit(c)) { /* TODO: Error: Forbidden character at end of numeric literal */ }
            } else {
                if(!isalnum(c)) { break; }
                if(!isdigit(c)) { /* TODO: Error: Forbidden character at end of numeric literal */ }
                if(has_bin_prefix) {
                    if(c != '0' && c != '1') { /* TODO: Error: Invalid digit in binary literal */ }
                } else if(has_zero_prefix) {
                    if(c == '8' || c == '9') { /* TODO: Error: Invalid digit in octal literal */ }
                }
            }
            c = next_char(this);
        }

        // Parse the string as a numeric literal
        if(has_exponent || has_decimal_point) {
            Token token;
            token.line = where;
            token.type = Token_Real;
            token.value.real = atof(start);
            return token;
        } else if(has_bin_prefix) {
            // Scan binary integer normally
            uint64_t bin = 0;

            char *end = input_pos;
            input_pos = start;
            c = next_char(this);
            while(c == '0' || c == '1') {
                uint64_t prev = bin;
                bin = bin * 2 + (c - '0');
                if(bin < prev) { /* TODO: Warning 64-bit overflow */ }
                c = next_char(this);
            }

            input_pos = end; // end of number may be after the last valid character

            Token token;
            token.line = where;
            token.type = Token_Integer;
            token.value.integer = bin;
            return token;
        } else {
            Token token;
            token.line = where;
            token.type = Token_Integer;
            token.value.integer = strtoll(start, nullptr, 0);
            return token;
        }
    } else if(c == '"') {
        // Scan text literal
        string text = scan_quoted_text(this);

        Token token;
        token.line = where;
        token.type = Token_Text;
        set_token_text(token, text);
        return token;
    } else if(c == '\'') {
        // Scan character literal
        string text = scan_quoted_text(this);
        if(text.length() > 1) { /* TODO: Error: Multiple characters in character literal */ }

        Token token;
        token.line = where;
        token.type = Token_Character;
        set_token_text(token, text);
        return token;
    } else if(c == '<') {
        // Scan data literal
        vector<uint8_t> data;

        bool odd = false;
        uint8_t pair = 0;
        while(c != '>' && c != EOF) {
            uint8_t digit;
            if(!isxdigit(c)) {
                /* TODO: Error: Non-hexidecimal character in hexstring */
                break;
            } else if(isupper(c)) {
                digit = c - 'A';
            } else if(islower(c)) {
                digit = c - 'a';
            } else {
                digit = c - '0';
            }

            odd = !odd;
            if(odd) {
                pair = digit;
            } else {
                pair += (pair << 4 | digit);
                data.push_back(pair);
            }

            c = next_char(this);
        }

        if(c == EOF) { /* TODO: Unterminated hexstring */ }
        else if(odd) { /* TODO: Error: Odd number of hex digits in hexstring */ }

        Token token;
        token.line = where;
        token.type = Token_Hexstring;
        set_token_data(token, data);
        return token;
    } else if(c == '/') {
        // Scan comments

        c = next_char(this);
        if(c == '/') {
            // Eat line-comment
            while(c == '\n' && c == '\r' && c != EOF) { c = next_char(this); }
            if(c != EOF) eat_char(this); // Eat newline character
            return scan_token();
        } else if(c == '*') {
            // Eat block-comment

            int block_depth = 0;
            c = next_char(this);
            while(c != EOF) {
                if(c == '/') {
                    c = next_char(this);
                    if(c != '*') continue;

                    block_depth += 1;
                } else if(c == '*') {
                    c = next_char(this);
                    if(c != '/') continue;

                    if(block_depth > 0) {
                        block_depth -= 1;
                    } else {
                        eat_char(this);
                        break;
                    }
                }
                c = next_char(this);
            }

            if(c == EOF && block_depth > 0) { /* TODO: Error: Unterminated block-comment */ }

            return scan_token();
        } else {
            // Its just a '/' operator
            Token token;
            token.line = where;
            token.type = (TokenType)c;
            return token;
        }
    } else if(c == EOF) {
        Token token;
        token.line = where;
        token.type = Token_Eof;
        return token;
    } else {
        Token token;
        token.line = where;
        token.type = (TokenType)c;
        return token;
    }
}

/*
Token::Token(const Token& other) :
    type(other.type),
    line(other.line),
    size(other.size)
{
    if(size == 0) {
        value = other.value;
    } else {
        if(type == Token_Text || type == Token_Identifier) {
            value.text = (char *)malloc(size);
            memcpy(value.text, other.value.text, size);
        } else if(type == Token_Hexstring) {
            value.data = (uint8_t *)malloc(size);
            memcpy(value.data, other.value.data, size);
        } else {
            value = other.value;
        }
    }
}

Token& Token::operator=(Token other)
{
    using std::swap;
    swap(this->type, other.type);
    swap(this->line, other.line);
    swap(this->size, other.size);
    swap(this->value, other.value);
    return *this;
}
*/

static char next_char(Lexer *lexer)
{
    eat_char(lexer);
    return peek_char(lexer);
}

static char peek_char(Lexer *lexer)
{
    return *lexer->input_pos;
}

static void eat_char(Lexer *lexer)
{
    char c = peek_char(lexer);
    LineInfo line = lexer->curr_line;
    if(c == '\n' || c == '\r') {
        line.text_pos = lexer->input_pos + 1;
        line.num += 1;
        line.col = 0;
    } else {
        line.col += 1;
    }

    lexer->input_pos += 1;
}

static void set_token_data(Token& token, const vector<uint8_t>& data)
{
    size_t size = data.size();
    token.size = size;
    token.value.data = (uint8_t *)malloc(size);
    memcpy(token.value.data, &data[0], size);
}

static void set_token_text(Token& token, const string& text)
{
    size_t len = text.length();
    token.size = len;
    token.value.text = (char *)malloc(len);
    memcpy(token.value.text, text.c_str(), len);
}

static string scan_quoted_text(Lexer *lexer)
{
    string text;
    char quote_mark = peek_char(lexer);
    char c = next_char(lexer);
    while(c != quote_mark && c != EOF) {
        if(c == '\n') {
            // A newline is not allowed within a text literal unless it is escaped.
            c = EOF;
            break;
        } else if(c == '\\') {
            // Backslash escapes the following character.
            // We also respect some C conventions.

            c = next_char(lexer);
            switch(c) {
            case 'a':
                text += '\a';
                c = next_char(lexer);
                break;
            case 'n':
                text += '\n';
                c = next_char(lexer);
                break;
            case 'r':
                text += '\r';
                c = next_char(lexer);
                break;
            case 't':
                text += '\t';
                c = next_char(lexer);
                break;
            case 'x':
                {
                    int hex = 0;
                    c = next_char(lexer);
                    for(int i = 0; i < 2 && isxdigit(c); i++) {
                        hex = hex * 16 + (isdigit(c) ? c - '0' : tolower(c) - 'a' + 10);
                        c = next_char(lexer);
                    }

                    text += (char)hex;
                }
                break;
            case '0':
                {
                    int oct = 0;
                    c = next_char(lexer);
                    for(int i = 0; i < 3 && (c >= '0' && c < '7'); i++) {
                        oct = oct * 8 + (c - '0');
                        c = next_char(lexer);
                    }

                    text += (char)oct;
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                {
                    int dec = 0;
                    c = next_char(lexer);
                    for(int i = 0; i < 3 && isdigit(c); i++) {
                        dec = dec * 10 + (c - '0');
                        c = next_char(lexer);
                    }

                    text += (char)dec;
                }
                break;
            case EOF:
                continue;
            default:
                text += c;
                c = next_char(lexer);
            }
        } else {
            text += c;
            c = next_char(lexer);
        }
    }

    if(c == EOF) { /* TODO: Error: Missing terminating '"' character */ }
    return text;
}

} // close namespace bamboo
