.intel_syntax noprefix
.text
  .globl _start

  _start:
    xor      rbp,rbp
    pop      rdi
    mov      rsi,rsp
    and      rsp,-16
    call     main
    mov      rdi,rax
    mov      rax,60
    syscall
    ret
