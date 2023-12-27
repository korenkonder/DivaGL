/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../gl.hpp"
#include "../gl_array_buffer.hpp"
#include "../gl_element_array_buffer.hpp"
#include "../types.hpp"
#include "../texture.hpp"

namespace Glitter {
    enum Direction {
        DIRECTION_BILLBOARD         = 0,
        DIRECTION_EMITTER_DIRECTION = 1,
        DIRECTION_PREV_POSITION     = 2,
        DIRECTION_EMIT_POSITION     = 3,
        DIRECTION_Y_AXIS            = 4,
        DIRECTION_X_AXIS            = 5,
        DIRECTION_Z_AXIS            = 6,
        DIRECTION_BILLBOARD_Y_AXIS  = 7,
        DIRECTION_PREV_POSITION_DUP = 8,
        DIRECTION_EMITTER_ROTATION  = 9,
        DIRECTION_EFFECT_ROTATION   = 10,
        DIRECTION_PARTICLE_ROTATION = 11,
    };

    enum DispType {
        DISP_NORMAL = 0,
        DISP_OPAQUE = 1,
        DISP_ALPHA  = 2,
    };

    enum FogType {
        FOG_NONE   = 0,
        FOG_DEPTH  = 1,
        FOG_HEIGHT = 2,
    };

    enum ParticleBlend {
        PARTICLE_BLEND_ZERO          = 0,
        PARTICLE_BLEND_TYPICAL       = 1,
        PARTICLE_BLEND_ADD           = 2,
        PARTICLE_BLEND_SUBTRACT      = 3,
        PARTICLE_BLEND_MULTIPLY      = 4,
        PARTICLE_BLEND_PUNCH_THROUGH = 5,
    };

    enum ParticleFlag {
        PARTICLE_NONE              = 0x00000,
        PARTICLE_LOOP              = 0x00001,
        PARTICLE_EMITTER_LOCAL     = 0x00004,
        PARTICLE_SCALE_Y_BY_X      = 0x00010,
        PARTICLE_REBOUND_PLANE     = 0x00020,
        PARTICLE_ROTATE_BY_EMITTER = 0x00040,
        PARTICLE_SCALE             = 0x00080,
        PARTICLE_TEXTURE_MASK      = 0x00100,
        PARTICLE_DEPTH_TEST        = 0x00200,
        PARTICLE_ROTATE_LOCUS      = 0x00800,
        PARTICLE_LOCAL             = 0x10000,
        PARTICLE_EMISSION          = 0x20000,
    };

    enum ParticleManagerFlag {
        PARTICLE_MANAGER_PAUSE               = 0x01,
        PARTICLE_MANAGER_NOT_DISP            = 0x02,
        PARTICLE_MANAGER_RESET_SCENE_COUNTER = 0x04,
        PARTICLE_MANAGER_READ_FILES          = 0x08,
    };

    enum ParticleType {
        PARTICLE_QUAD  = 0,
        PARTICLE_LINE  = 1,
        PARTICLE_LOCUS = 2,
    };

    enum Pivot {
        PIVOT_TOP_LEFT      = 0,
        PIVOT_TOP_CENTER    = 1,
        PIVOT_TOP_RIGHT     = 2,
        PIVOT_MIDDLE_LEFT   = 3,
        PIVOT_MIDDLE_CENTER = 4,
        PIVOT_MIDDLE_RIGHT  = 5,
        PIVOT_BOTTOM_LEFT   = 6,
        PIVOT_BOTTOM_CENTER = 7,
        PIVOT_BOTTOM_RIGHT  = 8,
    };
    
    enum SceneFlag {
        SCENE_NONE     = 0x00,
        SCENE_FLAG_1   = 0x01,
        SCENE_NOT_DISP = 0x02,
        SCENE_FLAG_3   = 0x04,
    };

    struct Animation;
    struct Buffer;
    struct Camera;
    struct Curve;
    struct Effect;
    struct Emitter;
    struct EmitterInst;
    struct ItemBase;
    struct Node;
    struct Particle;
    struct ParticleInst;
    struct Random;
    struct RenderElement;
    struct RenderGroup;
    struct RenderScene;

    struct BatchShaderData;
    struct Scene;
    struct GltParticleManager;

#define GPM Glitter::GltParticleManager* glt_particle_manager
#define GPM_VAL (glt_particle_manager)

    struct Buffer {
        vec3 position;
        vec2 uv[2];
        vec4 color;
    };

    struct LocusHistory {
        struct Data {
            vec3 translation;
            vec4 color;
            float_t scale;
        };

        struct Sub {
            prj::vector<Data> vec;
            int32_t count;
        };

        void* __vftable;
        Sub sub;
    };

    struct Particle {
        static void CreateBuffer(size_t data);
        static void DeleteBuffer(size_t data);
    };

    struct RenderElement {
        bool alive;
        uint8_t uv_counter;
        int32_t random;
        float_t frame;
        float_t life_time;
        float_t rebound_time;
        float_t frame_step_uv;
        float_t speed;
        float_t deceleration;
        vec2 uv;
        vec4 color;
        vec3 base_translation;
        vec3 translation;
        vec3 translation_prev;
        vec3 direction;
        vec3 acceleration;
        vec2 scale_particle;
        vec3 scale;
        vec3 rotation;
        vec3 rotation_add;
        float_t rot_z_cos;
        float_t rot_z_sin;
        float_t scale_all;
        vec2 uv_scroll;
        mat4 mat;
        LocusHistory* locus_history;
    };

