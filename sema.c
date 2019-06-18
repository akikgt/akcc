#include "xcc.h"

static Map *vars;
static Map *gvars;

static Node *arr_to_ptr(Node *base) {
    if (!(base->ty->ty == ARRAY))
        return base;

    Node *ret = malloc(sizeof(Node));
    ret->op = ND_ADDR;
    ret->ty = ptr_to(base->ty->arr_of);
    ret->expr = base;

    return ret;
}

static void swap(Node **a, Node **b) {
    Node *tmp = *a;
    *a = *b;
    *b = tmp;
}

Node *do_walk(Node *node, int decay);

Node *walk(Node *node) {
    return do_walk(node, 1);
}

Node *walk_nodecay(Node *node) {
    return do_walk(node, 0);
}

Node *do_walk(Node *node, int decay) {
    switch (node->op) {
        case ND_NUM:
            node->ty = int_ty();
            return node;
        case ND_VARDEF: // variable definition
        {
            Var *v = map_get(vars, node->name);
            node->ty = v->ty;
            if (node->init)
                node->init = walk(node->init);
            return node;
        }
        case ND_IDENT:  // identifier 
        {
            Var *v = map_get(vars, node->name);
            // lookup global variable
            if (!v)
                v = map_get(gvars, node->name);
            node->ty = v->ty;
            if (decay && node->ty->ty == ARRAY) {
                node = arr_to_ptr(node);
            }
            return node;
        }  
        case ND_RETURN: // return
            node->expr = walk(node->expr);
            return node;
        case ND_IF: // if
            node->cond = walk(node->cond);
            node->then = walk(node->then);
            if (node->els)
                node->els = walk(node->els);
            return node;
        case ND_WHILE: // while
            node->cond = walk(node->cond);
            node->body = walk(node->body);
            return node;
        case ND_FOR: // for
            if (node->init)
                node->init = walk(node->init);
            if (node->cond)
                node->cond = walk(node->cond);
            if (node->inc)
                node->inc = walk(node->inc);
            node->body = walk(node->body);
            return node;
        case ND_BLOCK: // block
            for (int i = 0; i < node->stmts->len; i++)
                node->stmts->data[i] = walk(node->stmts->data[i]); 
            return node;
        case ND_CALL: // function call
            for (int i = 0; i < node->args->len; i++)
                node->args->data[i] = walk(node->args->data[i]);
            return node;
        case ND_FUNC: // function definition
            for (int i = 0; i < node->args->len; i++)
                node->args->data[i] = walk(node->args->data[i]);
            node->body = walk(node->body);
            return node;
        case ND_ADDR: // address of operator ('&')
            node->expr = walk(node->expr);
            // TODO: pointer to specific type
            node->ty = new_ty(PTR, 8);
            return node;
        case ND_DEREF: // pointer dereference ('*')
            node->expr = walk(node->expr);
            node->ty = node->expr->ty->ptr_to;
            return node;
        case ND_SIZEOF: {
            node->expr = walk_nodecay(node->expr);
            Type *ty = node->expr->ty;
            node->val = ty->size;
            // convert to integer
            node->op = ND_NUM;
            node->ty = int_ty();
            return node;
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
            node->lhs = walk(node->lhs);
            node->rhs = walk(node->rhs);
            node->ty = node->lhs->ty;
            return node;
        case '+':
        case '-': {
            node->lhs = walk(node->lhs);
            node->rhs = walk(node->rhs);

            if (node->rhs->ty->ty == PTR) {
                if (node->lhs->ty->ty == PTR)
                    error("pointer %c pointer is not defined", node->op);
                swap(&node->lhs, &node->rhs);
            }

            /// pointer arithmetic
            if (node->lhs->ty->ty == PTR)
            {
                int size = node->lhs->ty->ptr_to->size;
                node->rhs = new_node_binop('*', node->rhs, new_node_num(size));
            }
            node->ty = node->lhs->ty;
            return node;
        }
        default:
            return node;
        }
}

void sema(Program *prog)
{
    gvars = prog->gvars;
    for (int i = 0; i < prog->fns->len - 1; i++) {
        Function *fn = prog->fns->data[i];
        vars = fn->vars;
        Node *node = fn->node;

        walk(node);
    }
}