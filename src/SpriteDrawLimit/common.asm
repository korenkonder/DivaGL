EXTERN ?originalspr__calc_sprite_texture_param_mid1@@3P6AXXZEA : QWORD
EXTERN ?originalspr__calc_sprite_texture_param_mid2@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_scale_mid@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_mid_spr_vertex_array_pos@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_mid_spr_vertex_array_uv@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_mid_spr_vertex_array_color@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_mid_spr_kind_arrow_b@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_mid_spr_kind_arrow_ab@@3P6AXXZEA : QWORD
EXTERN ?originalspr__draw_sprite_mid_spr_kind_triangle@@3P6AXXZEA : QWORD

EXTERN ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA : QWORD

.CODE
?implOfspr__calc_sprite_texture_param_mid1@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rax, [rbx+0B0h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rax, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__calc_sprite_texture_param_mid1@@3P6AXXZEA
?implOfspr__calc_sprite_texture_param_mid1@@YAXXZ ENDP

?implOfspr__calc_sprite_texture_param_mid2@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rax, [rbx+0B0h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rax, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__calc_sprite_texture_param_mid2@@3P6AXXZEA
?implOfspr__calc_sprite_texture_param_mid2@@YAXXZ ENDP

?implOfspr__draw_sprite_scale_mid@@YAXXZ PROC PUBLIC
    push rax
    push rdx

    mov rax, [rbx+0B0h]
    lea rdx, [rax+rax*2]
    mov rax, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rax+8]
    lea rcx, [rcx+rdx*8]

    pop rdx
    pop rax
    jmp ?originalspr__draw_sprite_scale_mid@@3P6AXXZEA
?implOfspr__draw_sprite_scale_mid@@YAXXZ ENDP

?implOfspr__draw_sprite_mid_spr_vertex_array_pos@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov r9, [rbx+0B0h]
    lea rdx, [r9+r9*2]
    mov r9, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [r9+8]
    lea r9, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__draw_sprite_mid_spr_vertex_array_pos@@3P6AXXZEA
?implOfspr__draw_sprite_mid_spr_vertex_array_pos@@YAXXZ ENDP

?implOfspr__draw_sprite_mid_spr_vertex_array_uv@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov r9, [rbx+0B0h]
    lea rdx, [r9+r9*2]
    mov r9, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [r9+8]
    lea r9, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__draw_sprite_mid_spr_vertex_array_uv@@3P6AXXZEA
?implOfspr__draw_sprite_mid_spr_vertex_array_uv@@YAXXZ ENDP

?implOfspr__draw_sprite_mid_spr_vertex_array_color@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov r9, [rbx+0B0h]
    lea rdx, [r9+r9*2]
    mov r9, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [r9+8]
    lea r9, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__draw_sprite_mid_spr_vertex_array_color@@3P6AXXZEA
?implOfspr__draw_sprite_mid_spr_vertex_array_color@@YAXXZ ENDP

?implOfspr__draw_sprite_mid_spr_kind_arrow_b@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rbx, [rbx+0B0h]
    lea rdx, [rbx+rbx*2]
    mov rbx, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rbx+8]
    lea rbx, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__draw_sprite_mid_spr_kind_arrow_b@@3P6AXXZEA
?implOfspr__draw_sprite_mid_spr_kind_arrow_b@@YAXXZ ENDP

?implOfspr__draw_sprite_mid_spr_kind_arrow_ab@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rbx, [rbx+0B0h]
    lea rdx, [rbx+rbx*2]
    mov rbx, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rbx+8]
    lea rbx, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__draw_sprite_mid_spr_kind_arrow_ab@@3P6AXXZEA
?implOfspr__draw_sprite_mid_spr_kind_arrow_ab@@YAXXZ ENDP

?implOfspr__draw_sprite_mid_spr_kind_triangle@@YAXXZ PROC PUBLIC
    push rcx
    push rdx

    mov rbx, [rbx+0B0h]
    lea rdx, [rbx+rbx*2]
    mov rbx, ?sprite_vertex_data@@3AEAUSpriteVertexData@spr@@EA
    mov rcx, [rbx+8]
    lea rbx, [rcx+rdx*8]

    pop rdx
    pop rcx
    jmp ?originalspr__draw_sprite_mid_spr_kind_triangle@@3P6AXXZEA
?implOfspr__draw_sprite_mid_spr_kind_triangle@@YAXXZ ENDP
END