/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "shader_ft.hpp"
#include "../KKdLib/str_utils.hpp"
#include "gl_state.hpp"
#include "render_context.hpp"
#include "uniform.hpp"

enum shader_ft_sub_enum {
    SHADER_FT_SUB_SHADER_FFP = 0,
    SHADER_FT_SUB_BLINN_VERT,
    SHADER_FT_SUB_BLINN_FRAG,
    SHADER_FT_SUB_ITEM_BLINN,
    SHADER_FT_SUB_STAGE_BLINN,
    SHADER_FT_SUB_SKIN_DEFAULT,
    SHADER_FT_SUB_SSS_SKIN,
    //SHADER_FT_SUB_SSS_FILTER,
    SHADER_FT_SUB_SSS_FILTER_MIN,
    SHADER_FT_SUB_SSS_FILTER_MIN_NPR,
    SHADER_FT_SUB_SSS_FILTER_GAUSS_2D,
    SHADER_FT_SUB_HAIR_DEFAULT,
    //SHADER_FT_SUB_HAIR_ANISO,
    SHADER_FT_SUB_HAIR_NPR1,
    SHADER_FT_SUB_CLOTH_DEFAULT,
    SHADER_FT_SUB_CLOTH_ANISO,
    SHADER_FT_SUB_CLOTH_NPR1,
    SHADER_FT_SUB_TIGHTS,
    SHADER_FT_SUB_SKY_DEFAULT,
    //SHADER_FT_SUB_EYE_BALL,
    //SHADER_FT_SUB_EYE_LENS,
    SHADER_FT_SUB_GLASS_EYE,
    //SHADER_FT_SUB_MEMBRANE,
    //SHADER_FT_SUB_SHADOWMAP,
    //SHADER_FT_SUB_ESM,
    SHADER_FT_SUB_ESM_GAUSS,
    //SHADER_FT_SUB_ESM_FILTER,
    SHADER_FT_SUB_ESM_FILTER_MIN,
    SHADER_FT_SUB_ESM_FILTER_EROSION,
    SHADER_FT_SUB_LIT_PROJ,
    SHADER_FT_SUB_SIMPLE,
    SHADER_FT_SUB_SILHOUETTE,
    SHADER_FT_SUB_LAMBERT,
    SHADER_FT_SUB_CONSTANT,
    //SHADER_FT_SUB_PEEL,
    SHADER_FT_SUB_TONEMAP,
    SHADER_FT_SUB_TONEMAP_NPR1,
    //SHADER_FT_SUB_REDUCE_TEX,
    SHADER_FT_SUB_GHOST,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_2,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_2_ALPHAMASK,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_4,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_4_EXTRACT,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_2,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_BLUR,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_4,
    //SHADER_FT_SUB_MAGNIFY,
    SHADER_FT_SUB_MAGNIFY_CONE,
    SHADER_FT_SUB_MAGNIFY_CONE2,
    SHADER_FT_SUB_MAGNIFY_DIFF,
    SHADER_FT_SUB_MAGNIFY_DIFF2,
    SHADER_FT_SUB_MAGNIFY_LINEAR,
    //SHADER_FT_SUB_MLAA,
    SHADER_FT_SUB_MLAA_EDGE,
    SHADER_FT_SUB_MLAA_AREA,
    SHADER_FT_SUB_MLAA_BLEND,
    SHADER_FT_SUB_CONTOUR,
    SHADER_FT_SUB_CONTOUR_NPR,
    //SHADER_FT_SUB_EXPOSURE,
    SHADER_FT_SUB_EXPOSURE_MINIFY,
    SHADER_FT_SUB_EXPOSURE_MEASURE,
    SHADER_FT_SUB_EXPOSURE_AVERAGE,
    //SHADER_FT_SUB_PP_GAUSS,
    SHADER_FT_SUB_PP_GAUSS_USUAL,
    SHADER_FT_SUB_PP_GAUSS_CONE,
    SHADER_FT_SUB_SUN,
    SHADER_FT_SUB_SUN_NO_TEXTURED,
    //SHADER_FT_SUB_FADE,
    SHADER_FT_SUB_WATER01,
    //SHADER_FT_SUB_WATER02,
    //SHADER_FT_SUB_WATER_RING,
    SHADER_FT_SUB_WATER_PARTICLE,
    //SHADER_FT_SUB_SNOW_PARTICLE,
    SHADER_FT_SUB_SNOW_PARTICLE,
    SHADER_FT_SUB_SNOW_PARTICLE_CPU,
    SHADER_FT_SUB_LEAF_PARTICLE,
    SHADER_FT_SUB_STAR,
    SHADER_FT_SUB_STAR_MILKY_WAY,
    //SHADER_FT_SUB_SNOW_RING,
    //SHADER_FT_SUB_SNOW_FOOTPRINT,
    //SHADER_FT_SUB_SNOW_TEX_SPACE_LIGHT,
    //SHADER_FT_SUB_SNOW_CALC_NORMAL,
    SHADER_FT_SUB_FLOOR,
    SHADER_FT_SUB_PUDDLE,
    SHADER_FT_SUB_SIMPLE_REFLECT,
    SHADER_FT_SUB_SIMPLE_REFRACT,
    SHADER_FT_SUB_RIPPLE_EMIT,
    SHADER_FT_SUB_RAIN,
    //SHADER_FT_SUB_VOLUME_LIGHT,
    //SHADER_FT_SUB_FENCE_ALPHA,
    SHADER_FT_SUB_RIPPLE,
    SHADER_FT_SUB_FOG_PTCL,
    SHADER_FT_SUB_PARTICLE,
    SHADER_FT_SUB_GLITTER_PARTICLE,
    //SHADER_FT_SUB_SHOW_VECTOR,
    SHADER_FT_SUB_FONT,
    SHADER_FT_SUB_MOVIE,
    //SHADER_FT_SUB_IMGFILTER,
    SHADER_FT_SUB_BOX4,
    SHADER_FT_SUB_BOX8,
    SHADER_FT_SUB_COPY,
    SHADER_FT_SUB_SPRITE,
    SHADER_FT_SUB_SHADER_END,
};

static const int32_t blinn_vert_vpt_unival_max[] = {
    //1, 1, 0, 1, 0, 0, 0, 3, 1, 1, 0, 0, // 11th added
    1, 0, 1, 0, 0, 0, 3, 1, 1, 0, 0, // 0th removed, 11th added
};

static const int32_t blinn_vert_fpt_unival_max[] = {
    //0, 0, 2, 1, 0, 1, 1, 3, 0, 0, 1, 1, // 11th added
    0, 2, 1, 0, 1, 1, 3, 0, 0, 1, 1, // 0th removed, 11th added
};

static const int32_t blinn_frag_vpt_unival_max[] = {
    //1, 1, 0, 1, 0, 0, 0, 3, 1, 1, 0, 0, // 11th added
    1, 0, 1, 0, 0, 0, 3, 1, 1, 0, 0, // 0th removed, 11th added
};

static const int32_t blinn_frag_fpt_unival_max[] = {
    //0, 0, 2, 1, 1, 1, 1, 3, 0, 0, 1, 1, // 11th added
    0, 2, 1, 1, 1, 1, 3, 0, 0, 1, 1, // 0th removed, 11th added
};

static const int32_t item_blinn_vpt_unival_max[] = {
    //1, 1, 1, 0, 0, 1, 0, 0, 0, 2, 0, 1, 0, 0,
    1, 1, 0, 0, 1, 0, 0, 0, 2, 0, 1, 0, 0, // 0th removed
};

static const int32_t item_blinn_fpt_unival_max[] = {
    //1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,
    0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, // 0th removed
};

static const int32_t stage_blinn_vpt_unival_max[] = {
    //1, 1, 1, 1, 0, 0, 1, 0, 3, 0, 0, 0, // 11th added
    1, 1, 1, 0, 0, 1, 0, 3, 0, 0, 0, // 0th removed, 11th added
};

static const int32_t stage_blinn_fpt_unival_max[] = {
    //0, 0, 0, 0, 2, 1, 1, 1, 3, 1, 1, 1, // 11th added
    0, 0, 0, 2, 1, 1, 1, 3, 1, 1, 1, // 0th removed, 11th added
};

static const int32_t skin_default_vpt_unival_max[] = {
    //1, 1, 1, 0, 2, 0, 1, 0, 0, 0,
    1, 1, 0, 2, 0, 1, 0, 0, 0, // 0th removed
};

static const int32_t skin_default_fpt_unival_max[] = {
    //1, 0, 1, 1, 0, 1, 0, 1, 1, 1,
    0, 1, 1, 0, 1, 0, 1, 1, 1, // 0th removed
};

static const int32_t sss_skin_vpt_unival_max[] = {
    //1, 1, 0, 0, 0, 1, 0, 0,
    1, 0, 0, 0, 1, 0, 0, // 0th removed
};

static const int32_t sss_skin_fpt_unival_max[] = {
    //1, 0, 1, 1, 1, 0, 1, 1,
    0, 1, 1, 1, 0, 1, 1, // 0th removed
};

/*static const int32_t sss_filter_vpt_unival_max[] = {
    //0, 3, 0,
    3, 0, // 0th removed
};

static const int32_t sss_filter_fpt_unival_max[] = {
    //1, 3, 1,
    3, 1, // 0th removed
};*/

static const int32_t sss_filter_min_vpt_unival_max[] = {
    //0, 0, 0,
    0, 0, // 0th removed
};

static const int32_t sss_filter_min_fpt_unival_max[] = {
    //0, 0, 0,
    0, 0, // 0th removed
};

static const int32_t sss_filter_min_npr_vpt_unival_max[] = {
    //0, 0, 0,
    0, 0, // 0th removed
};

static const int32_t sss_filter_min_npr_fpt_unival_max[] = {
    //0, 0, 0,
    0, 0, // 0th removed
};

static const int32_t sss_filter_gauss_2d_vpt_unival_max[] = {
    //0, 0, 0,
    0, 0, // 0th removed
};

static const int32_t sss_filter_gauss_2d_fpt_unival_max[] = {
    //1, 0, 0,
    0, 0, // 0th removed
};

/*static const int32_t hair_default_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 14th added
    1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 0th removed, 14th added
};

static const int32_t hair_default_fpt_unival_max[] = {
    //1, 0, 2, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, // 14th added
    0, 2, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, // 0th removed, 14th added
};

static const int32_t hair_aniso_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 14th added
    1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 0th removed, 14th added
};

static const int32_t hair_aniso_fpt_unival_max[] = {
    //1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, // 14th added
    0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, // 0th removed, 14th added
};*/

static const int32_t hair_default_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 14th added
    1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 0th removed, 14th added
};

static const int32_t hair_default_fpt_unival_max[] = {
    //1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, // 14th added
    0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, // 0th removed, 14th added
};

