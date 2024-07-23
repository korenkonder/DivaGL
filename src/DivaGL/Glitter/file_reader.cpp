/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"
#include "../../KKdLib/io/path.hpp"
#include "../../KKdLib/farc.hpp"
#include "../rob/rob.hpp"

namespace Glitter {
    FileReaderX::FileReaderX() : file_handler(), farc(),
        effect_group(), load_count(), state(), init_scene() {
        emission = -1.0f;
        hash = hash_murmurhash_empty;
    }

    FileReaderX::FileReaderX(const char* path, const char* file, float_t emission)
        : file_handler(), farc(), effect_group(), load_count(), state(), init_scene() {
        this->path = path ? path : "rom/particle_x/";
        this->file = file;
        this->emission = emission;
        this->hash = hash_utf8_murmurhash(file);
    }

    FileReaderX::FileReaderX(const wchar_t* path, const wchar_t* file, float_t emission)
        : file_handler(), farc(), effect_group(), load_count(), state(), init_scene() {
        char* path_temp = utf16_to_utf8(path);
        char* file_temp = utf16_to_utf8(file);
        this->path = path_temp ? path_temp : "rom/particle_x/";
        this->file = file_temp;
        this->emission = emission;
        this->hash = hash_utf8_murmurhash(file_temp);
        free_def(path_temp);
        free_def(file_temp);
    }

    FileReaderX::~FileReaderX() {
        delete farc;
        file_handler->~p_file_handler();
        _operator_delete(file_handler);
    }

    bool FileReaderX::CheckInit() {
        if (!effect_group)
            return true;

        bool ret = 0;
        if (!effect_group->scene_init) {
            float_t init_delta_frame = Glitter::glt_particle_manager_x->init_delta_frame;
            if (init_delta_frame > 0.0f) {
                if (effect_group->scene) {
                    if (effect_group->scene->ResetCheckInit(&init_delta_frame))
                        ret = true;
                    else
                        effect_group->scene_init = true;
                    glt_particle_manager_x->SetInitDeltaFrame(init_delta_frame);
                }
                else
                    effect_group->scene_init = true;
            }
            else
                ret = true;
        }

        if (!effect_group->buffer_init) {
            int32_t init_buffers = Glitter::glt_particle_manager_x->init_buffers;
            if (init_buffers <= 0)
                return 0;

            for (EffectX*& i : effect_group->effects) {
                if (!i)
                    continue;

                for (EmitterX*& j : i->emitters) {
                    if (!j || j->buffer_init)
                        continue;

                    for (ParticleX*& k : j->particles) {
                        if (!k || k->vao || k->vbo)
                            continue;
                        else if (k->data.type == PARTICLE_MESH || k->max_count <= 0 || init_buffers <= 0)
                            continue;

                        CreateBuffer(k->max_count, k->data.type == PARTICLE_QUAD, k->vao, k->vbo, k->ebo);
                        init_buffers--;
                        glt_particle_manager_x->DecrementInitBuffersByCount();
                    }

                    j->buffer_init = true;
                }
            }

            effect_group->buffer_init = true;
        }

        for (MeshX& i : effect_group->meshes)
            if (!i.ready && i.object_set != -1)
                if (objset_info_storage_load_obj_set_check_not_read(i.object_set))
                    ret = true;
                else
                    i.ready = true;

        if (!ret) {
            effect_group = 0;
            return true;
        }
        return false;
    }

    int32_t FileReaderX::GetEffectExtAnimBoneIndex(EffectExtAnimCharaNode node) {
        switch (node) {
        case EFFECT_EXT_ANIM_CHARA_HEAD:
            return MOT_BONE_FACE_ROOT;
        case EFFECT_EXT_ANIM_CHARA_MOUTH:
            return MOT_BONE_N_KUTI_U;
        case EFFECT_EXT_ANIM_CHARA_BELLY:
            return MOT_BONE_N_HARA_CP;
        case EFFECT_EXT_ANIM_CHARA_CHEST:
            return MOT_BONE_E_MUNE_CP;
        case EFFECT_EXT_ANIM_CHARA_LEFT_SHOULDER:
            return MOT_BONE_C_KATA_L;
        case EFFECT_EXT_ANIM_CHARA_LEFT_ELBOW:
            return MOT_BONE_J_UDE_L_WJ;
        case EFFECT_EXT_ANIM_CHARA_LEFT_ELBOW_DUP:
            return MOT_BONE_E_UDE_L_CP;
        case EFFECT_EXT_ANIM_CHARA_LEFT_HAND:
            return MOT_BONE_N_NAKA_L_EX;
        case EFFECT_EXT_ANIM_CHARA_RIGHT_SHOULDER:
            return MOT_BONE_C_KATA_R;
        case EFFECT_EXT_ANIM_CHARA_RIGHT_ELBOW:
            return MOT_BONE_J_UDE_R_WJ;
        case EFFECT_EXT_ANIM_CHARA_RIGHT_ELBOW_DUP:
            return MOT_BONE_E_UDE_R_CP;
        case EFFECT_EXT_ANIM_CHARA_RIGHT_HAND:
            return MOT_BONE_N_NAKA_R_EX;
        case EFFECT_EXT_ANIM_CHARA_LEFT_THIGH:
            return MOT_BONE_N_MOMO_C_L_WJ_EX;
        case EFFECT_EXT_ANIM_CHARA_LEFT_KNEE:
            return MOT_BONE_N_HIZA_L_WJ_EX;
        case EFFECT_EXT_ANIM_CHARA_LEFT_TOE:
            return MOT_BONE_KL_TOE_L_WJ;
        case EFFECT_EXT_ANIM_CHARA_RIGHT_THIGH:
            return MOT_BONE_N_MOMO_C_R_WJ_EX;
        case EFFECT_EXT_ANIM_CHARA_RIGHT_KNEE:
            return MOT_BONE_N_HIZA_R_WJ_EX;
        case EFFECT_EXT_ANIM_CHARA_RIGHT_TOE:
            return MOT_BONE_KL_TOE_R_WJ;
        default:
            return MOT_BONE_MAX;
        }
    }

