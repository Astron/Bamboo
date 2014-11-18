// Filename: lexer.h
#pragma once
#include "../dcfile/token.h"
namespace bamboo
{


struct Lexer
{
    Lexer(const char *buf, size_t bufsize);

    Token scan_token();

    const char *input_buf = nullptr;
    const char *input_pos = nullptr;
    size_t input_size = 0;

    LineInfo curr_line;
    int num_errors = 0;
};


} // close namespace bamboo
