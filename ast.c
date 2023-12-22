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
    int l_v, r_v;
    if (this->left->tag == Literal)
    {
        l_v = this->left->u_d.Literal.num;
    } 
    if (this->right->tag == Literal)
    {
        r_v = this->right->u_d.Literal.num;
    }
    if (this->mask == '*')
        this->Resolve = (void *(*)(void *, void *))mult;
    else
        this->Resolve = (void *(*)(void *, void *))add;
    return this->Resolve(&l_v, &r_v);
}

int main(void) {
    t_Ast *tree_two = AST_NODE(Operator,
                        AST_NODE(Literal, 2),
                        AST_NODE(Literal, 2),
                        '+', NULL);
    void *patata = operate(&tree_two->u_d.Operator);
    int *a = (int *)patata;
    printf("Value: %d", *a);
    return (0);
}