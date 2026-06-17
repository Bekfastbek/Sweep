#include "../include/anyarr.h"
#include "../include/parse_words.h"
#include "../include/tests.h"


int test_file_parse(void) {
    Stream stream = {0};
    stream.fp = fopen("../test.sp", "r");
    if (stream.fp == NULL) {
        stream.fp = fopen("test.sp", "r");
    }
    if (stream.fp == NULL) {
        perror("Error opening test.sp");
        return 1;
    }

    int result = parse_stream(stream);
    if (result != 0) {
        printf("FAIL file parse: %d\n", result);
        return 1;
    }

    printf("PASS file parse\n");
    return 0;
}
