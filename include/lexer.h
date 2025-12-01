#ifndef LEXER_H
#define LEXER_H
#include<stddef.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_ERR,

    // Keywords
    TOKEN_VAR,
    TOKEN_PTR,
    TOKEN_CONST,
    TOKEN_FUNC,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_RET,

    // Sweep Specific
    TOKEN_UNSAFE,
    TOKEN_SHARED,
    TOKEN_SPAWN,
    TOKEN_GPU,

    // DataTypes
    TOKEN_SIGNED,
    TOKEN_UNSIGNED,

    TOKEN_SHORT,
    TOKEN_LONG,

    TOKEN_NULL,
    TOKEN_INT,
    TOKEN_UINT, // Unsigned alias
    //TOKEN_UFLOAT, for future versions (might be useful for higher float ranges)
    //TOKEN_UDOUBLE, same as float
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
    TOKEN_VOID,
    TOKEN_CHAR,
    TOKEN_STR,
    TOKEN_BOOL,

    // User Defined
    TOKEN_STRUCT,
    TOKEN_ENUM,
    TOKEN_UNION,
    TOKEN_ALIAS,
    //TOKEN_CLASS, for future versions

    // Storage Class
    TOKEN_STATIC,
    TOKEN_AUTO,
    TOKEN_EXTERN,
    TOKEN_REGISTER,

    // Symbols
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_LBRACKET,     // [
    TOKEN_RBRACKET,     // ]
    TOKEN_COLON,        // :
    TOKEN_SEMICOLON,    // ;
    TOKEN_COMMA,        // ,
    TOKEN_DOT,          // .
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_STAR,         // * (Multiply / Dereference)
    TOKEN_SLASH,        // /
    TOKEN_PERCENT,      // % 
    TOKEN_EQ,           // =
    TOKEN_EQ_EQ,        // ==
    TOKEN_NOT,          // ! 
    TOKEN_NOT_EQ,       // !=
    TOKEN_GT,           // >
    TOKEN_GTE,          // >=
    TOKEN_LT,           // <
    TOKEN_LTE,          // <=
    TOKEN_AND,          // &  (Memory Address / Bitwise AND)
    TOKEN_OR,           // |  (Bitwise OR)
    TOKEN_AND_AND,      // && (Logical AND)
    TOKEN_OR_OR,        // || (Logical OR)

    // Values
    TOKEN_IDENTIFIER,
    TOKEN_INT_LIT,
    TOKEN_FLOAT_LIT,
    TOKEN_STRING_LIT,
    TOKEN_CHAR_LIT,
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

typedef struct {
    const char* start;
    const char* current;
    int line;
} Lexer;

void lexer_init(Lexer* lexer, const char* source);
Token lexer_next_token(Lexer* lexer);

#endif