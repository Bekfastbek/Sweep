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
    NODE_GPU_BLOCK,
    NODE_SPAWN,
    NODE_CALL,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR
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
            char* target_name;
            struct ASTNode* expr;
        } assign;

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
            int is_const;
            int is_shared;
        } var_decl;

        struct {
            char* name;
            TokenType return_type;
            struct ASTNode** params;
            struct ASTNode* body;
            // TODO:    Need to add parameters later
        } func_decl;

        struct {
            struct ASTNode* expr;
        } ret_stmt;

        struct {
            struct ASTNode* target;
        } spawn_stmt;

        struct {
            char* name; 
            struct ASTNode** args;
        } call;

        struct {
            struct ASTNode* condition;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } if_stmt;

        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_stmt;

        struct {
            struct ASTNode* init;
            struct ASTNode* condition;
            struct ASTNode* increment;
            struct ASTNode* body;
        } for_stmt;
    } data;
} ASTNode;

ASTNode* ast_new_int(int val);
ASTNode* ast_new_float(double val);
ASTNode* ast_new_unary(TokenType op, ASTNode* operand);
ASTNode* ast_new_bin_op(ASTNode* left, TokenType op, ASTNode* right);
ASTNode* ast_new_var_decl(const char* name, TokenType type, int ptr_depth, ASTNode* init, int is_const);
ASTNode* ast_new_func_decl(char* name, TokenType return_type, ASTNode** params, ASTNode* body);
ASTNode* ast_new_return(ASTNode* value);
ASTNode* ast_new_spawn(ASTNode* target);
ASTNode* ast_new_call(char* name, ASTNode** args);
ASTNode* ast_new_block();
ASTNode* ast_new_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch);
ASTNode* ast_new_while(ASTNode* cond, ASTNode* body);
ASTNode* ast_new_for(ASTNode* init, ASTNode* cond, ASTNode* inc, ASTNode* body);
ASTNode* ast_new_assign(char* name, ASTNode* expr);
void ast_block_add(ASTNode* block, ASTNode* child);

#endif