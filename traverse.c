#include "xcc.h"

static Map *vars;
static Map *gvars;

void traverse_and_print(Node *node, int depth) {
    switch (node->op) {
        case ND_NUM:
            printf("%*s ND_NUM:%d\n", depth, "", node->val);
            return;
        case ND_VARDEF: // variable definition
        {
            Var *v = map_get(vars, node->name);
            printf("%*s ND_VARDEF:%s\n", depth, "", v->name);
            if (node->init) {
                printf("%*s ND_ASSIGN:\n", depth, "");
                traverse_and_print(node->init, ++depth);
            }
            return;
        }
        case ND_IDENT:  // identifier 
        {
            Var *v = map_get(vars, node->name);
            // lookup global variable
            if (!v)
                v = map_get(gvars, node->name);
            printf("%*s ND_IDENT:%s\n", depth, "", v->name);
            return;
        }  
        case ND_RETURN: // return
            printf("%*s ND_RETURN\n", depth, "");
            traverse_and_print(node->expr, ++depth);
            return;
        case ND_IF: // if
            printf("%*s ND_IF\n", depth, "");
            traverse_and_print(node->cond, ++depth);
            traverse_and_print(node->then, ++depth);
            if (node->els)
                traverse_and_print(node->els, ++depth);
            return;
        case ND_WHILE: // while
            printf("%*s ND_WHILE\n", depth, "");
            traverse_and_print(node->cond, ++depth);
            traverse_and_print(node->body, ++depth);
            return;
        case ND_FOR: // for
            printf("%*s ND_FOR\n", depth, "");
            if (node->init)
                traverse_and_print(node->init, ++depth);
            if (node->cond)
                traverse_and_print(node->cond, ++depth);
            if (node->inc)
                traverse_and_print(node->inc, ++depth);
            traverse_and_print(node->body, ++depth);
            return;
        case ND_BLOCK: // block
            printf("%*s ND_BLOCK\n", depth, "");
            for (int i = 0; i < node->stmts->len; i++)
                traverse_and_print(node->stmts->data[i], ++depth); 
            return;
        case ND_CALL: // function call
            printf("%*s ND_CALL\n", depth, "");
            for (int i = 0; i < node->args->len; i++)
                traverse_and_print(node->args->data[i], ++depth);
            return;
        case ND_FUNC: // function definition
            printf("%*s ND_FUNC: %s\n", depth, "", node->name);
            for (int i = 0; i < node->args->len; i++)
                traverse_and_print(node->args->data[i], ++depth);
            traverse_and_print(node->body, ++depth);
            return;
        case ND_ADDR: // address of operator ('&')
            printf("%*s ND_ADDR\n", depth, "");
            traverse_and_print(node->expr, ++depth);
            return;
        case ND_DEREF: // pointer dereference ('*')
            printf("%*s ND_DEREF\n", depth, "");
            traverse_and_print(node->expr, ++depth);
            return;
        case ND_POST_INC: // post-increment/decrement
        case ND_POST_DEC:
            printf("%*s ND_POST_INC/DEC\n", depth, "");
            traverse_and_print(node->expr, ++depth);
            return;
        case ND_LOG_OR:
        case ND_LOG_AND:
            printf("%*s ND_LOG_OR/AND\n", depth, "");
            traverse_and_print(node->lhs, ++depth);
            traverse_and_print(node->rhs, ++depth);
            return;
        case ND_LOG_NOT:
            printf("%*s ND_LOG_NOT\n", depth, "");
            traverse_and_print(node->expr, ++depth);
            return;
        case ND_SIZEOF: {
            printf("%*s ND_SIZEOF\n", depth, "");
            if (node->expr->op == ND_TY_SIZE)
                return;
            traverse_and_print(node->expr, ++depth);
            return;
        }
        case ND_EQ: // ==
        case ND_NE: // !=
        case ND_LE: // <=
        case ND_GE: // >=
            printf("%*s ND_BINARY_OPs:%d\n", depth, "", node->op);
            traverse_and_print(node->lhs, ++depth);
            traverse_and_print(node->rhs, ++depth);
            return;
        case '=':
        case '<':
        case '*':
        case '/':
        case '%':
        case '+':
        case '-':
            printf("%*s ND_BINARY_OPs:%c\n", depth, "", node->op);
            traverse_and_print(node->lhs, ++depth);
            traverse_and_print(node->rhs, ++depth);
            return;
        default:
            return node;
        }
}

void traverse(Program *prog)
{
    gvars = prog->gvars;
    for (int i = 0; i < prog->fns->len - 1; i++) {
        Function *fn = prog->fns->data[i];
        vars = fn->vars;
        Node *node = fn->node;

        traverse_and_print(node, 0);
    }
}