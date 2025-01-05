/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../../KKdLib/f2/struct.hpp"
#include "../../KKdLib/hash.hpp"
#include "../../KKdLib/mat.hpp"
#include "../../KKdLib/vec.hpp"
#include "../GL/array_buffer.hpp"
#include "../GL/element_array_buffer.hpp"
#include "../color.hpp"
#include "../file_handler.hpp"
#include "../frame_rate_control.hpp"
#include "../gl.hpp"
#include "../object.hpp"
#include "../task.hpp"
#include "../types.hpp"
#include "../texture.hpp"

#define SHARED_GLITTER_BUFFER (1)

namespace Glitter {
    enum CurveFlag {
        CURVE_RANDOM_RANGE        = 0x01,
        CURVE_KEY_RANDOM_RANGE    = 0x02,
        CURVE_RANDOM_RANGE_NEGATE = 0x04,
        CURVE_STEP                = 0x08,
        CURVE_NEGATE              = 0x10,
        CURVE_RANDOM_RANGE_MULT   = 0x20,
        CURVE_BAKED               = 0x40,
        CURVE_BAKED_FULL          = 0x80,
    };
    
    enum CurveType {
        CURVE_TRANSLATION_X          = 0,
        CURVE_TRANSLATION_Y          = 1,
        CURVE_TRANSLATION_Z          = 2,
        CURVE_ROTATION_X             = 3,
        CURVE_ROTATION_Y             = 4,
        CURVE_ROTATION_Z             = 5,
        CURVE_SCALE_X                = 6,
        CURVE_SCALE_Y                = 7,
        CURVE_SCALE_Z                = 8,
        CURVE_SCALE_ALL              = 9,
        CURVE_COLOR_R                = 10,
        CURVE_COLOR_G                = 11,
        CURVE_COLOR_B                = 12,
        CURVE_COLOR_A                = 13,
        CURVE_COLOR_RGB_SCALE        = 14,
        CURVE_COLOR_R_2ND            = 15,
        CURVE_COLOR_G_2ND            = 16,
        CURVE_COLOR_B_2ND            = 17,
        CURVE_COLOR_A_2ND            = 18,
        CURVE_COLOR_RGB_SCALE_2ND    = 19,
        CURVE_EMISSION_INTERVAL      = 20,
        CURVE_PARTICLES_PER_EMISSION = 21,
        CURVE_U_SCROLL               = 22,
        CURVE_V_SCROLL               = 23,
        CURVE_U_SCROLL_ALPHA         = 24,
        CURVE_V_SCROLL_ALPHA         = 25,
        CURVE_U_SCROLL_2ND           = 26,
        CURVE_V_SCROLL_2ND           = 27,
        CURVE_U_SCROLL_ALPHA_2ND     = 28,
        CURVE_V_SCROLL_ALPHA_2ND     = 29,
    };

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
        DISP_OPAQUE = 0,
        DISP_NORMAL = 1,
        DISP_ALPHA  = 2, // 3 in X
        DISP_TYPE_2 = 3, // 2 in X
        DISP_LOCAL  = 4,
    };

    enum EffectExtAnimFlag {
        EFFECT_EXT_ANIM_SET_ONCE            = 0x00001,
        EFFECT_EXT_ANIM_TRANS_ONLY          = 0x00002,
        EFFECT_EXT_ANIM_NO_TRANS_X          = 0x00004,
        EFFECT_EXT_ANIM_NO_TRANS_Y          = 0x00008,
        EFFECT_EXT_ANIM_NO_TRANS_Z          = 0x00010,
        EFFECT_EXT_ANIM_NO_DRAW_IF_NO_DATA  = 0x00020,
        EFFECT_EXT_ANIM_GET_THEN_UPDATE     = 0x00040,
        EFFECT_EXT_ANIM_CHARA               = 0x10000,
    };

    enum EffectExtAnimCharaNode {
        EFFECT_EXT_ANIM_CHARA_NONE            = -1,
        EFFECT_EXT_ANIM_CHARA_HEAD            = 0,
        EFFECT_EXT_ANIM_CHARA_MOUTH           = 1,
        EFFECT_EXT_ANIM_CHARA_BELLY           = 2,
        EFFECT_EXT_ANIM_CHARA_CHEST           = 3,
        EFFECT_EXT_ANIM_CHARA_LEFT_SHOULDER   = 4,
        EFFECT_EXT_ANIM_CHARA_LEFT_ELBOW      = 5,
        EFFECT_EXT_ANIM_CHARA_LEFT_ELBOW_DUP  = 6,
        EFFECT_EXT_ANIM_CHARA_LEFT_HAND       = 7,
        EFFECT_EXT_ANIM_CHARA_RIGHT_SHOULDER  = 8,
        EFFECT_EXT_ANIM_CHARA_RIGHT_ELBOW     = 9,
        EFFECT_EXT_ANIM_CHARA_RIGHT_ELBOW_DUP = 10,
        EFFECT_EXT_ANIM_CHARA_RIGHT_HAND      = 11,
        EFFECT_EXT_ANIM_CHARA_LEFT_THIGH      = 12,
        EFFECT_EXT_ANIM_CHARA_LEFT_KNEE       = 13,
        EFFECT_EXT_ANIM_CHARA_LEFT_TOE        = 14,
        EFFECT_EXT_ANIM_CHARA_RIGHT_THIGH     = 15,
        EFFECT_EXT_ANIM_CHARA_RIGHT_KNEE      = 16,
        EFFECT_EXT_ANIM_CHARA_RIGHT_TOE       = 17,
        EFFECT_EXT_ANIM_CHARA_MAX             = 18,
    };

    enum EffectFlag {
        EFFECT_NONE       = 0x00,
        EFFECT_LOOP       = 0x01,
        EFFECT_LOCAL      = 0x02,
        EFFECT_ALPHA      = 0x04,
        EFFECT_FOG        = 0x08,
        EFFECT_FOG_HEIGHT = 0x10,
        EFFECT_EMISSION   = 0x20,
        EFFECT_USE_SEED   = 0x40,
    };

    enum EffectFileFlag {
        EFFECT_FILE_ALPHA      = 0x01,
        EFFECT_FILE_FOG        = 0x02,
        EFFECT_FILE_FOG_HEIGHT = 0x04,
        EFFECT_FILE_EMISSION   = 0x08,
        EFFECT_FILE_USE_SEED   = 0x10,
    };

    enum EffectInstFlag {
        EFFECT_INST_NONE                     = 0x0000000,
        EFFECT_INST_FREE                     = 0x0000001,
        EFFECT_INST_RESET_INIT               = 0x0000002,
        EFFECT_INST_EXT_ANIM                 = 0x0000004,
        EFFECT_INST_EXT_ANIM_TRANS           = 0x0000008,
        EFFECT_INST_EXT_ANIM_NON_INIT        = 0x0000010,
        EFFECT_INST_EXT_ANIM_CHARA           = 0x0000020,
        EFFECT_INST_EXT_ANIM_AUTH            = 0x0000040,
        EFFECT_INST_EXT_ANIM_SET_ONCE        = 0x0000080,
        EFFECT_INST_EXT_COLOR_SET            = 0x0000100,
        EFFECT_INST_EXT_COLOR                = 0x0000200,
        EFFECT_INST_EXT_SCALE                = 0x0000400,
        EFFECT_INST_EXT_ANIM_END             = 0x0000800,
        EFFECT_INST_NO_EXT_ANIM_TRANS_X      = 0x0001000,
        EFFECT_INST_NO_EXT_ANIM_TRANS_Y      = 0x0002000,
        EFFECT_INST_NO_EXT_ANIM_TRANS_Z      = 0x0004000,
        EFFECT_INST_EXT_ANIM_TRANS_ONLY      = 0x0008000,
        EFFECT_INST_FLAG_17                  = 0x0010000,
        EFFECT_INST_EXT_ANIM_GET_THEN_UPDATE = 0x0020000,
        EFFECT_INST_EXT_ANIM_MAT             = 0x0040000,
        EFFECT_INST_DISP                     = 0x0080000,
        EFFECT_INST_CAMERA                   = 0x0100000,
        EFFECT_INST_MESH                     = 0x0200000,
        EFFECT_INST_FLAG_23                  = 0x0400000,
        EFFECT_INST_JUST_INIT                = 0x0800000,
        EFFECT_INST_NOT_ENDED                = 0x1000000,
    };

    enum EmitterDirection {
        EMITTER_DIRECTION_BILLBOARD        = 0,
        EMITTER_DIRECTION_BILLBOARD_Y_AXIS = 1,
        EMITTER_DIRECTION_Y_AXIS           = 2,
        EMITTER_DIRECTION_X_AXIS           = 3,
        EMITTER_DIRECTION_Z_AXIS           = 4,
        EMITTER_DIRECTION_EFFECT_ROTATION  = 5,
    };

    enum EmitterEmission {
        EMITTER_EMISSION_ON_TIMER = 0,
        EMITTER_EMISSION_ON_START = 1,
        EMITTER_EMISSION_ON_END   = 2,
        EMITTER_EMISSION_EMITTED  = 3,
    };

    enum EmitterEmissionDirection {
        EMITTER_EMISSION_DIRECTION_NONE    = 0,
        EMITTER_EMISSION_DIRECTION_OUTWARD = 1,
        EMITTER_EMISSION_DIRECTION_INWARD  = 2,
    };

    enum EmitterFlag {
        EMITTER_NONE        = 0x00,
        EMITTER_LOOP        = 0x01,
        EMITTER_KILL_ON_END = 0x02,
        EMITTER_USE_SEED    = 0x04,
    };

    enum EmitterInstFlag {
        EMITTER_INST_NONE         = 0x00,
        EMITTER_INST_ENDED        = 0x01,
        EMITTER_INST_HAS_DISTANCE = 0x02,
    };

    enum EmitterTimerType {
        EMITTER_TIMER_BY_TIME     = 0,
        EMITTER_TIMER_BY_DISTANCE = 1,
    };

    enum EmitterType {
        EMITTER_BOX      = 0,
        EMITTER_CYLINDER = 1,
        EMITTER_SPHERE   = 2,
        EMITTER_MESH     = 3,
        EMITTER_POLYGON  = 4,
    };

    enum FogType {
        FOG_NONE   = 0,
        FOG_DEPTH  = 1,
        FOG_HEIGHT = 2,
    };

    enum KeyType {
        KEY_CONSTANT = 0,
        KEY_LINEAR   = 1,
        KEY_HERMITE  = 2,
    };

    enum ParticleBlend {
        PARTICLE_BLEND_ZERO          = 0,
        PARTICLE_BLEND_TYPICAL       = 1,
        PARTICLE_BLEND_ADD           = 2,
        PARTICLE_BLEND_SUBTRACT      = 3,
        PARTICLE_BLEND_MULTIPLY      = 4,
        PARTICLE_BLEND_PUNCH_THROUGH = 5,
    };

    enum ParticleDrawFlag {
        PARTICLE_DRAW_NONE              = 0x0,
        PARTICLE_DRAW_NO_BILLBOARD_CULL = 0x1,
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
    
    enum ParticleInstFlag {
        PARTICLE_INST_NONE     = 0x00,
        PARTICLE_INST_ENDED    = 0x01,
        PARTICLE_INST_NO_CHILD = 0x02,
    };

    enum ParticleManagerFlag {
        PARTICLE_MANAGER_PAUSE               = 0x01,
        PARTICLE_MANAGER_NOT_DISP            = 0x02,
        PARTICLE_MANAGER_RESET_SCENE_COUNTER = 0x04,
        PARTICLE_MANAGER_READ_FILES          = 0x08,
        PARTICLE_MANAGER_LOCAL               = 0x20,
    };
    
    enum ParticleSubFlag {
        PARTICLE_SUB_NONE       = 0x00000000,
        PARTICLE_SUB_UV_2ND_ADD = 0x00400000,
        PARTICLE_SUB_USE_CURVE  = 0x40000000,
    };

    enum ParticleType {
        PARTICLE_QUAD  = 0,
        PARTICLE_LINE  = 1, // 3 in X
        PARTICLE_LOCUS = 2,
        PARTICLE_MESH  = 3, // 1 in X
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
        SCENE_ENDED    = 0x08,
        SCENE_PAUSE    = 0x10,
    };

    enum UVIndexType {
        UV_INDEX_FIXED                  = 0,
        UV_INDEX_INITIAL_RANDOM_FIXED   = 1,
        UV_INDEX_RANDOM                 = 2,
        UV_INDEX_FORWARD                = 3,
        UV_INDEX_REVERSE                = 4,
        UV_INDEX_INITIAL_RANDOM_FORWARD = 5,
        UV_INDEX_INITIAL_RANDOM_REVERSE = 6,
        UV_INDEX_USER                   = 7,
    };

    struct Animation;
    struct AnimationX;
    struct Buffer;
    struct Counter;
    struct Curve;
    struct CurveX;
    struct Effect;
    class EffectX;
    struct EffectInst;
    struct EffectInstX;
    struct Emitter;
    class EmitterX;
    struct EmitterInst;
    struct EmitterInstX;
    class ItemBase;
    class ItemBaseX;
    struct Node;
    class NodeX;
    class Particle;
    class ParticleX;
    struct ParticleInst;
    struct ParticleInstX;
    struct RandomX;
    struct RenderElement;
    struct RenderElementX;
    struct RenderGroup;
    struct RenderGroupX;
    struct RenderScene;
    struct RenderSceneX;

    struct Scene;
    struct SceneX;
    class GltParticleManager;
    class GltParticleManagerX;

#define GPM Glitter::GltParticleManager* glt_particle_manager
#define GPM_VAL (glt_particle_manager)
#define GPMX Glitter::GltParticleManagerX* glt_particle_manager_x
#define GPMX_VAL (glt_particle_manager_x)

    extern const float_t min_emission;

    struct Animation {
        prj::vector<Curve*> curves;
    };

    struct AnimationX {
        prj::vector<CurveX*> curves;

        AnimationX();
        ~AnimationX();

        AnimationX& operator=(const AnimationX& anim);
    };

    struct Buffer {
        vec3 position;
        vec2 uv[2];
        vec4 color;
    };

    struct CurveX {
        struct Key {
            KeyType type;
            int32_t frame;
            union {
                float_t value;
                float_t max_value;
            };
            float_t tangent1;
            float_t tangent2;
            union {
                float_t random_range;
                float_t min_value;
            };

            Key();
            Key(KeyType type, int32_t frame, float_t value, float_t random_range);
            Key(KeyType type, int32_t frame, float_t value,
                float_t tangent1, float_t tangent2, float_t random_range);
        };

        CurveType type;
        bool repeat;
        int32_t start_time;
        int32_t end_time;
        CurveFlag flags;
        float_t random_range;
        prj::vector<Key> keys;
        uint32_t version;
        uint32_t keys_version;

        CurveX();
        virtual ~CurveX();

        bool GetValue(float_t frame,
            float_t* value, int32_t random_value, RandomX* random);
        float_t Interpolate(float_t frame, const CurveX::Key& curr,
            const CurveX::Key& next, KeyType key_type, RandomX* random);
        float_t InterpolateHermite(const CurveX::Key& curr,
            const CurveX::Key& next, float_t frame, RandomX* random);
        float_t InterpolateLinear(const CurveX::Key& curr,
            const CurveX::Key& next, float_t frame, RandomX* random);
        float_t Randomize(float_t value, RandomX* random);
        float_t RandomizeKey(const CurveX::Key& key, RandomX* random);

        static void GetKeyIndices(const prj::vector<CurveX::Key>& keys,
            float_t frame, size_t& curr, size_t& next);

        template <typename T>
        inline static T InterpolateHermite(const T p, const T dv,
            const T t1, const T t2, const T f1, const T f2, const T f) {
            const T df = (f2 - f1);
            const T t = (f - f1) / df;
            const T t_2 = t * t;
            const T t_3 = t_2 * t;
            return p
                + (t_3 - (T)2 * t_2 + t) * (t1 * df)
                + (t_3 - t_2) * (t2 * df)
                + ((T)3 * t_2 - (T)2 * t_3) * dv;
        };

        template <typename T>
        inline static T InterpolateLinear(const T p1, const T p2,
            const T f1, const T f2, const T f) {
            const T t = (f - f1) / (f2 - f1);
            return ((T)1 - t) * p1 + t * p2;
        };
    };

    class ItemBase {
    public:
        void* __vftable;
        prj::string name;
        Animation animation;
    };
    
    class ItemBaseX {
    public:
        AnimationX animation;

        ItemBaseX();
        virtual ~ItemBaseX();

        ItemBaseX& operator=(const ItemBaseX& item_base);
    };

    struct MeshX {
        uint32_t object_set;
        uint32_t object;
        bool load;
        bool ready;

        inline MeshX() : load(), ready() {
            object_set = -1;
            object = -1;
        }

        inline MeshX(uint32_t object_set) : object_set(object_set), load(), ready() {
            object = -1;
        }
    };

    class NodeX : public ItemBaseX {
    public:
        vec3 translation;
        vec3 rotation;
        vec3 scale;
        float_t scale_all;

        NodeX();
        virtual ~NodeX() override;

        NodeX& operator=(const NodeX& node);
    };
    
    class EffectX : public NodeX {
    public:
        struct ExtAnim {
            EffectExtAnimFlag flags;
            union {
                struct {
                    uint32_t a3da_uid;
                    object_info object;
                    int32_t instance_id;
                    char mesh_name[128];
                };
                struct {
                    int32_t chara_index;
                    int32_t bone_index;
                };
            };
        };

        struct Data {
            uint32_t name_hash;
            int32_t appear_time;
            int32_t life_time;
            int32_t start_time;
            color4u8 color;
            EffectX::ExtAnim* ext_anim;
            EffectFlag flags;
            float_t emission;
            int32_t seed;
            float_t ext_anim_end_time;

            Data();
        };

        EffectX::Data data;
        prj::vector<EmitterX*> emitters;
        uint32_t version;

        EffectX();
        virtual ~EffectX() override;

        EffectX& operator=(const EffectX& eff);
    };
    
    struct EffectGroup {
        void* __vftable;
        prj::vector<Effect*> effects;
        int32_t load_count;
        texture** textures;
        Scene* scene;
        float_t emission;
        bool not_loaded;
        bool scene_init;
        bool buffer_init;
    };

    struct EffectGroupX {
        prj::vector<EffectX*> effects;
        int32_t load_count;
        texture** resources;
        SceneX* scene;
        float_t emission;
        bool not_loaded;
        bool scene_init;
        bool buffer_init;
        prj::vector<MeshX> meshes;
#if SHARED_GLITTER_BUFFER
        Buffer* buffer;
        size_t max_count;
        GL::ArrayBuffer vbo;
        GL::ElementArrayBuffer ebo;
#endif

        EffectGroupX();
        virtual ~EffectGroupX();
    };

    struct RandomX {
        uint32_t value;
        uint8_t step;

        RandomX();

        float_t GetFloat(float_t value);
        float_t GetFloat(float_t min, float_t max);
        int32_t GetInt(int32_t value);
        int32_t GetInt(int32_t min, int32_t max);
        int32_t GetValue();
        vec3 GetVec3(const vec3& value);
        void Reset();
        void SetStep(uint8_t step);
        void SetValue(int32_t value);
        void StepValue();

        static int32_t GetMax();
    };

    struct Counter {
        int32_t value;

        void Increment();
        int32_t GetValue();
        void Reset();
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

    struct RenderSceneX {
        size_t disp_quad;
        size_t disp_locus;
        size_t disp_line;
        size_t disp_mesh;
        size_t ctrl_quad;
        size_t ctrl_locus;
        size_t ctrl_line;
        size_t ctrl_mesh;
        prj::vector<RenderGroupX*> groups;

        RenderSceneX();
        ~RenderSceneX();

        void Append(RenderGroupX* rend_group);
        void CalcDisp();
        void CalcDisp(RenderGroupX* rend_group);
        void CalcDispLine(RenderGroupX* rend_group);
        void CalcDispLocus(RenderGroupX* rend_group);
        void CalcDispQuad(RenderGroupX* rend_group);
        void CalcDispQuadDirectionRotation(RenderGroupX* rend_group, mat4* model_mat);
        void CalcDispQuadNormal(RenderGroupX* rend_group, mat4* model_mat, mat4* dir_mat);
        bool CanDisp(DispType disp_type, bool a3);
        void CheckUseCamera();
        void Ctrl(float_t delta_frame, bool copy_mats);
        void Disp(DispType disp_type);
        void Disp(RenderGroupX* rend_group);
        void DispMesh();
        void DispMesh(RenderGroupX* rend_group);
        size_t GetCtrlCount(ParticleType type);
        size_t GetDispCount(ParticleType type);

        static void CalcDispLocusSetPivot(Pivot pivot,
            float_t w, float_t& v00, float_t& v01);
        static void CalcDispQuadSetPivot(Pivot pivot,
            float_t w, float_t h, float_t& v00, float_t& v01, float_t& v10, float_t& v11);
    };
    
    struct EffectInstX {
        bool init;
        EffectX* effect;
        EffectX::Data data;
        float_t frame0;
        float_t frame1;
        vec4 color;
        vec3 translation;
        vec3 rotation;
        vec3 scale;
        float_t scale_all;
        mat4 mat;
        EffectInstFlag flags;
        size_t id;
        uint32_t random;
        float_t req_frame;
        vec4 ext_color;
        float_t ext_scale;
        vec3 ext_anim_scale;

        struct ExtAnim {
            union {
                struct {
                    int32_t object_index;
                    int32_t mesh_index;
                    int32_t a3da_id;
                    bool object_is_hrc;
                    int32_t a3da_uid;
                    object_info object;
                    int32_t instance_id;
                    const char* mesh_name;
                };
                struct {
                    int32_t chara_index;
                    int32_t bone_index;
                };
            };
            mat4 mat;
            vec3 translation;

            ExtAnim();
            ~ExtAnim();

            void Reset();
        };

        prj::vector<EmitterInstX*> emitters;
        mat4 mat_rot;
        mat4 mat_rot_eff_rot;
        RandomX random_shared;
        ExtAnim* ext_anim;
        RenderSceneX render_scene;

        EffectInstX(EffectX* eff, size_t id, SceneX* sc, bool appear_now, bool init, uint8_t load_flags = 0);
        virtual ~EffectInstX();

        void CalcDisp();
        void CheckUpdate();
        void CheckUseCamera();
        void Copy(EffectInstX* dst, float_t emission);
        void Ctrl(float_t delta_frame);
        void CtrlFlags(float_t delta_frame);
        void CtrlInit(float_t delta_frame);
        void CtrlMat();
        void Disp(DispType disp_type);
        void DispMesh();
        void Emit(float_t delta_frame, float_t emission);
        void EmitInit(float_t delta_frame, float_t emission);
        void Free(float_t emission, bool free);
        size_t GetCtrlCount(ParticleType type);
        size_t GetDispCount(ParticleType type);
        DispType GetDispType();
        void GetExtAnim();
        bool GetExtAnimMat(mat4* mat);
        bool GetExtAnimScale(vec3* ext_anim_scale, float_t* ext_scale);
        void GetExtColor(float_t& r, float_t& g, float_t& b, float_t& a);
        FogType GetFog();
        bool GetUseCamera();
        void GetValue();
        bool HasEnded(bool a2);
        void InitExtAnim();
        void RenderSceneCtrl(float_t delta_frame);
        void Reset(SceneX* sc);
        bool ResetCheckInit(SceneX* sc, float_t* init_delta_frame = 0);
        bool ResetInit(SceneX* sc, float_t* init_delta_frame = 0);
        void SetExtAnim(const mat4* a2, const mat4* a3, const vec3* trans, bool set_flags);
        void SetExtAnimMat(const mat4* mat);
        void SetExtColor(float_t r, float_t g, float_t b, float_t a, bool set);
        void SetExtScale(float_t scale);
    };

    class EmitterX : public NodeX {
    public:
        struct Box {
            vec3 size;

            Box();
        };

        struct Cylinder {
            float_t radius;
            float_t height;
            float_t start_angle;
            float_t end_angle;
            bool on_edge;
            EmitterEmissionDirection direction;

            Cylinder();
        };

        struct Polygon {
            float_t size;
            int32_t count;
            EmitterEmissionDirection direction;

            Polygon();
        };

        struct Sphere {
            float_t radius;
            float_t latitude;
            float_t longitude;
            bool on_edge;
            EmitterEmissionDirection direction;

            Sphere();
        };

        struct Data {
            int32_t start_time;
            int32_t life_time;
            int32_t loop_start_time;
            int32_t loop_end_time;
            EmitterFlag flags;
            vec3 rotation_add;
            vec3 rotation_add_random;
            EmitterTimerType timer;
            float_t emission_interval;
            float_t particles_per_emission;
            Direction direction;
            EmitterType type;
            EmitterX::Box box;
            EmitterX::Cylinder cylinder;
            EmitterX::Sphere sphere;
            EmitterX::Polygon polygon;
            int32_t seed;

            Data();
        };

        EmitterX::Data data;
        prj::vector<ParticleX*> particles;
        bool buffer_init;
        uint32_t version;

        EmitterX();
        virtual ~EmitterX() override;

        EmitterX& operator=(const EmitterX& emit);
    };

    struct EmitterInstX {
        EmitterX* emitter;
        vec3 translation;
        vec3 rotation;
        vec3 scale;
        mat4 mat;
        mat4 mat_rot;
        float_t scale_all;
        float_t emission_timer;
        EmitterX::Data data;
        float_t emission_interval;
        float_t particles_per_emission;
        RandomX* random_ptr;
        bool loop;
        EmitterEmission emission;
        float_t frame;
        EmitterInstFlag flags;
        uint32_t random;
        prj::vector<ParticleInstX*> particles;
        uint32_t counter;
        uint8_t step;

        EmitterInstX(EmitterX* emit, EffectInstX* eff_inst, float_t emission);
        virtual ~EmitterInstX();

        bool CheckUseCamera();
        void Copy(EmitterInstX* dst, float_t emission);
        void Ctrl(EffectInstX* eff_inst, float_t delta_frame);
        void CtrlInit(EffectInstX* eff_inst, float_t delta_frame);
        void CtrlMat(EffectInstX* eff_inst);
        void Emit(float_t delta_frame, float_t emission);
        void EmitInit(EffectInstX* eff_inst, float_t delta_frame, float_t emission);
        void EmitParticle(float_t emission, float_t frame);
        void Free(float_t emission, bool free);
        void GetValue();
        bool HasEnded(bool a2);
        void InitMesh(int32_t index, const vec3& scale,
            vec3& position, vec3& direction, RandomX* random);
        uint8_t RandomGetStep();
        void RandomStepValue();
        void RenderGroupCtrl(float_t delta_frame);
        void Reset();
    };

    struct FileReaderX {
        p_file_handler* file_handler;
        farc* farc;
        uint32_t* resource_hashes;
        EffectGroupX* effect_group;
        uint32_t hash;
        int32_t load_count;
        float_t emission;
        prj::string path;
        prj::string file;
        int32_t state;
        bool init_scene;

        FileReaderX();
        FileReaderX(const char* path, const char* file, float_t emission);
        FileReaderX(const wchar_t* path, const wchar_t* file, float_t emission);
        virtual ~FileReaderX();

        bool CheckInit();
        int32_t GetEffectExtAnimBoneIndex(EffectExtAnimCharaNode node);
        bool LoadFarc(const char* path, const char* file, uint32_t hash);
        void ParseAnimation(f2_struct* st, AnimationX* anim);
        void ParseCurve(f2_struct* st, AnimationX* anim);
        bool ParseDivaEffect(f2_struct* st);
        bool ParseDivaResource(f2_struct* st, EffectGroupX* eff_group);
        bool ParseEffect(f2_struct* st, EffectGroupX* eff_group);
        bool ParseEffectGroup(f2_struct* st, prj::vector<EffectX*>* vec, EffectGroupX* eff_group);
        bool ParseEmitter(f2_struct* st, EffectX* eff, EffectGroupX* eff_group);
        bool ParseParticle(f2_struct* st, EmitterX* emit, EffectX* eff, EffectGroupX* eff_group);
        bool Read();
        bool ReadFarc();
        void UnpackCurve(void* data, AnimationX* anim, CurveX* c,
            uint32_t count, uint32_t keys_version);
        bool UnpackDivaResource(f2_struct* st, EffectGroupX* eff_group);
        bool UnpackDivaResourceHashes(f2_struct* st, EffectGroupX* eff_group);
        bool UnpackEffect(void* data, EffectX* eff, int32_t efct_version);
        bool UnpackEmitter(void* data, EmitterX* emit, uint32_t emit_version);
        bool UnpackParticle(void* data, ParticleX* ptcl,
            uint32_t ptcl_version, EffectX* eff, EffectGroupX* eff_group);
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

    struct LocusHistoryX {
        struct Data {
            vec4 color;
            vec3 translation;
            float_t scale;

            Data();
        };

        prj::vector<Data> data;

        LocusHistoryX(size_t size);
        virtual ~LocusHistoryX();

        void Append(RenderElementX* rend_elem, ParticleInstX* ptcl_inst);
    };
    
    typedef prj::vector<prj::pair<GLint, GLsizei>> DrawListData;
    
    class Particle : ItemBase {
    public:
        struct Data {
            ParticleFlag flags;
            float_t life_time;
            ParticleType type;
            Pivot pivot;
            Direction draw_type;
            float_t z_offset;
            vec3 rotation;
            vec3 rotation_random;
            vec3 rotation_add;
            vec3 rotation_add_random;
            vec3 position_offset;
            vec3 position_offset_random;
            vec3 base_direction;
            vec3 direction_random;
            float_t speed;
            float_t speed_random;
            float_t deceleration;
            float_t deceleration_random;
            vec3 gravitational_acceleration;
            vec3 external_acceleration;
            vec3 external_acceleration_random;
            float_t reflection_coeff;
            float_t reflection_coeff_random;
            float_t dwordB4;
            color4u8_bgra color_int;
            vec4 color;
            UVIndexType uv_index_type;
            int32_t uv_index;
            float_t frame_step_uv;
            int32_t uv_index_start;
            int32_t uv_index_end;
            int32_t uv_index_count;
            vec2 uv_scroll_add;
            float_t uv_scroll_add_scale;
            vec2 split_uv;
            uint8_t split_u;
            uint8_t split_v;
            ParticleBlend blend_mode;
            ParticleBlend mask_blend_mode;
            int32_t dword104;
            ParticleSubFlag sub_flags;
            int32_t count;
            int32_t locus_history_size;
            int32_t locus_history_size_random;
            int32_t dword118;
            float_t emission;
            uint64_t tex_hash;
            uint64_t mask_tex_hash;
            int32_t texture;
            int32_t mask_texture;
            char dword138[32];
        };

        Glitter::Particle::Data data;
        Glitter::Buffer* buffer;
        GL::ArrayBuffer vbo;
        int32_t max_count;
        bool buffer_used;
        GLuint vao;
        GL::ElementArrayBuffer ebo;
        DrawListData* draw_list;

        static void CreateBuffer(Particle* ptcl);
        static void DeleteBuffer(Particle* ptcl);
        static void InitData(Particle* ptcl);
    };

    class ParticleX : public ItemBaseX {
    public:
        struct Mesh {
            uint32_t object;
            uint32_t object_set;
            //char mesh_name[0x40]; // Unused
            //uint32_t sub_mes;     // Unused
        };

        struct Data {
            ParticleFlag flags;
            int32_t life_time;
            int32_t life_time_random;
            int32_t fade_in;
            int32_t fade_in_random;
            int32_t fade_out;
            int32_t fade_out_random;
            ParticleType type;
            Pivot pivot;
            Direction draw_type;
            float_t z_offset;
            vec3 rotation;
            vec3 rotation_random;
            vec3 rotation_add;
            vec3 rotation_add_random;
            vec3 scale;
            vec3 scale_random;
            vec3 direction;
            vec3 direction_random;
            float_t speed;
            float_t speed_random;
            float_t deceleration;
            float_t deceleration_random;
            vec3 gravity;
            vec3 acceleration;
            vec3 acceleration_random;
            float_t reflection_coeff;
            float_t reflection_coeff_random;
            float_t rebound_plane_y;
            vec4 color;
            UVIndexType uv_index_type;
            int32_t uv_index;
            int32_t frame_step_uv;
            int32_t uv_index_start;
            int32_t uv_index_end;
            int32_t uv_index_count;
            vec2 uv_scroll_add;
            float_t uv_scroll_add_scale;
            vec2 uv_scroll_2nd_add;
            float_t uv_scroll_2nd_add_scale;
            vec2 split_uv;
            uint8_t split_u;
            uint8_t split_v;
            ParticleBlend blend_mode;
            ParticleBlend mask_blend_mode;
            ParticleSubFlag sub_flags;
            int32_t count;
            int32_t locus_history_size;
            int32_t locus_history_size_random;
            ParticleDrawFlag draw_flags;
            float_t emission;
            uint32_t tex_hash;
            uint32_t mask_tex_hash;
            GLuint texture;
            GLuint mask_texture;
            int32_t unk0;
            float_t unk1;
            int32_t unk2;
            int32_t unk3;
            int32_t unk4;
            ParticleX::Mesh mesh;

            Data();
        };

        ParticleX::Data data;
        Buffer* buffer;
        GLuint vao;
#if !SHARED_GLITTER_BUFFER
        GL::ArrayBuffer vbo;
        GL::ElementArrayBuffer ebo;
#endif
        int32_t max_count;
        bool buffer_used;
        int32_t version;

        ParticleX();
        virtual ~ParticleX() override;
    };

    struct ParticleInstX {
        ParticleX* particle;
        struct Data {
            ParticleX::Data data;
            ParticleInstFlag flags;
            RenderGroupX* render_group;
            RandomX* random_ptr;
            EffectInstX* effect;
            EmitterInstX* emitter;
            ParticleInstX* parent;
            ParticleX* particle;
            prj::vector<ParticleInstX*> children;

            Data();
            ~Data();
        } data;

        ParticleInstX(ParticleX* ptcl, EffectInstX* eff_inst,
            EmitterInstX* emit_inst, RandomX* random, float_t emission);
        ParticleInstX(ParticleInstX* parent, float_t emission);
        virtual ~ParticleInstX();

        void AccelerateParticle(RenderElementX* rend_elem,
            float_t delta_frame, RandomX* random);
        bool CheckUseCamera();
        void Copy(ParticleInstX* dst, float_t emission);
        void Emit(int32_t dup_count, int32_t count, float_t emission, float_t frame);
        void EmitParticle(RenderElementX* rend_elem, EmitterInstX* emit_inst,
            ParticleX::Data* ptcl_data, int32_t index, uint8_t step, RandomX* random);
        void GetColor(RenderElementX* rend_elem, float_t color_scale);
        bool GetExtAnimScale(vec3* ext_anim_scale, float_t* ext_scale);
        void GetExtColor(float_t& r, float_t& g, float_t& b, float_t& a);
        bool GetUseCamera();
        bool GetValue(RenderElementX* rend_elem, float_t frame, RandomX* random, float_t* color_scale);
        void Free(bool free);
        bool HasEnded(bool a2);
        void RenderGroupCtrl(float_t delta_frame);
        void Reset();
        void StepUVParticle(RenderElementX* rend_elem, float_t delta_frame, RandomX* random);
    };

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
        ParticleInst* particle_inst;
        DispType disp_type;
        FogType fog_type;
        GL::ArrayBuffer vbo;
        float_t emission;
        bool use_own_buffer;
        GLuint vao;
        GL::ElementArrayBuffer ebo;
        size_t disp;
        mat4 mat_draw;
        DrawListData* draw_list;

        static mat4 RotateToEmitPosition(RenderGroup* rend_group,
            RenderElement* rend_elem, vec3* vec);
        static mat4 RotateToPrevPosition(RenderGroup* rend_group,
            RenderElement* rend_elem, vec3* vec);

        static void CreateBuffer(RenderGroup* rend_group, Particle* ptcl);
        static void DeleteBuffer(RenderGroup* rend_group, Particle* ptcl);
        static void InitData(RenderGroup* rend_group);
    };

    struct RenderGroupX {
        ParticleFlag flags;
        ParticleType type;
        Direction draw_type;
        ParticleBlend blend_mode;
        ParticleBlend mask_blend_mode;
        Pivot pivot;
        int32_t split_u;
        int32_t split_v;
        vec2 split_uv;
        float_t z_offset;
        size_t count;
        size_t ctrl;
        size_t disp;
        GLuint texture;
        GLuint mask_texture;
        float_t frame;
        mat4 mat;
        mat4 mat_rot;
        mat4 mat_draw;
        RenderElementX* elements;
        Buffer* buffer;
        size_t max_count;
        RandomX* random_ptr;
        DispType disp_type;
        FogType fog_type;
        GLuint vao;
#if !SHARED_GLITTER_BUFFER
        GL::ArrayBuffer vbo;
        GL::ElementArrayBuffer ebo;
#endif
        float_t emission;
#if !SHARED_GLITTER_BUFFER
        bool use_own_buffer;
#endif
        DrawListData draw_list;

        ParticleInstX* particle;
        object_info object;
        bool use_culling;
        bool use_camera;

        RenderGroupX(ParticleInstX* ptcl_inst);
        virtual ~RenderGroupX();

        RenderElementX* AddElement(RenderElementX* rend_elem);
        bool CannotDisp();
        void CheckUseCamera();
        void Copy(RenderGroupX* dst);
        void Ctrl(float_t delta_frame, bool copy_mats);
        void CtrlParticle(RenderElementX* rend_elem, float_t delta_frame);
        void DeleteBuffers(bool a2);
        void Emit(ParticleX::Data* ptcl_data,
            EmitterInstX* emit_inst, int32_t dup_count, int32_t count, float_t frame);
        void Free();
        void FreeData();
        bool GetEmitterScale(vec3& emitter_scale);
        bool GetExtAnimScale(vec3* ext_anim_scale, float_t* ext_scale);
        bool HasEnded();

        static mat4 RotateMeshToEmitPosition(RenderGroupX* rend_group,
            RenderElementX* rend_elem, vec3* vec, vec3* trans);
        static mat4 RotateMeshToPrevPosition(RenderGroupX* rend_group,
            RenderElementX* rend_elem, vec3* vec, vec3* trans);
        static mat4 RotateToEmitPosition(RenderGroupX* rend_group,
            RenderElementX* rend_elem, vec3* vec);
        static mat4 RotateToPrevPosition(RenderGroupX* rend_group,
            RenderElementX* rend_elem, vec3* vec);
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

    struct RenderElementX {
        bool alive;
        uint8_t uv_index;
        bool disp;
        float_t frame;
        float_t life_time;
        float_t rebound_time;
        float_t frame_step_uv;
        float_t base_speed;
        float_t speed;
        float_t deceleration;
        vec2 uv;
        vec4 color;
        vec3 base_translation;
        vec3 base_direction;
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
        vec2 uv_scroll_2nd;
        float_t fade_out_frames;
        float_t fade_in_frames;
        mat4 mat;
        mat4 mat_draw;
        LocusHistoryX* locus_history;
        uint32_t random;
        uint8_t step;

        RenderElementX();

        void InitLocusHistory(ParticleInstX* ptcl_inst, RandomX* random);
        void InitMesh(EmitterInstX* emit_inst,
            ParticleX::Data* ptcl_data, int32_t index, RandomX* random);
    };

    struct SceneEffect {
        EffectInst* ptr;
        bool disp;
    };

    struct SceneCounter {
        uint32_t index : 8;
        uint32_t counter : 24;

        SceneCounter(uint32_t counter = 0);
        SceneCounter(uint32_t index, uint32_t counter);

        operator uint32_t() const { return (counter << 8) || index; }
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

        void CalcDisp();
        void Disp(DispType disp_type);
    };

    struct SceneEffectX {
        EffectInstX* ptr;
        bool disp;
    };

    struct SceneX {
        prj::vector<SceneEffectX> effects;
        SceneCounter counter;
        uint32_t hash;
        SceneFlag flags;
        float_t emission;
        float_t fade_frame_left;
        float_t fade_frame;
        EffectGroupX* effect_group;
        FrameRateControl* frame_rate;

        SceneX(SceneCounter counter, uint32_t hash, EffectGroupX* eff_group, bool a5);
        virtual ~SceneX();

        void CalcDisp();
        bool CanDisp(DispType disp_type, bool a3);
        void CheckUpdate(float_t delta_frame);
        bool Copy(EffectInstX* eff_inst, SceneX* dst);
        void Ctrl(float_t delta_frame);
        void Disp(DispType disp_type);
        void DispMesh();
        size_t GetCtrlCount(ParticleType ptcl_type);
        size_t GetDispCount(ParticleType ptcl_type);
        float_t GeFrameLifeTime(int32_t* life_time, size_t id);
        bool FreeEffect(uint32_t effect_hash, bool free);
        bool FreeEffectByID(size_t id, bool free);
        bool HasEnded(bool a2);
        bool HasEnded(size_t id, bool a3);
        void InitEffect(EffectX* eff, size_t id, bool appear_now, uint8_t load_flags = 0);
        bool ResetCheckInit(float_t* init_delta_frame = 0);
        bool ResetEffect(uint32_t effect_hash, size_t* id = 0);
        void SetEnded();
        void SetExtAnimMat(mat4* mat, size_t id);
        bool SetExtColor(float_t r, float_t g, float_t b, float_t a, bool set, uint32_t effect_hash);
        bool SetExtColorByID(float_t r, float_t g, float_t b, float_t a, bool set, size_t id);
        void SetExtScale(float_t scale, size_t id);
        void SetFrameRate(FrameRateControl* frame_rate);
        void SetReqFrame(size_t id, float_t req_frame);
    };

    class GltParticleManager : public app::Task {
    public:
        prj::map<uint64_t, EffectGroup> effect_groups;
        void* field_78;
        uint64_t field_80;
        prj::vector<Scene*> scenes;
        prj::vector<void*> file_readers;
        void* sys_frame_rate;
        ParticleManagerFlag flags;
        int32_t scene_load_counter;
        int32_t init_buffers_init;
        int32_t init_buffers;
        float_t init_delta_frame_base;
        float_t init_delta_frame;
        float_t emission;
        float_t delta_frame;
        prj::map<uint64_t, uint32_t> resources;
        uint32_t texture_counter;

        void CalcDisp();
        void DispScenes(DispType disp_type);
    };

    class GltParticleManagerX : app::Task {
    public:
        std::vector<SceneX*> scenes;
        std::vector<FileReaderX*> file_readers;
        std::map<uint32_t, EffectGroupX*> effect_groups;
        FrameRateControl* frame_rate;
        ParticleManagerFlag flags;
        int32_t scene_load_counter;
        int32_t init_buffers_base;
        int32_t init_buffers;
        float_t init_delta_frame_base;
        float_t init_delta_frame;
        float_t emission;
        float_t delta_frame;

        GltParticleManagerX();
        virtual ~GltParticleManagerX() override;

        virtual bool init() override;
        virtual bool ctrl() override;
        virtual bool dest() override;
        virtual void disp() override;
        virtual void basic() override;

        bool AppendEffectGroup(uint32_t hash, EffectGroupX* eff_group, FileReaderX* file_read);
        void BasicEffectGroups();
        void CalcDisp();
        bool CheckHasLocalEffect();
        bool CheckNoFileReaders(uint32_t hash);
        bool CheckSceneEnded(SceneCounter scene_counter);
        void CheckSceneHasLocalEffect(SceneX* sc);
        void CtrlScenes();
        void DecrementInitBuffersByCount(int32_t count = 1);
        void DispScenes(DispType disp_type);
        void FreeEffects();
        void FreeSceneEffect(SceneCounter scene_counter, bool force_kill = true);
        void FreeSceneEffect(uint32_t effect_group_hash, uint32_t effect_hash, bool force_kill = true);
        void FreeScene(uint32_t effect_group_hash);
        void FreeScenes();
        size_t GetCtrlCount(ParticleType type);
        size_t GetDispCount(ParticleType type);
        EffectGroupX* GetEffectGroup(uint32_t hash);
        size_t GetEffectsCount(uint32_t hash);
        bool GetPause();
        float_t GetSceneFrameLifeTime(SceneCounter scene_counter, int32_t* life_time);
        SceneCounter GetSceneCounter(uint8_t index = 0);
        uint32_t LoadFile(const char* file, const char* path, float_t emission, bool init_scene);
        SceneCounter LoadSceneEffect(uint32_t hash, bool appear_now = true, uint8_t load_flags = 0);
        void SetInitDeltaFrame(float_t value);
        void SetSceneEffectExtAnimMat(SceneCounter scene_counter, mat4* mat);
        void SetSceneEffectExtColor(SceneCounter scene_counter,
            float_t r, float_t g, float_t b, float_t a, bool set, uint32_t effect_hash);
        void SetSceneEffectExtScale(SceneCounter scene_counter, float_t scale);
        void SetSceneEffectReqFrame(SceneCounter scene_counter, float_t req_frame);
        void SetSceneFrameRate(SceneCounter scene_counter, FrameRateControl* frame_rate);
        void SetPause(bool value);
        void UnloadEffectGroup(uint32_t hash);
    };

    extern GltParticleManager* glt_particle_manager;
    extern GltParticleManagerX* glt_particle_manager_x;
    extern RandomX random_x;
    extern Counter& counter;

    extern void axis_angle_from_vectors(vec3& axis, float_t& angle, const vec3& vec0, const vec3& vec1);

    extern void CreateBuffer(size_t max_count, bool is_quad,
        Buffer*& buffer, GLuint& vao, GL::ArrayBuffer& vbo, GL::ElementArrayBuffer& ebo);
    extern void DeleteBuffer(Buffer*& buffer, GLuint& vao, GL::ArrayBuffer& vbo, GL::ElementArrayBuffer& ebo);

    extern void Init();
    extern void Free();

    extern void Patch();
}
