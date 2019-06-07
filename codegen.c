#include "xcc.h"
static int label_count = 0;
static Map *map;
static var_count;

char *regs[6] = {
    "rdi",
    "rsi",
    "rdx",
    "rcx",
    "r8",
    "r9",
};

void gen_func(Node *node) {
    /// currently, we use function scope
    /// TODO: block scope
    map = new_map();
    var_count = 0;

    printf(".global %s\n", node->name);
    printf("%s:\n", node->name);

    // Prologue
    printf(" push rbp\n");
    printf(" mov rbp, rsp\n");
    printf(" sub rsp, 208\n");  // make 26 local variables

    gen(node->body);

    // Epilogue
    printf(" pop rax\n");
    printf(" mov rsp, rbp\n");
    printf(" pop rbp\n");
    printf(" ret\n");
}

void gen_lval(Node *node) {

    if (node->ty != ND_IDENT) 
        error("lval is not valid variable");

    int offset;
    if (map_get(map, node->name) == NULL) {
        offset = (var_count + 1) * 8;
        map_put(map, node->name, (void *)offset);
        var_count++;
    }

    offset = (int)map_get(map, node->name);
    printf(" mov rax, rbp\n");
    printf(" sub rax, %d\n", offset);
    printf(" push rax\n");
}

void gen(Node *node) {

    switch (node->ty) {
        case ND_FUNC:
            gen_func(node);
            return;
        case ND_CALL:
            for (int i = 0; i < node->args->len; i++) {
                gen(node->args->data[i]);
            }

            for (int i = 0; i < node->args->len; i++)
                printf(" pop %s\n", regs[i]);

            ////
            // TODO: alignment RSP
            ////
            printf(" call %s\n", node->name);
            printf(" push rax\n");
            return;

        case ND_BLOCK:
            if (node->stmts->len == 0) {
                printf(" push 0\n");
                return;
            }

            for (int i = 0; i < node->stmts->len; i++) {
                gen(node->stmts->data[i]);
                printf(" pop rax\n");
            }

            printf(" push rax\n");
            return;

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

        case ND_FOR:
            if (node->init)
                gen(node->init);
            printf(".Lbegin%d:\n", label_count);
            if (node->cond)
                gen(node->cond);
            else
                printf(" push 1\n"); // always true
            printf(" pop rax\n");
            printf(" cmp rax, 0\n");
            printf(" je .Lend%d\n", label_count);
            gen(node->body);
            if (node->inc)
                gen(node->inc);
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
