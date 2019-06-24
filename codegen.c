#include "xcc.h"

static int label_count;
static Map *gvars;

char *regs[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
char *regs32[6] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};

int roundup(int x, int align) {
    return (x + align - 1) & ~(align - 1);
}

static char *get_reg(Type *ty, char r) {
    switch (ty->size) {
        case 1:  return (r == 'a') ? "al" : "dil";
        case 2:  return (r == 'a') ? "ax" : "di";
        case 4:  return (r == 'a') ? "eax" : "edi";
        case 8:  return (r == 'a') ? "rax" : "rdi";
        default:
            error("Unknown data size: %d", ty->size);
            break;
    }

    return 0;
}

static void emit_load(Node *node) {
    printf("  mov %s, [rax]\n", get_reg(node->ty, 'a'));
    if (node->ty->size == 1) {
        // TODO: consider movzx
        printf("  movsx eax, %s\n", get_reg(node->ty, 'a'));
    }
}

static void emit_binop(Node *node) {
    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->op)
    {
    case '+':
        printf("  add rax, rdi\n");
        break;
    case '-':
        printf("  sub rax, rdi\n");
        break;
    case '*':
        printf("  imul rdi\n");
        break;
    case '/':
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    case '%':
        printf("  cqo\n");
        printf("  idiv rdi\n"); // reminder will be in rdx
        printf("  mov rax, rdx\n");
        break;
    case '|':
        printf("  or rax, rdi\n");
        break;
    case '^':
        printf("  xor rax, rdi\n");
        break;
    case '&':
        printf("  and rax, rdi\n");
        break;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_NE:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    case '<':
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;

    default:
        error("Unknown operator");
        break;
    }

    printf("  push rax\n");
}

void gen_func(Function *fn) {
    Node *node = fn->node;
    if (node->op != ND_FUNC) 
        error("This is not function node");

    // Prologue
    printf(".global %s\n", node->name);
    printf("%s:\n", node->name);
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");

    // count total variable size
    int var_size = 0;
    for (int i = 0; i < fn->lvars->len; i++) {
        // TODO: alighnment for each local variable
        Var *var = fn->lvars->data[i];
        var_size += var->ty->size;
    }

    // Alignment RSP
    // For x86-64 ABI, roundup RSP to multiplies of 16
    printf("  sub rsp, %d\n", roundup(var_size, 16));  // make local variables

    // set parameters
    for (int i = 0; i < node->args->len; i++) {
        Node *param = node->args->data[i];
        gen_lval(param);
        // set each parameters to local variable address
        printf("  pop rax\n");
        if (param->ty->ty == ARRAY) {
            /// TODO: array parameter
            printf("  mov [rax], %s\n", regs[i]);
            continue;
        }
        switch (param->ty->size) {
            case 1:
            // TODO: make regs8 array and modify dil -> regs8[i]
                printf("  mov [rax], %s\n", "dil");
                break;
            case 2:
            case 4:
                printf("  mov [rax], %s\n", regs32[i]);
                break;
            case 8:
                printf("  mov [rax], %s\n", regs[i]);
                break;
            default:
                error("Unknown data size %d", param->ty->size);
                break;
        }
    }

    gen(node->body);

    // Epilogue
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
}

void gen_lval(Node *node) {
    if (node->op == ND_DEREF) {
        gen(node->expr);
        return;
    }

    if (node->op != ND_IDENT && node->op != ND_VARDEF) 
        error("lval is not valid variable. op: %d", node->op);

    Var *var = node->var;

    // Global variable
    if (!var->is_local) {
        // TODO: use var's member is_local to detect global variable
        if (!map_get(gvars, node->name))
            error("cannot find variable, %s", node->name);

        // printf("global variable\n");
        printf("  lea eax, %s[rip]\n", node->name);
        printf("  push rax\n");
        return;
    }

    int offset = var->offset;
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", offset);
    printf("  push rax\n");
}

