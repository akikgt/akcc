# fmt is .LSTR%d 
#gp is 8
#fp is  48
#overflow is  0x7ffee6adea80
#reg_save_area is  0x7ffee6ade9c0
# fmt is .LSTR%d 
# fmt is .LSTR%d 
#gp is 8
#fp is  48
#overflow is  0x7ffee6adea80
#reg_save_area is  0x7ffee6ade9c0
# fmt is .LSTR%d 
.intel_syntax noprefix
.data
.LSTR0: 
 .string "test%d %d %d %d"
.LSTR1: 
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
  push rax
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
  mov rax, 0
  call vsprintf
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
  mov rax, rbp
  sub rax, 4
  push rax
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
  lea rax, .LSTR0[rip]
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
  mov rax, 0
  call my_vsprintf
  push rax
  pop rax
# function call: printf
# function parameters
#ADDR: 
  lea rax, .LSTR1[rip]
  push rax
# function parameters
#ADDR: 
  mov rax, rbp
  sub rax, 132
  push rax
  pop rsi
  pop rdi
  mov rax, 0
  call printf
  push rax
  pop rax
  push rax
  pop rdi
  pop rax
  mov [rax], edi
  push rdi
  pop rax
  push rax
  pop rax
  mov rsp, rbp
  pop rbp
  ret
