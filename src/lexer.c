#include "../include/lexer.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void lexer_init(Lexer* lexer, const char* source) {
    lexer -> start = source;
    lexer -> current = source;
    lexer -> line = 1;
}

static bool is_at_end(Lexer* l) {
    return *l -> current == '\0';
}

static char advance(Lexer* l) {
    l -> current++;
    return l -> current[-1];
}

static char peek(Lexer* l) {
    return *l -> current;
}

static char peek_next(Lexer* l) {
    if(is_at_end(l)) { 
        return '\0';
    }
    return l -> current[1];
}

static Token make_token(Lexer* l, TokenType type) {
    Token token;
    token.type = type;
    token.start = l->start;
    token.length = (int)(l->current - l->start);
    token.line = l->line;
    return token;
}

static Token error_token(Lexer* l, const char* message) {
    Token token;
    token.type = TOKEN_ERR;
    token.start = message;
    token.length = (int)(l -> current - l -> start);
    token.line = l -> line;
    return token;
}

static void skip_whitespace(Lexer* l) {
    for(;;) {
        char c = peek(l);
        switch(c) {
            case ' ':
            case '\r':
            case '\t':
                advance(l);
                break;
            case '/':
                if (peek_next(l) == '/') {
                    while(peek(l) != '\n' && !is_at_end(l)) {
                        advance(l);
                    } 
                } else {
                        return;
                    }
                    break;
            default:
                return;
        }
    }
}

