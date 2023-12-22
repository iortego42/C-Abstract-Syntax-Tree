#ifndef AST_H
# define AST_H

/*
|   tag =>  Tag or Type of the data 
|   u_d =>  Data of the Tree
|
|   This is done to avoid bugs and avoid the program misinternpret
|   some region of the memory as an inapropiate data type.
|
|   u_d:    for every type of token will be an specific type of 
|           data defined. We can distinguish 3 of them:
|           - Operators: This ones will have left and right context
|                        of all the rest of the types, 2 operators
|                        can't be found together. they got a
|                        function which will be executed with the
|                        values of both context passed as params.
|           - Literals: The most simple data type, they do not have
|                       any context, just values. They can be
|                       obtained when expresions are resolved.
|           - Expression: This type will define an entire expression
|                         which can contain all the 3 existing types 
|                         inside. And expresion must be resolved in
|                         order to obtain a new group of Operators
|                         and Literals or just a Literal.
|
|
|
|
*/

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_Ast t_Ast;
typedef enum {
    OPERATOR,
    LITERAL
} t_tag;
typedef struct s_Operator t_O;
struct s_Operator {
    t_Ast   *left;
    t_Ast   *right;
    char    mask;
    void    *(*Resolve) (void *r_d, void *l_d);
    // void    *resolved_data;
    int     value;
};
typedef struct s_Literal t_L;
struct s_Literal {
    int     num;
    // void    *data;
    // void    (*freezer)(void *);
};
struct s_Ast {
    t_tag tag;
    union {
        t_O Operator; 
        t_L Literal;
        // struct s_Expression {} Espression;
    } u_d;
};

t_Ast   *new_ast(t_Ast tree);
#endif