    bool FileReaderX::LoadFarc(const char* path, const char* file, uint32_t hash) {
        if (this->hash != hash_murmurhash_empty)
            return false;

        this->path = path;
        this->file = file;

        prj::string path_temp;
        path_temp.assign(path);
        path_temp.append(file);
        path_temp.append(".farc");

        file_handler = new (_operator_new(sizeof(p_file_handler))) p_file_handler;
        farc = new ::farc;
        if (file_handler->read_file_path(path_temp.c_str(), prj::HeapCMallocTemp)) {
            this->hash = hash;
            load_count = 1;
            return true;
        }
        return false;
    }

    void FileReaderX::ParseAnimation(f2_struct* st, AnimationX* anim) {
        if (!st || !st->header.data_size
            || st->header.signature != reverse_endianness_uint32_t('ANIM'))
            return;

        for (f2_struct& i : st->sub_structs)
            ParseCurve(&i, anim);
    }

    void FileReaderX::ParseCurve(f2_struct* st, AnimationX* anim) {
        if (!st || !st->header.data_size
            || st->header.signature != reverse_endianness_uint32_t('CURV'))
            return;

        CurveX* c = new CurveX;
        if (!c)
            return;

        c->version = st->header.version;
        size_t d = (size_t)st->data.data();
        uint32_t keys_count;
        if (st->header.version == 1) {
            c->type = (CurveType) * (uint32_t*)d;
            c->repeat = *(uint32_t*)(d + 4) != 0;
            c->flags = (CurveFlag) * (uint32_t*)(d + 8);
            c->random_range = *(float_t*)(d + 12);
            keys_count = *(uint16_t*)(d + 16);
            c->start_time = *(uint16_t*)(d + 18);
            c->end_time = *(uint16_t*)(d + 20);
        }
        else {
            c->type = (CurveType) * (uint32_t*)d;
            c->repeat = *(uint32_t*)(d + 4) != 0;
            c->flags = (CurveFlag) * (uint32_t*)(d + 8);
            c->random_range = *(float_t*)(d + 12);
            keys_count = *(uint16_t*)(d + 28);
            c->start_time = *(uint16_t*)(d + 30);
            c->end_time = *(uint16_t*)(d + 32);
        }

        for (f2_struct& i : st->sub_structs)
            if (i.header.data_size && i.header.signature == reverse_endianness_uint32_t('KEYS')) {
                UnpackCurve(i.data.data(), anim, c, keys_count, i.header.version);
                break;
            }
        anim->curves.push_back(c);
    }

    bool FileReaderX::ParseDivaEffect(f2_struct* st) {
        if (!st || !st->header.data_size
            || st->header.signature != reverse_endianness_uint32_t('DVEF'))
            return false;

        uint32_t dvef_version = st->header.version;
        EffectGroupX* eff_group = new EffectGroupX;
        if (!eff_group)
            return false;

        eff_group->scene = 0;
        if (!ParseEffectGroup(st, &eff_group->effects, eff_group)) {
            eff_group->not_loaded = true;
            if (GPMX_VAL->AppendEffectGroup(hash, eff_group, this))
                return true;
        }
        else if (GPMX_VAL->AppendEffectGroup(hash, eff_group, this)) {
            for (MeshX& i : eff_group->meshes)
                if (i.object_set != -1) {
                    objset_info_storage_load_set(i.object_set);
                    i.load = true;
                }

            if (!init_scene)
                return true;

            effect_group = eff_group;
            int32_t id = 1;
            for (EffectX*& i : eff_group->effects) {
                if (i->data.start_time <= 0.0f) {
                    id++;
                    continue;
                }

                if (!effect_group->scene)
                    effect_group->scene = new SceneX(0, hash_murmurhash_empty, eff_group, true);

                if (effect_group->scene)
                    effect_group->scene->InitEffect(i, id, true);
                id++;
            }
            return true;
        }

        delete eff_group;
        return false;
    }

