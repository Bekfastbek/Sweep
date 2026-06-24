#ifndef SWEEP_PARSER_H
#define SWEEP_PARSER_H
#include <stdio.h>
#include <stdint.h>

typedef struct {
    FILE *fp;
    char* buf;
    int32_t peek;
    int32_t pos;
    uint32_t line;
    uint32_t column;
    uint32_t token_line;
    uint32_t token_column;
}Stream;
int parse_stream(Stream stream);

#endif