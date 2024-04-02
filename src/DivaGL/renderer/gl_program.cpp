/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "gl_program.hpp"
#include "../gl_state.hpp"

namespace renderer {
    bool GLProgram::compile_program(GLProgram::ShaderData* shader_data, int32_t count) {
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }

        program = glCreateProgram();

        for (int32_t i = 0; i < count; i++, shader_data++) {
            GLuint shader = glCreateShader(shader_data->type);
            if (!renderer::GLProgram::compile_shader(shader, shader_data->string, shader_data->length))
                return false;

            glAttachShader(program, shader);
            glDeleteShader(shader);
        }

        glLinkProgram(program);
        return !glGetErrorDLL();
    }
    
    bool GLProgram::compile_shader(GLuint shader, const char* string, int32_t length) {
        const char* strings[] = { string };
        int32_t lengths[] = { length };

        glShaderSource(shader, 1, strings, lengths);
        glCompileShader(shader);

        int32_t info_log_length;
        char info_log[0x400];
        glGetShaderInfoLog(shader, sizeof(info_log), &info_log_length, info_log);

        int32_t compile_status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
        return compile_status == GL_TRUE;
    }
}
