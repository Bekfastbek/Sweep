// This whole file is a mess so a self-reminder to organise everything later
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void advance(Parser* p) {
    p -> previous = p -> current;
    for(;;) {
        p -> current = lexer_next_token(p -> lexer);
        if(p -> current.type != TOKEN_ERR) {
            break;
        }
        fprintf(stderr, "[Line %d] Lex Error: %s\n", p -> current.line, p -> current.start);
    }
}

static int check(Parser* p, TokenType type) {
    return p -> current.type == type;
}

static int match(Parser* p, TokenType type) {
    if(check(p, type)) {
        advance(p);
        return 1;
    }
    return 0;
}

static void consume_terminator(Parser* p) {
    if(match(p, TOKEN_SEMICOLON)) {
        while(match(p, TOKEN_NEWLINE)) {
        }
        return;   
    }
    if(match(p, TOKEN_NEWLINE)) {
        while(match(p, TOKEN_NEWLINE)) {
        }
        return;
    }
    if(check(p, TOKEN_RBRACE) || check(p, TOKEN_EOF)) {
        return;
    }
    fprintf(stderr, "[Line %d] Error: Expected end of Statement (Newline or ';')\n", p->current.line);
    p->had_error = 1;
    advance(p);
}

static void consume(Parser* p, TokenType type, const char* message) {
    if( p -> current.type == type) {
        advance(p);
        return;
    }
    fprintf(stderr, "[Line %d] Parse Error: %s\n", p -> current.line, message);
    p -> had_error = 1;
}

static ASTNode* parse_expression(Parser* p);
static ASTNode* parse_statement(Parser* p);
static ASTNode* parse_block(Parser* p);
static ASTNode* parse_unary(Parser* p);
static ASTNode* parse_term(Parser* p);
static ASTNode* parse_logic(Parser *p); // Too lazy to fix ordering so I will just declare everything here for now but need to order properly soon

static ASTNode* parse_primary(Parser* p) {
    if(match(p, TOKEN_INT_LIT)) {
        int val = (int)strtol(p -> previous.start, NULL, 0);
        return ast_new_int(val);
    }
    if(match(p, TOKEN_FLOAT_LIT)) { 
        double val = atoi(p -> previous.start);
        return ast_new_float(val);
    }
    if(match(p, TOKEN_IDENTIFIER)) {
        int len = p -> previous.length;
        char* name = malloc(len + 1);
        if(!name) {
            fprintf(stderr, "Memory Error\n");
            exit(1);
        }
        memcpy(name, p -> previous.start, len);
        name[len] = '\0';
        if(match(p, TOKEN_LPAREN)) {
            ASTNode** args = NULL;
            if(!check(p, TOKEN_RPAREN)) {
                do {
                    ASTNode* arg = parse_expression(p);
                    arrput(args, arg);
                } while(match(p, TOKEN_COMMA)) {
                }
            }
            consume(p, TOKEN_RPAREN, "Expected ')' after arguments.");
            ASTNode* callnode = ast_new_call(name, args);
            free(name;)
            return callnode;
        }
        ASTNode* node = malloc(sizeof(ASTNode));
        memset(node, 0, sizeof(ASTNode));
        node -> type = NODE_IDENTIFIER;
        node -> data.name = name;
        return node;
    }
    if(match(p, TOKEN_LPAREN)) {
        ASTNode* expr = parse_expression(p);
        consume(p, TOKEN_RPAREN, "Expected ')' after expression.");
        return expr;
    }
    fprintf(stderr, "[Line %d] Error: Expected expression. \n", p -> current.line);
    return NULL;
}

static ASTNode* parse_unary(Parser* p) {
    while(match(p, TOKEN_MINUS) || match(p, TOKEN_NOT) || match(p, TOKEN_STAR) || match(p, TOKEN_AND)) {
        TokenType op = p -> previous.type;
        ASTNode* right = parse_unary(p);
        return ast_new_unary(op, right);
    }
    return parse_primary(p);
}

static ASTNode* parse_term(Parser* p) {
    ASTNode* left = parse_unary(p);
    while(match(p, TOKEN_STAR) || match(p, TOKEN_SLASH) || match(p, TOKEN_PERCENT)) {
        TokenType op = p -> previous.type;
        ASTNode* right = parse_unary(p);
        left = ast_new_bin_op(left, op, right);
    }
    return left;
}

