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


const array<string, 21> keywords = {
    "keyword", "struct", "class", "typedef", "import", "from",
    "char", "int8", "int16", "int32", "int64",
    "byte", "uint8", "uint16", "uint32", "uint64",
    "float32", "float64", "string", "blob",

    "dclass" // @NOTE(Kevin): I want to deprecate "dclass" in favor of "class"
};

const array<TokenType, 21> keyword_types = {
    Token_Keyword, Token_Struct, Token_Class, Token_Typedef, Token_Import, Token_ImportFrom,
    Token_Char, Token_Int8, Token_Int16, Token_Int32, Token_Int64,
    Token_Byte, Token_Uint8, Token_Uint16, Token_Uint32, Token_Uint64,
    Token_Float32, Token_Float64, Token_String, Token_Blob,

    Token_Class
};

static void add_error(Lexer *lexer, const LineInfo& where, const string& what);
static char next_char(Lexer *lexer);
static char peek_char(Lexer *lexer);
static void eat_char(Lexer *lexer);
static void set_token_data(Token& token, const vector<uint8_t>& data);
static void set_token_text(Token& token, const string& text);
static string scan_quoted_text(Lexer *lexer, const LineInfo& start);


static char format_buf[8];
const char *format_tokentype(TokenType type)
{
    switch(type) {
    case Token_NotAToken:
        return "uninitialized-token";
    case Token_Eof:
        return "end-of-input";
    case Token_Identifier:
        return "identifier";
    case Token_Class:
        return "keyword \"class\"";
    case Token_Struct:
        return "keyword \"struct\"";
    case Token_Typedef:
        return "keyword \"typedef\"";
    case Token_Keyword:
        return "keyword \"keyword\"";
    case Token_Import:
        return "keyword \"import\"";
    case Token_ImportFrom:
        return "keyword \"from\"";
    case Token_Char:
        return "type \"char\"";
    case Token_Int8:
        return "type \"int8\"";
    case Token_Int16:
        return "type \"int16\"";
    case Token_Int32:
        return "type \"int32\"";
    case Token_Int64:
        return "type \"int64\"";
    case Token_Uint8:
        return "type \"uint8\"";
    case Token_Uint16:
        return "type \"uint16\"";
    case Token_Uint32:
        return "type \"uint32\"";
    case Token_Uint64:
        return "type \"uint64\"";
    case Token_Float32:
        return "type \"float32\"";
    case Token_Float64:
        return "type \"float64\"";
    case Token_String:
        return "type \"string\"";
    case Token_Blob:
        return "type \"blob\"";
    case Token_Character:
        return "character literal";
    case Token_Integer:
        return "integer literal";
    case Token_Real:
        return "float literal";
    case Token_Text:
        return "string literal";
    case Token_Hexstring:
        return "hexstring literal";
    case '+':
        return "unary operator '+'";
    case '-':
        return "unary operator '-'";
    case '=':
        return "operator '='";
    case '/':
        return "operator '/'";
    case '%':
        return "operator '%'";
    default:
        if(isprint(type)) {
            snprintf(format_buf, sizeof(format_buf), "'%c'", type);
            return format_buf;
        } else {
            snprintf(format_buf, sizeof(format_buf), "\\x%x", type);
            return format_buf;
        }
    }
}

