#ifndef DFA_H
# define DFA_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

typedef struct s_DFA t_DFA;
typedef void (*t_Action)(void   *);
typedef enum {
    SPACE,
    SIGN,
    OP,
    DIG,
    DEFAULT,
    CHARS
} t_char;

typedef enum {
    NONE = -1,
    SUBS,
    ADD,
    MULT,
    DIV,
    OPS_NUM
} t_operators;
typedef struct s_sym {
    t_operators mask;
    int         priority;
    int         pos;
    bool        done;
} t_sym;
typedef enum {
    EMPTY_INPUT,
    SIGN_VALIDATION,
    OP_VALIDATION,
    DIGIT,
    SPACE_BTW_OP,
    SPACE_BTW_NUM,
    INVALID_INPUT,
    STATES
} t_state;

struct s_DFA {
    t_Action    action[STATES];
    t_Action    transaction[STATES][STATES];
    char        *alphabet[CHARS];
    t_sym       *syms;
    int         syms_c;
    int         state;
    int         prev_state;
};

const static char g_state[STATES][CHARS] = {
    //    \S, -, *, 3, DEFAULT
    [0] = {0, 1, 6, 3, 6}, // Empty Input
    [1] = {6, 6, 6, 3, 6}, // Sign validation
    [2] = {4, 6, 6, 6, 6}, // OP validation
    [3] = {5, 6, 6, 3, 6}, // Digit
    [4] = {4, 1, 6, 3, 6}, // Space between operator
    [5] = {5, 2, 2, 6, 6}, // Space between number
    [6] = {6, 6, 6, 6, 6}, // Invalid Input
};
/*const static char g_state[STATES][CHARS] = {
    //    SP, SIGN, OP, DIGIT, DEFAULT
    [EMPTY_INPUT] = {EMPTY_INPUT, SIGN_VALIDATION, INVALID_INPUT, DIGIT, INVALID_INPUT}, // Empty Input
    [SIGN_VALIDATION] = {INVALID_INPUT, INVALID_INPUT, INVALID_INPUT, DIGIT, INVALID_INPUT}, // Sign validation
    [OP_VALIDATION] = {OP_VALIDATION, INVALID_INPUT, INVALID_INPUT, DIGIT, INVALID_INPUT}, // OP validation
    [DIGIT] = {VALID_NUM, INVALID_INPUT, INVALID_INPUT, DIGIT, INVALID_INPUT}, // Digit
    [INVALID_INPUT] = {INVALID_INPUT, INVALID_INPUT, INVALID_INPUT, INVALID_INPUT, INVALID_INPUT}, // Invalid Input
    [VALID_NUM] = {VALID_NUM, SIGN_OR_OP, OP_VALIDATION, INVALID_INPUT, INVALID_INPUT}, // Valid number
    [SIGN_OR_OP] = {OP_VALIDATION, INVALID_INPUT, INVALID_INPUT, DIGIT, INVALID_INPUT} // Sign or OP
};*/

int             which_alphabet(char    **str, char c);
void            append_sym(int c[2]);
t_operators     asign_op(char mask);
void            sym_count();
void            eval(char  *str);
bool            lexer(char  *str);
static t_DFA l = (t_DFA){
    .action = {
        [OP_VALIDATION] = sym_count,
        [INVALID_INPUT] = (t_Action)printf, 
    },
    .alphabet= {
        [SPACE] = " ",
        [SIGN] = "-+",
        [OP] = "-+*/",
        [DIGIT] = "0123456789",
        [DEFAULT] = NULL
    },
    .syms = NULL,
    .prev_state=0,
    .state=0
};
const static t_sym g_sym[OPS_NUM] = 
{
    [ADD] = {
        .done = false,
        .priority = 1,
        .pos = -1,
        .mask = ADD
    },
    [MULT] = {
        .done = false,
        .priority = 2,
        .pos = -1,
        .mask = MULT
    },
    [DIV] = {
        .done = false,
        .priority = 2,
        .pos = -1,
        .mask = DIV
    },
    [SUBS] = {
        .done = false,
        .priority = 1,
        .pos = -1,
        .mask = SUBS
    },
};
#endif