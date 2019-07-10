#include "xcc.h"

static Node *maybe_decay(Node *base, int decay) {
    if (!(decay == 1 && base->ty->ty == ARRAY))
        return base;

    Node *ret = calloc(1, sizeof(Node));
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
            if (node->init)
                node->init = walk(node->init);
            return node;
        }
        case ND_IDENT:  // identifier 
        {
            node = maybe_decay(node, decay);
            return node;
        }  
        case ND_RETURN: // return
            node->expr = walk(node->expr);
            return node;
        case ND_IF: // if
            node->cond = walk(node->cond);
            node->then = walk(node->then);
            if (node->els) {
                node->els = walk(node->els);
            }
            return node;
        case ND_DO_WHILE: // do while
        case ND_SWITCH: // switch
            node->body = walk(node->body);
            node->cond = walk(node->cond);
            return node;
        case ND_CASE: // case
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
            for (int i = 0; i < node->stmts->len; i++) {
                Node *stmt = node->stmts->data[i];
                node->stmts->data[i] = walk(stmt); 
            }
            // TODO: type setting for block statement
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
            node->ty = new_ty(PTR, 8);
            node->ty->ptr_to = node->expr->ty;
            return node;
        case ND_DEREF: // pointer dereference ('*')
            node->expr = walk(node->expr);
            node->ty = node->expr->ty->ptr_to;
            return maybe_decay(node, decay);
        case ND_POST_INC: // post-increment/decrement
        case ND_POST_DEC:
            node->expr = walk(node->expr);
            node->ty = node->expr->ty;
            return node;
        case ND_LOG_OR:
        case ND_LOG_AND:
            node->lhs = walk(node->lhs);
            node->rhs = walk(node->rhs);
            node->ty = node->lhs->ty;
            return node;
        case ND_LOG_NOT:
            node->expr = walk(node->expr);
            node->ty = node->expr->ty;
            return node;
        case ND_BREAK:
        case ND_CONTINUE:
            return node;
        case '~':
            node->expr = walk(node->expr);
            node->ty = node->expr->ty;
            return node;
        case ND_SIZEOF: {
            if (node->expr->op == ND_TY_SIZE) {
                node->val = node->expr->val;
            }
            else {
                node->expr = walk_nodecay(node->expr);
                Type *ty = node->expr->ty;
                node->val = ty->size;
            }
            // convert to number node
            node->op = ND_NUM;
            node->ty = int_ty();
            return node;
        }
        case ND_EQ: // ==
        case ND_NE: // !=
        case ND_LE: // <=
        case ND_GE: // >=
            node->lhs = walk(node->lhs);
            node->rhs = walk(node->rhs);
            node->ty = node->lhs->ty;
            return node;
        case ND_DOT: {
            node->expr = walk(node->expr);
            Type *ty = node->expr->ty;
            Type *member = map_get(ty->members, node->name);
            node->ty = member;
            return maybe_decay(node, decay);
        }
        case '=':
            // TODO: check 9cc uses walk_nodecay to left hand side of '='
            node->lhs = walk(node->lhs);
            // if (node->lhs->op == ND_ADDR)
            //     error("ND_ADDR cannot be lval");
            node->rhs = walk(node->rhs);
            node->ty = node->lhs->ty;
            return node;
        case '<':
        case '*':
        case '/':
        case '%':
        case '|':
        case '^':
        case '&':
            node->lhs = walk(node->lhs);
            node->rhs = walk(node->rhs);
            node->ty = node->lhs->ty;
            return node;
        case '+': {
            node->lhs = walk(node->lhs);
            node->rhs = walk(node->rhs);

            if (node->rhs->ty->ty == PTR) {
                if (node->lhs->ty->ty == PTR) {
                    error("pointer %c pointer is not defined, %d", node->op, node->lhs->ty->size);
                }
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
        case '-': {
            node->lhs = walk(node->lhs);
            node->rhs = walk(node->rhs);

            if (node->rhs->ty->ty == PTR) {
                if (node->lhs->ty->ty == PTR) {
                // 'pointer - pointer' is allowed
                int size = node->lhs->ty->ptr_to->size;
                node = new_node_binop('/', node, new_node_num(size));
                node->ty = int_ty();
                return node;
                }
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
    for (int i = 0; i < prog->fns->len - 1; i++) {
        Function *fn = prog->fns->data[i];
        Node *node = fn->node;

        walk(node);
    }
}