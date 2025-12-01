#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/ast.h"

const char* token_type_to_str(TokenType type) {
    switch(type) {
        case TOKEN_VAR: return "VAR";
        case TOKEN_PTR: return "PTR";
        case TOKEN_CONST: return "CONST";
        case TOKEN_FUNC: return "FUNC";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_RET: return "RET";
        case TOKEN_UNSAFE: return "UNSAFE";
        case TOKEN_SHARED: return "SHARED";
        case TOKEN_GPU: return "GPU";
        case TOKEN_INT: return "TYPE(INT)";
        case TOKEN_FLOAT: return "TYPE(FLOAT)";
        case TOKEN_VOID: return "TYPE(VOID)";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INT_LIT: return "LITERAL(INT)";
        case TOKEN_FLOAT_LIT: return "LITERAL(FLOAT)";
        case TOKEN_STRING_LIT: return "LITERAL(STRING)";
        case TOKEN_LBRACE: return "{";
        case TOKEN_RBRACE: return "}";
        case TOKEN_LPAREN: return "(";
        case TOKEN_RPAREN: return ")";
        case TOKEN_SEMICOLON: return ";";
        case TOKEN_EQ: return "=";
        case TOKEN_EQ_EQ: return "==";
        case TOKEN_PLUS: return "+";
        case TOKEN_STAR: return "*";
        case TOKEN_AND: return "&";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERR: return "ERROR";
        default: return "UNKNOWN_TOKEN";
    }
}

int ast_test() {
    printf("Testing AST Construction...");
    ASTNode* five = ast_new_int(5);
    ASTNode* ten = ast_new_int(10);
    ASTNode* math = ast_new_bin_op(five, TOKEN_PLUS, ten);
    ASTNode* decl = ast_new_var_decl("x", TOKEN_INT, 0, math);
    printf("AST Built Successfully!\n");
    printf("Root Type: %d (Should be %d for VAR_DECL)\n", decl -> type, NODE_VAR_DECL);
    printf("Var Name: %s\n", decl -> data.var_decl.name);
    ASTNode* rhs = decl -> data.var_decl.init_expr;
    printf("Experession Operator: %d (Should be %d for +)\n", rhs -> data.bin_op.op, TOKEN_PLUS);
    return 0;
}

int main() {
    ast_test();
    const char* source = 
        "func main() {\n"
        "  // 1. Pointers & Types\n"
        "  var int x = 10;\n"
        "  ptr int p = &x;\n"
        "\n"
        "  // 2. Unsafe & GPU\n"
        "  unsafe {\n"
        "      *p = 20.5;\n"
        "  }\n"
        "  gpu for (i in 0..10) {}\n"
        "}";

    printf("Compiling...\n%s\n", source);
    Lexer l;
    lexer_init(&l, source);
    int count = 0;
    Token t = lexer_next_token(&l);
    while (t.type != TOKEN_EOF) {
        printf("[%02d] %-15s | '%.*s'\n", 
               t.line, 
               token_type_to_str(t.type), 
               t.length, 
               t.start);
        count++;
        if (count > 50) {
            printf("Aborting...\n");
            break;
        }

        t = lexer_next_token(&l);
    }

    printf("Test Completed!\n");
    return 0;
}