    bool FileReaderX::ParseDivaResource(f2_struct* st, EffectGroupX* eff_group) {
        if (!st || !st->header.data_size)
            return false;

        for (f2_struct& i : st->sub_structs)
            if (UnpackDivaResource(&i, eff_group))
                break;
        return true;
    }

    bool FileReaderX::ParseEffect(f2_struct* st, EffectGroupX* eff_group) {
        if (!st || !st->header.data_size
            || st->header.signature != reverse_endianness_uint32_t('EFCT'))
            return false;

        EffectX* eff = new EffectX;
        if (!UnpackEffect(st->data.data(), eff, st->header.version)) {
            delete eff;
            return false;
        }

        ParseAnimation(st->sub_structs.size() ? st->sub_structs.data() : 0, &eff->animation);

        for (f2_struct& i : st->sub_structs)
            ParseEmitter(&i, eff, eff_group);
        eff_group->effects.push_back(eff);
        return true;
    }

    bool FileReaderX::ParseEffectGroup(f2_struct* st, prj::vector<EffectX*>* vec,
        EffectGroupX* eff_group) {
        if (!st || !st->header.data_size
            || st->header.signature != reverse_endianness_uint32_t('DVEF'))
            return false;

        for (f2_struct& i : st->sub_structs) {
            if (!i.header.data_size)
                continue;

            if (i.header.signature == reverse_endianness_uint32_t('EFCT')) {
                if (!ParseEffect(&i, eff_group))
                    return false;
            }
            else if (i.header.signature == reverse_endianness_uint32_t('DVRS')
                && !UnpackDivaResourceHashes(&i, eff_group))
                return false;
        }
        return true;
    }

    bool FileReaderX::ParseEmitter(f2_struct* st, EffectX* eff, EffectGroupX* eff_group) {
        if (!st || !st->header.data_size || st->header.signature != reverse_endianness_uint32_t('EMIT'))
            return false;

        EmitterX* emit = new EmitterX;
        if (!UnpackEmitter(st->data.data(), emit, st->header.version)) {
            delete emit;
            return false;
        }

        ParseAnimation(st->sub_structs.size() ? st->sub_structs.data() : 0, &emit->animation);

        for (f2_struct& i : st->sub_structs)
            ParseParticle(&i, emit, eff, eff_group);
        eff->emitters.push_back(emit);
        return true;
    }

    bool FileReaderX::ParseParticle(f2_struct* st, EmitterX* emit, EffectX* eff, EffectGroupX* eff_group) {
        if (!st || !st->header.data_size || st->header.signature != reverse_endianness_uint32_t('PTCL'))
            return false;

        ParticleX* ptcl = new ParticleX;
        if (!UnpackParticle(st->data.data(), ptcl, st->header.version, eff, eff_group)) {
            delete ptcl;
            return false;
        }

        ParseAnimation(st->sub_structs.size() ? st->sub_structs.data() : 0, &ptcl->animation);
        emit->particles.push_back(ptcl);

        int32_t count = 0;
        switch (ptcl->data.type) {
        case Glitter::PARTICLE_QUAD:
            count = 250;
            if (ptcl->data.count)
                count = ptcl->data.count;
            count = 4 * count;
            break;
        case Glitter::PARTICLE_LINE:
            count = ptcl->data.locus_history_size + ptcl->data.locus_history_size_random + 1;
            break;
        case Glitter::PARTICLE_LOCUS:
            count = (ptcl->data.locus_history_size + ptcl->data.locus_history_size_random) * 2 + 1;
            break;
        case Glitter::PARTICLE_MESH:
        default:
            count = 0;
            break;
        }
        ptcl->max_count = count;
        return true;
    }

    bool FileReaderX::Read() {
        prj::string dve_file = file + ".dve";
        farc_file* dve_ff = farc->read_file(dve_file.c_str());
        if (!dve_ff)
            return false;

        f2_struct st;
        st.read(dve_ff->data, dve_ff->size);
        if (st.header.signature != reverse_endianness_uint32_t('DVEF'))
            return false;

        if (!ParseDivaEffect(&st)) {
            EffectGroupX* eff_group = new EffectGroupX;
            if (eff_group) {
                eff_group->not_loaded = true;
                if (!GPMX_VAL->AppendEffectGroup(hash, eff_group, this))
                    delete eff_group;
            }
            return false;
        }

        EffectGroupX* eff_group = GPMX_VAL->GetEffectGroup(hash);
        if (!eff_group)
            return false;

        prj::string drs_file = file + ".drs";
        farc_file* drs_ff = farc->read_file(drs_file.c_str());
        if (drs_ff) {
            f2_struct st;
            st.read(drs_ff->data, drs_ff->size);

            if (st.header.signature == reverse_endianness_uint32_t('DVRS'))
                ParseDivaResource(&st, eff_group);
        }
        return true;
    }

    bool FileReaderX::ReadFarc() {
        if (state) {
            if (state == 1)
                if (CheckInit())
                    state = 2;
                else
                    return false;
            return true;
        }

        if (file_handler && !file_handler->check_not_ready()) {
            farc->read(file_handler->get_data(), file_handler->get_size(), true);
            if (Read() && init_scene)
                state = 1;
            else
                return true;
        }
        return false;
    }

