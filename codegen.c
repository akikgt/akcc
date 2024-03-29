#include "akcc.h"

static int label_count;

char *regs[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
char *regs8[6] = {"dil", "sil", "dl", "cl", "r8b", "r9b"};
char *regs32[6] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};


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
    printf("# load value\n");
    printf("  mov %s, [rax]\n", get_reg(node->ty, 'a'));
    if (node->ty->size == 1) {
        // TODO: consider movzx
        printf("  movsx eax, %s\n", get_reg(node->ty, 'a'));
    }
    else if (node->ty->size == 4) {
        printf("# sign extend for INT\n");
        printf("  movsxd rax, eax\n");
    }
}

static void emit_binop(Node *node) {
    gen(node->lhs);
    gen(node->rhs);

// TODO: sign extended
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
        printf("# Less than\n");
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_SHL:
        printf("# Shift left\n");
        printf("  mov cl, dil\n");
        printf("  shl rax, cl\n");
        break;
    case ND_SHR:
        printf("# Shift right\n");
        printf("  mov cl, dil\n");
        printf("  shr rax, cl\n");
        break;
    case '<':
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ',':
        printf("# ',' operator\n");
        printf("  mov rax, rdi\n"); // save rhs result
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

    // calculate total local variable size
    int var_size = 0;
    if (fn->lvars->len != 0) {
        Var *last_var = vec_top(fn->lvars);
        var_size = last_var->offset;
    }

    // Alignment RSP
    // For x86-64 ABI, roundup RSP to multiplies of 16
    int rsp = roundup(var_size, 16);
    if (fn->is_variadic) {
        printf("# set up for variadic function\n");
        printf("  sub rsp, %d\n", rsp + 48); 
        // 8byte general purpose register * 6
        for (int i = 0; i < 6; i++) {
            rsp += 8;
            printf("  mov QWORD PTR [rbp - %d], %s\n", rsp, regs[5 - i]);
        }
        fn->reg_save_area = rsp;
        printf("# set up for variadic function end\n");
    }
    else
        printf("  sub rsp, %d\n", rsp); 

    // set parameters
    for (int i = 0; i < node->args->len; i++) {
        Node *param = node->args->data[i];
        if (param->op == ND_VARARGS)
            continue;
        gen_lval(param);
        // set each parameters to local variable address
        printf("  pop rax\n");
        switch (param->ty->size) {
            case 1:
                printf("  mov [rax], %s\n", regs8[i]);
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

    if (node->op == ND_DOT) {
        printf("#Get addressof Dot operator begin\n");
        gen_lval(node->expr);
        printf("  pop rax\n");
        printf("  add rax, %d\n", node->ty->offset);
        printf("  push rax\n");
        printf("#Get addressof Dot operator end\n");
        return;
    }

    // if (node->op != ND_IDENT && node->op != ND_VARDEF) 
    if (!node->can_be_lval)
        error("invalid lval. op: %d", node->op);

    Var *var = node->var;
    // Global variable
    if (!var->is_local) {
        printf("  lea rax, %s[rip]\n", node->name);
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
            printf("# function call: %s\n", node->name);
            for (int i = 0; i < node->args->len; i++) {
                printf("# function parameters\n");
                gen(node->args->data[i]);
            }

            // TODO: compiler friendly loop
            // for (int i = node->args->len - 1; i >= 0; i--)
            if (node->args->len >= 1) {
                for (int i = node->args->len - 1;; i--) {
                    printf("  pop %s\n", regs[i]);
                    if (i == 0)
                        break;
                }
            }
            
            printf("  mov eax, 0\n"); // AL must be 0 before call a variadic function
            // rsp alignment
            printf("  test rsp, 0x8\n");
            printf("  je .Lcall%d\n", label_count);
            // align
            printf("# not align on 16\n");
            printf("  sub rsp, 8\n");
            printf("  call %s\n", node->name);
            printf("  add rsp, 8\n");
            printf("  jmp .Lcall_end%d\n", label_count);

            printf(".Lcall%d:\n", label_count);
            // already aligned
            printf("  call %s\n", node->name);
            printf(".Lcall_end%d:\n", label_count++);
            printf("  push rax\n");
            return;

        case ND_VA_START: {
            printf("# Emit va_start\n");
            for (int i = 0; i < node->stmts->len; i++) {
                Node *stmt = node->stmts->data[i];
                gen(stmt);
                if (i == 2) {
                    printf("#overflow area\n");
                    printf("  lea rdi, [rbp + 16]\n");
                    printf("  pop rax\n");
                    printf("  mov QWORD PTR [rax], rdi\n");
                }
                else if (i == 3) {
                    Function * fn = node->fn;
                    printf("# %d\n", fn->reg_save_area);
                    printf("#reg_save_area\n");
                    printf("  lea rdi, [rbp - %d]\n", fn->reg_save_area);
                    printf("  pop rax\n");
                    printf("  mov QWORD PTR [rax], rdi\n");
                }
                else
                    printf("  pop rax\n");
            }

            printf("  push 0\n");
            return;
        }
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
            printf("# Return\n");
            if (node->expr) {
                gen(node->expr);
                printf("  pop rax\n");
            }
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

        case ND_SWITCH: {
            int label_num = label_count++;
            node->break_num = label_num;
            gen(node->cond);
            printf("  pop rax\n");
            // emit cases
            for (int i = 0; i < node->cases->len; i++) {
                Node *c = node->cases->data[i];
                c->switch_num = label_num;
                if (c->op == ND_DEFAULT) {
                    printf("  jmp .Lcase_default_%d\n", label_num);
                    continue;
                }
                printf("  cmp rax, %d\n", c->val);
                printf("  je .Lcase%d_%d\n", c->val, label_num);
            }
            printf("  jmp .Lend%d\n", label_num);

            gen(node->body);

            printf("  pop rax\n"); 
            printf(".Lend%d:\n", label_num);
            printf("  push rax\n"); // TODO: leave some value on tha stack for block statement
            return;
        }

        case ND_CASE:
            printf(".Lcase%d_%d:\n", node->val, node->switch_num);
            gen(node->body);
            return;

        case ND_DEFAULT:
            printf(".Lcase_default_%d:\n", node->switch_num);
            gen(node->body);
            return;

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

        case ND_DOT: {
            printf("# Dot operator\n");
            if (node->expr->op == ',' && node->expr->can_be_lval) {
                printf("# ',' operator\n");
                gen(node->expr->lhs);
                printf("  pop rax\n");  // discard result from lhs
                gen_lval(node->expr->rhs);
            }
            else {
                gen_lval(node->expr);
            }
            printf("  pop rax\n");
            printf("  add rax, %d\n", node->ty->offset);
            printf("  mov rsi, rax\n"); // save address of identifier for later use
            emit_load(node);
            printf("  push rax\n");
            break;
        }

        case ND_DEREF:
            gen(node->expr);
            printf("  pop rax\n");
            printf("  mov rsi, rax\n"); // save address of expr for later use
            emit_load(node);
            printf("  push rax\n");
            return;

        case ND_ADDR:
            printf("#ADDR: \n");
            gen_lval(node->expr); // RAX = address of IDENT
            return;

        case ND_CAST:
            gen(node->expr);
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
            printf("# Assignment\n");
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

char *get_size_directive(Type *ty) {
    switch (ty->ty) {
        case CHAR:
            return ".byte";
        case INT:
            return ".long";
        case PTR:
        case ARRAY:
            return ".quad";
        default:
            error("cannot get size directive");
    }
}

void gen_gvar(Var *v) {
    if (v->is_extern)
        return;

    if (!v->str_data) {
        printf("  .global %s\n", v->name);
    }

    printf("%s: \n", v->name);
    // TODO: clean up
    if (v->has_init) {
        if (v->str_data) {
            printf(" .string \"%s\"\n", v->str_data);
            return;
        }

        if (v->ty->ty != ARRAY) {
            printf("  %s  %d\n", get_size_directive(v->ty), v->init_val);
            return;
        }

        // array case 
        for (int i = 0; i < v->arr_data->len; i++) {
            Var *var = v->arr_data->data[i];
            if (var->ty->ty == ARRAY)
                printf("  .quad  %s\n", var->name);
            else
                printf("  %s  %d\n", get_size_directive(var->ty), var->init_val);
        }
    }
    else {
        printf("  .zero  %d\n", v->ty->size);
    }

    return;
}

void gen_x86(Program *prog) {
    printf(".intel_syntax noprefix\n");
    printf(".data\n");

    // Global variables
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