void gen(Node *node) {
    // printf("%d\n", node->op);
    if (node == NULL)
        error("node is null");

    switch (node->op) {
        case ND_VARDEF:
            gen_lval(node);
            if (!node->init)
                return;
            gen(node->init);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  mov [rax], %s\n", get_reg(node->ty, 'd'));
            printf("  push rdi\n");
            return;

        case ND_CALL:
            for (int i = 0; i < node->args->len; i++) {
                gen(node->args->data[i]);
            }

            for (int i = node->args->len - 1; i >= 0; i--)
                printf("  pop %s\n", regs[i]);

            printf("  mov rax, 0\n"); // AL must be 0 before call a variadic function
            printf("  call %s\n", node->name);
            printf("  push rax\n");
            return;

        case ND_BLOCK:
            if (node->stmts->len == 0) {
                printf("  push 0\n");
                return;
            }

            for (int i = 0; i < node->stmts->len; i++) {
                Node *stmt = node->stmts->data[i];
                    gen(stmt);
                    printf("  pop rax\n");
            }

            printf("  push rax\n");
            return;

        case ND_RETURN:
            gen(node->expr);
            printf("  pop rax\n");
            printf("  mov rsp, rbp\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            return;

        case ND_IF: {
            int label_num = label_count++;
            gen(node->cond);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lelse%d\n", label_num);
            gen(node->then);
            printf("  jmp .Lend%d\n", label_num);
            printf(".Lelse%d:\n", label_num);
            if (node->els)
                gen(node->els);
            else
                printf(" push rax\n"); // because when there is no 'else', 'if' leaves nothing on the stack
            printf(".Lend%d:\n", label_num);
            return;
        }
        
        case ND_WHILE: {
            int label_num = label_count++;
            node->break_num = label_num;
            node->continue_num = label_num;
            printf(".Lbegin%d:\n", label_num);
            gen(node->cond);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lend%d\n", label_num);
            gen(node->body);
            printf(".Lcontinue%d:\n", label_num);
            printf("  pop rax\n");      // discard last value returned from body
            printf("  jmp .Lbegin%d\n", label_num);
            printf(".Lend%d:\n", label_num);
            printf(" push rax\n"); // TODO: leave some value on tha stack for block statement
            return;
        }

        case ND_DO_WHILE: {
            int label_num = label_count++;
            node->break_num = label_num;
            node->continue_num = label_num;
            printf(".Lbegin%d:\n", label_num);
            gen(node->body);
            printf(".Lcontinue%d:\n", label_num);
            printf("  pop rax\n");      // discard last value returned from body
            gen(node->cond);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lend%d\n", label_num);
            printf("  jmp .Lbegin%d\n", label_num);
            printf(".Lend%d:\n", label_num);
            printf(" push rax\n"); // TODO: leave some value on tha stack for block statement
            return;
        }

        case ND_FOR: {
            int label_num = label_count++;
            node->break_num = label_num;
            node->continue_num = label_num;
            if (node->init) {
                gen(node->init);
                printf("  pop rax\n"); // TODO:discard last value returned from body
            }
            printf(".Lbegin%d:\n", label_num);
            if (node->cond)
                gen(node->cond);
            else
                printf("  push 1\n"); // always true
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lend%d\n", label_num);
            gen(node->body);
            printf(".Lcontinue%d:\n", label_num);
            printf("  pop rax\n");      // discard last value returned from body
            if (node->inc) {
                gen(node->inc);
                printf("  pop rax\n"); // TODO:discard last value returned from body
            }
            printf("  jmp .Lbegin%d\n", label_num);
            printf(".Lend%d:\n", label_num);
            printf("  push rax\n"); // TODO: leave some value on tha stack for block statement
            return;
        }

        case ND_BREAK:
            printf("  jmp .Lend%d\n", node->target->break_num);
            return;
        case ND_CONTINUE:
            printf("  push rax\n"); // TODO: leave some value on tha stack for block statement
            printf("  jmp .Lcontinue%d\n", node->target->continue_num);
            return;

        case ND_NUM:
            printf("  push %d\n", node->val);
            return;

        case ND_IDENT:
            gen_lval(node);
            printf("  pop rax\n");
            printf("  mov rsi, rax\n"); // save address of identifier for later use
            emit_load(node);
            printf("  push rax\n");
            return;

        case ND_DEREF:
            gen(node->expr);
            printf("  pop rax\n");
            printf("  mov rsi, rax\n"); // save address of expr for later use
            emit_load(node);
            printf("  push rax\n");
            return;

        case ND_ADDR:
            gen_lval(node->expr); // RAX = address of IDENT
            return;

        case ND_POST_INC: {
            gen(node->expr);
            printf("  add rax, 1\n");
            printf("  mov [rsi], %s\n", get_reg(node->ty, 'a')); // address of expr should be stored in rsi
            return;
        }
        case ND_POST_DEC: {
            gen(node->expr);
            printf("  sub rax, 1\n");
            printf("  mov [rsi], %s\n", get_reg(node->ty, 'a')); // address of expr should be stored in rsi
            return;
        }

        case ND_LOG_OR: {
            int label_num = label_count++;
            gen(node->lhs);
            printf("  pop rax\n");
            printf("  push rax\n");     // save lhs result for return value
            printf("  cmp rax, 0\n");
            printf("  jne .Lend%d\n", label_num);
            printf("  pop rax\n");  // discard lhs result
            gen(node->rhs);
            printf(".Lend%d:\n", label_num);
            return;
        }

        case ND_LOG_AND: {
            int label_num = label_count++;
            gen(node->lhs);
            printf("  pop rax\n");
            printf("  push rax\n");     // save lhs result for return value
            printf("  cmp rax, 0\n");
            printf("  je .Lend%d\n", label_num);
            printf("  pop rax\n");  // discard lhs result
            gen(node->rhs);
            printf(".Lend%d:\n", label_num);
            return;
        }

        case ND_LOG_NOT: {
            int label_num = label_count++;
            gen(node->expr);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lelse%d\n", label_num);
            printf("  push 0\n");       // if result != 0, return 0
            printf("  jmp .Lend%d\n", label_num);
            printf(".Lelse%d:\n", label_num);
            printf("  push 1\n");       // if result == 0, return 1
            printf(".Lend%d:\n", label_num);
            return;
        }

        case '~':
            gen(node->expr);
            printf("  pop rax\n");
            printf("  not rax\n");
            printf("  push rax\n");
            return;

        case '=':
            gen_lval(node->lhs);
            gen(node->rhs);

            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  mov [rax], %s\n", get_reg(node->ty, 'd'));
            printf("  push rdi\n");
            return;
        
        default:
            emit_binop(node);
            return;
    }
}

void gen_gvar(Var *v) {
    if (v->is_extern)
        return;

    printf("%s: \n", v->name);
    if (v->data == 0) {
        printf("  .zero  %d\n", v->ty->size);
    }
    else {
        printf(" .string %s\n", v->data);
    }

}


void gen_x86(Program *prog) {
    printf(".intel_syntax noprefix\n");
    printf(".data\n");

    // Global variables
    gvars = prog->gvars;
    for (int i = 0; i < prog->gvars->keys->len; i++) {
        Var *v = prog->gvars->vals->data[i];
        gen_gvar(v);
    }

    printf(".text\n");

    // Functions
    for (int i = 0; i < prog->fns->len - 1; i++)
    {
        gen_func(prog->fns->data[i]);
    }
}