    void FileReaderX::UnpackCurve(void* data, AnimationX* anim,
        CurveX* c, uint32_t count, uint32_t keys_version) {
        if (!data || !c || count < 1)
            return;

        c->keys_version = keys_version;
        size_t d = (size_t)data;

        CurveX::Key key;
        key.type = KEY_CONSTANT;
        key.frame = 0;
        key.value = 0.0f;
        key.tangent1 = 0.0f;
        key.tangent2 = 0.0f;
        key.random_range = 0.0f;

        prj::vector<CurveX::Key>& keys = c->keys;
        keys.reserve(count);
        if (c->keys_version == 2) {
            if (c->flags & CURVE_KEY_RANDOM_RANGE)
                for (size_t i = count; i; i--) {
                    key.type = (KeyType) * (int16_t*)d;
                    key.frame = *(int16_t*)(d + 2);
                    if (key.type == KEY_HERMITE) {
                        key.tangent1 = *(float_t*)(d + 4);
                        key.tangent2 = *(float_t*)(d + 8);
                        key.random_range = *(float_t*)(d + 12);
                        key.value = *(float_t*)(d + 16);
                        keys.push_back(key);
                        d += 20;
                    }
                    else {
                        key.random_range = *(float_t*)(d + 4);
                        key.value = *(float_t*)(d + 8);
                        keys.push_back(key);
                        d += 12;
                    }
                }
            else
                for (size_t i = count; i; i--) {
                    key.type = (KeyType) * (int16_t*)d;
                    key.frame = *(int16_t*)(d + 2);
                    if (key.type == KEY_HERMITE) {
                        key.tangent1 = *(float_t*)(d + 4);
                        key.tangent2 = *(float_t*)(d + 8);
                        key.value = *(float_t*)(d + 12);
                        keys.push_back(key);
                        d += 16;
                    }
                    else {
                        key.value = *(float_t*)(d + 4);
                        keys.push_back(key);
                        d += 8;
                    }
                }
        }
        else {
            if (c->flags & CURVE_KEY_RANDOM_RANGE)
                for (size_t i = count; i; i--) {
                    key.type = (KeyType) * (int16_t*)d;
                    key.frame = *(int16_t*)(d + 2);
                    if (key.type == KEY_HERMITE) {
                        key.tangent1 = *(float_t*)(d + 16);
                        key.tangent2 = *(float_t*)(d + 20);
                        key.random_range = *(float_t*)(d + 24);
                        key.value = *(float_t*)(d + 28);
                        keys.push_back(key);
                        d += 32;
                    }
                    else {
                        key.random_range = *(float_t*)(d + 8);
                        key.value = *(float_t*)(d + 12);
                        keys.push_back(key);
                        d += 16;
                    }
                }
            else
                for (size_t i = count; i; i--) {
                    key.type = (KeyType) * (int16_t*)d;
                    key.frame = *(int16_t*)(d + 2);
                    if (key.type == KEY_HERMITE) {
                        key.tangent1 = *(float_t*)(d + 4);
                        key.tangent2 = *(float_t*)(d + 8);
                        key.value = *(float_t*)(d + 12);
                        keys.push_back(key);
                        d += 16;
                    }
                    else {
                        key.value = *(float_t*)(d + 12);
                        keys.push_back(key);
                        d += 16;
                    }
                }
        }
    }

    bool FileReaderX::UnpackDivaResource(f2_struct* st, EffectGroupX* eff_group) {
        if (!st || !st->header.data_size
            || st->header.signature != reverse_endianness_uint32_t('TXPC'))
            return false;

        eff_group->resources_tex.unpack_file(st->data.data(), false);
        if (!eff_group->resources_count)
            return false;

        size_t count = eff_group->resources_tex.textures.size();

        if (count < 1 || eff_group->resources_count < 1
            || eff_group->resources_count != count)
            return false;

        texture_id* ids = force_malloc<texture_id>(count);
        for (size_t i = 0; i < count; i++) {
            ids[i] = texture_id(0x2A, GPM_VAL->texture_counter);
            GPM_VAL->texture_counter++;
        }

        if (!texture_txp_set_load(&eff_group->resources_tex, &eff_group->resources, ids)) {
            free_def(ids);
            return false;
        }
        free_def(ids);

        for (EffectX*& i : eff_group->effects) {
            if (!i)
                continue;

            for (EmitterX*& j : i->emitters) {
                if (!j)
                    continue;

                for (ParticleX*& k : j->particles) {
                    if (!k)
                        continue;

                    k->data.texture = 0;
                    k->data.mask_texture = 0;

                    if (k->data.type == PARTICLE_LINE
                        || k->data.type == PARTICLE_MESH)
                        continue;

                    for (size_t l = 0; l < eff_group->resources_count; l++) {
                        if (k->data.tex_hash == eff_group->resource_hashes[l])
                            k->data.texture = eff_group->resources[l]->glid;
                        if (k->data.mask_tex_hash == eff_group->resource_hashes[l])
                            k->data.mask_texture = eff_group->resources[l]->glid;
                    }
                }
            }
        }
        return true;
    }

