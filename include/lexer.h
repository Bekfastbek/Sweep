#ifndef SWEEP_LEXER_H
#define SWEEP_LEXER_H
#include <stdint.h>

int append_token(const char* stream);
int append_token_special(const char* value, uint16_t type);
int check_token(char* value, uint16_t *type);

#endif