static const int32_t hair_npr1_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 14th added
    1, 0, 0, 1, 0, 0, 3, 0, 2, 1, 0, 0, 0, 0, // 0th removed, 14th added
};

static const int32_t hair_npr1_fpt_unival_max[] = {
    //1, 0, 0, 0, 0, 1, 0, 3, 1, 0, 0, 0, 0, 0, 1, // 14th added
    0, 0, 0, 0, 1, 0, 3, 1, 0, 0, 0, 0, 0, 1, // 0th removed, 14th added
};

static const int32_t cloth_default_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, // 0th removed
};

static const int32_t cloth_default_fpt_unival_max[] = {
    //1, 0, 2, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1,
    0, 2, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, // 0th removed
};

static const int32_t cloth_aniso_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, // 0th removed
};

static const int32_t cloth_aniso_fpt_unival_max[] = {
    //1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 3, 0, 0, 1, 1,
    0, 0, 1, 0, 1, 1, 1, 0, 1, 3, 0, 0, 1, 1, // 0th removed
};

static const int32_t cloth_npr1_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, // 0th removed
};

static const int32_t cloth_npr1_fpt_unival_max[] = {
    //1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, // 0th removed
};

static const int32_t tights_vpt_unival_max[] = {
    //1, 1, 0, 0, 0, 2, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 2, 0, 1, 0, 0, 0, // 0th removed
};

static const int32_t tights_fpt_unival_max[] = {
    //1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
    1, 1, 1, 0, 1, 0, 1, 1, 1, // 0th removed
};

static const int32_t sky_default_vpt_unival_max[] = {
    0, 0, 0, 1, 1, 3, 0, // 6th added
};

static const int32_t sky_default_fpt_unival_max[] = {
    3, 3, 2, 0, 0, 3, 1, // 6th added
};

/*static const int32_t eye_ball_vpt_unival_max[] = {
    1, 0,
};

static const int32_t eye_ball_fpt_unival_max[] = {
    0, 1,
};*/

/*static const int32_t eye_lens_vpt_unival_max[] = {
    1, 0, 0,
};

static const int32_t eye_lens_fpt_unival_max[] = {
    0, 1, 1,
};*/

static const int32_t glass_eye_vpt_unival_max[] = {
    //1, 1, 0, 2, 0, 1, 0, 0,
    1, 0, 2, 0, 1, 0, 0, // 0th removed
};

static const int32_t glass_eye_fpt_unival_max[] = {
    //1, 1, 1, 0, 1, 0, 1, 1,
    1, 1, 0, 1, 0, 1, 1, // 0th removed
};

/*static const int32_t membrane_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t membrane_fpt_unival_max[] = {
    0, 1, 3,
};*/

/*static const int32_t shadowmap_vpt_unival_max[] = {
    1, 2, 1, 0, 0,
};

static const int32_t shadowmap_fpt_unival_max[] = {
    0, 2, 1, 1, 5,
};*/

/*static const int32_t esm_vpt_unival_max[] = {
    //1, 1, 2, 1,
    1, 2, 1, // 0th removed
};

static const int32_t esm_fpt_unival_max[] = {
    //0, 0, 2, 1,
    0, 2, 1, // 0th removed
};*/

static const int32_t esm_gauss_vpt_unival_max[] = {
    0,
};

static const int32_t esm_gauss_fpt_unival_max[] = {
    1,
};

/*static const int32_t esm_filter_vpt_unival_max[] = {
    0,
};

static const int32_t esm_filter_fpt_unival_max[] = {
    1,
};*/

static const int32_t esm_filter_min_vpt_unival_max[] = {
    0,
};

static const int32_t esm_filter_min_fpt_unival_max[] = {
    0,
};

static const int32_t esm_filter_erosion_vpt_unival_max[] = {
    0,
};

static const int32_t esm_filter_erosion_fpt_unival_max[] = {
    0,
};

static const int32_t lit_proj_vpt_unival_max[] = {
    //1, 1, 0, 1, 0, 0,
    1, 0, 1, 0, 0, // 0th removed
};

static const int32_t lit_proj_fpt_unival_max[] = {
    //0, 0, 1, 1, 2, 1,
    0, 1, 1, 2, 1, // 0th removed
};

static const int32_t simple_vpt_unival_max[] = {
    //1, 1, 0, 0, 1, 0,
    1, 0, // 0th removed, 3rd removed, 4th removed, 5th removed
};

static const int32_t simple_fpt_unival_max[] = {
    //0, 0, 2, 0, 0, 3,
    0, 2, // 0th removed, 3rd removed, 4th removed, 5th removed
};

static const int32_t silhouette_vpt_unival_max[] = {
    //1, 1, 1, 2, 1, 1, 0, 0, // 7th added
    1, 1, 2, 1, 1, 0, 0, // 0th removed, 7th added
};

static const int32_t silhouette_fpt_unival_max[] = {
    //0, 0, 1, 0, 0, 0, 1, 1, // 7th added
    0, 1, 0, 0, 0, 1, 1, // 0th removed, 7th added
};

static const int32_t lambert_vpt_unival_max[] = {
    //1, 1, 0, 1, 3, 1, 1, 0, 0, // 8th added
    1, 0, 1, 3, 1, 1, 0, 0, // 0th removed, 8th added
};

static const int32_t lambert_fpt_unival_max[] = {
    //0, 0, 2, 1, 3, 0, 0, 1, 1, // 8th added
    0, 2, 1, 3, 0, 0, 1, 1, // 0th removed, 8th added
};

static const int32_t constant_vpt_unival_max[] = {
    //1, 1, 0, 3, 1, 1, 0, // 6th added
    1, 0, 3, 1, 1, 0, // 0th removed, 6th added
};

static const int32_t constant_fpt_unival_max[] = {
    //0, 0, 2, 3, 0, 0, 1, // 6th added
    0, 2, 3, 0, 0, 1, // 0th removed, 6th added
};

/*static const int32_t peel_vpt_unival_max[] = {
    0, 1,
};

static const int32_t peel_fpt_unival_max[] = {
    1, 0,
};*/

static const int32_t tone_map_vpt_unival_max[] = {
    //0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, // 0th, 7th removed
};

static const int32_t tone_map_fpt_unival_max[] = {
    //0, 2, 2, 1, 1, 1, 0, 0,
    2, 2, 1, 1, 1, 0, // 0th, 7th removed
};

static const int32_t tone_map_npr1_vpt_unival_max[] = {
    //0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, // 0th, 7th removed
};

static const int32_t tone_map_npr1_fpt_unival_max[] = {
    //1, 0, 0, 1, 1, 0, 0, 1,
    0, 0, 1, 1, 0, 0, // 0th, 7th removed
};

/*static const int32_t reduce_tex_vpt_unival_max[] = {
    8, 1,
};

static const int32_t reduce_tex_fpt_unival_max[] = {
    8, 1,
};*/

static const int32_t reduce_tex_reduce_2_vpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_2_fpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_2_alphamask_vpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_2_alphamask_fpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_4_vpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_4_fpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_4_extract_vpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_4_extract_fpt_unival_max[] = {
    0, 0,
};

static const int32_t ghost_vpt_unival_max[] = {
    0, 0,
};

static const int32_t ghost_fpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_composite_2_vpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_composite_2_fpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_composite_blur_vpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_composite_blur_fpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_composite_4_vpt_unival_max[] = {
    0, 0,
};

static const int32_t reduce_tex_reduce_composite_4_fpt_unival_max[] = {
    0, 0,
};

/*static const int32_t magnify_vpt_unival_max[] = {
    7,
};

static const int32_t magnify_fpt_unival_max[] = {
    7,
};*/

static const int32_t magnify_linear_vpt_unival_max[] = {
    0,
};

static const int32_t magnify_linear_fpt_unival_max[] = {
    0,
};

static const int32_t magnify_diff_vpt_unival_max[] = {
    0,
};

static const int32_t magnify_diff_fpt_unival_max[] = {
    0,
};

static const int32_t magnify_diff2_vpt_unival_max[] = {
    0,
};

static const int32_t magnify_diff2_fpt_unival_max[] = {
    0,
};

static const int32_t magnify_cone_vpt_unival_max[] = {
    0,
};

static const int32_t magnify_cone_fpt_unival_max[] = {
    0,
};

static const int32_t magnify_cone2_vpt_unival_max[] = {
    0,
};

static const int32_t magnify_cone2_fpt_unival_max[] = {
    0,
};

/*static const int32_t mlaa_vpt_unival_max[] = {
    //0, 0, 2, 0,
    0, 2, 0, // 0th removed
};

static const int32_t mlaa_fpt_unival_max[] = {
    //1, 1, 2, 2,
    1, 2, 2, // 0th removed
};*/

static const int32_t mlaa_edge_vpt_unival_max[] = {
    0, 0, 0,
};

static const int32_t mlaa_edge_fpt_unival_max[] = {
    1, 0, 0,
};

static const int32_t mlaa_area_vpt_unival_max[] = {
    0, 0, 0,
};

static const int32_t mlaa_area_fpt_unival_max[] = {
    0, 0, 2,
};

static const int32_t mlaa_blend_vpt_unival_max[] = {
    0, 0, 0,
};

static const int32_t mlaa_blend_fpt_unival_max[] = {
    1, 0, 0,
};

static const int32_t contour_vpt_unival_max[] = {
    //0, 1,
    1, // 0th removed
};

static const int32_t contour_fpt_unival_max[] = {
    //1, 1,
    1, // 0th removed
};

static const int32_t contour_npr_vpt_unival_max[] = {
    -1,
};

static const int32_t contour_npr_fpt_unival_max[] = {
    -1,
};

/*static const int32_t exposure_vpt_unival_max[] = {
    2,
};

static const int32_t exposure_fpt_unival_max[] = {
    2,
};*/

static const int32_t exposure_minify_vpt_unival_max[] = {
    0,
};

static const int32_t exposure_minify_fpt_unival_max[] = {
    0,
};

static const int32_t exposure_measure_vpt_unival_max[] = {
    0,
};

static const int32_t exposure_measure_fpt_unival_max[] = {
    0,
};

static const int32_t exposure_average_vpt_unival_max[] = {
    0,
};

static const int32_t exposure_average_fpt_unival_max[] = {
    0,
};

/*static const int32_t pp_gauss_vpt_unival_max[] = {
    1,
};

static const int32_t pp_gauss_fpt_unival_max[] = {
    1,
};*/

static const int32_t pp_gauss_usual_vpt_unival_max[] = {
    0,
};

static const int32_t pp_gauss_usual_fpt_unival_max[] = {
    0,
};

static const int32_t pp_gauss_cone_vpt_unival_max[] = {
    0,
};

static const int32_t pp_gauss_cone_fpt_unival_max[] = {
    0,
};

static const int32_t sun_vpt_unival_max[] = {
    -1,
};

static const int32_t sun_fpt_unival_max[] = {
    -1,
};

static const int32_t sun_no_textured_vpt_unival_max[] = {
    -1,
};