    bool FileReaderX::UnpackDivaResourceHashes(f2_struct* st, EffectGroupX* eff_group) {
        if (eff_group->resources_count && eff_group->resource_hashes.size() > 0)
            return true;
        else if (!st || !st->header.data_size
            || st->header.signature != reverse_endianness_uint32_t('DVRS'))
            return false;

        size_t d = (size_t)st->data.data();
        if (!d)
            return false;

        uint32_t count = *(int32_t*)d;
        d += 8;

        eff_group->resources_count = count;
        if (count) {
            eff_group->resource_hashes = prj::vector<uint32_t>(count);
            uint32_t* resource_hashes = eff_group->resource_hashes.data();
            if (!resource_hashes)
                return false;

            for (size_t i = count; i; i--, resource_hashes++, d += sizeof(uint64_t))
                *resource_hashes = (uint32_t) * (uint64_t*)d;
        }
        return true;
    }

    bool FileReaderX::UnpackEffect(void* data, EffectX* eff, int32_t efct_version) {
        eff->version = efct_version;
        eff->scale = 1.0f;
        eff->data.start_time = 0;
        eff->data.ext_anim = 0;
        eff->data.flags = (EffectFlag)0;
        eff->data.name_hash = hash_murmurhash_empty;
        eff->data.seed = 0;

        if (eff->version < 8 || eff->version > 12)
            return false;

        size_t d = (size_t)data;
        eff->data.name_hash = (uint32_t) * (uint64_t*)data;
        eff->data.appear_time = *(int32_t*)(d + 8);
        eff->data.life_time = *(int32_t*)(d + 12);
        eff->data.start_time = *(int32_t*)(d + 16);
        eff->data.color.b = *(uint8_t*)(d + 20);
        eff->data.color.g = *(uint8_t*)(d + 21);
        eff->data.color.r = *(uint8_t*)(d + 22);
        eff->data.color.a = *(uint8_t*)(d + 23);
        if (*(int32_t*)(d + 24))
            enum_or(eff->data.flags, EFFECT_LOOP);
        eff->translation = *(vec3*)(d + 28);
        eff->rotation = *(vec3*)(d + 40);
        EffectFileFlag flags = (EffectFileFlag) * (int32_t*)(d + 52);
        d += 56;

        if (flags & EFFECT_FILE_ALPHA)
            enum_or(eff->data.flags, EFFECT_ALPHA);

        if (flags & EFFECT_FILE_FOG)
            enum_or(eff->data.flags, EFFECT_FOG);
        else if (flags & EFFECT_FILE_FOG_HEIGHT)
            enum_or(eff->data.flags, EFFECT_FOG_HEIGHT);

        if (flags & EFFECT_FILE_EMISSION)
            enum_or(eff->data.flags, EFFECT_EMISSION);

        if (flags & EFFECT_FILE_USE_SEED)
            enum_or(eff->data.flags, EFFECT_USE_SEED);

        if (eff->version != 8 && flags & 0x20)
            enum_or(eff->data.flags, 0x80);

        eff->data.emission = *(float_t*)d;
        eff->data.seed = *(int32_t*)(d + 4);
        eff->data.unk = *(float_t*)(d + 8);
        d += 12;

        if (eff->version != 8)
            d += 8;
        else
            d += 12;

        int32_t type = *(int32_t*)d;

        if (eff->version != 8)
            d += 4;
        else
            d += 8;

        if (type == 1)
            enum_or(eff->data.flags, EFFECT_LOCAL);
        else if (type == 2) {
            EffectX::ExtAnim* ext_anim = force_malloc<EffectX::ExtAnim>();
            eff->data.ext_anim = ext_anim;
            if (ext_anim) {
                ext_anim->chara_index = *(int32_t*)d;
                ext_anim->flags = (EffectExtAnimFlag) * (int32_t*)(d + 4);
                int32_t node_index = * (int32_t*)(d + 8);

                ext_anim->bone_index = GetEffectExtAnimBoneIndex((EffectExtAnimCharaNode)node_index);
                enum_or(ext_anim->flags, EFFECT_EXT_ANIM_CHARA_ANIM);
            }
        }
        else if (type == 3) {
            uint32_t object_hash = (uint32_t) * (uint64_t*)d;

            object_info object;
            object.id = object_hash & 0xFFFF;
            object.set_id = (object_hash >> 16) & 0xFFFF;
            if (object.not_null()) {
                if (eff->version == 8) {
                    EffectX::ExtAnim* ext_anim = force_malloc<EffectX::ExtAnim>();
                    eff->data.ext_anim = ext_anim;
                    if (ext_anim) {
                        ext_anim->object = object;
                        ext_anim->flags = (EffectExtAnimFlag) * (int32_t*)(d + 8);
                        ext_anim->instance_id = 0;
                        ext_anim->a3da_uid = -1;
                        if (*(char*)(d + 12)) {
                            strncpy_s(ext_anim->mesh_name, 0x80, (char*)(d + 12), 0x80);
                            ext_anim->mesh_name[0x7F] = 0;
                        }
                        else
                            ext_anim->mesh_name[0] = 0;
                    }
                }
                else if (eff->version == 10) {
                    EffectX::ExtAnim* ext_anim = force_malloc<EffectX::ExtAnim>();
                    eff->data.ext_anim = ext_anim;
                    if (ext_anim) {
                        ext_anim->object = object;
                        ext_anim->flags = (EffectExtAnimFlag) * (int32_t*)(d + 8);
                        ext_anim->instance_id = 0;
                        ext_anim->a3da_uid = -1;
                        if (*(char*)(d + 16)) {
                            strncpy_s(ext_anim->mesh_name, 0x80, (char*)(d + 16), 0x80);
                            ext_anim->mesh_name[0x7F] = 0;
                        }
                        else
                            ext_anim->mesh_name[0] = 0;
                    }
                }
                else {
                    EffectX::ExtAnim* ext_anim = force_malloc<EffectX::ExtAnim>();
                    eff->data.ext_anim = ext_anim;
                    if (ext_anim) {
                        ext_anim->object = object;
                        ext_anim->flags = (EffectExtAnimFlag) * (int32_t*)(d + 8);
                        ext_anim->instance_id = *(int32_t*)(d + 12);
                        ext_anim->a3da_uid = (int32_t) * (uint64_t*)(d + 16);

                        if (*(char*)(d + 32)) {
                            strncpy_s(ext_anim->mesh_name, 0x80, (char*)(d + 32), 0x80);
                            ext_anim->mesh_name[0x7F] = 0;
                        }
                        else
                            ext_anim->mesh_name[0] = 0;
                    }
                }
            }
        }
        return true;
    }

