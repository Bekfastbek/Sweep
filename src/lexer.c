#define ANYARR_IMPLEMENTATION
#include "../include/lexer.h"
#include <ctype.h>
#include "../include/anyarr.h"
#include "../include/tokens.h"
#include "../include/types.h"

HashMap lexer = {0};
typedef struct {
    uint16_t type;
    const char* value;
} Token;
int token_count = 0;

Token *token = nullptr;

int lexer_init(void) {
    map_init(&lexer);
    assign_map(&lexer);
    map_put(&lexer, "null", SWEEP_TYPE_NULL);
    map_put(&lexer, "bool", SWEEP_TYPE_BOOL);
    map_put(&lexer, "char", SWEEP_TYPE_CHAR);
    map_put(&lexer, "str", SWEEP_TYPE_STR);
    map_put(&lexer, "i8", SWEEP_TYPE_INT8);
    map_put(&lexer, "i16", SWEEP_TYPE_INT16);
    map_put(&lexer, "i32", SWEEP_TYPE_INT32);
    map_put(&lexer, "i64", SWEEP_TYPE_INT64);
    map_put(&lexer, "int", SWEEP_TYPE_INT);
    map_put(&lexer, "u8", SWEEP_TYPE_UINT8);
    map_put(&lexer, "u16", SWEEP_TYPE_UINT16);
    map_put(&lexer, "u32", SWEEP_TYPE_UINT32);
    map_put(&lexer, "u64", SWEEP_TYPE_UINT64);
    map_put(&lexer, "uint", SWEEP_TYPE_UINT);
    map_put(&lexer, "f16", SWEEP_TYPE_FLOAT16);
    map_put(&lexer, "f32", SWEEP_TYPE_FLOAT32);
    map_put(&lexer, "f64", SWEEP_TYPE_FLOAT64);
    map_put(&lexer, "float", SWEEP_TYPE_FLOAT);
    map_put(&lexer, "struct", SWEEP_TYPE_STRUCT);
    map_put(&lexer, "union", SWEEP_TYPE_UNION);
    map_put(&lexer, "enum", SWEEP_TYPE_ENUM);

    map_put(&lexer, "f32x4", SWEEP_SIMD_FLOAT32x4);
    map_put(&lexer, "i8x16", SWEEP_SIMD_INT8x16);
    map_put(&lexer, "i16x8", SWEEP_SIMD_INT16x8);
    map_put(&lexer, "i32x4", SWEEP_SIMD_INT32x4);
    map_put(&lexer, "i64x2", SWEEP_SIMD_INT64x2);
    map_put(&lexer, "u8x16", SWEEP_SIMD_UINT8x16);
    map_put(&lexer, "u16x8", SWEEP_SIMD_UINT16x8);
    map_put(&lexer, "u32x4", SWEEP_SIMD_UINT32x4);
    map_put(&lexer, "u64x2", SWEEP_SIMD_UINT64x2);
    map_put(&lexer, "cf16x8", SWEEP_SIMD_CFLOAT16x8);
    map_put(&lexer, "f64x2", SWEEP_SIMD_FLOAT64x2);

    map_put(&lexer, "i8x32", SWEEP_SIMD_INT8x32);
    map_put(&lexer, "i16x16", SWEEP_SIMD_INT16x16);
    map_put(&lexer, "i32x8", SWEEP_SIMD_INT32x8);
    map_put(&lexer, "i64x4", SWEEP_SIMD_INT64x4);
    map_put(&lexer, "u8x32", SWEEP_SIMD_UINT8x32);
    map_put(&lexer, "u16x16", SWEEP_SIMD_UINT16x16);
    map_put(&lexer, "u32x8", SWEEP_SIMD_UINT32x8);
    map_put(&lexer, "u64x4", SWEEP_SIMD_UINT64x4);
    map_put(&lexer, "cf16x16", SWEEP_SIMD_CFLOAT16x16);
    map_put(&lexer, "f32x8", SWEEP_SIMD_FLOAT32x8);

    map_put(&lexer, "f64x4", SWEEP_SIMD_FLOAT64x4);
    map_put(&lexer, "i8x64", SWEEP_SIMD_INT8x64);
    map_put(&lexer, "i16x32", SWEEP_SIMD_INT16x32);
    map_put(&lexer, "i32x16", SWEEP_SIMD_INT32x16);
    map_put(&lexer, "i64x8", SWEEP_SIMD_INT64x8);
    map_put(&lexer, "u8x64", SWEEP_SIMD_UINT8x64);
    map_put(&lexer, "u16x32", SWEEP_SIMD_UINT16x32);
    map_put(&lexer, "u32x16", SWEEP_SIMD_UINT32x16);
    map_put(&lexer, "u64x8", SWEEP_SIMD_UINT64x8);
    map_put(&lexer, "bf16x32", SWEEP_SIMD_BFLOAT16x32);
    map_put(&lexer, "f16x32", SWEEP_SIMD_FLOAT16x32);
    map_put(&lexer, "f32x16", SWEEP_SIMD_FLOAT32x16);
    map_put(&lexer, "f64x8", SWEEP_SIMD_FLOAT64x8);

    map_put(&lexer, "neon_i8x16", SWEEP_SIMD_NEON_INT8x16);
    map_put(&lexer, "neon_i16x8", SWEEP_SIMD_NEON_INT16x8);
    map_put(&lexer, "neon_i32x4", SWEEP_SIMD_NEON_INT32x4);
    map_put(&lexer, "neon_i64x2", SWEEP_SIMD_NEON_INT64x2);
    map_put(&lexer, "neon_u8x16", SWEEP_SIMD_NEON_UINT8x16);
    map_put(&lexer, "neon_u16x8", SWEEP_SIMD_NEON_UINT16x8);
    map_put(&lexer, "neon_u32x4", SWEEP_SIMD_NEON_UINT32x4);
    map_put(&lexer, "neon_u64x2", SWEEP_SIMD_NEON_UINT64x2);
    map_put(&lexer, "neon_bf16x8", SWEEP_SIMD_NEON_BFLOAT16x8);
    map_put(&lexer, "neon_f16x8", SWEEP_SIMD_NEON_FLOAT16x8);
    map_put(&lexer, "neon_f32x4", SWEEP_SIMD_NEON_FLOAT32x4);
    map_put(&lexer, "neon_f64x2", SWEEP_SIMD_NEON_FLOAT64x2);

    map_put(&lexer, "sve_i8", SWEEP_SIMD_SVE_INT8);
    map_put(&lexer, "sve_i16", SWEEP_SIMD_SVE_INT16);
    map_put(&lexer, "sve_i32", SWEEP_SIMD_SVE_INT32);
    map_put(&lexer, "sve_i64", SWEEP_SIMD_SVE_INT64);
    map_put(&lexer, "sve_u8", SWEEP_SIMD_SVE_UINT8);
    map_put(&lexer, "sve_u16", SWEEP_SIMD_SVE_UINT16);
    map_put(&lexer, "sve_u32", SWEEP_SIMD_SVE_UINT32);
    map_put(&lexer, "sve_u64", SWEEP_SIMD_SVE_UINT64);
    map_put(&lexer, "sve_f16", SWEEP_SIMD_SVE_FLOAT16);
    map_put(&lexer, "sve_bf16", SWEEP_SIMD_SVE_BFLOAT16);
    map_put(&lexer, "sve_f32", SWEEP_SIMD_SVE_FLOAT32);
    map_put(&lexer, "sve_f64", SWEEP_SIMD_SVE_FLOAT64);

    map_put(&lexer, "const", SWEEP_CONST);
    map_put(&lexer, "volatile", SWEEP_VOLATILE);
    map_put(&lexer, "ptr", SWEEP_PTR);
    map_put(&lexer, "inline", SWEEP_INLINE);
    map_put(&lexer, "scoped", SWEEP_SCOPED);
    map_put(&lexer, "shared", SWEEP_SHARED);
    map_put(&lexer, "atomic", SWEEP_ATOMIC);
    map_put(&lexer, "mutex", SWEEP_MUTEX);
    map_put(&lexer, "rw", SWEEP_RW);
    map_put(&lexer, "once", SWEEP_ONCE);
    map_put(&lexer, "func", SWEEP_FUNC);
    map_put(&lexer, "scope", SWEEP_SCOPE);
    map_put(&lexer, "destructor", SWEEP_DESTRUCTOR);
    map_put(&lexer, "err", SWEEP_ERR);
    map_put(&lexer, "switch", SWEEP_SWITCH);
    map_put(&lexer, "case", SWEEP_CASE);
    map_put(&lexer, "default", SWEEP_DEFAULT);
    map_put(&lexer, "if", SWEEP_IF);
    map_put(&lexer, "else", SWEEP_ELSE);
    map_put(&lexer, "for", SWEEP_FOR);
    map_put(&lexer, "while", SWEEP_WHILE);
    map_put(&lexer, "continue", SWEEP_CONTINUE);
    map_put(&lexer, "break", SWEEP_BREAK);
    map_put(&lexer, "__pause", SWEEP__PAUSE);
    map_put(&lexer, "__continue", SWEEP__CONTINUE);
    map_put(&lexer, "from", SWEEP_FROM);
    map_put(&lexer, "import", SWEEP_IMPORT);
    map_put(&lexer, "simd", SWEEP_SIMD);
    map_put(&lexer, "inherits", SWEEP_INHERITS);
    map_put(&lexer, "unsafe", SWEEP_UNSAFE);
    map_put(&lexer, "compiler", SWEEP_COMPILER);
    map_put(&lexer, "llvm", SWEEP_LLVM);
    map_put(&lexer, "clang", SWEEP_CLANG);
    map_put(&lexer, "asm", SWEEP_ASM);
    map_put(&lexer, "spawn", SWEEP_SPAWN);
    map_put(&lexer, "merge", SWEEP_MERGE);
    map_put(&lexer, "ret", SWEEP_RET);
    map_put(&lexer, "main", SWEEP_MAIN);

    map_put(&lexer, "->", SWEEP_ARROW);
    map_put(&lexer, "==", SWEEP_EQ);
    map_put(&lexer, "!=", SWEEP_NOTEQ);
    map_put(&lexer, ">=", SWEEP_GREATEREQ);
    map_put(&lexer, "<=", SWEEP_LESSEQ);
    map_put(&lexer, "<<", SWEEP_LSHIFT);
    map_put(&lexer, ">>", SWEEP_RSHIFT);
    map_put(&lexer, "//", SWEEP_COMMENT);
    map_put(&lexer, "/*", SWEEP_MULTICOMMENT);
    map_put(&lexer, "+=", SWEEP_PLUSASSIGN);
    map_put(&lexer, "-=", SWEEP_MINUSASSIGN);
    map_put(&lexer, "*=", SWEEP_STARASSIGN);
    map_put(&lexer, "/=", SWEEP_SLASHASSIGN);
    map_put(&lexer, "%=", SWEEP_PERCENTASSIGN);

    map_put(&lexer, "true", SWEEP_BOOL_TRUE_LITERAL);
    map_put(&lexer, "false", SWEEP_BOOL_FALSE_LITERAL);

    arena_alloc(anyarr_arena, sizeof(Token), (void**) &token); // arena_alloc also handles realloc and destruction automatically since it's virtual allocation
    return 0;
}

