#include "ast.h"

t_Ast   *new_ast_node(t_Ast tree) {
    t_Ast *root;

    root = malloc(sizeof(t_Ast));
    if (root)
        *root = tree;
    return (root);
}

bool free_ast_node(t_Ast    **node) {
    t_Ast   *ptr = *node;
    if (!ptr) return true;
    if (ptr->tag == Literal)
    {
        ptr->u_d.Literal.freezer(&ptr->u_d.Literal);
        ptr->u_d.Literal.data = NULL;
    }
    else if (ptr->tag == Operator){
        if (!free_ast_node(&ptr->u_d.Operator.left)) return false;
        if (!free_ast_node(&ptr->u_d.Operator.right)) return false;
    }
    else
        return false;
    free(*node);
    *node = NULL;
    return true;
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
    void    *l_v = NULL;
    void    *r_v = NULL;
    void    *data;
    if (this->left->tag == Literal)
    {
        l_v = this->left->u_d.Literal.data;
    } 
    else if (this->left->tag == Operator)
    {
        l_v = operate(&this->left->u_d.Operator);
        free_ast_node(&this->left);
    }
    if (this->right->tag == Literal)
    {
        r_v = this->right->u_d.Literal.data;
    }
    else if (this->right->tag == Operator)
    {
        r_v = operate(&this->right->u_d.Operator); // must clean after resolve, resolve value got leak
        free_ast_node(&this->right);
    }
    if (this->mask == '*')
        this->Resolve = (void *(*)(void *, void *))mult;
    else
        this->Resolve = (void *(*)(void *, void *))add;
    data = this->Resolve(l_v, r_v);
    if (!this->left) free(l_v);
    if (!this->right) free(r_v);
    return data;
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
    system("leaks test");
}

int main(void) {
    t_Ast *tree_two = AST_NODE(Operator,
                        AST_NODE(Literal, new_num(2), free_num),
                        AST_NODE(Operator,
                            AST_NODE(Operator, 
                                AST_NODE(Literal, new_num(2), free_num),
                                AST_NODE(Literal, new_num(3), free_num),
                                '*',
                                NULL),
                            AST_NODE(Literal, new_num(4), free_num),
                            '+',
                            NULL),
                        '+', 
                        NULL);
    void *patata = operate(&tree_two->u_d.Operator);
    int *a = (int *)patata;

    printf("Value: %d", *a);
    printf("Cleaning");
    free_ast_node(&tree_two);
    free(a); 
    // free(tree_two);
    atexit(leaks);
    return (0);
}