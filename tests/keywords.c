#include <stdio.h>
#include <stdint.h>
#include "../include/lexer.h"
#include "../include/tests.h"
#include "../include/tokens.h"
#include "../include/types.h"

static int check_keyword(char* text, uint16_t expected) {
    uint16_t actual;
    int result = check_token(text, &actual);
    if (result != 0 || actual != expected) {
        printf("FAIL keyword '%s': result=%d type=%u expected=%u\n", text, result, actual, expected);
        return 1;
    } else {
        printf("PASS keyword '%s' -> %u\n", text, actual);
        return 0;
    }
}

int test_keyword_tokens(void) {
    struct {
        char *text;
        uint16_t expected;
    } cases[] = {
        {"null", SWEEP_TYPE_NULL},
        {"bool", SWEEP_TYPE_BOOL},
        {"char", SWEEP_TYPE_CHAR},
        {"str", SWEEP_TYPE_STR},
        {"i8", SWEEP_TYPE_INT8},
        {"i16", SWEEP_TYPE_INT16},
        {"i32", SWEEP_TYPE_INT32},
        {"i64", SWEEP_TYPE_INT64},
        {"int", SWEEP_TYPE_INT},
        {"u8", SWEEP_TYPE_UINT8},
        {"u16", SWEEP_TYPE_UINT16},
        {"u32", SWEEP_TYPE_UINT32},
        {"u64", SWEEP_TYPE_UINT64},
        {"uint", SWEEP_TYPE_UINT},
        {"f16", SWEEP_TYPE_FLOAT16},
        {"f32", SWEEP_TYPE_FLOAT32},
        {"f64", SWEEP_TYPE_FLOAT64},
        {"float", SWEEP_TYPE_FLOAT},
        {"struct", SWEEP_TYPE_STRUCT},
        {"union", SWEEP_TYPE_UNION},
        {"enum", SWEEP_TYPE_ENUM},
        {"f32x4", SWEEP_SIMD_FLOAT32x4},
        {"i8x16", SWEEP_SIMD_INT8x16},
        {"i16x8", SWEEP_SIMD_INT16x8},
        {"i32x4", SWEEP_SIMD_INT32x4},
        {"i64x2", SWEEP_SIMD_INT64x2},
        {"u8x16", SWEEP_SIMD_UINT8x16},
        {"u16x8", SWEEP_SIMD_UINT16x8},
        {"u32x4", SWEEP_SIMD_UINT32x4},
        {"u64x2", SWEEP_SIMD_UINT64x2},
        {"cf16x8", SWEEP_SIMD_CFLOAT16x8},
        {"f64x2", SWEEP_SIMD_FLOAT64x2},
        {"i8x32", SWEEP_SIMD_INT8x32},
        {"i16x16", SWEEP_SIMD_INT16x16},
        {"i32x8", SWEEP_SIMD_INT32x8},
        {"i64x4", SWEEP_SIMD_INT64x4},
        {"u8x32", SWEEP_SIMD_UINT8x32},
        {"u16x16", SWEEP_SIMD_UINT16x16},
        {"u32x8", SWEEP_SIMD_UINT32x8},
        {"u64x4", SWEEP_SIMD_UINT64x4},
        {"cf16x16", SWEEP_SIMD_CFLOAT16x16},
        {"f32x8", SWEEP_SIMD_FLOAT32x8},
        {"f64x4", SWEEP_SIMD_FLOAT64x4},
        {"i8x64", SWEEP_SIMD_INT8x64},
        {"i16x32", SWEEP_SIMD_INT16x32},
        {"i32x16", SWEEP_SIMD_INT32x16},
        {"i64x8", SWEEP_SIMD_INT64x8},
        {"u8x64", SWEEP_SIMD_UINT8x64},
        {"u16x32", SWEEP_SIMD_UINT16x32},
        {"u32x16", SWEEP_SIMD_UINT32x16},
        {"u64x8", SWEEP_SIMD_UINT64x8},
        {"bf16x32", SWEEP_SIMD_BFLOAT16x32},
        {"f16x32", SWEEP_SIMD_FLOAT16x32},
        {"f32x16", SWEEP_SIMD_FLOAT32x16},
        {"f64x8", SWEEP_SIMD_FLOAT64x8},
        {"neon_i8x16", SWEEP_SIMD_NEON_INT8x16},
        {"neon_i16x8", SWEEP_SIMD_NEON_INT16x8},
        {"neon_i32x4", SWEEP_SIMD_NEON_INT32x4},
        {"neon_i64x2", SWEEP_SIMD_NEON_INT64x2},
        {"neon_u8x16", SWEEP_SIMD_NEON_UINT8x16},
        {"neon_u16x8", SWEEP_SIMD_NEON_UINT16x8},
        {"neon_u32x4", SWEEP_SIMD_NEON_UINT32x4},
        {"neon_u64x2", SWEEP_SIMD_NEON_UINT64x2},
        {"neon_bf16x8", SWEEP_SIMD_NEON_BFLOAT16x8},
        {"neon_f16x8", SWEEP_SIMD_NEON_FLOAT16x8},
        {"neon_f32x4", SWEEP_SIMD_NEON_FLOAT32x4},
        {"neon_f64x2", SWEEP_SIMD_NEON_FLOAT64x2},
        {"sve_i8", SWEEP_SIMD_SVE_INT8},
        {"sve_i16", SWEEP_SIMD_SVE_INT16},
        {"sve_i32", SWEEP_SIMD_SVE_INT32},
        {"sve_i64", SWEEP_SIMD_SVE_INT64},
        {"sve_u8", SWEEP_SIMD_SVE_UINT8},
        {"sve_u16", SWEEP_SIMD_SVE_UINT16},
        {"sve_u32", SWEEP_SIMD_SVE_UINT32},
        {"sve_u64", SWEEP_SIMD_SVE_UINT64},
        {"sve_f16", SWEEP_SIMD_SVE_FLOAT16},
        {"sve_bf16", SWEEP_SIMD_SVE_BFLOAT16},
        {"sve_f32", SWEEP_SIMD_SVE_FLOAT32},
        {"sve_f64", SWEEP_SIMD_SVE_FLOAT64},
        {"const", SWEEP_CONST},
        {"volatile", SWEEP_VOLATILE},
        {"ptr", SWEEP_PTR},
        {"inline", SWEEP_INLINE},
        {"scoped", SWEEP_SCOPED},
        {"shared", SWEEP_SHARED},
        {"atomic", SWEEP_ATOMIC},
        {"mutex", SWEEP_MUTEX},
        {"rw", SWEEP_RW},
        {"once", SWEEP_ONCE},
        {"func", SWEEP_FUNC},
        {"scope", SWEEP_SCOPE},
        {"destructor", SWEEP_DESTRUCTOR},
        {"err", SWEEP_ERR},
        {"switch", SWEEP_SWITCH},
        {"case", SWEEP_CASE},
        {"default", SWEEP_DEFAULT},
        {"if", SWEEP_IF},
        {"else", SWEEP_ELSE},
        {"for", SWEEP_FOR},
        {"while", SWEEP_WHILE},
        {"continue", SWEEP_CONTINUE},
        {"break", SWEEP_BREAK},
        {"__pause", SWEEP__PAUSE},
        {"__continue", SWEEP__CONTINUE},
        {"from", SWEEP_FROM},
        {"import", SWEEP_IMPORT},
        {"simd", SWEEP_SIMD},
        {"inherits", SWEEP_INHERITS},
        {"unsafe", SWEEP_UNSAFE},
        {"compiler", SWEEP_COMPILER},
        {"llvm", SWEEP_LLVM},
        {"clang", SWEEP_CLANG},
        {"asm", SWEEP_ASM},
        {"spawn", SWEEP_SPAWN},
        {"merge", SWEEP_MERGE},
        {"ret", SWEEP_RET},
        {"main", SWEEP_MAIN},
    };

    int failures = 0;
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        failures += check_keyword(cases[i].text, cases[i].expected);
    }
    return failures;
}
