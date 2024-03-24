/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "pvsr.hpp"

const pvsr* pvsr::read(const void* data, size_t size) {
    if (!data || !size) {
        return 0;
    }

    size_t d = (size_t)data;
    pvsr* sr = (pvsr*)data;
    if (sr->signature != reverse_endianness_uint32_t('pvsr'))
        return 0;

    if (sr->effect_array) {
        sr->effect_array = (pvsr_effect*)(d + (size_t)sr->effect_array);

        int32_t num_effect = sr->num_effect;
        for (int32_t i = 0; i < num_effect; i++) {
            pvsr_effect* eff = (pvsr_effect*)&sr->effect_array[i];

            if (eff->name)
                eff->name = (const char*)(d + (size_t)eff->name);
        }
    }

    if (sr->stage_effect_array) {
        sr->stage_effect_array = (pvsr_stage_effect*)(d + (size_t)sr->stage_effect_array);

        int32_t num_stage_effect = sr->num_stage_effect;
        for (int32_t i = 0; i < num_stage_effect; i++) {
            pvsr_stage_effect* eff = (pvsr_stage_effect*)&sr->stage_effect_array[i];

            if (eff->auth_3d_array) {
                eff->auth_3d_array = (pvsr_auth_3d*)(d + (size_t)eff->auth_3d_array);

                int32_t num_auth_3d = eff->num_auth_3d;
                for (int32_t i = 0; i < num_auth_3d; i++) {
                    pvsr_auth_3d* a3d = (pvsr_auth_3d*)&eff->auth_3d_array[i];

                    if (a3d->name)
                        a3d->name = (const char*)(d + (size_t)a3d->name);
                }
            }

            if (eff->glitter_array) {
                eff->glitter_array = (pvsr_glitter*)(d + (size_t)eff->glitter_array);

                int32_t num_glitter = eff->num_glitter;
                for (int32_t i = 0; i < num_glitter; i++) {
                    pvsr_glitter* glt = (pvsr_glitter*)&eff->glitter_array[i];

                    if (glt->name)
                        glt->name = (const char*)(d + (size_t)glt->name);
                }
            }
        }
    }

    if (sr->stage_effect_array) {
        sr->stage_effect_env_array = (pvsr_stage_effect_env*)(d + (size_t)sr->stage_effect_env_array);

        int32_t num_stage_effect_env = sr->num_stage_effect_env;
        for (int32_t i = 0; i < num_stage_effect_env; i++) {
            pvsr_stage_effect_env* eff_env = (pvsr_stage_effect_env*)&sr->stage_effect_env_array[i];

            if (eff_env->aet_array) {
                eff_env->aet_array = (pvsr_auth_2d*)(d + (size_t)eff_env->aet_array);

                int32_t num_aet = eff_env->num_aet;
                for (int32_t i = 0; i < num_aet; i++) {
                    pvsr_auth_2d* a2d = (pvsr_auth_2d*)&eff_env->aet_array[i];

                    if (a2d->name)
                        a2d->name = (const char*)(d + (size_t)a2d->name);
                }
            }
        }
    }

    if (sr->stage_effect_array) {
        sr->stage_change_effect_array = (pvsr_stage_change_effect*)(d + (size_t)sr->stage_change_effect_array);

        for (int32_t i = 0; i < PVSR_STAGE_EFFECT_COUNT; i++)
            for (int32_t j = 0; j < PVSR_STAGE_EFFECT_COUNT; j++) {
                pvsr_stage_change_effect* chg_eff = (pvsr_stage_change_effect*)&sr->
                    stage_change_effect_array[i * PVSR_STAGE_EFFECT_COUNT + j];

                if (chg_eff->auth_3d_array) {
                    chg_eff->auth_3d_array = (pvsr_auth_3d*)(d + (size_t)chg_eff->auth_3d_array);

                    int32_t num_auth_3d = chg_eff->num_auth_3d;
                    for (int32_t i = 0; i < num_auth_3d; i++) {
                        pvsr_auth_3d* a3d = (pvsr_auth_3d*)&chg_eff->auth_3d_array[i];

                        if (a3d->name)
                            a3d->name = (const char*)(d + (size_t)a3d->name);
                    }
                }

                if (chg_eff->glitter_array) {
                    chg_eff->glitter_array = (pvsr_glitter*)(d + (size_t)chg_eff->glitter_array);

                    int32_t num_glitter = chg_eff->num_glitter;
                    for (int32_t i = 0; i < num_glitter; i++) {
                        pvsr_glitter* glt = (pvsr_glitter*)&chg_eff->glitter_array[i];

                        if (glt->name)
                            glt->name = (const char*)(d + (size_t)glt->name);
                    }
                }
            }
    }
    return sr;
}