    bool FileReaderX::UnpackEmitter(void* data, EmitterX* emit, uint32_t emit_version) {
        emit->version = emit_version;
        size_t d = (size_t)data;
        emit->data.start_time = *(int32_t*)d;
        emit->data.life_time = *(int32_t*)(d + 4);
        emit->data.loop_start_time = *(int32_t*)(d + 8);
        emit->data.loop_end_time = *(int32_t*)(d + 12);
        emit->data.flags = (EmitterFlag) * (int32_t*)(d + 16);
        d += 20;

        if (emit->version < 3 || emit->version > 4)
            return false;

        emit->data.type = (EmitterType) * (int16_t*)d;
        emit->data.direction = (Direction) * (int16_t*)(d + 2);
        emit->data.emission_interval = *(float_t*)(d + 4);
        emit->data.particles_per_emission = *(float_t*)(d + 8);
        emit->data.timer = (EmitterTimerType) * (int16_t*)(d + 12);
        emit->data.seed = *(int32_t*)(d + 16);
        d += emit->version == 3 ? 36 : 32;

        emit->translation = *(vec3*)d;
        emit->rotation = *(vec3*)(d + 12);
        emit->scale = *(vec3*)(d + 24);
        emit->data.rotation_add = *(vec3*)(d + 36);
        emit->data.rotation_add_random = *(vec3*)(d + 48);
        d += 60;

        
        switch (emit->data.type) {
        case EMITTER_BOX:
            emit->data.box.size = *(vec3*)d;
            return true;
        case EMITTER_CYLINDER:
            emit->data.cylinder.radius = *(float_t*)d;
            emit->data.cylinder.height = *(float_t*)(d + 4);
            emit->data.cylinder.start_angle = *(float_t*)(d + 8);
            emit->data.cylinder.end_angle = *(float_t*)(d + 12);
            emit->data.cylinder.on_edge = *(int32_t*)(d + 16) & 1 ? true : false;
            emit->data.cylinder.direction = (EmitterEmissionDirection)
                (*(int32_t*)(d + 16) >> 1);
            return true;
        case EMITTER_SPHERE:
            emit->data.sphere.radius = *(float_t*)d;
            emit->data.sphere.latitude = *(float_t*)(d + 4);
            emit->data.sphere.longitude = *(float_t*)(d + 8);
            emit->data.sphere.on_edge = *(int32_t*)(d + 12) & 1 ? true : false;
            emit->data.sphere.direction = (EmitterEmissionDirection)
                (*(int32_t*)(d + 12) >> 1);
            return true;
        case EMITTER_POLYGON:
            emit->data.polygon.size = *(float_t*)d;
            emit->data.polygon.count = *(int32_t*)(d + 4);
            emit->data.polygon.direction = (EmitterEmissionDirection)
                (*(int32_t*)(d + 8) >> 1);
            return true;
        }
        return false;
    }

