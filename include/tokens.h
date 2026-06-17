#ifndef SWEEP_TOKENS_H
#define SWEEP_TOKENS_H
#include<stdint.h>

typedef enum: uint16_t {
    SWEEP_LBRACE = 0x0301,          // {
    SWEEP_RBRACE = 0x0302,          // }
    SWEEP_LBRACKET = 0x0303,        // [
    SWEEP_RBRACKET = 0x0304,        // ]
    SWEEP_LPAREN = 0x0305,          // (
    SWEEP_RPAREN = 0x0306,          // )

    SWEEP_SEMICOLON = 0x0307,       // ;
    SWEEP_COLON = 0x0308,           // :
    SWEEP_COMMA = 0x0309,           // ,
    SWEEP_DOT = 0x030A,             // .
    SWEEP_AT = 0x030B,              // @
    SWEEP_SQUOTE = 0x030C,          // ''
    SWEEP_DQUOTE = 0x030D,          // ""
    SWEEP_BSLASH = 0x030E,          // \

    SWEEP_PLUS = 0x0311,            // +
    SWEEP_MINUS = 0x0312,           // -
    SWEEP_STAR = 0x0313,            // *
    SWEEP_SLASH = 0x0314,           // /
    SWEEP_PERCENT = 0x0315,         // %
    SWEEP_ASSIGN = 0x0316,          // =

    SWEEP_GREATER = 0x0317,         // >
    SWEEP_LESS = 0x0318,            // <
    // No ternary operators because they ruin readability
    SWEEP_OR = 0x0319,              // |
    SWEEP_AND = 0x031A,             // &
    SWEEP_NOT = 0x031B,             // !
    SWEEP_BITNOT = 0x031C,          // ~
    SWEEP_XOR = 0x031D,             // ^
} SweepDelimiters;

typedef enum: uint16_t {
    SWEEP_ARROW = 0x0201,           // ->
    SWEEP_EQ = 0x0202,              // ==
    SWEEP_NOTEQ = 0x0203,           // !=
    SWEEP_GREATEREQ = 0x0204,       // >=
    SWEEP_LESSEQ = 0x0205,          // <=
    SWEEP_LSHIFT = 0x0206,          // <<
    SWEEP_RSHIFT = 0x0207,          // >>
    SWEEP_COMMENT = 0x0208,         // //
    SWEEP_MULTICOMMENT = 0x0209,    // /* */
    SWEEP_PLUSASSIGN = 0x020A,      // +=
    SWEEP_MINUSASSIGN = 0x020B,     // -=
    SWEEP_STARASSIGN = 0x020C,      // *=
    SWEEP_SLASHASSIGN = 0x020D,     // /=
    SWEEP_PERCENTASSIGN = 0x020E,   // %=
} SweepCompound;

typedef enum: uint16_t {
    SWEEP_CONST = 0x0401,
    SWEEP_VOLATILE = 0x0402,
    SWEEP_PTR = 0x0403,
    SWEEP_INLINE = 0x0404,
    SWEEP_SCOPED = 0x0405,

    SWEEP_SHARED = 0x0406,
    SWEEP_ATOMIC = 0x0407,
    SWEEP_MUTEX = 0x0408,
    SWEEP_RW = 0x0409,
    SWEEP_ONCE = 0x040A,
} SweepQualifier;

typedef enum: uint16_t {
    SWEEP_FUNC = 0x0501,
    SWEEP_SCOPE = 0x0502,
    SWEEP_DESTRUCTOR = 0x0503,

    SWEEP_ERR = 0x0504,
    SWEEP_SWITCH = 0x0505,
    SWEEP_CASE = 0x0506,
    SWEEP_DEFAULT = 0x0507,
    SWEEP_IF = 0x0508,
    SWEEP_ELSE = 0x0509,

    SWEEP_FOR = 0x050A,
    SWEEP_WHILE = 0x050B,
    SWEEP_CONTINUE = 0x050C,
    SWEEP_BREAK = 0x050D,

    SWEEP__PAUSE = 0x050E,      // __pause
    SWEEP__CONTINUE = 0x050F,   // __continue

    SWEEP_FROM = 0x0511,
    SWEEP_IMPORT = 0x0512,

    SWEEP_SIMD = 0x0513,
    SWEEP_INHERITS = 0x0514,

    SWEEP_UNSAFE = 0x0515,
    SWEEP_COMPILER = 0x0516,
    SWEEP_LLVM = 0x0517,
    SWEEP_CLANG = 0x0518,
    SWEEP_ASM = 0x0519,

    SWEEP_SPAWN = 0x051A,
    SWEEP_MERGE = 0x051B,

    SWEEP_RET = 0x051C,
    SWEEP_MAIN = 0x051D,
} SweepKeywords;

typedef enum: uint16_t {
    SWEEP_IDENTIFIER = 0x0601,
    SWEEP_INT_LITERAL = 0x0602,
    SWEEP_FLOAT_LITERAL = 0x0603,
    SWEEP_STR_LITERAL = 0x0604,
    SWEEP_CHAR_LITERAL = 0x0605,
    SWEEP_FLAGS = 0x0606,
    SWEEP_BOOL_TRUE_LITERAL = 0x0607,
    SWEEP_BOOL_FALSE_LITERAL = 0x0608,
    SWEEP_EOF = 0x0609,
    SWEEP_LLVM_IR = 0x060A,
    SWEEP_CLANG_BLOCK = 0x060B,
    SWEEP_ASM_x86_64 = 0x060C,
    SWEEP_ASM_ARM64 = 0x060D,
    SWEEP_ASM_x86 = 0x060E,
    SWEEP_ASM_ARM32 = 0x060F,
} SweepLiterals;

#endif