Lexer::Lexer(const char *buf, size_t bufsize) :
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

    LineInfo here = this->curr_line;

    if(isalpha(c)) {
        // Scan identifier

        std::string name;
        char c = peek_char(this);
        while(isalnum(c) || c == '_') {
            name += c;
            c = next_char(this);
        }

        for(unsigned int i = 0; i < keywords.size(); ++i) {
            if(name == "dclass") {
                // @NOTE(Kevin): If Bamboo works for everything, this change can be made
                /* print_warning(here, "\"dclass\" is deprecated, use \"class\" instead"); */
            }
            if(name == keywords[i]) {
                Token token;
                token.line = here;
                token.type = keyword_types[i];
                set_token_text(token, name);;
                return token;
            }
        }

        Token token;
        token.line = here;
        token.type = Token_Identifier;
        set_token_text(token, name);;
        return token;
    } else if(isdigit(c) || c == '.') {
        // Scan number literal

        const char *start = input_pos;

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
                if(has_decimal_point) {
                    add_error(this, here, "Too many decimal points in number");
                } else if(has_bin_prefix) {
                    add_error(this, here, "Invalid prefix \"0b\" for float literal");
                }
                has_decimal_point = true;
            } else if(c == 'e' || c == 'E') {
                if(has_exponent) {
                    add_error(this, here, "Too many decimal points in number");
                } else if(!has_bin_prefix) {
                    add_error(this, here, "Invalid prefix \"0b\" for float literal");
                }
                has_exponent = true;

                c = next_char(this);
                if(c == '-' || c == '+') { c = next_char(this); }
                continue;
            } else if(has_hex_prefix) {
                if(!isalnum(c)) { break; }
                if(!isxdigit(c)) {
                    string error = "Forbidden character '";
                    error += c;
                    error += "' at end of numeric literal";
                    add_error(this, here, error);
                }
            } else {
                if(!isalnum(c)) { break; }
                if(!isdigit(c)) {
                    string error = "Forbidden character '";
                    error += c;
                    error += "' at end of numeric literal";
                    add_error(this, here, error);
                }
                if(has_bin_prefix) {
                    if(c != '0' && c != '1') {
                        string error = "Invalid digit '";
                        error += c;
                        error += "'' in binary literal";
                        add_error(this, here, error);
                    }
                } else if(has_zero_prefix) {
                    if(c == '8' || c == '9') {
                        string error = "Invalid digit '";
                        error += c;
                        error += "'' in octal literal";
                        add_error(this, here, error);
                    }
                }
            }
            c = next_char(this);
        }

        // Parse the string as a numeric literal
        if(has_exponent || has_decimal_point) {
            Token token;
            token.line = here;
            token.type = Token_Real;
            token.value.real = atof(start);
            return token;
        } else if(has_bin_prefix) {
            // Scan binary integer normally
            uint64_t bin = 0;

            const char *end = input_pos;
            input_pos = start;
            c = next_char(this);
            while(c == '0' || c == '1') {
                bin = bin * 2 + (c - '0');
                c = next_char(this);
            }

            input_pos = end; // end of number may be after the last valid character

            Token token;
            token.line = here;
            token.type = Token_Integer;
            token.value.integer = bin;
            return token;
        } else {
            Token token;
            token.line = here;
            token.type = Token_Integer;
            token.value.integer = strtoll(start, nullptr, 0);
            return token;
        }
    } else if(c == '"') {
        // Scan text literal
        string text = scan_quoted_text(this, here);

        Token token;
        token.line = here;
        token.type = Token_Text;
        set_token_text(token, text);
        return token;
    } else if(c == '\'') {
        // Scan character literal
        string text = scan_quoted_text(this, here);
        if(text.length() > 1) {
            add_error(this, here, "Multiple characters in character literal");
        } else if(text.length() < 1) {
            add_error(this, here, "Empty character literal");
        }

        Token token;
        token.line = here;
        token.type = Token_Character;
        set_token_text(token, text);
        return token;
    } else if(c == '<') {
        // Scan hexstring literal
        vector<uint8_t> data;

        bool odd = false;
        uint8_t pair = 0;
        while(c != '>' && c != EOF) {
            uint8_t digit;
            if(!isxdigit(c)) {
                add_error(this, here, "Non-hexidecimal character in hexstring");

                // Assume literal stops at the first whitespace
                while(!isspace(c)) { c = next_char(this); }
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

        if(c == EOF) {
            add_error(this, here, "Missing terminating '>' for hexstring");
        } else if(odd) {
            add_error(this, here, "Odd number of hexidecimal digits in hexstring");
        }

        Token token;
        token.line = here;
        token.type = Token_Hexstring;
        set_token_data(token, data);
        return token;
    } else if(c == '/') {
        // Scan comments

        c = next_char(this);
        if(c == '/') {
            // Eat line-comment
            while(c != '\n' && c != '\r' && c != EOF) { c = next_char(this); }
            if(c != EOF) eat_char(this); // eat newline character
            return scan_token();
        } else if(c == '*') {
            // Eat block-comment

            bool is_nested = false;
            int block_depth = 1;
            c = next_char(this);
            while(c != EOF) {
                if(c == '/') {
                    c = next_char(this);
                    if(c != '*') continue;

                    block_depth += 1;
                    is_nested = true;
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

            if(c == EOF && block_depth > 0) {
                if(is_nested) {
                    add_error(this, here, "More \"/*\" than \"/*\" nested in block comment");
                } else {
                    add_error(this, here, "Missing terminating \"*/\" for block comment");
                }
            }
            return scan_token();
        } else {
            // Its just a '/' operator
            Token token;
            token.line = here;
            token.type = (TokenType)'/';
            return token;
        }
    } else if(c == EOF || c == '\0') {
        Token token;
        token.line = here;
        token.type = Token_Eof;
        return token;
    } else {
        eat_char(this);
        Token token;
        token.line = here;
        token.type = (TokenType)c;
        return token;
    }
}

static void add_error(Lexer* lexer, const LineInfo& where, const std::string& what)
{
    lexer->num_errors += 1;
    fprintf(stderr, "Error at line %d, column %d:\n", where.num, where.col);
    fprintf(stderr, "  %s\n", what.c_str());

    const char *eol = strpbrk(where.text_pos, "\n\r");
    string line(where.text_pos, eol);
    fprintf(stderr, "%s\n%*c\n", what.c_str(), where.col, '^');
}

static char next_char(Lexer *lexer)
{
    eat_char(lexer);
    return peek_char(lexer);
}

static char peek_char(Lexer *lexer)
{
    if((size_t)(lexer->input_pos - lexer->input_buf) < lexer->input_size) {
        return *lexer->input_pos;
    } else {
        return EOF;
    }
}

static void eat_char(Lexer *lexer)
{
    char c = peek_char(lexer);
    LineInfo& line = lexer->curr_line;
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
    token.value.text = (char *)malloc(len + 1);
    strncpy(token.value.text, text.c_str(), len);
    token.value.text[len] = '\0'; // Ensure null termination
}

static string scan_quoted_text(Lexer *lexer, const LineInfo& start)
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

    if(c == EOF) { add_error(lexer, start, "Missing terminating '\"' character"); }
    return text;
}


} // close namespace bamboo
