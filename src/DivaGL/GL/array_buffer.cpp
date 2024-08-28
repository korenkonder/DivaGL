/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "array_buffer.hpp"
#include "../gl_state.hpp"

namespace GL {
    void ArrayBuffer::Bind(bool force) {
        gl_state_bind_array_buffer(buffer, force);
    }

    void ArrayBuffer::Create(size_t size) {
        if (buffer)
            return;

        if (GL_VERSION_4_5) {
            glCreateBuffers(1, &buffer);
            glNamedBufferStorage(buffer, (GLsizeiptr)size, 0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        else {
            glGenBuffers(1, &buffer);
            gl_state_bind_array_buffer(buffer, true);
            if (GL_VERSION_4_4)
                glBufferStorage(GL_ARRAY_BUFFER, (GLsizeiptr)size,
                    0, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
            else
                glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, 0, GL_DYNAMIC_DRAW);
        }
    }

    void ArrayBuffer::Create(size_t size, const void* data, bool dynamic) {
        if (buffer)
            return;

        if (GL_VERSION_4_5) {
            glCreateBuffers(1, &buffer);
            glNamedBufferStorage(buffer, (GLsizeiptr)size, data,
                dynamic ? GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT : 0);
        }
        else {
            glGenBuffers(1, &buffer);
            gl_state_bind_array_buffer(buffer, true);
            if (GL_VERSION_4_4)
                glBufferStorage(GL_ARRAY_BUFFER, (GLsizeiptr)size, data,
                    dynamic ? GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT : 0);
            else
                glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data,
                    dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
    }

    void ArrayBuffer::Destroy() {
        if (buffer) {
            glDeleteBuffers(1, &buffer);
            buffer = 0;
        }
    }

    void* ArrayBuffer::MapMemory() {
        if (!buffer)
            return 0;

        void* data;
        if (GL_VERSION_4_5)
            data = glMapNamedBuffer(buffer, GL_WRITE_ONLY);
        else {
            gl_state_bind_array_buffer(buffer);
            data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

        if (data)
            return data;

        if (GL_VERSION_4_5)
            glUnmapNamedBuffer(buffer);
        else {
            glUnmapBuffer(GL_ARRAY_BUFFER);
            gl_state_bind_array_buffer(0);
        }
        return 0;
    }

    void ArrayBuffer::UnmapMemory() {
        if (!buffer)
            return;

        if (GL_VERSION_4_5)
            glUnmapNamedBuffer(buffer);
        else {
            glUnmapBuffer(GL_ARRAY_BUFFER);
            gl_state_bind_array_buffer(0);
        }
    }

    void ArrayBuffer::WriteMemory(size_t offset, size_t size, const void* data) {
        if (!buffer)
            return;

        if (GL_VERSION_4_5)
            glNamedBufferSubData(buffer, (GLsizeiptr)offset, (GLsizeiptr)size, data);
        else {
            gl_state_bind_array_buffer(buffer);
            glBufferSubData(GL_ARRAY_BUFFER, (GLsizeiptr)offset, (GLsizeiptr)size, data);
        }
    }
}
