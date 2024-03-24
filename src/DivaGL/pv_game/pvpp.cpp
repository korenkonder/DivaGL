/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "pvpp.hpp"

const pvpp* pvpp::read(const void* data, size_t size) {
    if (!data || !size) {
        return 0;
    }

    sizeof(pvpp_auth_3d);
    sizeof(pvpp_glitter);
    sizeof(pvpp_chara);
    sizeof(pvpp_effect);
    sizeof(pvpp_chara_effect);
    sizeof(pvpp_chara_effect_auth_3d);

    size_t d = (size_t)data;
    pvpp* pp = (pvpp*)data;
    if (pp->signature != reverse_endianness_uint32_t('pvpp'))
        return 0;

    if (pp->chara_array) {
        pp->chara_array = (pvpp_chara*)(d + (size_t)pp->chara_array);

        int32_t num_chara = pp->num_chara;
        for (int32_t i = 0; i < num_chara; i++) {
            pvpp_chara* chr = (pvpp_chara*)&pp->chara_array[i];

            if (chr->auth_3d_array) {
                chr->auth_3d_array = (pvpp_auth_3d*)(d + (size_t)chr->auth_3d_array);

                int32_t num_auth_3d = chr->num_auth_3d;
                for (int32_t i = 0; i < num_auth_3d; i++) {
                    pvpp_auth_3d* a3d = (pvpp_auth_3d*)&chr->auth_3d_array[i];

                    if (a3d->name)
                        a3d->name = (const char*)(d + (size_t)a3d->name);
                }
            }

            if (chr->glitter_array) {
                chr->glitter_array = (pvpp_glitter*)(d + (size_t)chr->glitter_array);

                int32_t num_glitter = chr->num_glitter;
                for (int32_t i = 0; i < num_glitter; i++) {
                    pvpp_glitter* glt = (pvpp_glitter*)&chr->glitter_array[i];

                    if (glt->name)
                        glt->name = (const char*)(d + (size_t)glt->name);
                }
            }

            pvpp_chara_effect*& chr_eff = *(pvpp_chara_effect**)&chr->chara_effect;
            if (chr_eff) {
                chr_eff = (pvpp_chara_effect*)(d + (size_t)chr_eff);

                if (chr_eff->auth_3d_array) {
                    chr_eff->auth_3d_array = (pvpp_chara_effect_auth_3d*)(d + (size_t)chr_eff->auth_3d_array);

                    int32_t num_auth_3d = chr_eff->num_auth_3d;
                    for (int32_t i = 0; i < num_auth_3d; i++) {
                        pvpp_chara_effect_auth_3d* a3d = (pvpp_chara_effect_auth_3d*)&chr_eff->auth_3d_array[i];

                        if (a3d->auth_3d)
                            a3d->auth_3d = (const char*)(d + (size_t)a3d->auth_3d);

                        if (a3d->object_set)
                            a3d->object_set = (const char*)(d + (size_t)a3d->object_set);
                    }
                }
            }
        }
    }

    if (pp->effect_array) {
        pp->effect_array = (pvpp_effect*)(d + (size_t)pp->effect_array);

        int32_t num_effect = pp->num_effect;
        for (int32_t i = 0; i < num_effect; i++) {
            pvpp_effect* eff = (pvpp_effect*)&pp->effect_array[i];

            if (eff->auth_3d_array) {
                eff->auth_3d_array = (pvpp_auth_3d*)(d + (size_t)eff->auth_3d_array);

                int32_t num_auth_3d = eff->num_auth_3d;
                for (int32_t i = 0; i < num_auth_3d; i++) {
                    pvpp_auth_3d* a3d = (pvpp_auth_3d*)&eff->auth_3d_array[i];

                    if (a3d->name)
                        a3d->name = (const char*)(d + (size_t)a3d->name);
                }
            }

            if (eff->glitter_array) {
                eff->glitter_array = (pvpp_glitter*)(d + (size_t)eff->glitter_array);

                int32_t num_glitter = eff->num_glitter;
                for (int32_t i = 0; i < num_glitter; i++) {
                    pvpp_glitter* glt = (pvpp_glitter*)&eff->glitter_array[i];

                    if (glt->name)
                        glt->name = (const char*)(d + (size_t)glt->name);
                }
            }
        }
    }
    return pp;
}