int append_token(const char* stream) {
    if (lexer.capacity == 0) {
        lexer_init();
    }
    Any local;
    Any* type = &local;
    const anyarr_result res = map_get_silent(&lexer, stream, &type);
    if (res != ANYARR_OK) {
        switch (stream[0]) {
            case '{':
                local = assign_any(SWEEP_LBRACE);
                break;
            case '}':
                local = assign_any(SWEEP_RBRACE);
                break;
            case '[':
                local = assign_any(SWEEP_LBRACKET);
                break;
            case ']':
                local = assign_any(SWEEP_RBRACKET);
                break;
            case '(':
                local = assign_any(SWEEP_LPAREN);
                break;
            case ')':
                local = assign_any(SWEEP_RPAREN);
                break;
            case ';':
                local = assign_any(SWEEP_SEMICOLON);
                break;
            case ':':
                local = assign_any(SWEEP_COLON);
                break;
            case ',':
                local = assign_any(SWEEP_COMMA);
                break;
            case '.':
                local = assign_any(SWEEP_DOT);
                break;
            case '@':
                if (stream[1] != '\0') {
                    local = assign_any(SWEEP_FLAGS);
                } else {
                    local = assign_any(SWEEP_AT);
                }
                break;
            case '\'':
                // '\n' 'A' '' ' \' '\ '
                // Just returning 1 until I make proper error catching
                // TODO: Make test cases for this since there's a lot of branches
                // branch 1 checked for null then checked for backslash, branch 2 checked for null or space
                // branch 3 checked for single quote i.e. closing therefore its a valid char literal? hopefully?
                //
                if (stream[1] != '\0' ) {
                    if (stream[1] == '\'') {
                        return 1; // empty char is not really valid
                    } else if (stream[1] == '\\') {
                        if (stream[2] == '\0') {
                            return 1;
                        } else if (stream[2] == ' ') {
                            return 1;
                        } else if (stream[3] != '\'') {
                            local = assign_any(SWEEP_CHAR_LITERAL);
                        }
                    } else if (stream[2] != '\'') {
                        return 1; // Probably a borked character
                    }
                } else {
                    local = assign_any(SWEEP_SQUOTE);
                }
                break;
            case '"':
                if (stream[1] != '\0') {
                    local = assign_any(SWEEP_STR_LITERAL);
                } else {
                    local = assign_any(SWEEP_DQUOTE);
                }
                break;
            case '\\':
                local = assign_any(SWEEP_BSLASH);
                break;
            case '+':
                local = assign_any(SWEEP_PLUS);
                break;
            case '-':
                local = assign_any(SWEEP_MINUS);
                break;
            case '*':
                local = assign_any(SWEEP_STAR);
                break;
            case '/':
                local = assign_any(SWEEP_SLASH);
                break;
            case '%':
                local = assign_any(SWEEP_PERCENT);
                break;
            case '=':
                local = assign_any(SWEEP_ASSIGN);
                break;
            case '>':
                local = assign_any(SWEEP_GREATER);
                break;
            case '<':
                local = assign_any(SWEEP_LESS);
                break;
            case '|':
                local = assign_any(SWEEP_OR);
                break;
            case '&':
                local = assign_any(SWEEP_AND);
                break;
            case '!':
                local = assign_any(SWEEP_NOT);
                break;
            case '~':
                local = assign_any(SWEEP_BITNOT);
                break;
            case '^':
                local = assign_any(SWEEP_XOR);
                break;
            default:
                bool is_float = false;
                if (stream[0] >= '0' && stream[0] <= '9') {
                    if (stream[1] == 'x' || stream[1] == 'X') {
                        if (stream[2] == '\0') {
                            return 1;
                        }
                        for (int i = 2; stream[i] != '\0'; i++) {
                            if (isxdigit((unsigned char)stream[i])) {
                            } else {
                                return 1;
                            }
                        }
                        local = assign_any(SWEEP_INT_LITERAL);
                        break;
                    }
                    for (int i = 0; stream[i] != '\0'; i++) {
                        if (stream[i] == '.') {
                            for (int j = i+1; stream[j] != '\0'; j++) {
                                if (stream[j] == '.') {
                                    return 1;
                                } else if (stream[j] >= '0' && stream[j] <= '9') {
                                } else {
                                    return 1;
                                }
                            }
                            local = assign_any(SWEEP_FLOAT_LITERAL);
                            is_float = true;
                            break;
                        }
                        if (stream[i] >= '0' && stream[i] <= '9') {
                        } else {
                            return 1;
                        }
                    }
                    if (!is_float) {
                        local = assign_any(SWEEP_INT_LITERAL);
                    }
                } else {
                    local = assign_any(SWEEP_IDENTIFIER);
                }
                break;
        }
    }
    token[token_count].type = (uint16_t)type->data.u;
    token[token_count].value = stream;
    printf("Type: %u   Value: %s\n", token[token_count].type, token[token_count].value);
    token_count++;
    return 0;
}

int append_token_special(const char* value, const uint16_t type) {
    token[token_count].type  = type;
    token[token_count].value = value;
    printf("Type: %u   Value: %s\n", token[token_count].type, token[token_count].value);
    token_count++;
    return 0;
}

int check_token(char* value, uint16_t *type) {
    append_token(value);
    value = token[token_count - 1].value;
    *type = token[token_count - 1].type;
    return 0;
}