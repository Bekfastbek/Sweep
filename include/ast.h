#ifndef AST_H
#define AST_H
#include "lexer.h"

typedef enum {
    NODE_INT_LIT,
    NODE_FLOAT_LIT,
    NODE_STRING_LIT,
    NODE_BOOL_LIT,
    NODE_IDENTIFIER,
    NODE_VAR_DECL,
    NODE_FUNC_DECL,
    NODE_BLOCK,
    NODE_BIN_OP,
    NODE_UNARY_OP,
    NODE_ASSIGN,
    NODE_RETURN,
    NODE_UNSAFE_BLOCK,
    NODE_GPU_BLOCK
} NodeType;

typedef struct ASTNode {
    NodeType type;

    union {
        int int_val;
        double float_val;
        char* string_val;
        int bool_val;
        char* name;

        struct {
            struct ASTNode* left;
            TokenType op;
            struct ASTNode* right;
        } bin_op;

        struct {
            TokenType op;
            struct ASTNode* operand;
        } unary_op;

        struct {
            struct ASTNode** statements;
        } block;

        struct {
            char* name;
            TokenType data_type;
            int pointer_depth;
            struct ASTNode* init_expr;
        } var_decl;

        struct {
            char* name;
            TokenType return_type;
            struct ASTNode* body;
            // TODO:    Need to add parameters later
        } func_decl;

        struct {
            struct ASTNode* expr;
        } ret_stmt;
    } data;
} ASTNode;

ASTNode* ast_new_int(int val);
ASTNode* ast_new_float(double val);
ASTNode* ast_new_unary(TokenType op, ASTNode* operand);
ASTNode* ast_new_bin_op(ASTNode* left, TokenType op, ASTNode* right);
ASTNode* ast_new_var_decl(const char* name, TokenType type, int ptr_depth, ASTNode* init);
ASTNode* ast_new_block();
void ast_block_add(ASTNode* block, ASTNode* child);

#endif