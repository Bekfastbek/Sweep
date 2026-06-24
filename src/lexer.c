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
    uint32_t line;
    uint32_t column;
} Token;
int token_count = 0;

Token *token = nullptr;
Arena lex_arena_storage;
Arena *lex_arena = &lex_arena_storage;


int lexer_init(void) {
    arena_init(lex_arena);
    map_init(&lexer, lex_arena);
    assign_map(&lexer, lex_arena);
    map_put(&lexer, "null", SWEEP_TYPE_NULL, lex_arena);
    map_put(&lexer, "bool", SWEEP_TYPE_BOOL, lex_arena);
    map_put(&lexer, "char", SWEEP_TYPE_CHAR, lex_arena);
    map_put(&lexer, "str", SWEEP_TYPE_STR, lex_arena);
    map_put(&lexer, "i8", SWEEP_TYPE_INT8, lex_arena);
    map_put(&lexer, "i16", SWEEP_TYPE_INT16, lex_arena);
    map_put(&lexer, "i32", SWEEP_TYPE_INT32, lex_arena);
    map_put(&lexer, "i64", SWEEP_TYPE_INT64, lex_arena);
    map_put(&lexer, "int", SWEEP_TYPE_INT, lex_arena);
    map_put(&lexer, "u8", SWEEP_TYPE_UINT8, lex_arena);
    map_put(&lexer, "u16", SWEEP_TYPE_UINT16, lex_arena);
    map_put(&lexer, "u32", SWEEP_TYPE_UINT32, lex_arena);
    map_put(&lexer, "u64", SWEEP_TYPE_UINT64, lex_arena);
    map_put(&lexer, "uint", SWEEP_TYPE_UINT, lex_arena);
    map_put(&lexer, "f16", SWEEP_TYPE_FLOAT16, lex_arena);
    map_put(&lexer, "f32", SWEEP_TYPE_FLOAT32, lex_arena);
    map_put(&lexer, "f64", SWEEP_TYPE_FLOAT64, lex_arena);
    map_put(&lexer, "float", SWEEP_TYPE_FLOAT, lex_arena);
    map_put(&lexer, "struct", SWEEP_TYPE_STRUCT, lex_arena);
    map_put(&lexer, "union", SWEEP_TYPE_UNION, lex_arena);
    map_put(&lexer, "enum", SWEEP_TYPE_ENUM, lex_arena);

    map_put(&lexer, "f32x4", SWEEP_SIMD_FLOAT32x4, lex_arena);
    map_put(&lexer, "i8x16", SWEEP_SIMD_INT8x16, lex_arena);
    map_put(&lexer, "i16x8", SWEEP_SIMD_INT16x8, lex_arena);
    map_put(&lexer, "i32x4", SWEEP_SIMD_INT32x4, lex_arena);
    map_put(&lexer, "i64x2", SWEEP_SIMD_INT64x2, lex_arena);
    map_put(&lexer, "u8x16", SWEEP_SIMD_UINT8x16, lex_arena);
    map_put(&lexer, "u16x8", SWEEP_SIMD_UINT16x8, lex_arena);
    map_put(&lexer, "u32x4", SWEEP_SIMD_UINT32x4, lex_arena);
    map_put(&lexer, "u64x2", SWEEP_SIMD_UINT64x2, lex_arena);
    map_put(&lexer, "cf16x8", SWEEP_SIMD_CFLOAT16x8, lex_arena);
    map_put(&lexer, "f64x2", SWEEP_SIMD_FLOAT64x2, lex_arena);

    map_put(&lexer, "i8x32", SWEEP_SIMD_INT8x32, lex_arena);
    map_put(&lexer, "i16x16", SWEEP_SIMD_INT16x16, lex_arena);
    map_put(&lexer, "i32x8", SWEEP_SIMD_INT32x8, lex_arena);
    map_put(&lexer, "i64x4", SWEEP_SIMD_INT64x4, lex_arena);
    map_put(&lexer, "u8x32", SWEEP_SIMD_UINT8x32, lex_arena);
    map_put(&lexer, "u16x16", SWEEP_SIMD_UINT16x16, lex_arena);
    map_put(&lexer, "u32x8", SWEEP_SIMD_UINT32x8, lex_arena);
    map_put(&lexer, "u64x4", SWEEP_SIMD_UINT64x4, lex_arena);
    map_put(&lexer, "cf16x16", SWEEP_SIMD_CFLOAT16x16, lex_arena);
    map_put(&lexer, "f32x8", SWEEP_SIMD_FLOAT32x8, lex_arena);

    map_put(&lexer, "f64x4", SWEEP_SIMD_FLOAT64x4, lex_arena);
    map_put(&lexer, "i8x64", SWEEP_SIMD_INT8x64, lex_arena);
    map_put(&lexer, "i16x32", SWEEP_SIMD_INT16x32, lex_arena);
    map_put(&lexer, "i32x16", SWEEP_SIMD_INT32x16, lex_arena);
    map_put(&lexer, "i64x8", SWEEP_SIMD_INT64x8, lex_arena);
    map_put(&lexer, "u8x64", SWEEP_SIMD_UINT8x64, lex_arena);
    map_put(&lexer, "u16x32", SWEEP_SIMD_UINT16x32, lex_arena);
    map_put(&lexer, "u32x16", SWEEP_SIMD_UINT32x16, lex_arena);
    map_put(&lexer, "u64x8", SWEEP_SIMD_UINT64x8, lex_arena);
    map_put(&lexer, "bf16x32", SWEEP_SIMD_BFLOAT16x32, lex_arena);
    map_put(&lexer, "f16x32", SWEEP_SIMD_FLOAT16x32, lex_arena);
    map_put(&lexer, "f32x16", SWEEP_SIMD_FLOAT32x16, lex_arena);
    map_put(&lexer, "f64x8", SWEEP_SIMD_FLOAT64x8, lex_arena);

    map_put(&lexer, "neon_i8x16", SWEEP_SIMD_NEON_INT8x16, lex_arena);
    map_put(&lexer, "neon_i16x8", SWEEP_SIMD_NEON_INT16x8, lex_arena);
    map_put(&lexer, "neon_i32x4", SWEEP_SIMD_NEON_INT32x4, lex_arena);
    map_put(&lexer, "neon_i64x2", SWEEP_SIMD_NEON_INT64x2, lex_arena);
    map_put(&lexer, "neon_u8x16", SWEEP_SIMD_NEON_UINT8x16, lex_arena);
    map_put(&lexer, "neon_u16x8", SWEEP_SIMD_NEON_UINT16x8, lex_arena);
    map_put(&lexer, "neon_u32x4", SWEEP_SIMD_NEON_UINT32x4, lex_arena);
    map_put(&lexer, "neon_u64x2", SWEEP_SIMD_NEON_UINT64x2, lex_arena);
    map_put(&lexer, "neon_bf16x8", SWEEP_SIMD_NEON_BFLOAT16x8, lex_arena);
    map_put(&lexer, "neon_f16x8", SWEEP_SIMD_NEON_FLOAT16x8, lex_arena);
    map_put(&lexer, "neon_f32x4", SWEEP_SIMD_NEON_FLOAT32x4, lex_arena);
    map_put(&lexer, "neon_f64x2", SWEEP_SIMD_NEON_FLOAT64x2, lex_arena);

    map_put(&lexer, "sve_i8", SWEEP_SIMD_SVE_INT8, lex_arena);
    map_put(&lexer, "sve_i16", SWEEP_SIMD_SVE_INT16, lex_arena);
    map_put(&lexer, "sve_i32", SWEEP_SIMD_SVE_INT32, lex_arena);
    map_put(&lexer, "sve_i64", SWEEP_SIMD_SVE_INT64, lex_arena);
    map_put(&lexer, "sve_u8", SWEEP_SIMD_SVE_UINT8, lex_arena);
    map_put(&lexer, "sve_u16", SWEEP_SIMD_SVE_UINT16, lex_arena);
    map_put(&lexer, "sve_u32", SWEEP_SIMD_SVE_UINT32, lex_arena);
    map_put(&lexer, "sve_u64", SWEEP_SIMD_SVE_UINT64, lex_arena);
    map_put(&lexer, "sve_f16", SWEEP_SIMD_SVE_FLOAT16, lex_arena);
    map_put(&lexer, "sve_bf16", SWEEP_SIMD_SVE_BFLOAT16, lex_arena);
    map_put(&lexer, "sve_f32", SWEEP_SIMD_SVE_FLOAT32, lex_arena);
    map_put(&lexer, "sve_f64", SWEEP_SIMD_SVE_FLOAT64, lex_arena);

    map_put(&lexer, "const", SWEEP_CONST, lex_arena);
    map_put(&lexer, "volatile", SWEEP_VOLATILE, lex_arena);
    map_put(&lexer, "ptr", SWEEP_PTR, lex_arena);
    map_put(&lexer, "inline", SWEEP_INLINE, lex_arena);
    map_put(&lexer, "scoped", SWEEP_SCOPED, lex_arena);
    map_put(&lexer, "shared", SWEEP_SHARED, lex_arena);
    map_put(&lexer, "atomic", SWEEP_ATOMIC, lex_arena);
    map_put(&lexer, "mutex", SWEEP_MUTEX, lex_arena);
    map_put(&lexer, "rw", SWEEP_RW, lex_arena);
    map_put(&lexer, "once", SWEEP_ONCE, lex_arena);
    map_put(&lexer, "func", SWEEP_FUNC, lex_arena);
    map_put(&lexer, "scope", SWEEP_SCOPE, lex_arena);
    map_put(&lexer, "destructor", SWEEP_DESTRUCTOR, lex_arena);
    map_put(&lexer, "err", SWEEP_ERR, lex_arena);
    map_put(&lexer, "switch", SWEEP_SWITCH, lex_arena);
    map_put(&lexer, "case", SWEEP_CASE, lex_arena);
    map_put(&lexer, "default", SWEEP_DEFAULT, lex_arena);
    map_put(&lexer, "if", SWEEP_IF, lex_arena);
    map_put(&lexer, "else", SWEEP_ELSE, lex_arena);
    map_put(&lexer, "for", SWEEP_FOR, lex_arena);
    map_put(&lexer, "while", SWEEP_WHILE, lex_arena);
    map_put(&lexer, "continue", SWEEP_CONTINUE, lex_arena);
    map_put(&lexer, "break", SWEEP_BREAK, lex_arena);
    map_put(&lexer, "__pause", SWEEP__PAUSE, lex_arena);
    map_put(&lexer, "__continue", SWEEP__CONTINUE, lex_arena);
    map_put(&lexer, "from", SWEEP_FROM, lex_arena);
    map_put(&lexer, "import", SWEEP_IMPORT, lex_arena);
    map_put(&lexer, "simd", SWEEP_SIMD, lex_arena);
    map_put(&lexer, "inherits", SWEEP_INHERITS, lex_arena);
    map_put(&lexer, "unsafe", SWEEP_UNSAFE, lex_arena);
    map_put(&lexer, "compiler", SWEEP_COMPILER, lex_arena);
    map_put(&lexer, "llvm", SWEEP_LLVM, lex_arena);
    map_put(&lexer, "clang", SWEEP_CLANG, lex_arena);
    map_put(&lexer, "asm", SWEEP_ASM, lex_arena);
    map_put(&lexer, "spawn", SWEEP_SPAWN, lex_arena);
    map_put(&lexer, "merge", SWEEP_MERGE, lex_arena);
    map_put(&lexer, "ret", SWEEP_RET, lex_arena);
    map_put(&lexer, "main", SWEEP_MAIN, lex_arena);

    map_put(&lexer, "->", SWEEP_ARROW, lex_arena);
    map_put(&lexer, "==", SWEEP_EQ, lex_arena);
    map_put(&lexer, "!=", SWEEP_NOTEQ, lex_arena);
    map_put(&lexer, ">=", SWEEP_GREATEREQ, lex_arena);
    map_put(&lexer, "<=", SWEEP_LESSEQ, lex_arena);
    map_put(&lexer, "<<", SWEEP_LSHIFT, lex_arena);
    map_put(&lexer, ">>", SWEEP_RSHIFT, lex_arena);
    map_put(&lexer, "//", SWEEP_COMMENT, lex_arena);
    map_put(&lexer, "/*", SWEEP_MULTICOMMENT, lex_arena);
    map_put(&lexer, "+=", SWEEP_PLUSASSIGN, lex_arena);
    map_put(&lexer, "-=", SWEEP_MINUSASSIGN, lex_arena);
    map_put(&lexer, "*=", SWEEP_STARASSIGN, lex_arena);
    map_put(&lexer, "/=", SWEEP_SLASHASSIGN, lex_arena);
    map_put(&lexer, "%=", SWEEP_PERCENTASSIGN, lex_arena);

    map_put(&lexer, "true", SWEEP_BOOL_TRUE_LITERAL, lex_arena);
    map_put(&lexer, "false", SWEEP_BOOL_FALSE_LITERAL, lex_arena);

    arena_alloc(lex_arena, sizeof(Token), (void**) &token); // arena_alloc also handles realloc and destruction automatically since it's virtual allocation
    return 0;
}