static const int32_t sun_no_textured_fpt_unival_max[] = {
    -1,
};

/*static const int32_t fade_vpt_unival_max[] = {
    5,
};

static const int32_t fade_fpt_unival_max[] = {
    5,
};*/

static const int32_t water01_vpt_unival_max[] = {
    1, 1, 0, 0, 0, 0, 0, 1,
};

static const int32_t water01_fpt_unival_max[] = {
    0, 0, 2, 1, 1, 1, 1, 1,
};

/*static const int32_t water02_vpt_unival_max[] = {
    -1,
};

static const int32_t water02_fpt_unival_max[] = {
    -1,
};*/

/*static const int32_t water_ring_vpt_unival_max[] = {
    1, 0,
};

static const int32_t water_ring_fpt_unival_max[] = {
    1, 1,
};*/

static const int32_t water_particle_vpt_unival_max[] = {
    -1,
};

static const int32_t water_particle_fpt_unival_max[] = {
    -1,
};

/*static const int32_t snow_particle_vpt_unival_max[] = {
    1,
};

static const int32_t snow_particle_fpt_unival_max[] = {
    1,
};*/

static const int32_t snow_particle_vpt_unival_max[] = {
    0,
};

static const int32_t snow_particle_fpt_unival_max[] = {
    0,
};

static const int32_t snow_particle_cpu_vpt_unival_max[] = {
    0,
};

static const int32_t snow_particle_cpu_fpt_unival_max[] = {
    0,
};

static const int32_t leaf_particle_vpt_unival_max[] = {
    -1,
};

static const int32_t leaf_particle_fpt_unival_max[] = {
    -1,
};

/*static const int32_t star_vpt_unival_max[] = {
    1,
};

static const int32_t star_fpt_unival_max[] = {
    1,
};*/

static const int32_t star_vpt_unival_max[] = {
    0,
};

static const int32_t star_fpt_unival_max[] = {
    0,
};

static const int32_t star_milky_way_vpt_unival_max[] = {
    0,
};

static const int32_t star_milky_way_fpt_unival_max[] = {
    0,
};

static const int32_t snow_ring_vpt_unival_max[] = {
    0, 0, 0, 1, 0,
};

static const int32_t snow_ring_fpt_unival_max[] = {
    2, 1, 1, 1, 1,
};

static const int32_t snow_footprint_vpt_unival_max[] = {
    -1,
};

static const int32_t snow_footprint_fpt_unival_max[] = {
    -1,
};

static const int32_t snow_tex_space_light_vpt_unival_max[] = {
    -1,
};

static const int32_t snow_tex_space_light_fpt_unival_max[] = {
    -1,
};

static const int32_t snow_calc_normal_vpt_unival_max[] = {
    -1,
};

static const int32_t snow_calc_normal_fpt_unival_max[] = {
    -1,
};

static const int32_t floor_vpt_unival_max[] = {
    0, 0, 1, 0, 3, 1, 1, 0,
};

static const int32_t floor_fpt_unival_max[] = {
    2, 1, 1, 1, 3, 0, 0, 1, // 7th added
};

static const int32_t puddle_vpt_unival_max[] = {
    0, 1, 0, 3,
};

static const int32_t puddle_fpt_unival_max[] = {
    1, 1, 1, 3,
};

static const int32_t simple_reflect_vpt_unival_max[] = {
    //1, 1, 2, 1, 1, 0, 0, 0, 0, 0,
    1, 2, 1, 1, 0, 0, 0, 0, 0, // 0th removed
};

static const int32_t simple_reflect_fpt_unival_max[] = {
    //0, 0, 2, 0, 0, 1, 1, 2, 1, 1,
    0, 2, 0, 0, 1, 1, 2, 1, 1, // 0th removed
};

static const int32_t simple_refract_vpt_unival_max[] = {
    1, 0,
};

static const int32_t simple_refract_fpt_unival_max[] = {
    0, 1,
};

static const int32_t ripple_emit_vpt_unival_max[] = {
    0, 0,
};

static const int32_t ripple_emit_fpt_unival_max[] = {
    1, 1,
};

static const int32_t rain_vpt_unival_max[] = {
    -1,
};

static const int32_t rain_fpt_unival_max[] = {
    -1,
};

/*static const int32_t volume_light_vpt_unival_max[] = {
    -1,
};

static const int32_t volume_light_fpt_unival_max[] = {
    -1,
};*/

/*static const int32_t fence_alpha_vpt_unival_max[] = {
    1, 3,
};

static const int32_t fence_alpha_fpt_unival_max[] = {
    0, 3,
};*/

static const int32_t ripple_vpt_unival_max[] = {
    0,
};

static const int32_t ripple_fpt_unival_max[] = {
    1,
};

static const int32_t fog_ptcl_vpt_unival_max[] = {
    -1,
};

static const int32_t fog_ptcl_fpt_unival_max[] = {
    -1,
};

static const int32_t particle_vpt_unival_max[] = {
    -1,
};

static const int32_t particle_fpt_unival_max[] = {
    -1,
};

static const int32_t glitter_particle_vpt_unival_max[] = {
    2, 0, 3, 0,
};

static const int32_t glitter_particle_fpt_unival_max[] = {
    2, 3, 3, 3,
};

/*static const int32_t show_vector_vpt_unival_max[] = {
    //1, 1, 3,
    1, 3, // 0th removed
};

static const int32_t show_vector_fpt_unival_max[] = {
    //0, 0, 0,
    0, 0, // 0th removed
};*/

static const int32_t font_vpt_unival_max[] = {
    //0,
    -1, // 0th removed, 1st added
};

static const int32_t font_fpt_unival_max[] = {
    //1,
    -1, // 0th removed, 1st added
};

static const int32_t movie_vpt_unival_max[] = {
    0,
};

static const int32_t movie_fpt_unival_max[] = {
    1,
};

/*static const int32_t imgfilter_vpt_unival_max[] = {
    0,
};

static const int32_t imgfilter_fpt_unival_max[] = {
    5,
};*/

static const int32_t box4_vpt_unival_max[] = {
    0,
};

static const int32_t box4_fpt_unival_max[] = {
    0,
};

static const int32_t box8_vpt_unival_max[] = {
    0,
};

static const int32_t box8_fpt_unival_max[] = {
    0,
};

static const int32_t copy_vpt_unival_max[] = {
    0,
};

static const int32_t copy_fpt_unival_max[] = {
    0,
};
static const int32_t sprite_vpt_unival_max[] = {
    0, 0, 0,
};

static const int32_t sprite_fpt_unival_max[] = {
    3, 3, 2,
};

static const shader_sub_table BLINN_table[] = {
    {
        SHADER_FT_SUB_BLINN_VERT,
        blinn_vert_vpt_unival_max,
        blinn_vert_fpt_unival_max,
        "blinn_per_vert",
        "blinn_per_vert",
    },
    {
        SHADER_FT_SUB_BLINN_FRAG,
        blinn_frag_vpt_unival_max,
        blinn_frag_fpt_unival_max,
        "blinn_per_frag",
        "blinn_per_frag",
    },
};

static const shader_sub_table ITEM_table[] = {
    {
        SHADER_FT_SUB_ITEM_BLINN,
        item_blinn_vpt_unival_max,
        item_blinn_fpt_unival_max,
        "item_blinn",
        "item_blinn",
    },
};

static const shader_sub_table STAGE_table[] = {
    {
        SHADER_FT_SUB_STAGE_BLINN,
        stage_blinn_vpt_unival_max,
        stage_blinn_fpt_unival_max,
        "stage_blinn",
        "stage_blinn",
    },
};

static const shader_sub_table SKIN_table[] = {
    {
        SHADER_FT_SUB_SKIN_DEFAULT,
        skin_default_vpt_unival_max,
        skin_default_fpt_unival_max,
        "skin_default",
        "skin_default",
    },
};

static const shader_sub_table SSS_SKIN_table[] = {
    {
        SHADER_FT_SUB_SSS_SKIN,
        sss_skin_vpt_unival_max,
        sss_skin_fpt_unival_max,
        "sss_skin",
        "sss_skin",
    },
};

/*static const shader_sub_table SSS_FILT_table[] = {
    {
        SHADER_FT_SUB_SSS_FILTER,
        sss_filter_vpt_unival_max,
        sss_filter_fpt_unival_max,
        "sss_filter",
        "sss_filter",
    },
};*/

static const shader_sub_table SSS_FILT_table[] = {
    {
        SHADER_FT_SUB_SSS_FILTER_MIN,
        sss_filter_min_vpt_unival_max,
        sss_filter_min_fpt_unival_max,
        "sss_filter_min",
        "sss_filter_min",
    },
    {
        SHADER_FT_SUB_SSS_FILTER_MIN_NPR,
        sss_filter_min_npr_vpt_unival_max,
        sss_filter_min_npr_fpt_unival_max,
        "sss_filter_min",
        "sss_filter_min_npr",
    },
    {
        SHADER_FT_SUB_SSS_FILTER_GAUSS_2D,
        sss_filter_gauss_2d_vpt_unival_max,
        sss_filter_gauss_2d_fpt_unival_max,
        "sss_filter_gauss_2d",
        "sss_filter_gauss_2d",
    },
};

/*static const shader_sub_table HAIR_table[] = {
    {
        SHADER_FT_SUB_HAIR_DEFAULT,
        hair_default_vpt_unival_max,
        hair_default_fpt_unival_max,
        "hair_default",
        "hair_default",
    },
    {
        SHADER_FT_SUB_HAIR_ANISO,
        hair_aniso_vpt_unival_max,
        hair_aniso_fpt_unival_max,
        "hair_default",
        "hair_aniso",
    },
    {
        SHADER_FT_SUB_HAIR_NPR1,
        hair_npr1_vpt_unival_max,
        hair_npr1_fpt_unival_max,
        "hair_default",
        "hair_npr1",
    },
};*/

static const shader_sub_table HAIR_table[] = {
    {
        SHADER_FT_SUB_HAIR_DEFAULT,
        hair_default_vpt_unival_max,
        hair_default_fpt_unival_max,
        "hair_default",
        "hair_default",
    },
    {
        SHADER_FT_SUB_HAIR_NPR1,
        hair_npr1_vpt_unival_max,
        hair_npr1_fpt_unival_max,
        "hair_default",
        "hair_npr1",
    },
};

static const shader_sub_table CLOTH_table[] = {
    {
        SHADER_FT_SUB_CLOTH_DEFAULT,
        cloth_default_vpt_unival_max,
        cloth_default_fpt_unival_max,
        "cloth_default",
        "cloth_default",
    },
    {
        SHADER_FT_SUB_CLOTH_ANISO,
        cloth_aniso_vpt_unival_max,
        cloth_aniso_fpt_unival_max,
        "cloth_default",
        "cloth_aniso",
    },
    {
        SHADER_FT_SUB_CLOTH_NPR1,
        cloth_npr1_vpt_unival_max,
        cloth_npr1_fpt_unival_max,
        "cloth_default",
        "cloth_npr1",
    },
};

