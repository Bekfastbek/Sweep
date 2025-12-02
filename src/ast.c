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

ASTNode* ast_new_var_decl(const char* name, TokenType type, int ptr_depth, ASTNode* init, int is_const, int is_shared) {
    ASTNode* node = alloc_node(NODE_VAR_DECL);
    node -> data.var_decl.name = strdup(name);
    node -> data.var_decl.data_type = type;
    node -> data.var_decl.pointer_depth = ptr_depth;
    node -> data.var_decl.init_expr = init;
    node -> data.var_decl.is_const = is_const;
    node -> data.var_decl.is_shared = is_shared;
    return node;
}

ASTNode* ast_new_func_decl(char* name, TokenType return_type, ASTNode** params, ASTNode* body) {
    ASTNode* node = alloc_node(NODE_FUNC_DECL);
    node -> data.func_decl.name = strdup(name);
    node -> data.func_decl.return_type = return_type
    node -> data.func_decl.params = params;
    node -> data.func_decl.body = body;
    return node;
}

ASTNode* ast_new_return(ASTNode* value) {
    ASTNode* node = alloc_node(NODE_RETURN);
    node -> data.ret_stmt.value = value;
    return node;
}

ASTNode* ast_new_spawn(ASTNode* target) {
    ASTNode* node = alloc_node(NODE_SPAWN);
    node -> data.spawn_stmt.target = target;
    return node;
}

ASTNode* ast_new_call(char* name, ASTNode** args) {
    ASTNode* node = alloc_node(NODE_CALL);
    node -> data.call.name = strdup(name);
    node -> data.call.args = args;
    return node;
}

ASTNode* ast_new_block() {
    ASTNode* node = alloc_node(NODE_BLOCK);
    node -> data.block.statements = NULL;
    return node;
}

ASTNode* ast_new_if(ASTNode* cond, ASTNode* then_branch, ASTNode* else_branch){
    ASTNode* node = alloc_node(NODE_IF);
    node -> data.if_stmt.condition = cond;
    node -> data.if_stmt.then_branch = then_branch;
    node -> data.if_stmt.else_branch = else_branch;
    return node;
}

ASTNode* ast_new_while(ASTNode* cond, ASTNode* body){
    ASTNode* node = alloc_node(NODE_WHILE);
    node -> data.while_stmt.condition = cond;
    node -> data.while_stmt.body = body;
    return node;
}

ASTNode* ast_new_for(ASTNode* init, ASTNode* cond, ASTNode* inc, ASTNode* body){
    ASTNode* node = alloc_node(NODE_FOR);
    node -> data.for_stmt.init = init;
    node -> data.for_stmt.condition = cond;
    node -> data.for_stmt.increment = inc;
    node -> data.for_stmt.body = body;
    return node;
}

ASTNode* ast_new_assign(char* name, ASTNode* expr) {
    ASTNode* node = alloc_node(NODE_ASSIGN);
    node -> data.assign.target_name = strdup(name);
    node -> data.assign.expr = expr;
    return node;
}

void ast_block_add(ASTNode* block, ASTNode* child) {
    if(block -> type != NODE_BLOCK && block -> type != NODE_UNSAFE_BLOCK && block -> type != NODE_GPU_BLOCK) {
        fprintf(stderr, "[AST] Error: Trying to add statements to non-block\n");
        return;
    }
    arrput(block -> data.block.statements, child);
}