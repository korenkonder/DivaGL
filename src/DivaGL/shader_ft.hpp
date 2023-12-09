/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "shader.hpp"

enum shader_ft_enum {
    SHADER_FT_FFP = 0,
    SHADER_FT_BLINN,
    SHADER_FT_ITEM,
    SHADER_FT_STAGE,
    SHADER_FT_SKIN,
    SHADER_FT_SSS_SKIN,
    SHADER_FT_SSS_FILT,
    SHADER_FT_HAIR,
    SHADER_FT_CLOTH,
    SHADER_FT_TIGHTS,
    SHADER_FT_SKY,
    //SHADER_FT_EYEBALL,
    //SHADER_FT_EYELENS,
    SHADER_FT_GLASEYE,
    //SHADER_FT_MEMBRAN,
    //SHADER_FT_SHDMAP,
    //SHADER_FT_ESM,
    SHADER_FT_ESMGAUSS,
    SHADER_FT_ESMFILT,
    SHADER_FT_LITPROJ,
    SHADER_FT_SIMPLE,
    SHADER_FT_SIL,
    SHADER_FT_LAMBERT,
    SHADER_FT_CONSTANT,
    //SHADER_FT_PEEL,
    SHADER_FT_TONEMAP,
    SHADER_FT_REDUCE,
    SHADER_FT_MAGNIFY,
    SHADER_FT_MLAA,
    SHADER_FT_CONTOUR,
    SHADER_FT_CONTOUR_NPR,
    SHADER_FT_EXPOSURE,
    SHADER_FT_GAUSS,
    SHADER_FT_SUN,
    SHADER_FT_SUN_NO_TEXTURED,
    //SHADER_FT_FADE,
    SHADER_FT_WATER01,
    //SHADER_FT_WATER02,
    //SHADER_FT_WATRING,
    SHADER_FT_W_PTCL,
    SHADER_FT_SNOW_PT,
    SHADER_FT_LEAF_PT,
    SHADER_FT_STAR,
    //SHADER_FT_SNORING,
    //SHADER_FT_SN_FOOT,
    //SHADER_FT_SN_TSL,
    //SHADER_FT_SN_NRM,
    SHADER_FT_FLOOR,
    SHADER_FT_PUDDLE,
    SHADER_FT_S_REFL,
    SHADER_FT_S_REFR,
    SHADER_FT_RIPEMIT,
    SHADER_FT_RAIN,
    //SHADER_FT_VOLLIT,
    //SHADER_FT_FENCE,
    SHADER_FT_RIPPLE,
    SHADER_FT_FOGPTCL,
    SHADER_FT_PARTICL,
    SHADER_FT_GLITTER_PT,
    //SHADER_FT_SHOW_VEC,
    SHADER_FT_FONT,
    SHADER_FT_MOVIE,
    SHADER_FT_IMGFILT,
    SHADER_FT_SPRITE,
    SHADER_FT_END,
    SHADER_FT_MAX,
};

extern const shader_table shader_ft_table[];
extern const size_t shader_ft_table_size;
extern const shader_bind_func shader_ft_bind_func_table[];
extern const size_t shader_ft_bind_func_table_size;

extern shader_set_data shaders_ft;

extern int32_t shader_ft_get_index_by_name(const char* name);