static const shader_sub_table TIGHTS_table[] = {
    {
        SHADER_FT_SUB_TIGHTS,
        tights_vpt_unival_max,
        tights_fpt_unival_max,
        "tights",
        "tights",
    },
};

static const shader_sub_table SKY_table[] = {
    {
        SHADER_FT_SUB_SKY_DEFAULT,
        sky_default_vpt_unival_max,
        sky_default_fpt_unival_max,
        "sky_default",
        "sky_default",
    },
};

/*static const shader_sub_table EYEBALL_table[] = {
    {
        SHADER_FT_SUB_EYE_BALL,
        eye_ball_vpt_unival_max,
        eye_ball_fpt_unival_max,
        "eye_ball",
        "eye_ball",
    },
};*/

/*static const shader_sub_table EYELENS_table[] = {
    {
        SHADER_FT_SUB_EYE_LENS,
        eye_lens_vpt_unival_max,
        eye_lens_fpt_unival_max,
        "eye_lens",
        "eye_lens",
    },
};*/

static const shader_sub_table GLASEYE_table[] = {
    {
        SHADER_FT_SUB_GLASS_EYE,
        glass_eye_vpt_unival_max,
        glass_eye_fpt_unival_max,
        "glass_eye",
        "glass_eye",
    },
};

/*static const shader_sub_table MEMBRAN_table[] = {
    {
        SHADER_FT_SUB_MEMBRANE,
        membrane_vpt_unival_max,
        membrane_fpt_unival_max,
        "membrane",
        "membrane",
    },
};*/

/*static const shader_sub_table SHDMAP_table[] = {
    {
        SHADER_FT_SUB_SHADOWMAP,
        shadowmap_vpt_unival_max,
        shadowmap_fpt_unival_max,
        "shadowmap",
        "shadowmap",
    },
};*/

/*static const shader_sub_table ESM_table[] = {
    {
        SHADER_FT_SUB_ESM,
        esm_vpt_unival_max,
        esm_fpt_unival_max,
        "esm",
        "esm",
    },
};*/

static const shader_sub_table ESMGAUSS_table[] = {
    {
        SHADER_FT_SUB_ESM_GAUSS,
        esm_gauss_vpt_unival_max,
        esm_gauss_fpt_unival_max,
        "esm_gauss",
        "esm_gauss",
    },
};

/*static const shader_sub_table ESMFILT_table[] = {
    {
        SHADER_FT_SUB_ESM_FILTER,
        esm_filter_vpt_unival_max,
        esm_filter_fpt_unival_max,
        "esm_filter",
        "esm_filter",
    },
};*/

static const shader_sub_table ESMFILT_table[] = {
    {
        SHADER_FT_SUB_ESM_FILTER_MIN,
        esm_filter_min_vpt_unival_max,
        esm_filter_min_fpt_unival_max,
        "esm_filter_min",
        "esm_filter_min",
    },
    {
        SHADER_FT_SUB_ESM_FILTER_EROSION,
        esm_filter_erosion_vpt_unival_max,
        esm_filter_erosion_fpt_unival_max,
        "esm_filter_erosion",
        "esm_filter_erosion",
    },
};

static const shader_sub_table LITPROJ_table[] = {
    {
        SHADER_FT_SUB_LIT_PROJ,
        lit_proj_vpt_unival_max,
        lit_proj_fpt_unival_max,
        "lit_proj",
        "lit_proj",
    },
};

static const shader_sub_table SIMPLE_table[] = {
    {
        SHADER_FT_SUB_SIMPLE,
        simple_vpt_unival_max,
        simple_fpt_unival_max,
        "simple",
        "simple",
    },
};

static const shader_sub_table SIL_table[] = {
    {
        SHADER_FT_SUB_SILHOUETTE,
        silhouette_vpt_unival_max,
        silhouette_fpt_unival_max,
        "silhouette",
        "silhouette",
    },
};

static const shader_sub_table LAMBERT_table[] = {
    {
        SHADER_FT_SUB_LAMBERT,
        lambert_vpt_unival_max,
        lambert_fpt_unival_max,
        "lambert",
        "lambert",
    },
};

static const shader_sub_table CONSTANT_table[] = {
    {
        SHADER_FT_SUB_CONSTANT,
        constant_vpt_unival_max,
        constant_fpt_unival_max,
        "constant",
        "constant",
    },
};

/*static const shader_sub_table PEEL_table[] = {
    {
        SHADER_FT_SUB_PEEL,
        peel_vpt_unival_max,
        peel_fpt_unival_max,
        "depth_peel",
        "depth_peel",
    },
};*/

static const shader_sub_table TONEMAP_table[] = {
    {
        SHADER_FT_SUB_TONEMAP,
        tone_map_vpt_unival_max,
        tone_map_fpt_unival_max,
        "tone_map",
        "tone_map",
    },
    {
        SHADER_FT_SUB_TONEMAP_NPR1,
        tone_map_npr1_vpt_unival_max,
        tone_map_npr1_fpt_unival_max,
        "tone_map_npr1",
        "tone_map_npr1",
    },
};

/*static const shader_sub_table REDUCE_table[] = {
    {
        SHADER_FT_SUB_REDUCE_TEX,
        reduce_tex_vpt_unival_max,
        reduce_tex_fpt_unival_max,
        "reduce_tex",
        "reduce_tex",
    },
};*/

static const shader_sub_table REDUCE_table[] = {
    {
        SHADER_FT_SUB_REDUCE_TEX_REDUCE_2,
        reduce_tex_reduce_2_vpt_unival_max,
        reduce_tex_reduce_2_fpt_unival_max,
        "reduce_tex_reduce_2",
        "reduce_tex_reduce_2",
    },
    {
        SHADER_FT_SUB_REDUCE_TEX_REDUCE_2_ALPHAMASK,
        reduce_tex_reduce_2_alphamask_vpt_unival_max,
        reduce_tex_reduce_2_alphamask_fpt_unival_max,
        "reduce_tex_reduce_2_alphamask",
        "reduce_tex_reduce_2_alphamask",
    },
    {
        SHADER_FT_SUB_REDUCE_TEX_REDUCE_4,
        reduce_tex_reduce_4_vpt_unival_max,
        reduce_tex_reduce_4_fpt_unival_max,
        "reduce_tex_reduce_4",
        "reduce_tex_reduce_4",
    },
    {
        SHADER_FT_SUB_REDUCE_TEX_REDUCE_4_EXTRACT,
        reduce_tex_reduce_4_extract_vpt_unival_max,
        reduce_tex_reduce_4_extract_fpt_unival_max,
        "reduce_tex_reduce_4_extract",
        "reduce_tex_reduce_4_extract",
    },
    {
        SHADER_FT_SUB_GHOST,
        ghost_vpt_unival_max,
        ghost_fpt_unival_max,
        "ghost",
        "ghost",
    },
    {
        SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_2,
        reduce_tex_reduce_composite_2_vpt_unival_max,
        reduce_tex_reduce_composite_2_fpt_unival_max,
        "reduce_tex_reduce_composite_2",
        "reduce_tex_reduce_composite_2",
    },
    {
        SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_BLUR,
        reduce_tex_reduce_composite_blur_vpt_unival_max,
        reduce_tex_reduce_composite_blur_fpt_unival_max,
        "reduce_tex_reduce_composite_blur",
        "reduce_tex_reduce_composite_blur",
    },
    {
        SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_4,
        reduce_tex_reduce_composite_4_vpt_unival_max,
        reduce_tex_reduce_composite_4_fpt_unival_max,
        "reduce_tex_reduce_composite_4",
        "reduce_tex_reduce_composite_4",
    },
};

/*static const shader_sub_table MAGNIFY_table[] = {
    {
        SHADER_FT_SUB_MAGNIFY,
        magnify_vpt_unival_max,
        magnify_fpt_unival_max,
        "magnify",
        "magnify",
    },
};*/

static const shader_sub_table MAGNIFY_table[] = {
    {
        SHADER_FT_SUB_MAGNIFY_LINEAR,
        magnify_linear_vpt_unival_max,
        magnify_linear_fpt_unival_max,
        "magnify_linear",
        "magnify_linear",
    },
    {
        SHADER_FT_SUB_MAGNIFY_DIFF,
        magnify_diff_vpt_unival_max,
        magnify_diff_fpt_unival_max,
        "magnify_diff",
        "magnify_diff",
    },
    {
        SHADER_FT_SUB_MAGNIFY_DIFF2,
        magnify_diff2_vpt_unival_max,
        magnify_diff2_fpt_unival_max,
        "magnify_diff2",
        "magnify_diff2",
    },
    {
        SHADER_FT_SUB_MAGNIFY_CONE,
        magnify_cone_vpt_unival_max,
        magnify_cone_fpt_unival_max,
        "magnify_cone",
        "magnify_cone",
    },
    {
        SHADER_FT_SUB_MAGNIFY_CONE2,
        magnify_cone2_vpt_unival_max,
        magnify_cone2_fpt_unival_max,
        "magnify_cone2",
        "magnify_cone2",
    },
};

/*static const shader_sub_table MLAA_table[] = {
    {
        SHADER_FT_SUB_MLAA,
        mlaa_vpt_unival_max,
        mlaa_fpt_unival_max,
        "mlaa",
        "mlaa",
    },
};*/

static const shader_sub_table MLAA_table[] = {
    {
        SHADER_FT_SUB_MLAA_EDGE,
        mlaa_edge_vpt_unival_max,
        mlaa_edge_fpt_unival_max,
        "mlaa_edge",
        "mlaa_edge",
    },
    {
        SHADER_FT_SUB_MLAA_AREA,
        mlaa_area_vpt_unival_max,
        mlaa_area_fpt_unival_max,
        "mlaa_area",
        "mlaa_area",
    },
    {
        SHADER_FT_SUB_MLAA_BLEND,
        mlaa_blend_vpt_unival_max,
        mlaa_blend_fpt_unival_max,
        "mlaa_blend",
        "mlaa_blend",
    },
};

static const shader_sub_table CONTOUR_table[] = {
    {
        SHADER_FT_SUB_CONTOUR,
        contour_vpt_unival_max,
        contour_fpt_unival_max,
        "contour",
        "contour",
    },
};

static const shader_sub_table CONTOUR_NPR_table[] = {
    {
        SHADER_FT_SUB_CONTOUR_NPR,
        contour_npr_vpt_unival_max,
        contour_npr_fpt_unival_max,
        "contour_npr",
        "contour_npr",
    },
};

/*static const shader_sub_table EXPOSURE_table[] = {
    {
        SHADER_FT_SUB_EXPOSURE,
        exposure_vpt_unival_max,
        exposure_fpt_unival_max,
        "exposure",
        "exposure",
    },
};*/