static TokenType check_keyword(Lexer* l, int start, int length, const char* rest, TokenType type) {
    if(l -> current - l -> start == start + length && memcmp(l -> start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Lexer* l) {
    switch(l -> start[0]) {
        case 'c':
            if( l -> current - l -> start > 1) {
                switch(l -> start[1]) {
                    case 'h':
                        return check_keyword(l, 2, 3, "ar", TOKEN_CHAR);
                    case 'o':
                        return check_keyword(l, 2, 3, "nst", TOKEN_CONST);
                }
            }
            break;
        case 'e':
            if(l -> current - l -> start > 1) {
                switch(l -> start[1]) {
                    case 'l':
                        return check_keyword(l, 2, 2, "se", TOKEN_ELSE);
                    case 'n':
                        return check_keyword(l, 2, 2, "um", TOKEN_ENUM);
                    case 'x':
                        return check_keyword(l, 2, 4, "tern", TOKEN_EXTERN);
                }
            }
            break;
        case 'f':
            if (l -> current - l -> start > 1) {
                switch(l -> start[1]) {
                    case 'a':
                        return check_keyword(l, 2, 3, "lse", TOKEN_INT_LIT);
                    case 'l':
                        return check_keyword(l, 2, 3, "oat", TOKEN_FLOAT);
                    case 'o':
                        return check_keyword(l, 2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return check_keyword(l, 2, 2, "nc", TOKEN_FUNC);
                }
            }
            break;
        case 'g':
            return check_keyword(l, 1, 2, "pu", TOKEN_GPU);
        case 'i':
            if(l -> current - l -> start > 1) {
                if(l -> start[1] == 'f') {
                    return TOKEN_IF;
                }
                return check_keyword(l, 1, 2, "nt", TOKEN_INT);
            }
            break;
        case 'p':
            return check_keyword(l, 1, 2, "tr", TOKEN_PTR);
        case 'r':
            return check_keyword(l, 1, 2, "et", TOKEN_RET);
        case 's':
            if(l -> current - l -> start > 1) {
                switch(l -> start[1]) {
                    case 'h':
                    case 'p':
                    case 't':
                        if(l -> start[2] == 'r') {
                            return TOKEN_STR;
                        }
                        return check_keyword(l, 2, 4, "ruct", TOKEN_STRUCT);
                }
            }
            break;
        case 't':
            return check_keyword(l, 1, 3, "rue", TOKEN_INT_LIT);
        case 'u':
             if (l->current - l->start > 1) {
                 switch (l->start[1]) {
                     case 'i': return check_keyword(l, 2, 2, "nt", TOKEN_UINT);
                     case 'n':
                         if (l->current - l->start > 2) {
                            if (l->start[2] == 's') {
                                if (l->current - l->start == 6) {
                                    return check_keyword(l, 3, 3, "afe", TOKEN_UNSAFE);
                                }
                                return check_keyword(l, 3, 5, "igned", TOKEN_UNSIGNED);
                            }
                            if (l->start[2] == 'i') return check_keyword(l, 3, 2, "on", TOKEN_UNION);
                         }
                         break;
                 }
             }
             break;
        case 'v':
            if(l -> current - l -> start > 1) {
                    if(l -> start[1] == 'a') {
                        return check_keyword(l, 2, 1, "r", TOKEN_VAR);
                    } 
                    if(l -> start[1] == 'o') {
                        return check_keyword(l, 2, 2, "id", TOKEN_VOID);
                    }
                }
                break;
        case 'w':
            return check_keyword(l, 1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token string(Lexer* l) {
    while(peek(l) != '"' && !is_at_end(l)) {
        if(peek(l) == '\n') {
            l -> line++;
        }
        advance(l);
    }
    if(is_at_end(l)) {
        return error_token(l, "Unterminated String.");
    }
    advance(l);
    return make_token(l, TOKEN_STRING_LIT);
}

static Token character(Lexer* l) {
    if(is_at_end(l)) {
        return error_token(l, "Unterminated char.");
    }
    advance(l);
    if(peek(l) != '\'') {
        return error_token(l, "Expected closing '");
    }
    advance(l);
    return make_token(l, TOKEN_CHAR_LIT);
}

static Token number(Lexer* l) {
    char prev = l -> current[-1];
    if(prev == '0' && (peek(l) == 'x' || peek(l) == 'X')) {
        advance(l);
        while(isxdigit(peek(l))) {
            advance(l);
        }
        return make_token(l, TOKEN_INT_LIT);
    }
    while(isdigit(peek(l))) {
        advance(l);
    }
    if(peek(l) == '.' && isdigit(peek_next(l))) {
        advance(l);
        while(isdigit(peek(l))) {
            advance(l);
        }
        return make_token(l, TOKEN_FLOAT_LIT);  
    }
    return make_token(l, TOKEN_INT_LIT);
}

static Token identifier(Lexer* l) {
    while(isalnum(peek(l)) || peek(l) == '_') {
        advance(l);
    }
    return make_token(l, identifier_type(l));
}

Token lexer_next_token(Lexer* l) {
    skip_whitespace(l);
    l -> start = l -> current;
    if(is_at_end(l)) {
        return make_token(l, TOKEN_EOF);
    }
    char c = advance(l);
    if(c == '\n') {
        l -> line++;
        return make_token(l, TOKEN_NEWLINE);
    }
    if(isalpha(c) || c == '_') {
        return identifier(l);
    }
    if(isdigit(c)) {
        return number(l);
    }
    switch (c) {
        case '(': return make_token(l, TOKEN_LPAREN);
        case ')': return make_token(l, TOKEN_RPAREN);
        case '{': return make_token(l, TOKEN_LBRACE);
        case '}': return make_token(l, TOKEN_RBRACE);
        case '[': return make_token(l, TOKEN_LBRACKET);
        case ']': return make_token(l, TOKEN_RBRACKET);
        case ';': return make_token(l, TOKEN_SEMICOLON);
        case ',': return make_token(l, TOKEN_COMMA);
        case '.': return make_token(l, TOKEN_DOT);
        case '-': return make_token(l, TOKEN_MINUS);
        case '+': return make_token(l, TOKEN_PLUS);
        case '/': return make_token(l, TOKEN_SLASH);
        case '*': return make_token(l, TOKEN_STAR);
        case '%': return make_token(l, TOKEN_PERCENT);
        case '!':
            return make_token(l, (peek(l) == '=') ? (advance(l), TOKEN_NOT_EQ) : TOKEN_NOT);
        case '=':
            return make_token(l, (peek(l) == '=') ? (advance(l), TOKEN_EQ_EQ) : TOKEN_EQ);
        case '<':
            return make_token(l, (peek(l) == '=') ? (advance(l), TOKEN_LTE) : TOKEN_LT);
        case '>':
            return make_token(l, (peek(l) == '=') ? (advance(l), TOKEN_GTE) : TOKEN_GT);
        case '&':
            return make_token(l, (peek(l) == '&') ? (advance(l), TOKEN_AND_AND) : TOKEN_AND);
        case '|':
            return make_token(l, (peek(l) == '|') ? (advance(l), TOKEN_OR_OR) : TOKEN_OR);
        case '"':
            return string(l);
        case '\'':
            return character(l);
    }
    return error_token(l, "Unexpected Character.");
}