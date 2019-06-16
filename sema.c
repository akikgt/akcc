#include "xcc.h"

static Map *vars;

Type *new_ty(int ty, int size) {
    Type *ret = malloc(sizeof(Type));
    ret->ty = ty;
    ret->size = size;
    return ret;
}

Type *int_ty() {
    Type *ty = new_ty(INT, 4);
    ty->ptr_to = NULL;
    return ty;
}

Node *arr_to_ptr(Node *base) {
    if (!(base->ty->ty == ARRAY))
        return base;

    Node *ret = malloc(sizeof(Node));
    ret->op = ND_ADDR;
    ret->ty = ptr_to(base->ty->arr_of);
    ret->expr = base;

    return base;
}

void walk(Node *node) {
    switch (node->op) {
        case ND_NUM:
            node->ty = int_ty();
            return;
        case ND_VARDEF: // variable definition
        {
            Var *v = map_get(vars, node->name);
            node->ty = v->ty;
            if (node->init)
                walk(node->init);
            return;
        }
        case ND_IDENT:  // identifier 
        {
            Var *v = map_get(vars, node->name);
            node->ty = v->ty;
            return;
        }  
        case ND_RETURN: // return
            walk(node->expr);
            return;
        case ND_IF: // if
            walk(node->cond);
            walk(node->then);
            if (node->els)
                walk(node->els);
            return;
        case ND_WHILE: // while
            walk(node->cond);
            walk(node->body);
            return;
        case ND_FOR: // for
            if (node->init)
                walk(node->init);
            if (node->cond)
                walk(node->cond);
            if (node->inc)
                walk(node->inc);
            walk(node->body);
            return;
        case ND_BLOCK: // block
            for (int i = 0; i < node->stmts->len; i++)
                walk(node->stmts->data[i]); 
            return;
        case ND_CALL: // function call
            for (int i = 0; i < node->args->len; i++)
                walk(node->args->data[i]);
            return;
        case ND_FUNC: // function definition
            for (int i = 0; i < node->args->len; i++)
                walk(node->args->data[i]);
            walk(node->body);
            return;
        case ND_ADDR: // address of operator ('&')
            walk(node->expr);
            node->ty = new_ty(PTR, 8);
            return;
        case ND_DEREF: // pointer dereference ('*')
            walk(node->expr);
            node->ty = node->expr->ty->ptr_to;
            return;
        case ND_SIZEOF: {
            walk(node->expr);
            Type *ty = node->expr->ty;
            node->val = ty->size;
            // convert to integer
            node->op = ND_NUM;
            node->ty = int_ty();
            return;
        }
        case ND_EQ: // ==
        case ND_NE: // !=
        case ND_LE: // <=
        case ND_GE: // >=
        case '=':
        case '<':
        case '*':
        case '/':
        case '%':
            walk(node->lhs);
            walk(node->rhs);
            node->ty = node->lhs->ty;
            return;
        case '+':
        case '-': {
            walk(node->lhs);
            walk(node->rhs);
            /// pointer arithmetic
            if (node->lhs->ty->ty == PTR || node->lhs->ty->ty == ARRAY)
            {
                int size = node->lhs->ty->ptr_to->size;
                node->rhs = new_node('*', node->rhs, new_node_num(size));
            }
            node->ty = node->lhs->ty;
            return;
        }
        default:
            return;
        }
}

void sema(Vector *nodes)
{
    for (int i = 0; i < nodes->len - 1; i++) {
        Function *fn = nodes->data[i];
        vars = fn->vars;
        Node *node = fn->node;

        walk(node);
    }
}