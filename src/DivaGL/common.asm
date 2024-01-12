pushaq MACRO
    push rax
    push rcx
    push rdx
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
ENDM

popaq MACRO
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdx
    pop rcx
    pop rax
ENDM

EXTERN ?pv_game_load_state_13_tail_impl@@YAX_K@Z : PROC

.DATA
pv_game_load_state_13_tail_jump QWORD 00000001401043ECh

.CODE

PUBLIC ?implOfpv_game_load_state_13_tail@@YAXXZ

?implOfpv_game_load_state_13_tail@@YAXXZ :
    pushaq
    mov rcx, r14
    call ?pv_game_load_state_13_tail_impl@@YAX_K@Z
    popaq
    jmp [pv_game_load_state_13_tail_jump]
END