static ASTNode* parse_logic(Parser* p) {
    ASTNode* left = parse_term(p);
    while(match(p, TOKEN_PLUS) || match(p, TOKEN_MINUS) || match(p, TOKEN_LT) || match(p, TOKEN_GT) || match(p, TOKEN_EQ_EQ)) {
        TokenType op = p -> previous.type;
        ASTNode* right = parse_term(p);
        left = ast_new_bin_op(left, op, right);
    }
    return left;
}

static ASTNode* parse_expression(Parser* p) {
    ASTNode* left = parse_logic(p);
    if (match(p, TOKEN_EQ)) {
        ASTNode* value = parse_expression(p);
        if (left->type == NODE_IDENTIFIER) {
            char* name = left->data.name;
            return ast_new_assign(name, value);
        }
        fprintf(stderr, "[Line %d] Error: Invalid assignment target.\n", p->current.line);
    }
    return left;
}

static int parse_type_info(Parser* p, TokenType* out_type) {
    int depth = 0;
    while(match(p, TOKEN_PTR)) {
        depth++;
    }
    if(match(p, TOKEN_INT)) {
        *out_type = TOKEN_INT;
    } else if(match(p, TOKEN_FLOAT)) {
        *out_type = TOKEN_FLOAT;
    } else if(match(p, TOKEN_VOID)) {
        *out_type = TOKEN_VOID;
    } else {
        consume(p, TOKEN_INT, "Expected valid type (int, float, etc.)");
    }
    return depth;
}

static ASTNode* parse_var_decl(Parser* p, int is_const, int is_shared, int start_depth) {
    TokenType type;
    int parsed_depth = parse_type_info(p, &type);
    int total_depth = start_depth + parsed_depth;
    consume(p, TOKEN_IDENTIFIER, "Expected a variable name.");
    char name_buff[64];
    int len = p -> previous.length < 63 ? p -> previous.length : 63;
    memcpy(name_buff, p -> previous.start, len);
    name_buff[len] = '\0';
    ASTNode* init = NULL;
    if(match(p, TOKEN_EQ)) {
        init = parse_expression(p);
    }
    consume_terminator(p);
    return ast_new_var_decl(name_buff, type, total_depth, init, is_const, is_shared);
}

static ASTNode* parse_function(Parser* p) {
    consume(p, TOKEN_IDENTIFIER, "Expected function name.");
    char* name = strdup_token(p -> previous);
    consume(p, TOKEN_LPAREN, "Expected '(' after function name.");
    ASTNode** params = NULL;
    if (!check(p, TOKEN_RPAREN)) {
        do {
            TokenType type;
            int depth = parse_type_info(p, &type);
            consume(p, TOKEN_IDENTIFIER, "Expected param name.");
            char pname[64];
            int l = p->previous.length < 63 ? p->previous.length : 63;
            memcpy(pname, p->previous.start, l);
            pname[l] = '\0';
            ASTNode* param = ast_new_var_decl(pname, type, depth, NULL, 0, 0);
            arrput(params, param);
        } while (match(p, TOKEN_COMMA)) {
        }
    }
    consume(p, TOKEN_RPAREN, "Expected ')' after params.");
    TokenType ret = TOKEN_VOID;
    if (check(p, TOKEN_INT) || check(p, TOKEN_FLOAT) || check(p, TOKEN_VOID) || check(p, TOKEN_PTR)) {
        parse_type_info(p, &ret);
    }
    consume(p, TOKEN_LBRACE, "Expected '{' for function body.");
    ASTNode* body = parse_block(p);
    return ast_new_func_decl(name, ret, params, body);
}

static ASTNode* parse_block(Parser* p) {
    ASTNode* block = ast_new_block();
    while(!check(p, TOKEN_RBRACE) && !check(p, TOKEN_EOF)) {
        if(match(p, TOKEN_NEWLINE)) {
            continue;
        }
        ASTNode* stmt = parse_statement(p);
        if(stmt) {
            ast_block_add(block, stmt);
        }
    }
    consume(p, TOKEN_RBRACE, "Expected '}' after block.");
    return block;
}

static ASTNode* parse_if_statement(Parser* p) {
    ASTNode* condition = parse_expression(p);
    consume(p, TOKEN_LBRACE, "Expected '{' after if condition.");
    ASTNode* then_branch = parse_block(p);
    ASTNode* else_branch = NULL;
    while(check(p, TOKEN_NEWLINE)) {
        advance(p);
    }
    if(match(p, TOKEN_ELSE)) {
        if(check(p, TOKEN_IF)) {
            match(p, TOKEN_IF);
            else_branch = parse_if_statement(p);
        } else {
            consume(p, TOKEN_LBRACE, "Expected '{' after else condition.");
            else_branch = parse_block(p);
        }
    }
    return ast_new_if(condition, then_branch, else_branch);
}

