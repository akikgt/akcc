#include "xcc.h"
static label_count = 0;

void gen_lval(Node *node) {
    if (node->ty != ND_IDENT) 
        error("lval is not valid variable");

    // int offset = ('z' - *node->name + 1) * 8;
    int offset = (int)map_get(map, node->name);
    printf(" mov rax, rbp\n");
    printf(" sub rax, %d\n", offset);
    printf(" push rax\n");
}

void gen(Node *node) {
    if (node->ty == ND_RETURN) {
        gen(node->lhs);
        printf(" pop rax\n");
        printf(" mov rsp, rbp\n");
        printf(" pop rbp\n");
        printf(" ret\n");
        return;
    }

    if (node->ty == ND_IF) {
        gen(node->lhs); 
        printf(" pop rax\n");
        printf(" cmp rax, 0\n");
        printf(" je .Lend%d\n", label_count);
        gen(node->rhs);
        printf(".Lend%d:\n", label_count);
        label_count++;
        return;
    }

    if (node->ty == ND_NUM) {
        printf(" push %d\n", node->val);
        return;
    }

    if (node->ty == ND_IDENT) {
        gen_lval(node);
        printf(" pop rax\n");
        printf(" mov rax, [rax]\n");
        printf(" push rax\n");
        return;
    }

    if (node->ty == '=') {
        gen_lval(node->lhs);
        gen(node->rhs);

        printf(" pop rdi\n");
        printf(" pop rax\n");
        printf(" mov [rax], rdi\n");
        printf(" push rdi\n");
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf(" pop rdi\n");
    printf(" pop rax\n");

    switch (node->ty) {
        case '+':
            printf(" add rax, rdi\n");
            break;
        case '-':
            printf(" sub rax, rdi\n");
            break;
        case '*':
            printf(" imul rdi\n");
            break;
        case '/':
            printf(" cqo\n");
            printf(" idiv rdi\n");
            break;
        case ND_EQ:
            printf(" cmp rax, rdi\n");
            printf(" sete al\n");
            printf(" movzb rax, al\n");
            break;
        case ND_NE:
            printf(" cmp rax, rdi\n");
            printf(" setne al\n");
            printf(" movzb rax, al\n");
            break;
        case ND_LE:
            printf(" cmp rax, rdi\n");
            printf(" setle al\n");
            printf(" movzb rax, al\n");
            break;
        case '<':
            printf(" cmp rax, rdi\n");
            printf(" setl al\n");
            printf(" movzb rax, al\n");
            break;
    }

    printf(" push rax\n");
}
