#include "xcc.h"
static int label_count = 0;

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

    switch (node->ty) {
        case ND_RETURN:
            gen(node->lhs);
            printf(" pop rax\n");
            printf(" mov rsp, rbp\n");
            printf(" pop rbp\n");
            printf(" ret\n");
            return;

        case ND_IF:
            gen(node->cond);
            printf(" pop rax\n");
            printf(" cmp rax, 0\n");

            if (!node->els)
            {
                printf(" je .Lend%d\n", label_count);
                gen(node->then);
                printf(".Lend%d:\n", label_count);
                label_count++;
                return;
            }

            printf(" je .Lelse%d\n", label_count);
            gen(node->then);
            printf(" jmp .Lend%d\n", label_count);
            printf(".Lelse%d:\n", label_count);
            gen(node->els);
            printf(".Lend%d:\n", label_count);
            label_count++;
            return;
        
        case ND_WHILE:
            printf(".Lbegin%d:\n", label_count);
            gen(node->cond);
            printf(" pop rax\n");
            printf(" cmp rax, 0\n");
            printf(" je .Lend%d\n", label_count);
            gen(node->then);
            printf(" jmp .Lbegin%d\n", label_count);
            printf(".Lend%d:\n", label_count);
            return;

        case ND_NUM:
            printf(" push %d\n", node->val);
            return;

        case ND_IDENT:
            gen_lval(node);
            printf(" pop rax\n");
            printf(" mov rax, [rax]\n");
            printf(" push rax\n");
            return;

        case '=':
            gen_lval(node->lhs);
            gen(node->rhs);

            printf(" pop rdi\n");
            printf(" pop rax\n");
            printf(" mov [rax], rdi\n");
            printf(" push rdi\n");
            return;
    }

    /// operators requiring two operands
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
