#include <stdint.h>
#include <stdio.h>
#include "../include/lexer.h"
#include "../include/tests.h"
#include "../include/tokens.h"

int test_char_lit(void) {
    struct {
        char *text;
        int expected;
        uint16_t expected_type;
    } cases[] = {
        {"'\\n'", 0, SWEEP_CHAR_LITERAL},
        {"' '", 0, SWEEP_CHAR_LITERAL},
        {"'\\''", 0, SWEEP_CHAR_LITERAL},
        {"''", 1, 0},
        {"'AA'", 1, 0},
        {"A", 0, SWEEP_IDENTIFIER},
    };

    int failures = 0;
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        uint16_t actual_type = 0;
        const int result = check_token(cases[i].text, &actual_type);
        if (result != cases[i].expected || (result == 0 && actual_type != cases[i].expected_type)) {
            printf("FAIL char lit %s -> result=%d type=%u expected_result=%d expected_type=%u\n",
                   cases[i].text,
                   result,
                   actual_type,
                   cases[i].expected,
                   cases[i].expected_type);
            failures++;
        } else {
            printf("PASS char lit %s -> %u\n", cases[i].text, actual_type);
        }
    }
    return failures;
}
