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

void walk(Node *node) {
    switch (node->op) {
        case ND_NUM:
            // printf("walk number nodes\n");
            node->ty = int_ty();
            return;
        case ND_VARDEF: // variable definition
        {
            // printf("walk var definition nodes\n");
            Var *v = map_get(vars, node->name);
            // printf("%s\n", node->name);
            node->ty = v->ty;
            // printf("%d\n", node->ty->size);
            if (node->init)
                walk(node->init);
            return;
        }
        case ND_IDENT:  // identifier 
        {
            // printf("walk identifier nodes\n");
            Var *v = map_get(vars, node->name);
            // printf("%s\n", node->name);
            node->ty = v->ty;
            // printf("%d\n", node->ty->size);
            return;
        }  
        case ND_RETURN: // return
            // printf("walk return nodes\n");
            walk(node->expr);
            return;
        case ND_IF: // if
            // printf("walk if-else statement nodes\n");
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
            // printf("walk block statement nodes\n");
            for (int i = 0; i < node->stmts->len; i++)
                walk(node->stmts->data[i]); 
            return;
        case ND_CALL: // function call
            for (int i = 0; i < node->args->len; i++)
                walk(node->args->data[i]);
            return;
        case ND_FUNC: // function definition
            // printf("walk function definition nodes\n");
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
            // if (node->expr->ty->ty == ARRAY) {
            //     printf("aaaa\n");
            //     // printf("%d\n", node->ty->ty);
            //     // node->ty->ty = PTR;
            //     // node->ty->size = 8;
            //     // node->ty = int_ty();
            // }
            // else
            node->ty = node->expr->ty->ptr_to;
            // printf("%d\n", node->ty->ty);
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
            // if (node->ty->ty == ARRAY) {
            //     node->ty->ty = PTR;
            //     node->ty->size = 8;
            // }
            return;
        case '+':
        case '-': {
            walk(node->lhs);
            walk(node->rhs);
            /// pointer arithmetic
            if (node->lhs->ty->ty == PTR || node->lhs->ty->ty == ARRAY)
            {
                int size = node->lhs->ty->ptr_to->size;
                Node *rhs = node->rhs;
                rhs->val = rhs->val * size;
            }
            // printf("%d\n", node->lhs->ty->size);
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