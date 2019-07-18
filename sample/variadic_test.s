# 1
# 2
# fmt is .LSTR%d 
#gp is 8
#fp is  48
#overflow is  0x7ffe30821830
#reg_save_area is  0x7ffe30821770
# fmt is .LSTR%d 
# fmt is .LSTR%d 
#gp is 8
#fp is  48
#overflow is  0x7ffe30821520
#reg_save_area is  0x7ffe30821460
# fmt is .LSTR%d 
# fmt is .LSTR%d 
#gp is 8
#fp is  48
#overflow is  0x7ffe30821520
#reg_save_area is  0x7ffe30821460
# fmt is .LSTR%d 
# fmt is .LSTR%d 
#gp is 8
#fp is  48
#overflow is  0x7ffe30821520
#reg_save_area is  0x7ffe30821460
# fmt is .LSTR%d 
# 3
# 4
.intel_syntax noprefix
.data
.LSTR0: 
 .string "test\n"
.LSTR1: 
 .string "test\n"
.LSTR2: 
 .string "test%d %d %d %d"
.LSTR3: 
 .string "%s\n"
.text
.global my_vsprintf
my_vsprintf:
  push rbp
  mov rbp, rsp
# set up for variadic function
  sub rsp, 96
  mov QWORD PTR [rbp - 56], r9
  mov QWORD PTR [rbp - 64], r8
  mov QWORD PTR [rbp - 72], rcx
  mov QWORD PTR [rbp - 80], rdx
  mov QWORD PTR [rbp - 88], rsi
  mov QWORD PTR [rbp - 96], rdi
# set up for variadic function end
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov [rax], rdi
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov [rax], rsi
  mov rax, rbp
  sub rax, 40
  push rax
  pop rax
# Emit va_start
# Assignment
#Get addressof Dot operator begin
#ADDR: 
  mov rax, rbp
  sub rax, 40
  push rax
  pop rax
  add rax, 0
  push rax
#Get addressof Dot operator end
  push 16
  pop rdi
  pop rax
  mov [rax], edi
  push rdi
  pop rax
# Assignment
#Get addressof Dot operator begin
#ADDR: 
  mov rax, rbp
  sub rax, 40
  push rax
  pop rax
  add rax, 4
  push rax
#Get addressof Dot operator end
  push 48
  pop rdi
  pop rax
  mov [rax], edi
  push rdi
  pop rax
#ADDR: 
#Get addressof Dot operator begin
#ADDR: 
  mov rax, rbp
  sub rax, 40
  push rax
  pop rax
  add rax, 8
  push rax
#Get addressof Dot operator end
#overflow area
  lea rdi, [rbp + 16]
  pop rax
  mov QWORD PTR [rax], rdi
#ADDR: 
#Get addressof Dot operator begin
#ADDR: 
  mov rax, rbp
  sub rax, 40
  push rax
  pop rax
  add rax, 16
  push rax
#Get addressof Dot operator end
# 96
#reg_save_area
  lea rdi, [rbp - 96]
  pop rax
  mov QWORD PTR [rax], rdi
  push 0
  pop rax
# function call: vsprintf
# function parameters
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov rsi, rax
# load value
  mov rax, [rax]
  push rax
# function parameters
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov rsi, rax
# load value
  mov rax, [rax]
  push rax
# function parameters
#ADDR: 
  mov rax, rbp
  sub rax, 40
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov eax, 0
  test rsp, 0x8
  je .Lcall0
# not align on 16
  sub rsp, 8
  call vsprintf
  add rsp, 8
  jmp .Lcall_end0
.Lcall0:
  call vsprintf
.Lcall_end0:
  push rax
  pop rax
  push 0
  pop rax
  push rax
  pop rax
  mov rsp, rbp
  pop rbp
  ret
.global main
main:
  push rbp
  mov rbp, rsp
  sub rsp, 144
# function call: printf
# function parameters
#ADDR: 
  lea rax, .LSTR0[rip]
  push rax
  pop rdi
  mov eax, 0
  test rsp, 0x8
  je .Lcall1
# not align on 16
  sub rsp, 8
  call printf
  add rsp, 8
  jmp .Lcall_end1
.Lcall1:
  call printf
.Lcall_end1:
  push rax
  pop rax
  mov rax, rbp
  sub rax, 4
  push rax
# function call: printf
# function parameters
#ADDR: 
  lea rax, .LSTR1[rip]
  push rax
  pop rdi
  mov eax, 0
  test rsp, 0x8
  je .Lcall2
# not align on 16
  sub rsp, 8
  call printf
  add rsp, 8
  jmp .Lcall_end2
.Lcall2:
  call printf
.Lcall_end2:
  push rax
  pop rax
  mov rax, rbp
  sub rax, 132
  push rax
  pop rax
# function call: my_vsprintf
# function parameters
#ADDR: 
  mov rax, rbp
  sub rax, 132
  push rax
# function parameters
#ADDR: 
  lea rax, .LSTR2[rip]
  push rax
# function parameters
  push 1
# function parameters
  push 2
# function parameters
  push 3
# function parameters
  push 4
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
  mov eax, 0
  test rsp, 0x8
  je .Lcall3
# not align on 16
  sub rsp, 8
  call my_vsprintf
  add rsp, 8
  jmp .Lcall_end3
.Lcall3:
  call my_vsprintf
.Lcall_end3:
  push rax
  pop rax
# function call: printf
# function parameters
#ADDR: 
  lea rax, .LSTR3[rip]
  push rax
# function parameters
#ADDR: 
  mov rax, rbp
  sub rax, 132
  push rax
  pop rsi
  pop rdi
  mov eax, 0
  test rsp, 0x8
  je .Lcall4
# not align on 16
  sub rsp, 8
  call printf
  add rsp, 8
  jmp .Lcall_end4
.Lcall4:
  call printf
.Lcall_end4:
  push rax
  pop rax
  push rax
  pop rdi
  pop rax
  mov [rax], edi
  push rdi
  pop rax
# Return
  push 0
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  pop rax
  push rax
  pop rax
  mov rsp, rbp
  pop rbp
  ret