    struct EffectGroup {
        void* __vftable;
        std::vector<void*> effects; // Glitter::Effect
        int32_t load_count;
        texture** textures;
        void* scene; // Glitter::Scene
        float_t emission;
        bool not_loaded;
        bool scene_init;
        bool buffer_init;
    };

    typedef prj::vector<prj::pair<GLint, GLsizei>> DrawListData;

    struct RenderGroup {
        void* __vftable;
        ParticleFlag flags;
        ParticleType type;
        Direction draw_type;
        ParticleBlend blend_mode;
        ParticleBlend mask_blend_mode;
        int32_t dword1C;
        Pivot pivot;
        int32_t split_u;
        int32_t split_v;
        vec2 split_uv;
        float_t z_offset;
        int32_t count;
        int32_t ctrl;
        int32_t texture;
        int32_t mask_texture;
        prj::string name;
        float_t frame;
        mat4 mat;
        mat4 mat_rot;
        bool has_mask;
        int32_t dwordF0;
        int32_t dwordF4;
        RenderElement* elements;
        Buffer* buffer;
        int32_t max_count;
        int32_t dword10C;
        void* particle_inst; // Glitter::ParticleInst
        DispType disp_type;
        FogType fog_type;
        GLArrayBuffer vbo;
        float_t emission;
        bool use_own_buffer;
        GLuint vao;
        GLElementArrayBuffer ebo;
        size_t disp;
        mat4 mat_draw;
        DrawListData* draw_list;

        static mat4 RotateToEmitPosition(RenderGroup* rend_group,
            RenderElement* rend_elem, vec3* vec);
        static mat4 RotateToPrevPosition(RenderGroup* rend_group,
            RenderElement* rend_elem, vec3* vec);

        static void CreateBuffer(RenderGroup* rend_group);
        static void DeleteBuffer(RenderGroup* rend_group);
    };

    struct RenderScene {
        prj::vector<RenderGroup*> render_groups;
        int32_t disp_quad;
        int32_t disp_locus;
        int32_t disp_line;
        int32_t ctrl_quad;
        int32_t ctrl_locus;
        int32_t ctrl_line;

        void CalcDisp();
        void CalcDisp(RenderGroup* rend_group);
        void Disp(Glitter::DispType disp_type);
        void Disp(RenderGroup* rend_group);

        static void CalcDispLine(RenderGroup* rend_group);
        static void CalcDispLocus(RenderGroup* rend_group);
        static void CalcDispQuad(RenderGroup* rend_group);
        static void CalcDispQuadDirectionRotation(
            RenderGroup* rend_group, mat4* model_mat, mat4* dir_mat);
        static void CalcDispQuadNormal(
            RenderGroup* rend_group, mat4* model_mat, mat4* dir_mat);

        static void CalcDispLocusSetPivot(Pivot pivot,
            float_t w, float_t& v00, float_t& v01);
        static void CalcDispQuadSetPivot(Pivot pivot,
            float_t w, float_t h, float_t& v00, float_t& v01, float_t& v10, float_t& v11);
    };

    struct SceneEffect {
        void* ptr; // Glitter::EffectInst
        bool draw;
    };

    struct Scene {
        void* __vftable;
        prj::vector<SceneEffect*> effects;
        uint32_t load_counter;
        uint64_t hash;
        SceneFlag flags;
        float_t emission;
        EffectGroup* effect_group;
        RenderScene render_scene;

        void Disp(DispType disp_type);

        static void CalcDisp(Scene* sc);
    };

    struct GltParticleManager {
        void* __vftable;
        uint8_t app_task_data[0x60];
        prj::map<uint64_t, EffectGroup> effect_groups;
        void* field_78;
        uint64_t field_80;
        prj::vector<Scene*> scenes;
        prj::vector<void*> file_readers;
        void* sys_frame_rate;
        ParticleManagerFlag flags;
        int32_t scene_load_counter;
        int32_t available_buffers_init;
        int32_t available_buffers;
        float_t field_D0;
        float_t field_D4;
        float_t emission;
        float_t delta_frame;
        prj::map<uint64_t, uint32_t> resources;
        uint32_t texture_counter;
        int32_t field_F4;
        int32_t field_F8;
        int32_t random;
        int32_t counter;
        int32_t field_104;

        void DispScenes(DispType disp_type);

        static void Disp(GPM);
    };

    extern GPM;

    extern void axis_angle_from_vectors(vec3* axis, float_t* angle, const vec3* vec0, const vec3* vec1);

    extern void CreateBuffer(int32_t max_count, bool is_quad,
        GLuint& vao, GLArrayBuffer& vbo, GLElementArrayBuffer& ebo);
    extern void DeleteBuffer(GLuint& vao, GLArrayBuffer& vbo, GLElementArrayBuffer& ebo);

    extern void Patch();
}