    bool FileReaderX::UnpackParticle(void* data, ParticleX* ptcl,
        uint32_t ptcl_version, EffectX* eff, EffectGroupX* eff_group) {
        ptcl->version = ptcl_version;

        uint8_t r;
        uint8_t b;
        uint8_t g;
        uint8_t a;
        ParticleBlend blend_mode;
        ParticleBlend mask_blend_mode;
        uint32_t tex_hash;
        uint32_t mask_tex_hash;
        int32_t frame_step_uv;
        int32_t uv_index_start;
        int32_t uv_index;
        int32_t uv_index_end;
        UVIndexType uv_index_type;
        uint8_t split_u;
        uint8_t split_v;
        int32_t unk2;
        int32_t unk3;
        int32_t unk4;

        ptcl->data.mesh.object_set = -1;
        ptcl->data.mesh.object = -1;
        //ptcl->data.mesh.mesh_name[0] = 0;
        //ptcl->data.mesh.sub_mesh = -1;

        size_t d = (size_t)data;
        ptcl->data.life_time = *(int32_t*)d;
        ptcl->data.life_time_random = *(int32_t*)(d + 4);
        ptcl->data.fade_in = *(int32_t*)(d + 8);
        ptcl->data.fade_in_random = *(int32_t*)(d + 12);
        ptcl->data.fade_out = *(int32_t*)(d + 16);
        ptcl->data.fade_out_random = *(int32_t*)(d + 20);
        ptcl->data.type = (ParticleType) * (int32_t*)(d + 24);
        ptcl->data.draw_type = (Direction) * (int32_t*)(d + 28);

        ptcl->data.unk0 = 0;
        ptcl->data.unk1 = -1.0f;

        if (ptcl->version < 4 || ptcl->version > 5)
            return false;

        ptcl->data.rotation = *(vec3*)(d + 32);
        ptcl->data.rotation_random = *(vec3*)(d + 44);
        ptcl->data.rotation_add = *(vec3*)(d + 56);
        ptcl->data.rotation_add_random = *(vec3*)(d + 68);
        ptcl->data.scale = *(vec3*)(d + 80);
        ptcl->data.scale_random = *(vec3*)(d + 92);
        ptcl->data.z_offset = *(float_t*)(d + 104);
        ptcl->data.pivot = (Pivot) * (int32_t*)(d + 108);
        ptcl->data.flags = (ParticleFlag) * (int32_t*)(d + 112);
        ptcl->data.unk0 = *(int32_t*)(d + 116);

        if (eff->data.flags & EFFECT_LOCAL)
            enum_or(ptcl->data.flags, PARTICLE_LOCAL);
        if (eff->data.flags & EFFECT_EMISSION)
            enum_or(ptcl->data.flags, PARTICLE_EMISSION);

        ptcl->data.uv_scroll_2nd_add = *(vec2*)(d + 120);
        ptcl->data.uv_scroll_2nd_add_scale = *(float_t*)(d + 128);
        ptcl->data.speed = *(float_t*)(d + 136);
        ptcl->data.speed_random = *(float_t*)(d + 140);
        ptcl->data.deceleration = *(float_t*)(d + 144);
        ptcl->data.deceleration_random = *(float_t*)(d + 148);
        ptcl->data.direction = *(vec3*)(d + 152);
        ptcl->data.direction_random = *(vec3*)(d + 164);
        ptcl->data.gravity = *(vec3*)(d + 176);
        ptcl->data.acceleration = *(vec3*)(d + 188);
        ptcl->data.acceleration_random = *(vec3*)(d + 200);
        ptcl->data.reflection_coeff = *(float_t*)(d + 212);
        ptcl->data.reflection_coeff_random = *(float_t*)(d + 216);
        ptcl->data.rebound_plane_y = *(float_t*)(d + 220);
        ptcl->data.uv_scroll_add = *(vec2*)(d + 224);
        ptcl->data.uv_scroll_add_scale = *(float_t*)(d + 232);
        ptcl->data.sub_flags = (ParticleSubFlag) * (int32_t*)(d + 236);
        ptcl->data.count = *(int32_t*)(d + 240);
        ptcl->data.draw_flags = (ParticleDrawFlag) * (int32_t*)(d + 244);
        ptcl->data.unk1 = *(float_t*)(d + 248);
        ptcl->data.emission = *(float_t*)(d + 252);
        d += 256;

        if (ptcl->version == 4) {
            ptcl->data.direction *= 10.0f;
            ptcl->data.direction_random *= 10.0f;
        }

        if (ptcl->data.emission >= min_emission)
            enum_or(ptcl->data.flags, PARTICLE_EMISSION);

        ptcl->data.locus_history_size = 0;
        ptcl->data.locus_history_size_random = 0;

        switch (ptcl->data.type) {
        case 3:
            ptcl->data.type = PARTICLE_LINE;
            break;
        case 1:
            ptcl->data.type = PARTICLE_MESH;
            break;
        }

        bool has_tex;
        if (ptcl->data.type == PARTICLE_QUAD)
            has_tex = true;
        else if (ptcl->data.type == PARTICLE_LINE)
            has_tex = false;
        else if (ptcl->data.type == PARTICLE_LOCUS) {
            ptcl->data.locus_history_size = *(uint16_t*)(d + 0);
            ptcl->data.locus_history_size_random = *(uint16_t*)(d + 2);
            d += 4;
            has_tex = true;
        }
        else if (ptcl->data.type == PARTICLE_MESH) {
            ptcl->data.mesh.object = (uint32_t) * (uint64_t*)d;
            ptcl->data.mesh.object_set = (uint32_t) * (uint64_t*)(d + 8);
            //if (*(char*)(d + 10))
            //    strncpy_s(ptcl->data.mesh.mesh_name, (char*)(d + 16), 0x40);
            //else
            //    ptcl->data.mesh.mesh_name[0] = 0;
            //ptcl->data.mesh.sub_mesh = (uint32_t) * (uint64_t*)(d + 80);
            d += 88;
            has_tex = false;

            eff_group->meshes.push_back((uint32_t)ptcl->data.mesh.object_set);
        }
        else
            return false;

        ptcl->data.texture = 0;
        ptcl->data.mask_texture = 0;

        tex_hash = hash_murmurhash_empty;
        mask_tex_hash = hash_murmurhash_empty;
        unk2 = 0;
        unk3 = 0;

        if (has_tex)
            tex_hash = (uint32_t) * (uint64_t*)d;
        r = *(uint8_t*)(d + 8);
        g = *(uint8_t*)(d + 9);
        b = *(uint8_t*)(d + 10);
        a = *(uint8_t*)(d + 11);
        blend_mode = (ParticleBlend) * (int32_t*)(d + 12);
        unk2 = *(int32_t*)(d + 16);
        split_u = (uint8_t) * (int32_t*)(d + 20);
        split_v = (uint8_t) * (int32_t*)(d + 24);
        uv_index_type = (UVIndexType) * (int32_t*)(d + 28);
        uv_index = *(int16_t*)(d + 32);
        frame_step_uv = *(int16_t*)(d + 34);
        uv_index_start = *(int32_t*)(d + 36);
        uv_index_end = *(int32_t*)(d + 40);
        unk3 = *(int32_t*)(d + 44);
        d += 48;

        if (ptcl->data.flags & PARTICLE_TEXTURE_MASK) {
            if (has_tex)
                mask_tex_hash = (uint32_t) * (uint64_t*)d;
            mask_blend_mode = (ParticleBlend) * (int32_t*)(d + 8);
            unk4 = *(int32_t*)(d + 12);
        }
        else {
            mask_blend_mode = PARTICLE_BLEND_TYPICAL;
            unk4 = 0xFF;
        }

        ptcl->data.tex_hash = tex_hash;
        ptcl->data.mask_tex_hash = mask_tex_hash;
        ptcl->data.color.x = (float_t)r;
        ptcl->data.color.y = (float_t)g;
        ptcl->data.color.z = (float_t)b;
        ptcl->data.color.w = (float_t)a;
        ptcl->data.blend_mode = blend_mode;
        ptcl->data.mask_blend_mode = mask_blend_mode;
        ptcl->data.split_u = split_u;
        ptcl->data.split_v = split_v;
        ptcl->data.uv_index_type = uv_index_type;
        ptcl->data.uv_index = max_def(uv_index, 0);
        ptcl->data.frame_step_uv = frame_step_uv;
        ptcl->data.uv_index_start = max_def(uv_index_start, 0);
        ptcl->data.uv_index_end = uv_index_end;
        ptcl->data.split_uv.x = (float_t)split_u;
        ptcl->data.split_uv.y = (float_t)split_v;
        ptcl->data.unk2 = unk2;
        ptcl->data.unk3 = unk3;
        ptcl->data.unk4 = unk4;

        ptcl->data.split_uv = vec2::rcp(ptcl->data.split_uv);
        ptcl->data.color *= (float_t)(1.0 / 255.0);

        int32_t uv_max_count = (int32_t)(split_u * split_v);
        if (uv_max_count)
            uv_max_count--;

        switch (uv_index_type) {
        case UV_INDEX_FIXED:
        case UV_INDEX_RANDOM:
        case UV_INDEX_FORWARD:
        case UV_INDEX_REVERSE:
        case UV_INDEX_USER:
            ptcl->data.uv_index &= uv_max_count;
            break;
        case UV_INDEX_INITIAL_RANDOM_FIXED:
        case UV_INDEX_INITIAL_RANDOM_FORWARD:
        case UV_INDEX_INITIAL_RANDOM_REVERSE:
            if (uv_index < uv_index_start)
                ptcl->data.uv_index = uv_index_start;
            else if (uv_index_end >= 0 && uv_index > uv_index_end)
                ptcl->data.uv_index = uv_index_end;

            ptcl->data.uv_index = min_def(ptcl->data.uv_index, uv_max_count);
            break;
        }

        if (uv_index_end >= 0)
            ptcl->data.uv_index_count = uv_index_end - uv_index_start + 1;
        else
            ptcl->data.uv_index_count = uv_max_count - uv_index_start;

        if (ptcl->data.uv_index_count < 0)
            ptcl->data.uv_index_count = 0;
        return true;
    }
}