static ASTNode* parse_while_statement(Parser* p) {
    ASTNode* condition = parse_expression(p);
    consume(p, TOKEN_LBRACE, "Expected '{' after while condition.");
    ASTNode* body = parse_block(p);
    return ast_new_while(condition, body);
}

static ASTNode* parse_for_statement(Parser* p) {
    ASTNode* init = NULL;
    if(!match(p, TOKEN_SEMICOLON)) {
        if(match(p, TOKEN_VAR)) {
            init = parse_var_decl(p, 0, 0);
        } else if(match(p, TOKEN_PTR)) {
            init = parse_var_decl(p, 0, 1);
        } else if(match(p, TOKEN_CONST)) {
            init = parse_var_decl(p, 1, 0);
        } else {
            init = parse_expression(p);
            consume(p, TOKEN_SEMICOLON, "Expected ';' after loop initializer.");
        }
    }
    ASTNode* cond = NULL;
    if(!match(p, TOKEN_SEMICOLON)) {
        cond = parse_expression(p);
        consume(p, TOKEN_SEMICOLON, "Expected ';' after loop condition.");
    }
    ASTNode* inc = NULL;
    if(!check(p, TOKEN_LBRACE)) {
        inc = parse_expression(p);
    }
    consume(p, TOKEN_LBRACE, "Expected '{' after for clauses.");
    ASTNode* body = parse_block(p);
    return ast_new_for(init, cond, inc, body);
}

static ASTNode* parse_statement(Parser* p) {
    if(match(p, TOKEN_VAR)) {
        return parse_var_decl(p, 0, 0, 0);
    }
    if(match(p, TOKEN_PTR)) {
        return parse_var_decl(p, 0, 0, 1);
    }
    if(match(p, TOKEN_CONST)) {
        return parse_var_decl(p, 1, 0, 0);
    }
    if(match(p, TOKEN_SHARED)) {
        return parse_var_decl(p, 0, 1, 0);
    }
    if(match(p, TOKEN_UNSAFE)) {
        consume(p, TOKEN_LBRACE, "Expected '{' after Unsafe.");
        ASTNode* block = parse_block(p);
        block -> type = NODE_UNSAFE_BLOCK;
        return block;
    }
    if(match(p, TOKEN_GPU)) {
        consume(p, TOKEN_LBRACE, "Expected '{' after GPU");
        ASTNode* block = parse_block(p);
        block -> type = NODE_GPU_BLOCK;
        return block;
    }
    if(match(p, TOKEN_LBRACE)) {
        return parse_block(p);
    }
    if(match(p, TOKEN_FUNC)) {
        return parse_function(p);
    }
    if(match(p, TOKEN_RET)) {
        ASTNode* val = NULL;
        if(!check(p, TOKEN_NEWLINE) && !check(p, TOKEN_RBRACE)) {
            var = parse_expression(p);
        }
        consume_terminator(p);
        return ast_new_return(val);
    }
    if(match(p, TOKEN_SPAWN)) {
        ASTNode* target = parse_expression(p);
        consume_terminator(p);
        return ast_new_spawn(target);
    }
    if(match(p, TOKEN_IF)) {
        return parse_if_statement(p);
    }
    if(match(p, TOKEN_ELSE)) {
        consume(p, TOKEN_ELSE, "Unexpected 'else' without a previous 'if'.");
        return NULL;
    }
    if(match(p, TOKEN_WHILE)) {
        return parse_while_statement(p);
    }
    if(match(p, TOKEN_FOR)) {
        return parse_for_statement(p);
    }
    ASTNode* expr = parse_expression(p);
    consume_terminator(p);
    return expr;
}

void parser_init(Parser* parser, Lexer* Lexer) {
    parser -> lexer = Lexer;
    parser -> had_error = 0;
    parser -> panic_mode = 0;
    advance(parser);
}

ASTNode* parser_parse(Parser* p) {
    ASTNode* root = ast_new_block();
    printf("[DEBUG] Parser loop started...\n");
    while(!match(p, TOKEN_EOF)) {
        if(match(p, TOKEN_NEWLINE)) {
            continue;
        }
        ASTNode* stmt = parse_statement(p);
        if(stmt) {
            ast_block_add(root, stmt);
        }
    }
    printf("[DEBUG] Parser loop finished.\n");
    return root;
}