static const shader_sub_table EXPOSURE_table[] = {
    {
        SHADER_FT_SUB_EXPOSURE_MINIFY,
        exposure_minify_vpt_unival_max,
        exposure_minify_fpt_unival_max,
        "exposure_minify",
        "exposure_minify",
    },
    {
        SHADER_FT_SUB_EXPOSURE_MEASURE,
        exposure_measure_vpt_unival_max,
        exposure_measure_fpt_unival_max,
        "exposure_measure",
        "exposure_measure",
    },
    {
        SHADER_FT_SUB_EXPOSURE_AVERAGE,
        exposure_average_vpt_unival_max,
        exposure_average_fpt_unival_max,
        "exposure_average",
        "exposure_average",
    },
};

/*static const shader_sub_table GAUSS_table[] = {
    {
        SHADER_FT_SUB_PP_GAUSS,
        pp_gauss_vpt_unival_max,
        pp_gauss_fpt_unival_max,
        "pp_gauss",
        "pp_gauss",
    },
};*/

static const shader_sub_table GAUSS_table[] = {
    {
        SHADER_FT_SUB_PP_GAUSS_USUAL,
        pp_gauss_usual_vpt_unival_max,
        pp_gauss_usual_fpt_unival_max,
        "pp_gauss_usual",
        "pp_gauss_usual",
    },
    {
        SHADER_FT_SUB_PP_GAUSS_CONE,
        pp_gauss_cone_vpt_unival_max,
        pp_gauss_cone_fpt_unival_max,
        "pp_gauss_cone",
        "pp_gauss_cone",
    },
};

static const shader_sub_table SUN_table[] = {
    {
        SHADER_FT_SUB_SUN,
        sun_vpt_unival_max,
        sun_fpt_unival_max,
        "sun",
        "sun",
    },
};

static const shader_sub_table SUN_NO_TEXTURED_table[] = {
    {
        SHADER_FT_SUB_SUN_NO_TEXTURED,
        sun_no_textured_vpt_unival_max,
        sun_no_textured_fpt_unival_max,
        "sun_no_textured",
        "sun_no_textured",
    },
};

/*static const shader_sub_table FADE_table[] = {
    {
        SHADER_FT_SUB_FADE,
        fade_vpt_unival_max,
        fade_fpt_unival_max,
        "fade",
        "fade",
    },
};*/

static const shader_sub_table WATER01_table[] = {
    {
        SHADER_FT_SUB_WATER01,
        water01_vpt_unival_max,
        water01_fpt_unival_max,
        "water01",
        "water01",
    },
};

/*static const shader_sub_table WATER02_table[] = {
    {
        SHADER_FT_SUB_WATER02,
        water02_vpt_unival_max,
        water02_fpt_unival_max,
        "water02",
        "water02",
    },
};*/

/*static const shader_sub_table WATRING_table[] = {
    {
        SHADER_FT_SUB_WATER_RING,
        water_ring_vpt_unival_max,
        water_ring_fpt_unival_max,
        "water_ring",
        "water_ring",
    },
};*/

static const shader_sub_table W_PTCL_table[] = {
    {
        SHADER_FT_SUB_WATER_PARTICLE,
        water_particle_vpt_unival_max,
        water_particle_fpt_unival_max,
        "water_particle",
        "water_particle",
    },
};

/*static const shader_sub_table SNOW_PT_table[] = {
    {
        SHADER_FT_SUB_SNOW_PARTICLE,
        snow_particle_vpt_unival_max,
        snow_particle_fpt_unival_max,
        "snow_particle",
        "snow_particle",
    },
};*/

static const shader_sub_table SNOW_PT_table[] = {
    {
        SHADER_FT_SUB_SNOW_PARTICLE,
        snow_particle_vpt_unival_max,
        snow_particle_fpt_unival_max,
        "snow_particle",
        "snow_particle",
    },
    {
        SHADER_FT_SUB_SNOW_PARTICLE_CPU,
        snow_particle_cpu_vpt_unival_max,
        snow_particle_cpu_fpt_unival_max,
        "snow_particle_cpu",
        "snow_particle_cpu",
    },
};

static const shader_sub_table LEAF_PT_table[] = {
    {
        SHADER_FT_SUB_LEAF_PARTICLE,
        leaf_particle_vpt_unival_max,
        leaf_particle_fpt_unival_max,
        "leaf_particle",
        "leaf_particle",
    },
};

static const shader_sub_table STAR_table[] = {
    {
        SHADER_FT_SUB_STAR,
        star_vpt_unival_max,
        star_fpt_unival_max,
        "star",
        "star",
    },
    {
        SHADER_FT_SUB_STAR_MILKY_WAY,
        star_milky_way_vpt_unival_max,
        star_milky_way_fpt_unival_max,
        "star_milky_way",
        "star_milky_way",
    },
};

/*static const shader_sub_table SNORING_table[] = {
    {
        SHADER_FT_SUB_SNOW_RING,
        snow_ring_vpt_unival_max,
        snow_ring_fpt_unival_max,
        "snow_ring",
        "snow_ring",
    },
};

static const shader_sub_table SN_FOOT_table[] = {
    {
        SHADER_FT_SUB_SNOW_FOOTPRINT,
        snow_footprint_vpt_unival_max,
        snow_footprint_fpt_unival_max,
        "snow_footprint",
        "snow_footprint",
    },
};

static const shader_sub_table SN_TSL_table[] = {
    {
        SHADER_FT_SUB_SNOW_TEX_SPACE_LIGHT,
        snow_tex_space_light_vpt_unival_max,
        snow_tex_space_light_fpt_unival_max,
        "snow_tex_space_light",
        "snow_tex_space_light",
    },
};

static const shader_sub_table SN_NRM_table[] = {
    {
        SHADER_FT_SUB_SNOW_CALC_NORMAL,
        snow_calc_normal_vpt_unival_max,
        snow_calc_normal_fpt_unival_max,
        "snow_calc_normal",
        "snow_calc_normal",
    },
};*/

static const shader_sub_table FLOOR_table[] = {
    {
        SHADER_FT_SUB_FLOOR,
        floor_vpt_unival_max,
        floor_fpt_unival_max,
        "floor",
        "floor",
    },
};

static const shader_sub_table PUDDLE_table[] = {
    {
        SHADER_FT_SUB_PUDDLE,
        puddle_vpt_unival_max,
        puddle_fpt_unival_max,
        "puddle",
        "puddle",
    },
};

static const shader_sub_table S_REFL_table[] = {
    {
        SHADER_FT_SUB_SIMPLE_REFLECT,
        simple_reflect_vpt_unival_max,
        simple_reflect_fpt_unival_max,
        "simple_reflect",
        "simple_reflect",
    },
};

static const shader_sub_table S_REFR_table[] = {
    {
        SHADER_FT_SUB_SIMPLE_REFRACT,
        simple_refract_vpt_unival_max,
        simple_refract_fpt_unival_max,
        "simple_refract",
        "simple_refract",
    },
};

static const shader_sub_table RIPEMIT_table[] = {
    {
        SHADER_FT_SUB_RIPPLE_EMIT,
        ripple_emit_vpt_unival_max,
        ripple_emit_fpt_unival_max,
        "ripple_emit",
        "ripple_emit",
    },
};

static const shader_sub_table RAIN_table[] = {
    {
        SHADER_FT_SUB_RAIN,
        rain_vpt_unival_max,
        rain_fpt_unival_max,
        "rain",
        "rain",
    },
};

/*static const shader_sub_table VOLLIT_table[] = {
    {
        SHADER_FT_SUB_VOLUME_LIGHT,
        volume_light_vpt_unival_max,
        volume_light_fpt_unival_max,
        "volume_light",
        "volume_light",
    },
};*/

/*static const shader_sub_table FENCE_table[] = {
    {
        SHADER_FT_SUB_FENCE_ALPHA,
        fence_alpha_vpt_unival_max,
        fence_alpha_fpt_unival_max,
        "fence_alpha",
        "fence_alpha",
    },
};*/

static const shader_sub_table RIPPLE_table[] = {
    {
        SHADER_FT_SUB_RIPPLE,
        ripple_vpt_unival_max,
        ripple_fpt_unival_max,
        "ripple",
        "ripple",
    },
};

static const shader_sub_table FOGPTCL_table[] = {
    {
        SHADER_FT_SUB_FOG_PTCL,
        fog_ptcl_vpt_unival_max,
        fog_ptcl_fpt_unival_max,
        "fog_ptcl",
        "fog_ptcl",
    },
};

static const shader_sub_table PARTICL_table[] = {
    {
        SHADER_FT_SUB_PARTICLE,
        particle_vpt_unival_max,
        particle_fpt_unival_max,
        "particle",
        "particle",
    },
};

static const shader_sub_table GLITTER_PT_table[] = {
    {
        SHADER_FT_SUB_GLITTER_PARTICLE,
        glitter_particle_vpt_unival_max,
        glitter_particle_fpt_unival_max,
        "glitter_particle",
        "glitter_particle",
    },
};

/*static const shader_sub_table SHOW_VEC_table[] = {
    {
        SHADER_FT_SUB_SHOW_VECTOR,
        show_vector_vpt_unival_max,
        show_vector_fpt_unival_max,
        "show_vector",
        "show_vector",
    },
};*/

static const shader_sub_table FONT_table[] = {
    {
        SHADER_FT_SUB_FONT,
        font_vpt_unival_max,
        font_fpt_unival_max,
        "font",
        "font",
    },
};

static const shader_sub_table MOVIE_table[] = {
    {
        SHADER_FT_SUB_MOVIE,
        movie_vpt_unival_max,
        movie_fpt_unival_max,
        "movie",
        "movie",
    },
};

/*static const shader_sub_table IMGFILT_table[] = {
    {
        SHADER_FT_SUB_IMGFILTER,
        imgfilter_vpt_unival_max,
        imgfilter_fpt_unival_max,
        "imgfilter",
        "imgfilter",
    },
};*/

static const shader_sub_table IMGFILT_table[] = {
    {
        SHADER_FT_SUB_BOX4,
        box4_vpt_unival_max,
        box4_fpt_unival_max,
        "box4",
        "box4",
    },
    {
        SHADER_FT_SUB_BOX8,
        box8_vpt_unival_max,
        box8_fpt_unival_max,
        "box8",
        "box8",
    },
    {
        SHADER_FT_SUB_COPY,
        copy_vpt_unival_max,
        copy_fpt_unival_max,
        "copy",
        "copy",
    },
};

static const shader_sub_table SPRITE_table[] = {
    {
        SHADER_FT_SUB_SPRITE,
        sprite_vpt_unival_max,
        sprite_fpt_unival_max,
        "sprite",
        "sprite",
    },
};

static const uniform_name BLINN_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_TEXTURE_COUNT,
    U_LIGHT_0,
    U_NORMAL,
    U_SPECULAR,
    U_ENV_MAP,
    U_FOG_HEIGHT,
    U_MORPH,
    U_MORPH_COLOR,
    U_LIGHT_1,
    U_ALPHA_TEST, // 11th added
};

