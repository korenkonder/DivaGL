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
    SHADER_FT_SUB_SSS_FILTER_MIN,
    SHADER_FT_SUB_SSS_FILTER_MIN_NPR,
    SHADER_FT_SUB_SSS_FILTER_GAUSS_2D,
    SHADER_FT_SUB_HAIR_DEFAULT,
    SHADER_FT_SUB_HAIR_NPR1,
    SHADER_FT_SUB_CLOTH_DEFAULT,
    SHADER_FT_SUB_CLOTH_ANISO,
    SHADER_FT_SUB_CLOTH_NPR1,
    SHADER_FT_SUB_TIGHTS,
    SHADER_FT_SUB_SKY_DEFAULT,
    SHADER_FT_SUB_GLASS_EYE,
    SHADER_FT_SUB_ESM_GAUSS,
    SHADER_FT_SUB_ESM_FILTER_MIN,
    SHADER_FT_SUB_ESM_FILTER_EROSION,
    SHADER_FT_SUB_LIT_PROJ,
    SHADER_FT_SUB_SIMPLE,
    SHADER_FT_SUB_SILHOUETTE,
    SHADER_FT_SUB_LAMBERT,
    SHADER_FT_SUB_CONSTANT,
    SHADER_FT_SUB_TONEMAP,
    SHADER_FT_SUB_TONEMAP_NPR1,
    SHADER_FT_SUB_GHOST,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_2,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_2_ALPHAMASK,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_4,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_4_EXTRACT,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_2,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_BLUR,
    SHADER_FT_SUB_REDUCE_TEX_REDUCE_COMPOSITE_4,
    SHADER_FT_SUB_MAGNIFY_CONE,
    SHADER_FT_SUB_MAGNIFY_CONE2,
    SHADER_FT_SUB_MAGNIFY_DIFF,
    SHADER_FT_SUB_MAGNIFY_DIFF2,
    SHADER_FT_SUB_MAGNIFY_LINEAR,
    SHADER_FT_SUB_MLAA_EDGE,
    SHADER_FT_SUB_MLAA_AREA,
    SHADER_FT_SUB_MLAA_BLEND,
    SHADER_FT_SUB_CONTOUR,
    SHADER_FT_SUB_CONTOUR_NPR,
    SHADER_FT_SUB_EXPOSURE_MINIFY,
    SHADER_FT_SUB_EXPOSURE_MEASURE,
    SHADER_FT_SUB_EXPOSURE_AVERAGE,
    SHADER_FT_SUB_PP_GAUSS_USUAL,
    SHADER_FT_SUB_PP_GAUSS_CONE,
    SHADER_FT_SUB_SUN,
    SHADER_FT_SUB_SUN_NO_TEXTURED,
    SHADER_FT_SUB_WATER01,
    SHADER_FT_SUB_WATER_PARTICLE,
    SHADER_FT_SUB_SNOW_PARTICLE,
    SHADER_FT_SUB_SNOW_PARTICLE_CPU,
    SHADER_FT_SUB_LEAF_PARTICLE,
    SHADER_FT_SUB_STAR,
    SHADER_FT_SUB_STAR_MILKY_WAY,
    SHADER_FT_SUB_FLOOR,
    SHADER_FT_SUB_PUDDLE,
    SHADER_FT_SUB_SIMPLE_REFLECT,
    SHADER_FT_SUB_SIMPLE_REFRACT,
    SHADER_FT_SUB_RIPPLE_EMIT,
    SHADER_FT_SUB_RAIN,
    SHADER_FT_SUB_RIPPLE,
    SHADER_FT_SUB_FOG_PTCL,
    SHADER_FT_SUB_PARTICLE,
    SHADER_FT_SUB_GLITTER_PARTICLE,
    SHADER_FT_SUB_FONT,
    SHADER_FT_SUB_BOX4,
    SHADER_FT_SUB_BOX8,
    SHADER_FT_SUB_COPY,
    SHADER_FT_SUB_SPRITE,
    SHADER_FT_SUB_DOF_RENDER_TILE,
    SHADER_FT_SUB_DOF_GATHER_TILE,
    SHADER_FT_SUB_DOF_DOWNSAMPLE,
    SHADER_FT_SUB_DOF_MAIN_FILTER,
    SHADER_FT_SUB_DOF_UPSAMPLE,
    SHADER_FT_SUB_TRANSPARENCY,
    SHADER_FT_SUB_SHADER_END,
};

static const int32_t blinn_per_vert_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t blinn_per_vert_fpt_unival_max[] = {
    0, 0, 1,
};

static const int32_t blinn_per_frag_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t blinn_per_frag_fpt_unival_max[] = {
    0, 0, 1,
};

static const int32_t item_blinn_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t item_blinn_fpt_unival_max[] = {
    0, 0, 1,
};

static const int32_t stage_blinn_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t stage_blinn_fpt_unival_max[] = {
    0, 0, 1,
};

static const int32_t skin_default_vpt_unival_max[] = {
    1, 0,
};

static const int32_t skin_default_fpt_unival_max[] = {
    0, 1,
};

static const int32_t sss_skin_vpt_unival_max[] = {
    1, 0,
};

static const int32_t sss_skin_fpt_unival_max[] = {
    0, 1,
};

