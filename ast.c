#include "ast.h"
/*
OLD OPERATE
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
}*/

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
    system("leaks test");
}
t_Ast   *new_ast_node(t_Ast tree) {
    t_Ast *root;

    root = malloc(sizeof(t_Ast));
    if (root)
        *root = tree;
    return (root);
}

bool free_ast_node(t_Ast    **node) {
    t_Ast   *ptr;
    if (!node || !*node) return true;
    ptr = *node;
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

void    asign_resolver(t_O *this) {
    if (this->mask == '*')
        this->Resolve = (void *(*)(void *, void *))mult;
    else if (this->mask == '+')
        this->Resolve = (void *(*)(void *, void *))add;
    else
        this->Resolve = NULL;
}

void    *operate(t_Ast  **this) {
    void    *r_v;
    void    *l_v;
    void    *data;

    if (this[0]->tag != Operator)
        return NULL; 
    if (this[0]->u_d.Operator.Resolve == NULL)
        asign_resolver(&this[0]->u_d.Operator);
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

    int *a = (int *)solve_ast(tree_two);

    printf("Value: %d\n", *a);
    printf("Cleaning\n");
    free(a); 
    free_ast_node(&tree_two);
    atexit(leaks);
    return (0);
}