static const uniform_name ITEM_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_MORPH,
    U_SPECULAR_IBL,
    U_SPECULAR,
    U_NORMAL,
    U_TEXTURE_COUNT,
    U_ENV_MAP,
    U_SELF_SHADOW,
    U_FOG,
    U_ALPHA_TEST,
    U_SHADOW,
    U_CHARA_COLOR,
    U_TONE_CURVE,
};

static const uniform_name STAGE_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_MORPH,
    U_MORPH_COLOR,
    U_TEXTURE_COUNT,
    U_ENV_MAP,
    U_LIGHT_0,
    U_NORMAL,
    U_FOG_HEIGHT,
    U_LIGHT_1,
    U12,
    U_ALPHA_TEST, // 11th added
};

static const uniform_name SKIN_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_NORMAL,
    U_SELF_SHADOW,
    U_FOG,
    U_ALPHA_TEST,
    U_SHADOW,
    U_NPR,
    U_CHARA_COLOR,
    U_TONE_CURVE,
};

static const uniform_name SSS_SKIN_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U37,
    U_ALPHA_TEST,
    U_SELF_SHADOW,
    U_SHADOW,
    U_NPR,
    U26,
};

static const uniform_name SSS_FILT_uniform[] = {
    //U16, // 0th removed
    U_SSS_FILTER,
    U_NPR,
};

static const uniform_name HAIR_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_SPECULAR_IBL,
    U_SPECULAR,
    U_NORMAL,
    U_TRANSPARENCY,
    U_TRANSLUCENCY,
    U_ANISO,
    U_SELF_SHADOW,
    U_FOG,
    U_SHADOW,
    U_NPR,
    U_CHARA_COLOR,
    U_TONE_CURVE,
    U_ALPHA_TEST, // 14th added
};

static const uniform_name CLOTH_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_SPECULAR_IBL,
    U_SPECULAR,
    U_NORMAL,
    U_TRANSPARENCY,
    U_ENV_MAP,
    U_SELF_SHADOW,
    U_FOG,
    U_ALPHA_TEST,
    U_ANISO,
    U_SHADOW,
    U_NPR,
    U_CHARA_COLOR,
    U_TONE_CURVE,
};

static const uniform_name TIGHTS_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_SPECULAR,
    U_ENV_MAP,
    U_SELF_SHADOW,
    U_FOG,
    U_ALPHA_TEST,
    U_SHADOW,
    U_NPR,
    U_CHARA_COLOR,
    U_TONE_CURVE,
};

static const uniform_name SKY_uniform[] = {
    U_TEX_0_TYPE,
    U_TEX_1_TYPE,
    U_TEXTURE_COUNT,
    U_MORPH,
    U_MORPH_COLOR,
    U_FOG_HEIGHT,
    U_ALPHA_TEST, // 6th added
};

/*static const uniform_name EYEBALL_uniform[] = {
    U_BONE_MAT,
    U_ENV_MAP,
};*/

/*static const uniform_name EYELENS_uniform[] = {
    U_BONE_MAT,
    U_SPECULAR,
    U_ENV_MAP,
};*/

static const uniform_name GLASEYE_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_SELF_SHADOW,
    U_FOG,
    U18,
    U_SHADOW,
    U_CHARA_COLOR,
    U_TONE_CURVE,
};

/*static const uniform_name MEMBRAN_uniform[] = {
    U_BONE_MAT,
    U_NORMAL,
    U_MEMBRANE,
};*/

/*static const uniform_name SHDMAP_uniform[] = {
    U_BONE_MAT,
    U_TEXTURE_COUNT,
    U_TRANSPARENCY,
    U_LIGHT_1,
    U2D,
};*/

/*static const uniform_name ESM_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_TEXTURE_COUNT,
    U_TRANSPARENCY,
};*/

static const uniform_name ESMGAUSS_uniform[] = {
    U_LIGHT_PROJ,
};

static const uniform_name ESMFILT_uniform[] = {
    U_ESM_FILTER,
};

static const uniform_name LITPROJ_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_SPECULAR,
    U_NORMAL,
    U_TEXTURE_COUNT,
    U_TRANSPARENCY,
};

static const uniform_name SIMPLE_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_TEXTURE_COUNT,
    //U_NORMAL, // 3rd removed
    //U_INSTANCE, // 4th removed
    //U2E, // 5th removed
};

static const uniform_name SIL_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_TRANSPARENCY,
    U_TEXTURE_COUNT,
    U_MORPH,
    U_MORPH_COLOR,
    U0A,
    U_ALPHA_TEST, // 7th added
};

static const uniform_name LAMBERT_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_TEXTURE_COUNT,
    U_LIGHT_0,
    U_FOG_HEIGHT,
    U_MORPH,
    U_MORPH_COLOR,
    U_LIGHT_1,
    U_ALPHA_TEST, // 8th added
};

static const uniform_name CONSTANT_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_TEXTURE_COUNT,
    U_FOG_HEIGHT,
    U_MORPH,
    U_MORPH_COLOR,
    U_ALPHA_TEST, // 6th added
};

/*static const uniform_name PEEL_uniform[] = {
    U_DEPTH_PEEL,
    U_BONE_MAT,
};*/

static const uniform_name TONEMAP_uniform[] = {
    //U16, // 0th removed
    U_TONE_MAP,
    U_FLARE,
    U_SCENE_FADE,
    U_AET_BACK,
    U_LIGHT_PROJ,
    U_NPR,
    //U25, 7th removed
};

static const uniform_name REDUCE_uniform[] = {
    U_REDUCE,
    U_ALPHA_MASK,
};

static const uniform_name MAGNIFY_uniform[] = {
    U_MAGNIFY,
};

static const uniform_name MLAA_uniform[] = {
    //U16, // 0th removed
    U_ALPHA_MASK,
    U_MLAA,
    U_MLAA_SEARCH,
};

static const uniform_name CONTOUR_uniform[] = {
    //U16, // 0th removed
    U24,
};

static const uniform_name CONTOUR_NPR_uniform[] = {
    U_INVALID,
};

static const uniform_name EXPOSURE_uniform[] = {
    U_EXPOSURE,
};

static const uniform_name GAUSS_uniform[] = {
    U_GAUSS,
};

static const uniform_name SUN_uniform[] = {
    U_INVALID,
};

static const uniform_name SUN_NO_TEXTURED_uniform[] = {
    U_INVALID,
};

/*static const uniform_name FADE_uniform[] = {
    U_FADE,
};*/

static const uniform_name WATER01_uniform[] = {
    U_MORPH,
    U_MORPH_COLOR,
    U_TEXTURE_COUNT,
    U_NORMAL,
    U_WATER_REFLECT,
    U_ENV_MAP,
    U_SPECULAR,
    U_FOG_HEIGHT,
};

/*static const uniform_name WATER02_uniform[] = {
    U_INVALID,
};*/

/*static const uniform_name WATRING_uniform[] = {
    U_LIGHT_0,
    U_LIGHT_1,
};*/

static const uniform_name W_PTCL_uniform[] = {
    U_INVALID,
};

static const uniform_name SNOW_PT_uniform[] = {
    U_SNOW_PARTICLE,
};

static const uniform_name LEAF_PT_uniform[] = {
    U_INVALID,
};

static const uniform_name STAR_uniform[] = {
    U_STAR,
};

/*static const uniform_name SNORING_uniform[] = {
    U_TEXTURE_COUNT,
    U_NORMAL,
    U_SPECULAR,
    U_LIGHT_0,
    U_LIGHT_1,
};

static const uniform_name SN_FOOT_uniform[] = {
    U_INVALID,
};

static const uniform_name SN_TSL_uniform[] = {
    U_INVALID,
};

static const uniform_name SN_NRM_uniform[] = {
    U_INVALID,
};*/

static const uniform_name FLOOR_uniform[] = {
    U_TEXTURE_COUNT,
    U_NORMAL,
    U_LIGHT_0,
    U_LIGHT_1,
    U_FOG_HEIGHT,
    U_MORPH,
    U_MORPH_COLOR,
    U_ALPHA_TEST, // 7th added
};

static const uniform_name PUDDLE_uniform[] = {
    U_NORMAL,
    U_LIGHT_0,
    U_LIGHT_1,
    U_FOG_HEIGHT,
};

static const uniform_name S_REFL_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_REFLECT,
    U_MORPH,
    U_MORPH_COLOR,
    U_ALPHA_TEST,
    U_CLIP_PLANE,
    U_TEXTURE_COUNT,
    U_TONE_CURVE,
    U_CHARA_COLOR,
};

static const uniform_name S_REFR_uniform[] = {
    U_BONE_MAT,
    U_TRANSPARENCY,
};

static const uniform_name RIPEMIT_uniform[] = {
    U_RIPPLE,
    U_RIPPLE_EMIT,
};

static const uniform_name RAIN_uniform[] = {
    U_INVALID,
};

/*static const uniform_name VOLLIT_uniform[] = {
    U_INVALID,
};*/

/*static const uniform_name FENCE_uniform[] = {
    U_INSTANCE,
    U_FOG_HEIGHT,
};*/

static const uniform_name RIPPLE_uniform[] = {
    U_RIPPLE,
};

static const uniform_name FOGPTCL_uniform[] = {
    U_INVALID,
};

static const uniform_name PARTICL_uniform[] = {
    U_INVALID,
};

static const uniform_name GLITTER_PT_uniform[] = {
    U_TEXTURE_COUNT,
    U_TEXTURE_BLEND,
    U_FOG_HEIGHT,
    U_ALPHA_BLEND,
};

/*static const uniform_name SHOW_VEC_uniform[] = {
    //U16, // 0th removed
    U_BONE_MAT,
    U_SHOW_VECTOR,
};*/

static const uniform_name FONT_uniform[] = {
    //U16, // 0th removed
    U_INVALID, // 0th removed, 1st added
};

static const uniform_name MOVIE_uniform[] = {
    U_MOVIE,
};

static const uniform_name IMGFILT_uniform[] = {
    U_IMAGE_FILTER,
};

static const uniform_name SPRITE_uniform[] = {
    U_TEX_0_TYPE,
    U_TEX_1_TYPE,
    U_COMBINER,
};

static const bool BLINN_permut[] = {
    // true, // 0th removed
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    true,
    true,
    false,
    false, // 11th added
};

static const bool ITEM_permut[] = {
    //true, // 0th removed
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
};

static const bool STAGE_permut[] = {
    //true, // 0th removed
    true,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false, // 11th added
};

static const bool SKIN_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
};

static const bool SSS_SKIN_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    false,
    false,
    false,
};

static const bool SSS_FILT_permut[] = {
    //true, // 0th removed
    true,
    true,
};

static const bool HAIR_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false, // 14th added
};

static const bool CLOTH_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
};

