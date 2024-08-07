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

EXTERN ?pv_game_load_state_4_tail_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_state_4_tail@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_state_6_head_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_state_6_head@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_state_10_head_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_state_10_head@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_state_13_head_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_state_13_head@@3P6AXXZEA : QWORD

.DATA
pv_game_load_jump_end QWORD 00000001401043ECh

.CODE
?implOfpv_game_load_state_4_tail@@YAXXZ PROC PUBLIC
    pushaq
    mov rcx, r14
    call ?pv_game_load_state_4_tail_impl@@YA_NXZ
    test al, al
    popaq
    jz end_func
    jmp pv_game_load_jump_end
end_func:
    jmp ?originalpv_game_load_state_4_tail@@3P6AXXZEA
?implOfpv_game_load_state_4_tail@@YAXXZ ENDP

?implOfpv_game_load_state_6_head@@YAXXZ PROC PUBLIC
    pushaq
    mov rcx, r14
    call ?pv_game_load_state_6_head_impl@@YA_NXZ
    test al, al
    popaq
    jz end_func
    jmp pv_game_load_jump_end
end_func:
    jmp ?originalpv_game_load_state_6_head@@3P6AXXZEA
?implOfpv_game_load_state_6_head@@YAXXZ ENDP

?implOfpv_game_load_state_10_head@@YAXXZ PROC PUBLIC
    pushaq
    call ?pv_game_load_state_10_head_impl@@YA_NXZ
    test al, al
    popaq
    jz end_func
    jmp pv_game_load_jump_end
end_func:
    jmp ?originalpv_game_load_state_10_head@@3P6AXXZEA
?implOfpv_game_load_state_10_head@@YAXXZ ENDP

?implOfpv_game_load_state_13_head@@YAXXZ PROC PUBLIC
    pushaq
    mov rcx, r14
    call ?pv_game_load_state_13_head_impl@@YA_NXZ
    test al, al
    popaq
    jz end_func
    jmp pv_game_load_jump_end
end_func:
    jmp ?originalpv_game_load_state_13_head@@3P6AXXZEA
?implOfpv_game_load_state_13_head@@YAXXZ ENDP
END