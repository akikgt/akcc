#include "xcc.h"

static Map *vars;

Type *new_ty() {
    Type *ty = malloc(sizeof(Type));
    return ty;
}

Type *int_ty() {
    Type *ty = new_ty();
    ty->ty = INT;
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
            // printf("walk var definition nodes\n");
            return;
        case ND_IDENT:  // identifier 
        {
            // printf("walk identifier nodes\n");
            Var *v = map_get(vars, node->name);
            node->ty = v->ty;
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
            return;
        case ND_DEREF: // pointer dereference ('*')
            walk(node->expr);
            return;
        case ND_SIZEOF: {
            walk(node->expr);
            Type *ty = node->expr->ty;
            if (ty->ty == PTR)
                node->val = 8;
            if (ty->ty == INT)
                node->val = 4;
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
            if (node->lhs->ty->ty == PTR)
            {
                int size;
                if (node->rhs->ty->ty == PTR)
                    size = 8;
                if (node->rhs->ty->ty == INT)
                    size = 4;
                    node->rhs->val = node->rhs->val * size;
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