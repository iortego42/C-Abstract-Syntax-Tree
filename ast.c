#include "ast.h"

t_Ast   *new_ast(t_Ast tree) {
    t_Ast *root;

    root = malloc(sizeof(t_Ast));
    if (root)
        *root = tree;
    return (root);
}


int add(int a, int b) {
    return (a + b);
}

int *mult(int *a, int *b)
// void *mult(void *r_d, void *l_d)
{
    // int a,b;
    // a = *(int *)r_d;
    // b = *(int *)l_d;
    int * v = malloc (sizeof(int));
    *v = *a * *b;
    return v;
}

void    *operate(t_O *this) {
    int l_v, r_v;
    if (this->left->tag == LITERAL)
    {
        l_v = this->left->u_d.Literal.num;
    } 
    if (this->right->tag == LITERAL)
    {
        r_v = this->right->u_d.Literal.num;
    }
    if (this->mask == '*')
        this->Resolve = (void *(*)(void *, void *))mult;
    else
        this->Resolve = (void *(*)(void *, void *))mult;
    return this->Resolve(&l_v, &r_v);
}

int main(void) {
    t_Ast tree = (t_Ast){
        .tag = OPERATOR,
        .u_d.Operator = (t_O){
            .right = new_ast((t_Ast){
                .tag = LITERAL,
                .u_d.Literal = (t_L){.num = 5 }}),
            .left = new_ast((t_Ast){
                .tag = LITERAL,
                .u_d.Literal = (t_L){.num = 7 }}),
            .mask = '*',
        }
    };
    void * patata = operate(&tree.u_d.Operator);
    int *a = (int *)patata;
    printf("Value: %d", *a);
    return (0);
}