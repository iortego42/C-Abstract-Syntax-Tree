#include "ast.h"

int which_alphabet(char    **str,   char c)
{
    int i;
    int b;
    
    i = 0;
    if (!str || !*str) 
        return -1;
    while (str[i] != NULL)
    {
        b = 0;
        while (str[i][b])
        {
            if (str[i][b] == c)
                return (i);
            b++;
        }
        i++;
    }
    return (i);
}

void    sym_count()
{
    l.syms_c++;
}

t_operators    asign_op(char mask) {
    t_operators var;
    int         len;

    len = strlen(l.alphabet[OP]);
    var = 0;
    while (var < len)
    {
        if (mask == l.alphabet[OP][var])
            return (var);
        var++;
    }
    return NONE;
}

void    eval(char    *str)
{
    int index;
    
    index = 0;
    if (!str) 
        return ;
    while (str[index] != 0)
    {
        l.state = g_state[l.state][which_alphabet(l.alphabet, str[index])];    
        if (l.action[l.state])
            l.action[l.state](&str[index]);
        if (l.transaction[l.prev_state][l.state])
            l.transaction[l.prev_state][l.state](&str[index]);
        l.prev_state = l.state;
        index++;
    }
}

void    append_sym(int c[2])
{
    t_operators mask;

    mask = asign_op(c[0]);
    if (mask == NONE)
        return ((void)"42Madrid");
    l.syms[l.syms_c] = g_sym[mask];
    l.syms[l.syms_c].pos = c[1];
    l.syms_c++;
}
bool    get_syms(char   *str)
{
    int         c[2];

    c[1] = 0;
    c[0] = str[c[1]];
    l.state = 0;
    l.prev_state = 0;
    l.action[OP_VALIDATION] = (t_Action)append_sym;
    while(c[0] != 0) 
    {
        l.state = g_state[l.state][which_alphabet(l.alphabet, c[0])];
        if (l.state == OP_VALIDATION)
            l.action[OP_VALIDATION](c);
        c[1]++;
        c[0] = str[c[1]];
    }
    Data.sym = l.syms;
    return (true);
}

bool    lexer(char    *str)
{
    eval(str);
    if (l.state == INVALID_INPUT)
        return (false);
    l.syms = malloc(sizeof(t_sym) * (l.syms_c + 1));
    if (l.syms == NULL)
        return (false);
    l.syms[l.syms_c].mask = NONE;
    l.syms_c = 0;
    return (get_syms(str));
}

// int main (int argc, char *argv[])
// {
//     if (argc != 2)
//         return (1);
//     eval(argv[1]);  
//     if (l.state != INVALID_INPUT)
//         printf("done\n");
//     else 
//         printf("State -> %d\n", l.state);
//     printf("Symbols [%d]\n", l.syms_c);

//     return (0);
// }