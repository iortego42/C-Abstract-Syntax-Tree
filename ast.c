#include "ast.h"

t_Ast   *new_ast(t_Ast tree) {
    t_Ast *root;

    root = malloc(sizeof(t_Ast));
    if (root)
        *root = tree;
    return (root);
}


int *add(int *a, int *b) {

    int * v = malloc (sizeof(int));
    *v = *a + *b;
    return v;
}

int *mult(int *a, int *b)
{
    int * v = malloc (sizeof(int));
    *v = *a * *b;
    return v;
}

void    *operate(t_O *this) {
    void *l_v = NULL;
    void *r_v = NULL;
    if (this->left->tag == Literal)
    {
        l_v = this->left->u_d.Literal.data;
    } 
    else if (this->left->tag == Operator)
    {
        l_v = operate(&this->left->u_d.Operator);
    }
    if (this->right->tag == Literal)
    {
        r_v = this->right->u_d.Literal.data;
    }
    else if (this->right->tag == Operator)
    {
        r_v = operate(&this->right->u_d.Operator);
    }
    if (this->mask == '*')
        this->Resolve = (void *(*)(void *, void *))mult;
    else
        this->Resolve = (void *(*)(void *, void *))add;
    return this->Resolve(l_v, r_v);
}

int *new_num(int n){
    int *num = malloc(sizeof(int));
    if (num)
        *num = n;
    return num;
}

int main(void) {
    t_Ast *tree_two = AST_NODE(Operator,
                        AST_NODE(Literal, new_num(2)),
                        AST_NODE(Literal, new_num(2)),
                        '+', NULL);
    void *patata = operate(&tree_two->u_d.Operator);
    int *a = (int *)patata;
    printf("Value: %d", *a);
    return (0);
}