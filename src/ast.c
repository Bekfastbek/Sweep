#include "../include/ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include "../include/stb_ds.h"

static ASTNode* alloc_node(NodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if(!node) {
        fprintf(stderr, "[AST] Fatal: Out of Memory.\n");
        exit(1);
    }
    memset(node, 0, sizeof(ASTNode));
    node -> type = type;
    return node;
}

ASTNode* ast_new_int(int val) {
    ASTNode* node = alloc_node(NODE_INT_LIT);
    node -> data.int_val = val;
    return node;
}

ASTNode* ast_new_float(double val) {
    ASTNode* node = alloc_node(NODE_FLOAT_LIT);
    node -> data.float_val = val;
    return node;
}

ASTNode* ast_new_unary(TokenType op, ASTNode* operand) {
    ASTNode* node = alloc_node(NODE_UNARY_OP);
    node -> data.unary_op.op = op;
    node -> data.unary_op.operand = operand;
    return node;
}

ASTNode* ast_new_bin_op(ASTNode* left, TokenType op, ASTNode* right) {
    ASTNode* node = alloc_node(NODE_BIN_OP);
    node -> data.bin_op.left = left;
    node -> data.bin_op.op = op;
    node -> data.bin_op.right = right;
    return node;
}

ASTNode* ast_new_var_decl(const char* name, TokenType type, int ptr_depth, ASTNode* init) {
    ASTNode* node = alloc_node(NODE_VAR_DECL);
    node -> data.var_decl.name = strdup(name);
    node -> data.var_decl.data_type = type;
    node -> data.var_decl.pointer_depth = ptr_depth;
    node -> data.var_decl.init_expr = init;
    return node;
}

ASTNode* ast_new_block() {
    ASTNode* node = alloc_node(NODE_BLOCK);
    node -> data.block.statements = NULL;
    return node;
}

void ast_block_add(ASTNode* block, ASTNode* child) {
    if(block -> type != NODE_BLOCK && block -> type != NODE_UNSAFE_BLOCK && block -> type != NODE_GPU_BLOCK) {
        fprintf(stderr, "[AST] Error: Trying to add statements to non-block\n");
        return;
    }
    arrput(block -> data.block.statements, child);
}