static const int32_t sss_filter_min_vpt_unival_max[] = {
    -1,
};

static const int32_t sss_filter_min_fpt_unival_max[] = {
    -1,
};

static const int32_t sss_filter_min_npr_fpt_unival_max[] = {
    -1,
};

static const int32_t sss_filter_gauss_2d_vpt_unival_max[] = {
   -1,
};

static const int32_t sss_filter_gauss_2d_fpt_unival_max[] = {
   -1,
};

static const int32_t hair_default_vpt_unival_max[] = {
    1, 0,
};

static const int32_t hair_default_fpt_unival_max[] = {
    0, 1,
};

static const int32_t hair_npr1_fpt_unival_max[] = {
    0, 1,
};

static const int32_t cloth_default_vpt_unival_max[] = {
    1, 0,
};

static const int32_t cloth_default_fpt_unival_max[] = {
    0, 1,
};

static const int32_t cloth_aniso_fpt_unival_max[] = {
    0, 1,
};

static const int32_t cloth_npr1_fpt_unival_max[] = {
    0, 1,
};

static const int32_t tights_vpt_unival_max[] = {
    1, 0,
};

static const int32_t tights_fpt_unival_max[] = {
    0, 1,
};

static const int32_t sky_default_vpt_unival_max[] = {
    1, 0,
};

static const int32_t sky_default_fpt_unival_max[] = {
    0, 1,
};

static const int32_t glass_eye_vpt_unival_max[] = {
    1,
};

static const int32_t glass_eye_fpt_unival_max[] = {
    0,
};

static const int32_t esm_gauss_vpt_unival_max[] = {
    -1,
};

static const int32_t esm_gauss_fpt_unival_max[] = {
    -1,
};

static const int32_t esm_filter_min_vpt_unival_max[] = {
    -1,
};

static const int32_t esm_filter_min_fpt_unival_max[] = {
    -1,
};

static const int32_t esm_filter_erosion_vpt_unival_max[] = {
    -1,
};

static const int32_t esm_filter_erosion_fpt_unival_max[] = {
    -1,
};

static const int32_t lit_proj_vpt_unival_max[] = {
    1, 0,
};

static const int32_t lit_proj_fpt_unival_max[] = {
    0, 1,
};

static const int32_t simple_vpt_unival_max[] = {
    1,
};

static const int32_t simple_fpt_unival_max[] = {
    0,
};

static const int32_t silhouette_vpt_unival_max[] = {
    1, 1,
};

static const int32_t silhouette_fpt_unival_max[] = {
    0, 0,
};

static const int32_t lambert_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t lambert_fpt_unival_max[] = {
    0, 0, 1,
};

static const int32_t constant_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t constant_fpt_unival_max[] = {
    0, 0, 1,
};

static const int32_t tone_map_vpt_unival_max[] = {
    0, 1,
};

static const int32_t tone_map_fpt_unival_max[] = {
    2, 1,
};

static const int32_t tone_map_npr1_vpt_unival_max[] = {
    0, 1,
};

static const int32_t tone_map_npr1_fpt_unival_max[] = {
    0, 1,
};

static const int32_t reduce_tex_reduce_2_vpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_2_fpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_2_alphamask_vpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_2_alphamask_fpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_4_vpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_4_fpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_4_extract_vpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_4_extract_fpt_unival_max[] = {
    -1,
};

static const int32_t ghost_vpt_unival_max[] = {
    -1,
};

static const int32_t ghost_fpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_composite_2_vpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_composite_2_fpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_composite_blur_vpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_composite_blur_fpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_composite_4_vpt_unival_max[] = {
    -1,
};

static const int32_t reduce_tex_reduce_composite_4_fpt_unival_max[] = {
    -1,
};

static const int32_t magnify_linear_vpt_unival_max[] = {
    -1,
};

static const int32_t magnify_linear_fpt_unival_max[] = {
    -1,
};

static const int32_t magnify_diff_vpt_unival_max[] = {
    -1,
};

static const int32_t magnify_diff_fpt_unival_max[] = {
    -1,
};

static const int32_t magnify_diff2_vpt_unival_max[] = {
    -1,
};

static const int32_t magnify_diff2_fpt_unival_max[] = {
    -1,
};

static const int32_t magnify_cone_vpt_unival_max[] = {
    -1,
};

static const int32_t magnify_cone_fpt_unival_max[] = {
    -1,
};

static const int32_t magnify_cone2_vpt_unival_max[] = {
    -1,
};

static const int32_t magnify_cone2_fpt_unival_max[] = {
    -1,
};

static const int32_t mlaa_edge_vpt_unival_max[] = {
    0, 0,
};

static const int32_t mlaa_edge_fpt_unival_max[] = {
    1, 0,
};

static const int32_t mlaa_area_vpt_unival_max[] = {
    0, 0,
};

static const int32_t mlaa_area_fpt_unival_max[] = {
    0, 2,
};

static const int32_t mlaa_blend_vpt_unival_max[] = {
    0, 0,
};

static const int32_t mlaa_blend_fpt_unival_max[] = {
    1, 0,
};

static const int32_t contour_vpt_unival_max[] = {
    -1,
};

static const int32_t contour_fpt_unival_max[] = {
    -1,
};

static const int32_t contour_npr_vpt_unival_max[] = {
    -1,
};

