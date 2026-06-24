#ifndef SWEEP_LEXER_H
#define SWEEP_LEXER_H
#include <stdint.h>

int append_token(const char* stream, uint32_t line, uint32_t column);
int append_token_special(const char* value, uint16_t type, uint32_t line, uint32_t column);
int check_token(const char* value, uint16_t *type);

#endif