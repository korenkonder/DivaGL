/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shader_storage_buffer.hpp"
#include "../gl_state.hpp"

namespace GL {
    void ShaderStorageBuffer::Bind(int32_t index, bool force) {
        gl_state_bind_shader_storage_buffer_base(index, buffer, force);
    }

    void ShaderStorageBuffer::Create(size_t size) {
        if (buffer)
            return;

        if (GL_VERSION_4_5) {
            glCreateBuffers(1, &buffer);
            glNamedBufferStorage(buffer, (GLsizeiptr)size, 0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        else {
            glGenBuffers(1, &buffer);
            gl_state_bind_shader_storage_buffer(buffer, true);
            if (GL_VERSION_4_4)
                glBufferStorage(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)size,
                    0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
            else
                glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)size, 0, GL_DYNAMIC_DRAW);
        }
    }

    void ShaderStorageBuffer::Create(size_t size, const void* data, bool dynamic) {
        if (buffer)
            return;

        if (GL_VERSION_4_5) {
            glCreateBuffers(1, &buffer);
            glNamedBufferStorage(buffer, (GLsizeiptr)size, data,
                dynamic ? GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT : 0);
        }
        else {
            glGenBuffers(1, &buffer);
            gl_state_bind_shader_storage_buffer(buffer, true);
            if (GL_VERSION_4_4)
                glBufferStorage(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)size, data,
                    dynamic ? GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT : 0);
            else
                glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)size, data,
                    dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
    }

    void ShaderStorageBuffer::Destroy() {
        if (buffer) {
            glDeleteBuffers(1, &buffer);
            buffer = 0;
        }
    }

    void* ShaderStorageBuffer::MapMemory() {
        if (!buffer)
            return 0;

        void* data;
        if (GL_VERSION_4_5)
            data = glMapNamedBuffer(buffer, GL_WRITE_ONLY);
        else {
            gl_state_bind_shader_storage_buffer(buffer);
            data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
        }

        if (data)
            return data;

        if (GL_VERSION_4_5)
            glUnmapNamedBuffer(buffer);
        else {
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            gl_state_bind_shader_storage_buffer(0);
        }
        return 0;
    }

    void* ShaderStorageBuffer::MapMemory(size_t offset, size_t length) {
        if (!buffer)
            return 0;

        void* data;
        if (GL_VERSION_4_5)
            data = glMapNamedBufferRange(buffer, (GLintptr)offset, (GLsizeiptr)length, GL_WRITE_ONLY);
        else {
            gl_state_bind_shader_storage_buffer(buffer);
            data = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, (GLintptr)offset, (GLsizeiptr)length, GL_WRITE_ONLY);
        }

        if (data)
            return data;

        if (GL_VERSION_4_5)
            glUnmapNamedBuffer(buffer);
        else {
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            gl_state_bind_shader_storage_buffer(0);
        }
        return 0;
    }

    void ShaderStorageBuffer::UnmapMemory() {
        if (!buffer)
            return;

        if (GL_VERSION_4_5)
            glUnmapNamedBuffer(buffer);
        else {
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
            gl_state_bind_shader_storage_buffer(0);
        }
    }

    void ShaderStorageBuffer::WriteMemory(size_t offset, size_t size, const void* data) {
        if (!buffer)
            return;

        if (GL_VERSION_4_5)
            glNamedBufferSubData(buffer, (GLsizeiptr)offset, (GLsizeiptr)size, data);
        else {
            gl_state_bind_shader_storage_buffer(buffer);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)offset, (GLsizeiptr)size, data);
        }
    }
}
