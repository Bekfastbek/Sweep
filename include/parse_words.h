#ifndef SWEEP_PARSER_H
#define SWEEP_PARSER_H
#include <stdio.h>

typedef struct {
    FILE *fp;
    char* buf;
    int peek;
    int pos;
}Stream;
int parse_stream(Stream stream);

#endif