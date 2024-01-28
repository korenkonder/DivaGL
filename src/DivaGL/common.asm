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

EXTERN ?pv_game_load_state_6_tail_impl@@YAX_K@Z : PROC
EXTERN ?originalpv_game_load_state_6_tail@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_state_8_tail_impl@@YAX_K@Z : PROC
EXTERN ?originalpv_game_load_state_8_tail@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_state_9_mid_impl@@YA_NXZ : PROC
EXTERN ?originalpv_game_load_state_9_mid@@3P6AXXZEA : QWORD

EXTERN ?pv_game_load_state_13_tail_impl@@YAX_K@Z : PROC
EXTERN ?originalpv_game_load_state_13_tail@@3P6AXXZEA : QWORD

.CODE
?implOfpv_game_load_state_6_tail@@YAXXZ PROC PUBLIC
    pushaq
    mov rcx, r14
    call ?pv_game_load_state_6_tail_impl@@YAX_K@Z
    popaq
    jmp ?originalpv_game_load_state_6_tail@@3P6AXXZEA
?implOfpv_game_load_state_6_tail@@YAXXZ ENDP

?implOfpv_game_load_state_8_tail@@YAXXZ PROC PUBLIC
    pushaq
    mov rcx, r14
    call ?pv_game_load_state_8_tail_impl@@YAX_K@Z
    popaq
    jmp ?originalpv_game_load_state_8_tail@@3P6AXXZEA
?implOfpv_game_load_state_8_tail@@YAXXZ ENDP

?implOfpv_game_load_state_9_mid@@YAXXZ PROC PUBLIC
    pushaq
    call ?pv_game_load_state_9_mid_impl@@YA_NXZ
    test al, al
    popaq
    cmovne ebx,r15d
    jmp ?originalpv_game_load_state_9_mid@@3P6AXXZEA
?implOfpv_game_load_state_9_mid@@YAXXZ ENDP

?implOfpv_game_load_state_13_tail@@YAXXZ PROC PUBLIC
    pushaq
    mov rcx, r14
    call ?pv_game_load_state_13_tail_impl@@YAX_K@Z
    popaq
    jmp ?originalpv_game_load_state_13_tail@@3P6AXXZEA
?implOfpv_game_load_state_13_tail@@YAXXZ ENDP
END