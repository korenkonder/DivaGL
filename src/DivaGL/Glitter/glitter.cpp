/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../gl_state.hpp"
#include "../wrap.hpp"
#include <Helpers.h>

namespace Glitter {
    const float_t min_emission = 0.01f;

    void axis_angle_from_vectors(vec3* axis, float_t* angle, const vec3* vec1, const vec3* vec2) {
        *axis = vec3::cross(*vec1, *vec2);
        *angle = vec3::length(*axis);

        if (*angle >= 0.000001f)
            *angle = asinf(clamp_def(*angle, -1.0f, 1.0f));
        else {
            *angle = 0.0f;
            *axis = vec3::cross(vec3(0.0f, 1.0f, 0.0f), *vec1);
            if (vec3::length(*axis) < 0.000001f)
                *axis = vec3::cross(vec3(0.0f, 0.0f, 1.0f), *vec1);
        }

        if (vec3::dot(*vec1, *vec2) < 0.0f)
            *angle = (float_t)M_PI - *angle;
    }

    void CreateBuffer(size_t max_count, bool is_quad,
        Buffer*& buffer, GLuint& vao, GL::ArrayBuffer& vbo, GL::ElementArrayBuffer& ebo) {
        if (buffer) {
            _operator_delete(buffer);
            buffer = 0;
        }

        buffer = (Buffer*)_operator_new(sizeof(Buffer) * max_count);

        glGenVertexArrays(1, &vao);
        gl_state_bind_vertex_array(vao, true);

        static const GLsizei buffer_size = sizeof(Buffer);

        vbo.Create(buffer_size * max_count);
        vbo.Bind(true);

        if (is_quad) {
            size_t count = max_count / 4 * 5;
            uint32_t* ebo_data = (uint32_t*)_operator_new(sizeof(uint32_t) * count);
            for (size_t i = 0, j = 0, k = count; k; i += 5, j += 4, k -= 5) {
                ebo_data[i + 0] = (uint32_t)(j + 0);
                ebo_data[i + 1] = (uint32_t)(j + 1);
                ebo_data[i + 2] = (uint32_t)(j + 3);
                ebo_data[i + 3] = (uint32_t)(j + 2);
                ebo_data[i + 4] = 0xFFFFFFFF;
            }

            ebo.Create(sizeof(uint32_t) * count, ebo_data);
            ebo.Bind(true);
            _operator_delete(ebo_data);
        }

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, buffer_size,
            (void*)offsetof(Buffer, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, buffer_size,
            (void*)offsetof(Buffer, uv));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, buffer_size,
            (void*)offsetof(Buffer, color));

        gl_state_bind_array_buffer(0);
        gl_state_bind_vertex_array(0);
        if (is_quad)
            gl_state_bind_element_array_buffer(0);
    }

    void DeleteBuffer(Buffer*& buffer, GLuint& vao, GL::ArrayBuffer& vbo, GL::ElementArrayBuffer& ebo) {
        ebo.Destroy();
        vbo.Destroy();

        if (vao) {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }

        if (buffer) {
            _operator_delete(buffer);
            buffer = 0;
        }
    }

    // Particle
    const char* _000000014039BC4E_patch_data
        = "\x48\x89\xE9\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x80\xFF\xD0\xEB\x67";
    const char* _00000001403AE5DE_patch_data
        = "\x48\x8B\xCB\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xD0\xEB\x03";
    const char* _00000001403AEA75_patch_data
        = "\x48\x89\xD9\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x80\xFF\xD0\xEB\x30";

    // RenderGroup
    const char* _00000001403A5B5B_patch_data
        = "\x48\x8B\xCE\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xD0\xEB\x17";
    const char* _00000001403A5C60_patch_data
        = "\x48\x8B\xD1\x48\x8B\xCE\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x80\xFF\xD0\xE9\xD9\x00\x00\x00";
    const char* _00000001403A8541_patch_data
        = "\x48\x89\xD9\x48\x8B\xD7\x48\xB8\x00\x00\x00\x00\x00\x00\x00\x80\xFF\xD0\xEB\x3B";

    void Init() {
        glt_particle_manager_x = new (_operator_new(sizeof(GltParticleManagerX))) GltParticleManagerX;
    }

    void Free() {
        if (glt_particle_manager_x) {
            glt_particle_manager_x->~GltParticleManagerX();
            _operator_delete(glt_particle_manager_x);
            glt_particle_manager_x = 0;
        }
    }

    void Patch() {
        uint8_t buf[0x100];

        // Particle
        WRITE_MEMORY(0x000000014039A396, uint32_t, sizeof(Particle));

        memcpy(buf, _000000014039BC4E_patch_data, 0x11);
        *(uint64_t*)&buf[0x05] = (uint64_t)Particle::CreateBuffer;
        WRITE_MEMORY_STRING(0x000000014039BC4E, buf, 0x11);

        memcpy(buf, _00000001403AE5DE_patch_data, 0x11);
        *(uint64_t*)&buf[0x05] = (uint64_t)Particle::InitData;
        WRITE_MEMORY_STRING(0x00000001403AE5DE, buf, 0x11);

        memcpy(buf, _00000001403AEA75_patch_data, 0x11);
        *(uint64_t*)&buf[0x05] = (uint64_t)Particle::DeleteBuffer;
        WRITE_MEMORY_STRING(0x00000001403AEA75, buf, 0x11);

        // RenderGroup
        WRITE_MEMORY(0x00000001403A9740, uint32_t, sizeof(RenderGroup));

        memcpy(buf, _00000001403A5B5B_patch_data, 0x17);
        *(uint64_t*)&buf[0x05] = (uint64_t)RenderGroup::InitData;
        WRITE_MEMORY_STRING(0x00000001403A5B5B, buf, 0x17);

        memcpy(buf, _00000001403A5C60_patch_data, 0x17);
        *(uint64_t*)&buf[0x08] = (uint64_t)RenderGroup::CreateBuffer;
        WRITE_MEMORY_STRING(0x00000001403A5C60, buf, 0x17);

        memcpy(buf, _00000001403A8541_patch_data, 0x14);
        *(uint64_t*)&buf[0x08] = (uint64_t)RenderGroup::DeleteBuffer;
        WRITE_MEMORY_STRING(0x00000001403A8541, buf, 0x14);

        // GltParticleManager
        WRITE_MEMORY(0x00000001409EB880 + 0x20, uint64_t, (uint64_t)GltParticleManager::Disp);
    }
}