int append_token(const char* stream, const uint32_t line, const uint32_t column) {
    if (lexer.capacity == 0) {
        lexer_init();
    }
    Any local;
    Any* type = &local;
    const anyarr_result res = map_get_silent(&lexer, stream, &type);
    if (res != ANYARR_OK) {
        switch (stream[0]) {
            case '{':
                local = assign_any(SWEEP_LBRACE, lex_arena);
                break;
            case '}':
                local = assign_any(SWEEP_RBRACE, lex_arena);
                break;
            case '[':
                local = assign_any(SWEEP_LBRACKET, lex_arena);
                break;
            case ']':
                local = assign_any(SWEEP_RBRACKET, lex_arena);
                break;
            case '(':
                local = assign_any(SWEEP_LPAREN, lex_arena);
                break;
            case ')':
                local = assign_any(SWEEP_RPAREN, lex_arena);
                break;
            case ';':
                local = assign_any(SWEEP_SEMICOLON, lex_arena);
                break;
            case ':':
                local = assign_any(SWEEP_COLON, lex_arena);
                break;
            case ',':
                local = assign_any(SWEEP_COMMA, lex_arena);
                break;
            case '.':
                local = assign_any(SWEEP_DOT, lex_arena);
                break;
            case '@':
                if (stream[1] != '\0') {
                    local = assign_any(SWEEP_FLAGS, lex_arena);
                } else {
                    local = assign_any(SWEEP_AT, lex_arena);
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
                            local = assign_any(SWEEP_CHAR_LITERAL, lex_arena);
                        }
                    } else if (stream[2] != '\'') {
                        return 1; // Probably a borked character
                    }
                } else {
                    local = assign_any(SWEEP_SQUOTE, lex_arena);
                }
                break;
            case '"':
                if (stream[1] != '\0') {
                    local = assign_any(SWEEP_STR_LITERAL, lex_arena);
                } else {
                    local = assign_any(SWEEP_DQUOTE, lex_arena);
                }
                break;
            case '\\':
                local = assign_any(SWEEP_BSLASH, lex_arena);
                break;
            case '+':
                local = assign_any(SWEEP_PLUS, lex_arena);
                break;
            case '-':
                local = assign_any(SWEEP_MINUS, lex_arena);
                break;
            case '*':
                local = assign_any(SWEEP_STAR, lex_arena);
                break;
            case '/':
                local = assign_any(SWEEP_SLASH, lex_arena);
                break;
            case '%':
                local = assign_any(SWEEP_PERCENT, lex_arena);
                break;
            case '=':
                local = assign_any(SWEEP_ASSIGN, lex_arena);
                break;
            case '>':
                local = assign_any(SWEEP_GREATER, lex_arena);
                break;
            case '<':
                local = assign_any(SWEEP_LESS, lex_arena);
                break;
            case '|':
                local = assign_any(SWEEP_OR, lex_arena);
                break;
            case '&':
                local = assign_any(SWEEP_AND, lex_arena);
                break;
            case '!':
                local = assign_any(SWEEP_NOT, lex_arena);
                break;
            case '~':
                local = assign_any(SWEEP_BITNOT, lex_arena);
                break;
            case '^':
                local = assign_any(SWEEP_XOR, lex_arena);
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
                        local = assign_any(SWEEP_INT_LITERAL, lex_arena);
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
                            local = assign_any(SWEEP_FLOAT_LITERAL, lex_arena);
                            is_float = true;
                            break;
                        }
                        if (stream[i] >= '0' && stream[i] <= '9') {
                        } else {
                            return 1;
                        }
                    }
                    if (!is_float) {
                        local = assign_any(SWEEP_INT_LITERAL, lex_arena);
                    }
                } else {
                    local = assign_any(SWEEP_IDENTIFIER, lex_arena);
                }
                break;
        }
    }
    token[token_count].type = (uint16_t)type->data.u;
    token[token_count].value = stream;
    token[token_count].line = line;
    token[token_count].column = column;
    // printf("Type: %u   Value: %s\n", token[token_count].type, token[token_count].value, lex_arena);
    token_count++;
    return 0;
}

int append_token_special(const char* value, const uint16_t type, const uint32_t line, const uint32_t column) {
    token[token_count].type  = type;
    token[token_count].value = value;
    token[token_count].line = line;
    token[token_count].column = column;
    // printf("Type: %u   Value: %s\n", token[token_count].type, token[token_count].value, lex_arena);
    token_count++;
    return 0;
}

int check_token(const char* value, uint16_t *type) {
    append_token(value, 0, 0);
    value = (char*) token[token_count - 1].value;
    *type = token[token_count - 1].type;
    return 0;
}