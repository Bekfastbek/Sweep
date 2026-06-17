#include <stdio.h>
#include "../include/anyarr.h"
#include "../include/parse_words.h"
#include "../include/tests.h"

int test_keyword_parse(void) {
    Stream stream = {0};
    stream.fp = fopen("../tests/keywords.sp", "r");
    if (stream.fp == NULL) {
        stream.fp = fopen("tests/keywords.sp", "r");
    }
    if (stream.fp == NULL) {
        perror("Error opening ../tests/keywords.sp");
        return 1;
    }

    int result = parse_stream(stream);
    if (result != 0) {
        printf("FAIL keyword parse: %d\n", result);
        return 1;
    } else {
        printf("PASS keyword parse\n");
        return 0;
    }
}
