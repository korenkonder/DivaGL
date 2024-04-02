/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/default.hpp"
#include "../../KKdLib/vec.hpp"
#include "../gl_state.hpp"
#include "../shared.hpp"

namespace renderer {
    struct GLProgram {
        struct ShaderData {
            GLenum type;
            const char* string;
            int32_t length;
        };

        GLuint program;

        bool compile_program(GLProgram::ShaderData* shader_data, int32_t count);

        static bool compile_shader(GLuint shader, const char* string, int32_t length);

        inline void use_program() {
            gl_state_use_program(program);
        }
    };

    static_assert(sizeof(renderer::GLProgram) == 0x04, "\"renderer::GLProgram\" struct should have a size of 0x04");
};