static const int32_t contour_npr_fpt_unival_max[] = {
    -1,
};

static const int32_t exposure_minify_vpt_unival_max[] = {
    -1,
};

static const int32_t exposure_minify_fpt_unival_max[] = {
    -1,
};

static const int32_t exposure_measure_vpt_unival_max[] = {
    -1,
};

static const int32_t exposure_measure_fpt_unival_max[] = {
    -1,
};

static const int32_t exposure_average_vpt_unival_max[] = {
    -1,
};

static const int32_t exposure_average_fpt_unival_max[] = {
    -1,
};

static const int32_t pp_gauss_usual_vpt_unival_max[] = {
    -1,
};

static const int32_t pp_gauss_usual_fpt_unival_max[] = {
    -1,
};

static const int32_t pp_gauss_cone_vpt_unival_max[] = {
    -1,
};

static const int32_t pp_gauss_cone_fpt_unival_max[] = {
    -1,
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

static const int32_t water01_vpt_unival_max[] = {
    1,
};

static const int32_t water01_fpt_unival_max[] = {
    0,
};

static const int32_t water_particle_vpt_unival_max[] = {
    -1,
};

static const int32_t water_particle_fpt_unival_max[] = {
    -1,
};

static const int32_t snow_particle_vpt_unival_max[] = {
    -1,
};

static const int32_t snow_particle_fpt_unival_max[] = {
    -1,
};

static const int32_t snow_particle_cpu_vpt_unival_max[] = {
    -1,
};

static const int32_t snow_particle_cpu_fpt_unival_max[] = {
    -1,
};

static const int32_t leaf_particle_vpt_unival_max[] = {
    -1,
};

static const int32_t leaf_particle_fpt_unival_max[] = {
    -1,
};

static const int32_t star_vpt_unival_max[] = {
    -1,
};

static const int32_t star_fpt_unival_max[] = {
    -1,
};

static const int32_t star_milky_way_vpt_unival_max[] = {
    -1,
};

static const int32_t star_milky_way_fpt_unival_max[] = {
    -1,
};

static const int32_t floor_vpt_unival_max[] = {
    0, 0,
};

static const int32_t floor_fpt_unival_max[] = {
    1, 1,
};

static const int32_t puddle_vpt_unival_max[] = {
    -1,
};

static const int32_t puddle_fpt_unival_max[] = {
    -1,
};

static const int32_t simple_reflect_vpt_unival_max[] = {
    1, 1, 0,
};

static const int32_t simple_reflect_fpt_unival_max[] = {
    0, 0, 1,
};

static const int32_t simple_refract_vpt_unival_max[] = {
    1,
};

static const int32_t simple_refract_fpt_unival_max[] = {
    0,
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
    -1,
};

static const int32_t glitter_particle_fpt_unival_max[] = {
    -1,
};

static const int32_t font_vpt_unival_max[] = {
    -1,
};

static const int32_t font_fpt_unival_max[] = {
    -1,
};

static const int32_t box4_vpt_unival_max[] = {
    -1,
};

static const int32_t box4_fpt_unival_max[] = {
    -1,
};

static const int32_t box8_vpt_unival_max[] = {
    -1,
};

static const int32_t box8_fpt_unival_max[] = {
    -1,
};

static const int32_t copy_vpt_unival_max[] = {
    -1,
};

static const int32_t copy_fpt_unival_max[] = {
    -1,
};

static const int32_t sprite_vpt_unival_max[] = {
    0, 0, 0,
};

static const int32_t sprite_fpt_unival_max[] = {
    3, 3, 2,
};

static const int32_t dof_common_vpt_unival_max[] = {
    0,
};

static const int32_t dof_render_tile_fpt_unival_max[] = {
    1,
};

static const int32_t dof_gather_tile_fpt_unival_max[] = {
    0,
};

static const int32_t dof_downsample_fpt_unival_max[] = {
    1,
};

static const int32_t dof_main_filter_fpt_unival_max[] = {
    1,
};

static const int32_t dof_upsample_fpt_unival_max[] = {
    1,
};

static const int32_t transparency_vpt_unival_max[] = {
    -1,
};

static const int32_t transparency_fpt_unival_max[] = {
    -1,
};

#define shader_sub_table_struct(sub_index, vp, fp) \
{ \
    SHADER_FT_SUB_##sub_index, \
    vp##_vpt_unival_max, \
    fp##_fpt_unival_max, \
    #vp, \
    #fp, \
}

static const shader_sub_table BLINN_table[] = {
    shader_sub_table_struct(BLINN_VERT, blinn_per_vert, blinn_per_vert),
    shader_sub_table_struct(BLINN_FRAG, blinn_per_frag, blinn_per_frag),
};

static const shader_sub_table ITEM_table[] = {
    shader_sub_table_struct(BLINN_VERT, item_blinn, item_blinn),
};

static const shader_sub_table STAGE_table[] = {
    shader_sub_table_struct(STAGE_BLINN, stage_blinn, stage_blinn),
};

static const shader_sub_table SKIN_table[] = {
    shader_sub_table_struct(SKIN_DEFAULT, skin_default, skin_default),
};

static const shader_sub_table SSS_SKIN_table[] = {
    shader_sub_table_struct(SSS_SKIN, sss_skin, sss_skin),
};

static const shader_sub_table SSS_FILT_table[] = {
    shader_sub_table_struct(SSS_FILTER_MIN, sss_filter_min, sss_filter_min),
    shader_sub_table_struct(SSS_FILTER_MIN_NPR, sss_filter_min, sss_filter_min_npr),
    shader_sub_table_struct(SSS_FILTER_GAUSS_2D, sss_filter_gauss_2d, sss_filter_gauss_2d),
};

static const shader_sub_table HAIR_table[] = {
    shader_sub_table_struct(HAIR_DEFAULT, hair_default, hair_default),
    shader_sub_table_struct(HAIR_NPR1, hair_default, hair_npr1),
};

static const shader_sub_table CLOTH_table[] = {
    shader_sub_table_struct(CLOTH_DEFAULT, cloth_default, cloth_default),
    shader_sub_table_struct(CLOTH_ANISO, cloth_default, cloth_aniso),
    shader_sub_table_struct(CLOTH_NPR1, cloth_default, cloth_npr1),
};

static const shader_sub_table TIGHTS_table[] = {
    shader_sub_table_struct(TIGHTS, tights, tights),
};

static const shader_sub_table SKY_table[] = {
    shader_sub_table_struct(SKY_DEFAULT, sky_default, sky_default),
};

static const shader_sub_table GLASEYE_table[] = {
    shader_sub_table_struct(GLASS_EYE, glass_eye, glass_eye),
};

static const shader_sub_table ESMGAUSS_table[] = {
    shader_sub_table_struct(ESM_GAUSS, esm_gauss, esm_gauss),
};

static const shader_sub_table ESMFILT_table[] = {
    shader_sub_table_struct(ESM_FILTER_MIN, esm_filter_min, esm_filter_min),
    shader_sub_table_struct(ESM_FILTER_EROSION, esm_filter_erosion, esm_filter_erosion),
};

static const shader_sub_table LITPROJ_table[] = {
    shader_sub_table_struct(LIT_PROJ, lit_proj, lit_proj),
};

static const shader_sub_table SIMPLE_table[] = {
    shader_sub_table_struct(SIMPLE, simple, simple),
};

static const shader_sub_table SIL_table[] = {
    shader_sub_table_struct(SILHOUETTE, silhouette, silhouette),
};

static const shader_sub_table LAMBERT_table[] = {
    shader_sub_table_struct(LAMBERT, lambert, lambert),
};

static const shader_sub_table CONSTANT_table[] = {
    shader_sub_table_struct(CONSTANT, constant, constant),
};

static const shader_sub_table TONEMAP_table[] = {
    shader_sub_table_struct(TONEMAP, tone_map, tone_map),
    shader_sub_table_struct(TONEMAP_NPR1, tone_map_npr1, tone_map_npr1),
};

static const shader_sub_table REDUCE_table[] = {
    shader_sub_table_struct(REDUCE_TEX_REDUCE_2,
        reduce_tex_reduce_2, reduce_tex_reduce_2),
    shader_sub_table_struct(REDUCE_TEX_REDUCE_2_ALPHAMASK,
        reduce_tex_reduce_2_alphamask, reduce_tex_reduce_2_alphamask),
    shader_sub_table_struct(REDUCE_TEX_REDUCE_4,
        reduce_tex_reduce_4, reduce_tex_reduce_4),
    shader_sub_table_struct(REDUCE_TEX_REDUCE_4_EXTRACT,
        reduce_tex_reduce_4_extract, reduce_tex_reduce_4_extract),
    shader_sub_table_struct(GHOST, ghost, ghost),
    shader_sub_table_struct(REDUCE_TEX_REDUCE_COMPOSITE_2,
        reduce_tex_reduce_composite_2, reduce_tex_reduce_composite_2),
    shader_sub_table_struct(REDUCE_TEX_REDUCE_COMPOSITE_BLUR,
        reduce_tex_reduce_composite_blur, reduce_tex_reduce_composite_blur),
    shader_sub_table_struct(REDUCE_TEX_REDUCE_COMPOSITE_4,
        reduce_tex_reduce_composite_4, reduce_tex_reduce_composite_4),
};

static const shader_sub_table MAGNIFY_table[] = {
    shader_sub_table_struct(MAGNIFY_LINEAR, magnify_linear, magnify_linear),
    shader_sub_table_struct(MAGNIFY_DIFF, magnify_diff, magnify_diff),
    shader_sub_table_struct(MAGNIFY_DIFF2, magnify_diff2, magnify_diff2),
    shader_sub_table_struct(MAGNIFY_CONE, magnify_cone, magnify_cone),
    shader_sub_table_struct(MAGNIFY_CONE2, magnify_cone2, magnify_cone2),
};

static const shader_sub_table MLAA_table[] = {
    shader_sub_table_struct(MLAA_EDGE, mlaa_edge, mlaa_edge),
    shader_sub_table_struct(MLAA_AREA, mlaa_area, mlaa_area),
    shader_sub_table_struct(MLAA_BLEND, mlaa_blend, mlaa_blend),
};

static const shader_sub_table CONTOUR_table[] = {
    shader_sub_table_struct(CONTOUR, contour, contour),
};

static const shader_sub_table CONTOUR_NPR_table[] = {
    shader_sub_table_struct(CONTOUR_NPR, contour_npr, contour_npr),
};

static const shader_sub_table EXPOSURE_table[] = {
    shader_sub_table_struct(EXPOSURE_MINIFY, exposure_minify, exposure_minify),
    shader_sub_table_struct(EXPOSURE_MEASURE, exposure_measure, exposure_measure),
    shader_sub_table_struct(EXPOSURE_AVERAGE, exposure_average, exposure_average),
};

static const shader_sub_table GAUSS_table[] = {
    shader_sub_table_struct(PP_GAUSS_USUAL, pp_gauss_usual, pp_gauss_usual),
    shader_sub_table_struct(PP_GAUSS_CONE, pp_gauss_cone, pp_gauss_cone),
};

static const shader_sub_table SUN_table[] = {
    shader_sub_table_struct(SUN, sun, sun),
};

static const shader_sub_table SUN_NO_TEXTURED_table[] = {
    shader_sub_table_struct(SUN_NO_TEXTURED, sun_no_textured, sun_no_textured),
};

static const shader_sub_table WATER01_table[] = {
    shader_sub_table_struct(WATER01, water01, water01),
};

static const shader_sub_table W_PTCL_table[] = {
    shader_sub_table_struct(WATER_PARTICLE, water_particle, water_particle),
};

static const shader_sub_table SNOW_PT_table[] = {
    shader_sub_table_struct(SNOW_PARTICLE, snow_particle, snow_particle),
    shader_sub_table_struct(SNOW_PARTICLE_CPU, snow_particle_cpu, snow_particle_cpu),
};

static const shader_sub_table LEAF_PT_table[] = {
    shader_sub_table_struct(LEAF_PARTICLE, leaf_particle, leaf_particle),
};

static const shader_sub_table STAR_table[] = {
    shader_sub_table_struct(STAR, star, star),
    shader_sub_table_struct(STAR_MILKY_WAY, star_milky_way, star_milky_way),
};

static const shader_sub_table FLOOR_table[] = {
    shader_sub_table_struct(FLOOR, floor, floor),
};

static const shader_sub_table PUDDLE_table[] = {
    shader_sub_table_struct(PUDDLE, puddle, puddle),
};

static const shader_sub_table S_REFL_table[] = {
    shader_sub_table_struct(SIMPLE_REFLECT, simple_reflect, simple_reflect),
};

static const shader_sub_table S_REFR_table[] = {
    shader_sub_table_struct(SIMPLE_REFRACT, simple_refract, simple_refract),
};

static const shader_sub_table RIPEMIT_table[] = {
    shader_sub_table_struct(RIPPLE_EMIT, ripple_emit, ripple_emit),
};

static const shader_sub_table RAIN_table[] = {
    shader_sub_table_struct(RAIN, rain, rain),
};

static const shader_sub_table RIPPLE_table[] = {
    shader_sub_table_struct(RIPPLE, ripple, ripple),
};

static const shader_sub_table FOGPTCL_table[] = {
    shader_sub_table_struct(FOG_PTCL, fog_ptcl, fog_ptcl),
};

static const shader_sub_table PARTICL_table[] = {
    shader_sub_table_struct(PARTICLE, particle, particle),
};

static const shader_sub_table GLITTER_PT_table[] = {
    shader_sub_table_struct(GLITTER_PARTICLE, glitter_particle, glitter_particle),
};

static const shader_sub_table FONT_table[] = {
    shader_sub_table_struct(FONT, font, font),
};

static const shader_sub_table IMGFILT_table[] = {
    shader_sub_table_struct(BOX4, box4, box4),
    shader_sub_table_struct(BOX8, box8, box8),
    shader_sub_table_struct(COPY, copy, copy),
};

static const shader_sub_table SPRITE_table[] = {
    shader_sub_table_struct(SPRITE, sprite, sprite),
};

static const shader_sub_table DOF_table[] = {
    shader_sub_table_struct(DOF_RENDER_TILE, dof_common, dof_render_tile),
    shader_sub_table_struct(DOF_GATHER_TILE, dof_common, dof_gather_tile),
    shader_sub_table_struct(DOF_DOWNSAMPLE, dof_common, dof_downsample),
    shader_sub_table_struct(DOF_MAIN_FILTER, dof_common, dof_main_filter),
    shader_sub_table_struct(DOF_UPSAMPLE, dof_common, dof_upsample),
};

static const shader_sub_table TRANSPARENCY_table[] = {
    shader_sub_table_struct(TRANSPARENCY, transparency, transparency),
};

#undef shader_sub_table_struct

static const uniform_name BLINN_uniform[] = {
    U_SKINNING,
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name ITEM_uniform[] = {
    U_SKINNING,
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name STAGE_uniform[] = {
    U_SKINNING,
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name SKIN_uniform[] = {
    U_SKINNING,
    U_ALPHA_TEST,
};

static const uniform_name SSS_SKIN_uniform[] = {
    U_SKINNING,
    U_ALPHA_TEST,
};

static const uniform_name SSS_FILT_uniform[] = {
    U_INVALID,
};

static const uniform_name HAIR_uniform[] = {
    U_SKINNING,
    U_ALPHA_TEST,
};

static const uniform_name CLOTH_uniform[] = {
    U_SKINNING,
    U_ALPHA_TEST,
};

static const uniform_name TIGHTS_uniform[] = {
    U_SKINNING,
    U_ALPHA_TEST,
};

static const uniform_name SKY_uniform[] = {
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name GLASEYE_uniform[] = {
    U_SKINNING,
};

static const uniform_name ESMGAUSS_uniform[] = {
    U_INVALID,
};

static const uniform_name ESMFILT_uniform[] = {
    U_INVALID,
};

static const uniform_name LITPROJ_uniform[] = {
    U_SKINNING,
    U_ALPHA_TEST,
};

static const uniform_name SIMPLE_uniform[] = {
    U_SKINNING,
};

static const uniform_name SIL_uniform[] = {
    U_SKINNING,
    U_MORPH,
};

static const uniform_name LAMBERT_uniform[] = {
    U_SKINNING,
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name CONSTANT_uniform[] = {
    U_SKINNING,
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name TONEMAP_uniform[] = {
    U_TONE_MAP,
    U_AET_BACK,
};

static const uniform_name REDUCE_uniform[] = {
    U_INVALID,
};

static const uniform_name MAGNIFY_uniform[] = {
    U_INVALID,
};

static const uniform_name MLAA_uniform[] = {
    U_ALPHA_MASK,
    U_MLAA_SEARCH,
};

static const uniform_name CONTOUR_uniform[] = {
    U_INVALID,
};

static const uniform_name CONTOUR_NPR_uniform[] = {
    U_INVALID,
};

static const uniform_name EXPOSURE_uniform[] = {
    U_INVALID,
};

static const uniform_name GAUSS_uniform[] = {
    U_INVALID,
};

static const uniform_name SUN_uniform[] = {
    U_INVALID,
};

static const uniform_name SUN_NO_TEXTURED_uniform[] = {
    U_INVALID,
};

static const uniform_name WATER01_uniform[] = {
    U_MORPH,
};

static const uniform_name W_PTCL_uniform[] = {
    U_INVALID,
};

static const uniform_name SNOW_PT_uniform[] = {
    U_INVALID,
};

static const uniform_name LEAF_PT_uniform[] = {
    U_INVALID,
};

static const uniform_name STAR_uniform[] = {
    U_INVALID,
};

static const uniform_name FLOOR_uniform[] = {
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name PUDDLE_uniform[] = {
    U_INVALID,
};

static const uniform_name S_REFL_uniform[] = {
    U_SKINNING,
    U_MORPH,
    U_ALPHA_TEST,
};

static const uniform_name S_REFR_uniform[] = {
    U_SKINNING,
};

static const uniform_name RIPEMIT_uniform[] = {
    U_RIPPLE,
    U_RIPPLE_EMIT,
};

static const uniform_name RAIN_uniform[] = {
    U_INVALID,
};

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
    U_INVALID,
};

static const uniform_name FONT_uniform[] = {
    U_INVALID,
};

static const uniform_name IMGFILT_uniform[] = {
    U_INVALID,
};

static const uniform_name SPRITE_uniform[] = {
    U_TEX_0_TYPE,
    U_TEX_1_TYPE,
    U_COMBINER,
};

static const uniform_name DOF_uniform[] = {
    U_DOF,
};

static const uniform_name TRANSPARENCY_uniform[] = {
    U_INVALID,
};

struct glass_eye_struct {
    const vec4 tex_model_param;
    const vec4 eb_tex_model_param;
    const float_t n_1;
    const float_t n_2;
    const float_t scale_const;
    const float_t trsmit_coef_const;
    const vec3 ellipsoid_radius_const;
    const vec3 iris_radius_const;
    const vec3 pupil_radius_const;
    const float_t lens_depth_const;
    const vec3 eb_radius_const;
    float_t trsmit_coef;
    vec3 ellipsoid_radius;
    vec3 iris_radius;
    vec3 pupil_radius;
    float_t lens_depth;
    vec3 eb_radius;
    uint32_t frame;
    vec4 tex_offset;
    bool eye_twitch;
};

#define shader_table_struct(n) \
{ \
    #n, \
    SHADER_FT_##n, \
    sizeof(n##_table) / sizeof(shader_sub_table), \
    n##_table, \
    n##_uniform[0] > 0 ? sizeof(n##_uniform) / sizeof(uniform_name) : 0, \
    n##_uniform, \
}

const shader_table shader_ft_table[] = {
    {
        "SHADER_FFP",
        SHADER_FT_FFP,
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
    shader_table_struct(GLASEYE),
    shader_table_struct(ESMGAUSS),
    shader_table_struct(ESMFILT),
    shader_table_struct(LITPROJ),
    shader_table_struct(SIMPLE),
    shader_table_struct(SIL),
    shader_table_struct(LAMBERT),
    shader_table_struct(CONSTANT),
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
    shader_table_struct(WATER01),
    shader_table_struct(W_PTCL),
    shader_table_struct(SNOW_PT),
    shader_table_struct(LEAF_PT),
    shader_table_struct(STAR),
    shader_table_struct(FLOOR),
    shader_table_struct(PUDDLE),
    shader_table_struct(S_REFL),
    shader_table_struct(S_REFR),
    shader_table_struct(RIPEMIT),
    shader_table_struct(RAIN),
    shader_table_struct(RIPPLE),
    shader_table_struct(FOGPTCL),
    shader_table_struct(PARTICL),
    shader_table_struct(GLITTER_PT),
    shader_table_struct(FONT),
    shader_table_struct(IMGFILT),
    shader_table_struct(SPRITE),
    shader_table_struct(DOF),
    shader_table_struct(TRANSPARENCY),
};

#undef shader_table_struct

const size_t shader_ft_table_size =
    sizeof(shader_ft_table) / sizeof(shader_table);

static void glass_eye_calc(glass_eye_struct* glass_eye);
static void glass_eye_set(glass_eye_struct* glass_eye);
static void shader_bind_blinn(shader_set_data* set, shader* shad);
static void shader_bind_cloth(shader_set_data* set, shader* shad);
static void shader_bind_hair(shader_set_data* set, shader* shad);
static void shader_bind_glass_eye(shader_set_data* set, shader* shad);
static void shader_bind_tone_map(shader_set_data* set, shader* shad);
static void shader_bind_sss_filter(shader_set_data* set, shader* shad);
static void shader_bind_esm_filter(shader_set_data* set, shader* shad);
static void shader_bind_reduce_tex(shader_set_data* set, shader* shad);
static void shader_bind_magnify(shader_set_data* set, shader* shad);
static void shader_bind_mlaa(shader_set_data* set, shader* shad);
static void shader_bind_exposure(shader_set_data* set, shader* shad);
static void shader_bind_gauss(shader_set_data* set, shader* shad);
static void shader_bind_snow_particle(shader_set_data* set, shader* shad);
static void shader_bind_star(shader_set_data* set, shader* shad);
static void shader_bind_imgfilter(shader_set_data* set, shader* shad);
static void shader_bind_dof(shader_set_data* set, shader* shad);

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
        SHADER_FT_GLASEYE,
        shader_bind_glass_eye,
    },
    {
        SHADER_FT_TONEMAP,
        shader_bind_tone_map,
    },
    {
        SHADER_FT_SSS_FILT,
        shader_bind_sss_filter,
    },
    {
        SHADER_FT_ESMFILT,
        shader_bind_esm_filter,
    },
    {
        SHADER_FT_REDUCE,
        shader_bind_reduce_tex,
    },
    {
        SHADER_FT_MAGNIFY,
        shader_bind_magnify,
    },
    {
        SHADER_FT_MLAA,
        shader_bind_mlaa,
    },
    {
        SHADER_FT_EXPOSURE,
        shader_bind_exposure,
    },
    {
        SHADER_FT_GAUSS,
        shader_bind_gauss,
    },
    {
        SHADER_FT_SNOW_PT,
        shader_bind_snow_particle,
    },
    {
        SHADER_FT_STAR,
        shader_bind_star,
    },
    {
        SHADER_FT_IMGFILT,
        shader_bind_imgfilter,
    },
    {
        SHADER_FT_DOF,
        shader_bind_dof,
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
    false,
};

int32_t shader_ft_get_index_by_name(const char* name) {
    if (!str_utils_compare(name, "EYEBALL"))
        return SHADER_FT_GLASEYE;
    return -1;
}

static void glass_eye_calc(glass_eye_struct* glass_eye) {
    const float_t scale_const = glass_eye->scale_const;
    glass_eye->trsmit_coef = glass_eye->trsmit_coef_const / scale_const;
    glass_eye->eb_radius = glass_eye->eb_radius_const;
    glass_eye->ellipsoid_radius = glass_eye->ellipsoid_radius_const * scale_const;
    glass_eye->iris_radius = glass_eye->iris_radius_const * scale_const;
    glass_eye->pupil_radius = glass_eye->pupil_radius_const * scale_const;
    glass_eye->lens_depth = glass_eye->lens_depth_const * scale_const;

    if (!glass_eye->eye_twitch) {
        glass_eye->tex_offset = 0.0f;
        return;
    }

    uint32_t(FASTCALL * get_frame_counter)() = (uint32_t(FASTCALL*)())0x0000000140192E00;

    uint32_t frame = get_frame_counter();
    if (frame == glass_eye->frame)
        return;

    glass_eye->frame = frame;
    if (frame == (frame / 90) * 90) {
        float_t(FASTCALL * rand_state_array_get_float)(int32_t index) = (float_t(FASTCALL*)(int32_t index))0x00000001404FFE60;
        glass_eye->tex_offset.x = (rand_state_array_get_float(0) - 0.5f) * 0.015f;
        glass_eye->tex_offset.y = (rand_state_array_get_float(0) - 0.5f) * 0.015f;
    }

    float_t phase = (float_t)(uint8_t)frame * (float_t)(M_PI * (1.0 / 128.0));
    float_t main_wobble = sinf(sinf(phase * 27.0f) + phase * 2.0f);
    float_t secondary_wobble = sinf(phase * 23.0f);
    glass_eye->tex_offset.z = (main_wobble + secondary_wobble) * 0.5f * 0.01f;
    glass_eye->tex_offset.w = 0.0f;
}

static void glass_eye_set(glass_eye_struct* glass_eye) {
    glass_eye_batch_shader_data glass_eye_batch = {};

    vec4 temp;
    *(vec3*)&temp = glass_eye->ellipsoid_radius * glass_eye->ellipsoid_radius;
    temp.w = temp.z;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_ellipsoid_radius = temp;

    *(vec3*)&temp = glass_eye->ellipsoid_radius;
    temp.w = 1.0f;
    glass_eye_batch.g_ellipsoid_scale = temp;

    glass_eye_batch.g_tex_model_param = glass_eye->tex_model_param;
    glass_eye_batch.g_tex_offset = glass_eye->tex_offset;

    *(vec3*)&temp = glass_eye->eb_radius * glass_eye->eb_radius;
    temp.w = temp.z;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_eb_radius = temp;

    glass_eye_batch.g_eb_tex_model_param = glass_eye->eb_tex_model_param;

    float_t r_0 = (glass_eye->n_1 - glass_eye->n_2) / (glass_eye->n_1 + glass_eye->n_2);
    r_0 *= r_0;
    glass_eye_batch.g_fresnel = { 1.0f - r_0, r_0, 0.0f, 0.0f };

    float_t v3 = (glass_eye->n_1 * glass_eye->n_1) / (glass_eye->n_2 * glass_eye->n_2);
    glass_eye_batch.g_refract1 = { v3, 1.0f - v3, glass_eye->n_1 / glass_eye->n_2, 0.0f };

    float_t v4 = (glass_eye->n_2 * glass_eye->n_2) / (glass_eye->n_1 * glass_eye->n_1);
    glass_eye_batch.g_refract2 = { v4, 1.0f - v4, glass_eye->n_2 / glass_eye->n_1, 0.0f };

    *(vec3*)&temp = glass_eye->iris_radius * glass_eye->iris_radius;
    temp.w = -1.0f;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_iris_radius = temp;

    *(vec3*)&temp = glass_eye->ellipsoid_radius * glass_eye->ellipsoid_radius;
    temp.w = -1.0f;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_cornea_radius = temp;

    *(vec3*)&temp = glass_eye->pupil_radius * glass_eye->pupil_radius;
    temp.w = -1.0f;
    *(vec3*)&temp = vec3::rcp(*(vec3*)&temp);
    glass_eye_batch.g_pupil_radius = temp;

    *(vec2*)&temp = *(vec2*)&glass_eye->tex_model_param * *(vec2*)&glass_eye->iris_radius;
    temp.z = glass_eye->trsmit_coef * 1.442695f;
    temp.w = glass_eye->lens_depth;
    *(vec2*)&temp = vec2::rcp(*(vec2*)&temp);
    glass_eye_batch.g_tex_scale = temp;

    rctx->glass_eye_batch_ubo.WriteMemory(glass_eye_batch);
    rctx->glass_eye_batch_ubo.Bind(3);
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

static void shader_bind_glass_eye(shader_set_data* set, shader* shad) {
    uniform->arr[U_EYE_LENS] = 0;
    if (set->shaders[SHADER_FT_GLASEYE].bind(set, SHADER_FT_SUB_GLASS_EYE) >= 0) {
        glass_eye_calc(&glass_eye);
        glass_eye_set(&glass_eye);
    }
}

static void shader_bind_tone_map(shader_set_data* set, shader* shad) {
    shad->bind(set, uniform->arr[U_NPR] == 1
        ? SHADER_FT_SUB_TONEMAP_NPR1 : SHADER_FT_SUB_TONEMAP);
}

static void shader_bind_sss_filter(shader_set_data* set, shader* shad) {
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

static void shader_bind_esm_filter(shader_set_data* set, shader* shad) {
    switch (uniform->arr[U_ESM_FILTER]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_ESM_FILTER_MIN);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_ESM_FILTER_EROSION);
        break;
    }
}

static void shader_bind_reduce_tex(shader_set_data* set, shader* shad) {
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

static void shader_bind_magnify(shader_set_data* set, shader* shad) {
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

static void shader_bind_mlaa(shader_set_data* set, shader* shad) {
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

static void shader_bind_exposure(shader_set_data* set, shader* shad) {
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

static void shader_bind_gauss(shader_set_data* set, shader* shad) {
    switch (uniform->arr[U_GAUSS]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_PP_GAUSS_USUAL);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_PP_GAUSS_CONE);
        break;
    }
}

static void shader_bind_snow_particle(shader_set_data* set, shader* shad) {
    switch (uniform->arr[U_SNOW_PARTICLE]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_SNOW_PARTICLE_CPU);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_SNOW_PARTICLE);
        break;
    }
}

static void shader_bind_star(shader_set_data* set, shader* shad) {
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

static void shader_bind_dof(shader_set_data* set, shader* shad) {
    switch (uniform->arr[U_DOF_STAGE]) {
    case 0:
        shad->bind(set, SHADER_FT_SUB_DOF_RENDER_TILE);
        break;
    case 1:
        shad->bind(set, SHADER_FT_SUB_DOF_GATHER_TILE);
        break;
    case 2:
        shad->bind(set, SHADER_FT_SUB_DOF_DOWNSAMPLE);
        break;
    case 3:
        shad->bind(set, SHADER_FT_SUB_DOF_MAIN_FILTER);
        break;
    case 4:
        shad->bind(set, SHADER_FT_SUB_DOF_UPSAMPLE);
        break;
    }
}