static const bool TIGHTS_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
};

static const bool SKY_permut[] = {
    false,
    false,
    false,
    true,
    true,
    false,
    false, // 6th added
};

/*static const bool EYEBALL_permut[] = {
    true,
    false,
};*/

/*static const bool EYELENS_permut[] = {
    true,
    false,
    false,
};*/

static const bool GLASEYE_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    false,
    false,
    false,
};

/*static const bool MEMBRAN_permut[] = {
    true,
    false,
    false,
};*/

/*static const bool SHDMAP_permut[] = {
    true,
    false,
    false,
    false,
    true,
};*/

/*static const bool ESM_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
};*/

static const bool ESMGAUSS_permut[] = {
    true,
};

static const bool ESMFILT_permut[] = {
    true,
};

static const bool LITPROJ_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    false,
};

static const bool SIMPLE_permut[] = {
    //true, // 0th removed
    true,
    false,
    //false, // 3rd removed
    //false, // 4th removed
    //false, // 5th removed
};

static const bool SIL_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    true,
    false,
    false,
    false, // 7th added
};

static const bool LAMBERT_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    false,
    true,
    true,
    false,
    false, // 8th added
};

static const bool CONSTANT_permut[] = {
    //true, // 0th removed
    true,
    false,
    false,
    true,
    true,
    false, // 6th added
};

/*static const bool PEEL_permut[] = {
    false,
    true,
};*/

static const bool TONEMAP_permut[] = {
    //true, // 0th removed
    false,
    false,
    false,
    false,
    false,
    false,
    false,
};

static const bool REDUCE_permut[] = {
    true,
    true,
};

static const bool MAGNIFY_permut[] = {
    true,
};

static const bool MLAA_permut[] = {
    //true, // 0th removed
    true,
    true,
    true,
};

static const bool CONTOUR_permut[] = {
    //true, // 0th removed
    false,
};

static const bool CONTOUR_NPR_permut[] = {
    false,
};

static const bool EXPOSURE_permut[] = {
    true,
};

static const bool GAUSS_permut[] = {
    true,
};

static const bool SUN_permut[] = {
    false,
};

static const bool SUN_NO_TEXTURED_permut[] = {
    false,
};

/*static const bool FADE_permut[] = {
    false,
};*/

static const bool WATER01_permut[] = {
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
};

static const bool WATER02_permut[] = {
    false,
};

/*static const bool WATRING_permut[] = {
    false,
    false,
};*/

static const bool W_PTCL_permut[] = {
    false,
};

static const bool SNOW_PT_permut[] = {
    false,
};

static const bool LEAF_PT_permut[] = {
    false,
};

static const bool STAR_permut[] = {
    false,
};

/*static const bool SNORING_permut[] = {
    false,
    false,
    false,
    false,
    false,
};

static const bool SN_FOOT_permut[] = {
    false,
};

static const bool SN_TSL_permut[] = {
    false,
};

static const bool SN_NRM_permut[] = {
    false,
};*/

static const bool FLOOR_permut[] = {
    false,
    false,
    false,
    false,
    false,
    true,
    true,
    false, // 7th added
};

static const bool PUDDLE_permut[] = {
    false,
    false,
    false,
    false,
};

static const bool S_REFL_permut[] = {
    //true, // 0th removed
    true,
    true,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
};

static const bool S_REFR_permut[] = {
    true,
    false,
};

static const bool RIPEMIT_permut[] = {
    true,
    true,
};

static const bool RAIN_permut[] = {
    false,
};

/*static const bool VOLLIT_permut[] = {
    false,
};*/

/*static const bool FENCE_permut[] = {
    false,
    false,
};*/

static const bool RIPPLE_permut[] = {
    true,
};

static const bool FOGPTCL_permut[] = {
    false,
};

static const bool PARTICL_permut[] = {
    false,
};

static const bool GLITTER_PT_permut[] = {
    false,
    false,
    false,
    false,
};

/*static const bool SHOW_VEC_permut[] = {
    //true, // 0th removed
    false,
    false,
};*/

static const bool FONT_permut[] = {
    //true, // 0th removed
    false, // 0th removed, 1st added
};

static const bool MOVIE_permut[] = {
    true,
};

static const bool IMGFILT_permut[] = {
    false,
};

static const bool SPRITE_permut[] = {
    false,
    false,
    false,
};

struct glass_eye_struct {
    vec4 field_0;
    vec4 field_10;
    float_t field_20;
    float_t field_24;
    float_t field_28;
    float_t field_2C;
    vec3 field_30;
    vec3 field_3C;
    vec3 field_48;
    float_t field_54;
    vec3 field_58;
    float_t field_64;
    vec3 field_68;
    vec3 field_74;
    vec3 field_80;
    float_t field_8C;
    vec3 field_90;
    uint32_t frame;
    vec4 field_A0;
    bool field_B0;
    bool field_B1;
    bool field_B2;
    bool field_B3;
    int32_t field_B4;
    int32_t field_B8;
    int32_t field_BC;
};

