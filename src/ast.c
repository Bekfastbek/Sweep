#include <stdint.h>


typedef enum: uint16_t {
    OP_ADDITION = 0x0F0,
    OP_SUBTRACTION = 0x0F1,
    OP_MULTIPLY = 0x0F2,
    OP_DIVIDE = 0x0F3,
    STATE_NUMBER = 0x0F4,
    STATE_FLOAT = 0x0F5,
    STATE_IDENTIFIER = 0x0F6,
} State;


typedef struct {
    State state;
    union {

    };
    struct AST* left;
    struct AST* right;
} AST;