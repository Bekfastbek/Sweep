#include "../include/anyarr.h"
#include "../include/parse_words.h"
#include "../include/tests.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--test") == 0) {
        int failures = 0;
        failures += test_char_lit();
        failures += test_keyword_tokens();
        failures += test_keyword_parse();
        failures += test_file_parse();
        if (failures != 0) {
            fprintf(stderr, "FAIL test suite: %d failure(s)\n", failures);
            return 1;
        }
        printf("PASS test suite\n");
        return 0;
    }

    Stream stream = {0};
    arena_alloc(ARENA_CTX, sizeof(stream), (void**) &stream.buf);
    stream.fp = fopen("../tests/compiler_test.sp", "r");
    if (stream.fp == nullptr) {
        perror("Error opening tests/compiler_test.sp");
        return 1;
    }

    int result = parse_stream(stream);
    // Safety net just in case
    if (result != 0) {
        fprintf(stderr, "Error parsing stream\n");
        return 1;
    }

    return 0;
}
