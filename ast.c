#include "ast.h"

void    *solve_ast(t_Ast    *this) {
    if (this->tag == Literal)
        return (this->u_d.Literal.data);
    else if (this->tag == Operator)
        return (operate(&this));
    else
        return (NULL);
}

int *new_num(int n){
    int *num = malloc(sizeof(int));
    if (num)
        *num = n;
    return num;
}

void free_num(t_L *this) {
    free(this->data); 
}

void leaks(){
    system("leaks -q test");
}
t_Ast   *new_ast_node(t_Ast tree) {
    t_Ast *root;

    root = malloc(sizeof(t_Ast));
    if (!root)
        return (NULL);
    *root = tree;
    if (root->tag == Expression)
        rev_constructor(root);
    return (root);
}

bool free_ast_node(t_Ast    **node) {
    t_Ast   *ptr;
    if (!node || !*node)
        return true;
    ptr = *node;
    if (ptr->tag == Literal)
    {
        ptr->u_d.Literal.freezer(&ptr->u_d.Literal);
        ptr->u_d.Literal.data = NULL;
    }
    else if (ptr->tag == Operator){
        if (!free_ast_node(&ptr->u_d.Operator.left)
            || !free_ast_node(&ptr->u_d.Operator.right))
            return false;
    }
    else if (ptr->tag == Expression)
        ptr = NULL; 
    else
        return false;
    free(*node);
    *node = NULL;
    return true;
}

void    constructor(t_Ast   *this)
{
    int i;
    int prior;

    if (this->tag != Expression)
        return (void)"";
    prior = 1;
    while (prior < 3)
    {
        i = 0;
        while (Data.sym[i].mask != NONE) {
            if (Data.sym[i].priority == prior && !Data.sym[i].done
                && Data.sym[i].pos < this->u_d.Expression.end
                && Data.sym[i].pos > this->u_d.Expression.start)
            {
                to_operator(this, Data.sym[i]);
                Data.sym[i].done = true;
                return ;
            }
            i++;
        }
        prior++;
    }
    to_literal(this);
}

void rev_constructor(t_Ast  *this)
{
    int i;
    int prior;

    if (this->tag != Expression)
        return (void)"42Madrid";
    prior = 1;
    while (prior < 3)
    {
        i = 0;
        while (Data.sym[i].mask != NONE)
            i++;
        while (i-- > 0)
        {
            if (Data.sym[i].priority == prior && !Data.sym[i].done
                && Data.sym[i].pos < this->u_d.Expression.end
                && Data.sym[i].pos > this->u_d.Expression.start)
            {
                to_operator(this, Data.sym[i]);
                Data.sym[i].done = true;
                return ;
            }
        }
        prior++;
    }
    to_literal(this);
}

void    to_operator(t_Ast   *this, t_sym    op)
{
    int     l_start;
    int     l_end;
    int     r_start;
    int     r_end;

    if (this->tag != Expression)
        return ;
    l_start = this->u_d.Expression.start;
    l_end = op.pos; // revisar posiciones
    r_start = op.pos + 1;
    r_end = this->u_d.Expression.end;
    this->u_d.Operator.left = AST_NODE(Expression, l_start, l_end);
    this->u_d.Operator.right = AST_NODE(Expression, r_start, r_end);
    this->u_d.Operator.mask = op.mask;
    this->u_d.Operator.pos = op.pos;
    this->tag = Operator;
    GET_RESOLVER(this->u_d.Operator);
}

void    to_literal(t_Ast    *this)
{
    char    *num_str;
    int     len;

    len = this->u_d.Expression.end - this->u_d.Expression.start;
    num_str = malloc(len);
    strlcpy(num_str, Data.cmd + this->u_d.Expression.start, len);
    this->tag = Literal;
    this->u_d.Literal.data = malloc(sizeof(int));
    *(int *)this->u_d.Literal.data = atoi(num_str); 
    this->u_d.Literal.freezer = free_num;
    free(num_str);
}

t_operators    asign_op(char mask) {
    t_operators var;
    if (mask == '+')
        var = ADD;
    else if (mask == '*')
        var = MULT;
    else if (mask == '-')
        var = SUBS;
    else if (mask == '/')
        var = DIV;
    else
        var = NONE;
    return var;
}


void    *operate(t_Ast  **this) {
    void    *r_v;
    void    *l_v;
    void    *data;

    if (this[0]->tag != Operator)
        return NULL; 
    if (this[0]->u_d.Operator.left->tag == Literal)
        l_v = this[0]->u_d.Operator.left->u_d.Literal.data;
    else 
        l_v = operate(&this[0]->u_d.Operator.left);
    if (this[0]->u_d.Operator.right->tag == Literal)
        r_v = this[0]->u_d.Operator.right->u_d.Literal.data;
    else 
        r_v = operate(&this[0]->u_d.Operator.right);
    data = this[0]->u_d.Operator.Resolve(l_v, r_v);
    if (!this[0]->u_d.Operator.left)
        free(l_v);
    if (!this[0]->u_d.Operator.right)
        free(r_v);
    if (!free_ast_node(this)) printf("There was an Error\n");
    return (data);
}

t_sym   *get_syms(char  *str)
{
    int     i, s_c, symbols;
    t_sym   *syms;

    i = 0;
    symbols = 0;
    while (str[i])
    {
        if (asign_op(str[i]) != NONE)
            symbols++;
        i++;
    }
    i = 0;
    syms = malloc(sizeof(t_sym) * (symbols + 1));
    if (!syms)
        return (NULL);
    syms[symbols].mask = NONE;
    s_c = 0;
    while (str[i])
    {
        if (asign_op(str[i]))
        {
            syms[s_c].done = false;
            syms[s_c].mask = asign_op(str[i]);
            syms[s_c].pos = i;
            syms[s_c].priority = 1;
            if (syms[s_c].mask == MULT || syms[s_c].mask == DIV)
                syms[s_c].priority = 2;
            s_c++;
        }
        i++;
    }
    return (syms); 
}

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        printf("Error. Usage: %s \"mathematical operation\"", argv[0]);
        return (1);
    }
    Data.cmd = argv[1];
    Data.sym = get_syms(Data.cmd);
    Data.tree = AST_NODE(Expression, 0, strlen(Data.cmd) + 1);
    int *a = (int *)solve_ast(Data.tree);

    printf("Value: %d\n", *a);
    printf("Cleaning\n");
    free(a); 
    atexit(leaks);
    return (0);
}