#define shader_table_struct(n) \
{ \
    #n, \
    SHADER_FT_##n, \
    sizeof(n##_table) / sizeof(shader_sub_table), \
    n##_table, \
    sizeof(n##_uniform) / sizeof(uniform_name), \
    n##_uniform, \
    n##_permut, \
}

const shader_table shader_ft_table[] = {
    {
        "SHADER_FFP",
        SHADER_FT_FFP,
        0,
        0,
        0,
        0,
        0,
    },
    shader_table_struct(BLINN),
    shader_table_struct(ITEM),
    shader_table_struct(STAGE),
    shader_table_struct(SKIN),
    shader_table_struct(SSS_SKIN),
    shader_table_struct(SSS_FILT),
    shader_table_struct(HAIR),
    shader_table_struct(CLOTH),
    shader_table_struct(TIGHTS),
    shader_table_struct(SKY),
    //shader_table_struct(EYEBALL),
    //shader_table_struct(EYELENS),
    shader_table_struct(GLASEYE),
    //shader_table_struct(MEMBRAN),
    //shader_table_struct(SHDMAP),
    //shader_table_struct(ESM),
    shader_table_struct(ESMGAUSS),
    shader_table_struct(ESMFILT),
    shader_table_struct(LITPROJ),
    shader_table_struct(SIMPLE),
    shader_table_struct(SIL),
    shader_table_struct(LAMBERT),
    shader_table_struct(CONSTANT),
    //shader_table_struct(PEEL),
    shader_table_struct(TONEMAP),
    shader_table_struct(REDUCE),
    shader_table_struct(MAGNIFY),
    shader_table_struct(MLAA),
    shader_table_struct(CONTOUR),
    shader_table_struct(CONTOUR_NPR),
    shader_table_struct(EXPOSURE),
    shader_table_struct(GAUSS),
    shader_table_struct(SUN),
    shader_table_struct(SUN_NO_TEXTURED),
    //shader_table_struct(FADE),
    shader_table_struct(WATER01),
    //shader_table_struct(WATER02),
    //shader_table_struct(WATRING),
    shader_table_struct(W_PTCL),
    shader_table_struct(SNOW_PT),
    shader_table_struct(LEAF_PT),
    shader_table_struct(STAR),
    /*shader_table_struct(SNORING),
    shader_table_struct(SN_FOOT),
    shader_table_struct(SN_TSL),
    shader_table_struct(SN_NRM),*/
    shader_table_struct(FLOOR),
    shader_table_struct(PUDDLE),
    shader_table_struct(S_REFL),
    shader_table_struct(S_REFR),
    shader_table_struct(RIPEMIT),
    shader_table_struct(RAIN),
    //shader_table_struct(VOLLIT),
    //shader_table_struct(FENCE),
    shader_table_struct(RIPPLE),
    shader_table_struct(FOGPTCL),
    shader_table_struct(PARTICL),
    shader_table_struct(GLITTER_PT),
    //shader_table_struct(SHOW_VEC),
    shader_table_struct(FONT),
    shader_table_struct(MOVIE),
    shader_table_struct(IMGFILT),
    shader_table_struct(SPRITE),
};

#undef shader_table_struct
#undef shader_table_struct

const size_t shader_ft_table_size =
    sizeof(shader_ft_table) / sizeof(shader_table);

static void glass_eye_calc(glass_eye_struct* glass_eye);
static void glass_eye_set(glass_eye_struct* glass_eye);
static void shader_bind_blinn(shader_set_data* set, shader* shad);
static void shader_bind_cloth(shader_set_data* set, shader* shad);
static void shader_bind_hair(shader_set_data* set, shader* shad);
static void shader_bind_eye_ball(shader_set_data* set, shader* shad);
static void shader_bind_tone_map(shader_set_data* set, shader* shad);
static void shader_bind_sss_filter(shader_set_data* set, shader* shad);    // Added
static void shader_bind_esm_filter(shader_set_data* set, shader* shad);    // Added
static void shader_bind_reduce_tex(shader_set_data* set, shader* shad);    // Added
static void shader_bind_magnify(shader_set_data* set, shader* shad);       // Added
static void shader_bind_mlaa(shader_set_data* set, shader* shad);          // Added
static void shader_bind_exposure(shader_set_data* set, shader* shad);      // Added
static void shader_bind_gauss(shader_set_data* set, shader* shad);         // Added
static void shader_bind_snow_particle(shader_set_data* set, shader* shad); // Added
static void shader_bind_star(shader_set_data* set, shader* shad);          // Added
static void shader_bind_imgfilter(shader_set_data* set, shader* shad);     // Added

const shader_bind_func shader_ft_bind_func_table[] = {
    {
        SHADER_FT_BLINN,
        shader_bind_blinn,
    },
    {
        SHADER_FT_CLOTH,
        shader_bind_cloth,
    },
    {
        SHADER_FT_HAIR,
        shader_bind_hair,
    },
    {
        //SHADER_FT_EYEBALL,
        SHADER_FT_GLASEYE,
        shader_bind_eye_ball,
    },
    {
        SHADER_FT_TONEMAP,
        shader_bind_tone_map,
    },
    { // Added
        SHADER_FT_SSS_FILT,
        shader_bind_sss_filter,
    },
    { // Added
        SHADER_FT_ESMFILT,
        shader_bind_esm_filter,
    },
    { // Added
        SHADER_FT_REDUCE,
        shader_bind_reduce_tex,
    },
    { // Added
        SHADER_FT_MAGNIFY,
        shader_bind_magnify,
    },
    { // Added
        SHADER_FT_MLAA,
        shader_bind_mlaa,
    },
    { // Added
        SHADER_FT_EXPOSURE,
        shader_bind_exposure,
    },
    { // Added
        SHADER_FT_GAUSS,
        shader_bind_gauss,
    },
    { // Added
        SHADER_FT_SNOW_PT,
        shader_bind_snow_particle,
    },
    { // Added
        SHADER_FT_STAR,
        shader_bind_star,
    },
    { // Added
        SHADER_FT_IMGFILT,
        shader_bind_imgfilter,
    },
};

const size_t shader_ft_bind_func_table_size =
    sizeof(shader_ft_bind_func_table) / sizeof(shader_bind_func);

shader_set_data shaders_ft;

static glass_eye_struct glass_eye = {
    { 5.0f, 5.0f, 0.5f, 0.5f },
    { 2.5f, 2.5f, 0.5f, 0.5f },
    1.0f,
    1.45f,
    0.02f,
    0.85f,
    { 1.0f, 1.2f, 0.45f },
    { 1.0f, 1.2f, 0.25f },
    { 0.5f, 0.6f, 0.4f },
    -0.2f,
    { 1.0f, 1.0f, 1.0f },
    0.0f,
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    0.0,
    { 0.0f, 0.0f, 0.0f },
    0,
    { 0.0f, 0.0f, 0.0f, 0.0f },
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

int32_t shader_ft_get_index_by_name(const char* name) {
    if (!str_utils_compare(name, "EYEBALL"))
        return SHADER_FT_GLASEYE;
    return -1;
}

static void glass_eye_calc(glass_eye_struct* glass_eye) {
    float_t v2 = glass_eye->field_28;
    glass_eye->field_64 = glass_eye->field_2C / v2;
    glass_eye->field_90 = glass_eye->field_58;
    glass_eye->field_68 = glass_eye->field_30 * v2;
    glass_eye->field_74 = glass_eye->field_3C * v2;
    glass_eye->field_80 = glass_eye->field_48 * v2;
    glass_eye->field_8C = glass_eye->field_54 * v2;
    if (glass_eye->field_B0 == 0) {
        glass_eye->field_A0 = 0.0f;
        return;
    }

    uint32_t(FASTCALL * get_frame_counter)() = (uint32_t(FASTCALL*)())0x0000000140192E00;

    uint32_t frame = get_frame_counter();
    if (frame == glass_eye->frame)
        return;

    glass_eye->frame = frame;
    if (frame == (frame / 90) * 90) {
        float_t(FASTCALL * rand_state_array_get_float)(int32_t index) = (float_t(FASTCALL*)(int32_t index))0x00000001404FFE60;
        glass_eye->field_A0.x = (rand_state_array_get_float(0) - 0.5f) * 0.015f;
        glass_eye->field_A0.y = (rand_state_array_get_float(0) - 0.5f) * 0.015f;
    }

    float_t v17 = (float_t)(uint8_t)frame * (float_t)(M_PI * (1.0 / 128.0));
    float_t v19 = sinf(sinf(v17 * 27.0f) + v17 * 2.0f);
    float_t v20 = sinf(v17 * 23.0f);
    glass_eye->field_A0.w = 0.0f;
    glass_eye->field_A0.z = (v19 + v20) * 0.5f * 0.01f;
}

static void glass_eye_set(glass_eye_struct* glass_eye) {
    glass_eye_batch_shader_data glass_eye_batch = {};

    vec4 temp;
    *(vec3*)&temp = glass_eye->field_68 * glass_eye->field_68;
    temp.w = temp.z;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_ellipsoid_radius = temp;

    *(vec3*)&temp = glass_eye->field_68;
    temp.w = 1.0f;
    glass_eye_batch.g_ellipsoid_scale = temp;

    glass_eye_batch.g_tex_model_param = glass_eye->field_0;
    glass_eye_batch.g_tex_offset = glass_eye->field_A0;

    *(vec3*)&temp = glass_eye->field_90 * glass_eye->field_90;
    temp.w = temp.z;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_eb_radius = temp;

    glass_eye_batch.g_eb_tex_model_param = glass_eye->field_10;

    float_t v2 = (glass_eye->field_20 - glass_eye->field_24) / (glass_eye->field_20 + glass_eye->field_24);
    v2 *= v2;
    glass_eye_batch.g_fresnel = { 1.0f - v2, v2, 0.0f, 0.0f };

    float_t v3 = (glass_eye->field_20 * glass_eye->field_20) / (glass_eye->field_24 * glass_eye->field_24);
    glass_eye_batch.g_refract1 = { v3, 1.0f - v3, glass_eye->field_20 / glass_eye->field_24, 0.0f };

    float_t v4 = (glass_eye->field_24 * glass_eye->field_24) / (glass_eye->field_20 * glass_eye->field_20);
    glass_eye_batch.g_refract2 = { v4, 1.0f - v4, glass_eye->field_24 / glass_eye->field_20, 0.0f };

    *(vec3*)&temp = glass_eye->field_74 * glass_eye->field_74;
    temp.w = -1.0f;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_iris_radius = temp;

    *(vec3*)&temp = glass_eye->field_68 * glass_eye->field_68;
    temp.w = -1.0f;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_cornea_radius = temp;

    *(vec3*)&temp = glass_eye->field_80 * glass_eye->field_80;
    temp.w = -1.0f;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_pupil_radius = temp;

    *(vec2*)&temp = *(vec2*)&glass_eye->field_0 * *(vec2*)&glass_eye->field_74;
    temp.z = glass_eye->field_64 * 1.442695f;
    temp.w = glass_eye->field_8C;
    *(vec2*)&temp = vec2::rcp(*(vec2*)&temp);
    glass_eye_batch.g_tex_scale = temp;

    rctx->glass_eye_batch_ubo.WriteMemory(glass_eye_batch);
    rctx->glass_eye_batch_ubo.Bind(4);
}

static void shader_bind_blinn(shader_set_data* set, shader* shad) {
    shad->bind(set, uniform->arr[U_NORMAL]
        ? SHADER_FT_SUB_BLINN_FRAG : SHADER_FT_SUB_BLINN_VERT);
}

static void shader_bind_cloth(shader_set_data* set, shader* shad) {
    shad->bind(set, uniform->arr[U_NPR] ? SHADER_FT_SUB_CLOTH_NPR1
        : (uniform->arr[U_ANISO] ? SHADER_FT_SUB_CLOTH_ANISO : SHADER_FT_SUB_CLOTH_DEFAULT));
}

static void shader_bind_hair(shader_set_data* set, shader* shad) {
    shad->bind(set, uniform->arr[U_NPR] ? SHADER_FT_SUB_HAIR_NPR1 : SHADER_FT_SUB_HAIR_DEFAULT);
}

static void shader_bind_eye_ball(shader_set_data* set, shader* shad) {
    uniform->arr[U18] = 0;
    if (set->shaders[SHADER_FT_GLASEYE].bind(set, SHADER_FT_SUB_GLASS_EYE) >= 0) {
        glass_eye_calc(&glass_eye);
        glass_eye_set(&glass_eye);
    }
}

static void shader_bind_tone_map(shader_set_data* set, shader* shad) {
    shad->bind(set, uniform->arr[U_NPR] == 1
        ? SHADER_FT_SUB_TONEMAP_NPR1 : SHADER_FT_SUB_TONEMAP);
}

static void shader_bind_sss_filter(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_SSS_FILTER]) {
    case 0:
        shad->bind(set, uniform->arr[U_NPR]
            ? SHADER_FT_SUB_SSS_FILTER_MIN_NPR : SHADER_FT_SUB_SSS_FILTER_MIN);
        break;
    case 3:
        shad->bind(set, SHADER_FT_SUB_SSS_FILTER_GAUSS_2D);
        break;
    }
}

static void shader_bind_esm_filter(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_ESM_FILTER]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_ESM_FILTER_MIN);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_ESM_FILTER_EROSION);
        break;
    }
}

static void shader_bind_reduce_tex(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_REDUCE]) {
    case 0:
        switch (uniform->arr[U_ALPHA_MASK]) {
        case 0:
            shad->bind(set, SHADER_FT_SUB_REDUCE_TEX_REDUCE_2);
            break;
        default:
            shad->bind(set, SHADER_FT_SUB_REDUCE_TEX_REDUCE_2_ALPHAMASK);
            break;
        }
        break;
    case 1:
        switch (uniform->arr[U_ALPHA_MASK]) {
        case 0:
            shad->bind(set, SHADER_FT_SUB_REDUCE_TEX_REDUCE_4);
            break;
        }
        break;
    case 3:
        switch (uniform->arr[U_ALPHA_MASK]) {
        case 0:
            shad->bind(set, SHADER_FT_SUB_REDUCE_TEX_REDUCE_4_EXTRACT);
            break;
        }
        break;
    case 4:
        switch (uniform->arr[U_ALPHA_MASK]) {
        case 0:
            shad->bind(set, SHADER_FT_SUB_GHOST);
            break;
        }
        break;
    case 5:
        switch (uniform->arr[U_ALPHA_MASK]) {
        case 0:
            shad->bind(set, SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_2);
            break;
        }
        break;
    case 6:
        switch (uniform->arr[U_ALPHA_MASK]) {
        case 0:
            shad->bind(set, SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_BLUR);
            break;
        }
        break;
    case 7:
        switch (uniform->arr[U_ALPHA_MASK]) {
        case 0:
            shad->bind(set, SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_4);
            break;
        }
        break;
    }
}

static void shader_bind_magnify(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_MAGNIFY]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_MAGNIFY_LINEAR);
        break;
    case 2:
        shad->bind(set, SHADER_FT_SUB_MAGNIFY_DIFF);
        break;
    case 3:
        shad->bind(set, SHADER_FT_SUB_MAGNIFY_DIFF2);
        break;
    case 4:
        shad->bind(set, SHADER_FT_SUB_MAGNIFY_CONE);
        break;
    case 5:
        shad->bind(set, SHADER_FT_SUB_MAGNIFY_CONE2);
        break;
    }
}

static void shader_bind_mlaa(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_MLAA]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_MLAA_EDGE);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_MLAA_AREA);
        break;
    case 2:
        shad->bind(set, SHADER_FT_SUB_MLAA_BLEND);
        break;
    }
}

static void shader_bind_exposure(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_EXPOSURE]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_EXPOSURE_MINIFY);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_EXPOSURE_MEASURE);
        break;
    case 2:
        shad->bind(set, SHADER_FT_SUB_EXPOSURE_AVERAGE);
        break;
    }
}

static void shader_bind_gauss(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_GAUSS]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_PP_GAUSS_USUAL);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_PP_GAUSS_CONE);
        break;
    }
}

static void shader_bind_snow_particle(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_SNOW_PARTICLE]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_SNOW_PARTICLE_CPU);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_SNOW_PARTICLE);
        break;
    }
}

static void shader_bind_star(shader_set_data* set, shader* shad) { // Added
    switch (uniform->arr[U_STAR]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_STAR);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_STAR_MILKY_WAY);
        break;
    }
}

static void shader_bind_imgfilter(shader_set_data* set, shader* shad) {
    switch (uniform->arr[U_IMAGE_FILTER]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_BOX4);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_BOX8);
        break;
    case 5:
        shad->bind(set, SHADER_FT_SUB_COPY);
        break;
    }
}
