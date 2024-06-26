/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "color_change_dw.hpp"
#include "../rob/rob.hpp"
#include "../texture.hpp"
#include <Helpers.h>

class ColorChangeDw : public dw::Shell {
public:
    dw::ListBox* object_set;
    dw::ListBox* texture_name;
    dw::Widget* type;
    dw::Widget* format;
    dw::Widget* size;
    dw::Button* show_button;
    dw::Button* auto_resize_button;
    dw::ListBox* item_chara;
    dw::ListBox* item_names;
    dw::ListBox* item_texture_names;
    dw::Button* over_write;
    dw::Button* inverse;
    dw::Slider* blend_r;
    dw::Slider* blend_g;
    dw::Slider* blend_b;
    dw::Slider* offset_r;
    dw::Slider* offset_g;
    dw::Slider* offset_b;
    dw::Slider* hue;
    dw::Slider* value;
    dw::Slider* saturation;
    dw::Slider* contrast;
    dw::Button* copy_button;
    dw::Button* paset_button;
    dw::Button* reset_button;
    dw::Button* save_button;
    dw::RowLayout* row_layout;
    int32_t obj_set_index;
    int32_t obj_set_id;
    bool set;
    int32_t color_change_index;
    prj::vector<::texture*> texture;
    prj::vector<::color_tone> color_tone;
    ::color_tone color_tone_temp;
    bool show;
    bool auto_resize;
    ::chara_index chara_index;
    int32_t item_index;
    size_t item_texture_index;

    ::texture* GetChgTex();
    ::color_tone* GetColorTone();
    void GetColorToneDefault(::color_tone& value);
    ::texture* GetOrgTex();
    void GetSetColorTone();
    bool LoadColorTone();
    bool LoadTexture();
    void ReloadData();
    void ResetColorTone();
    void ResetData();
    void ResetTexture();
    void UpdateData();
};

static_assert(sizeof(ColorChangeDw) == 0x2F8, "\"ColorChangeDw\" struct should have a size of 0x2F8");

ColorChangeDw*& color_change_dw = *(ColorChangeDw**)0x000000014118FF00;

prj::vector<prj::vector<void*>>* color_change_dw_textures; // Added

HOOK(void, FASTCALL, ColorChangeDw__ReloadData, 0x00000001402C20B0, ColorChangeDw* _this) {
    _this->ReloadData();
}

HOOK(void, FASTCALL, ColorChangeDw__Draw, 0x00000001402C2170, ColorChangeDw* _this) {
    _this->UpdateData();

    void (FASTCALL * ColorChangeDw__UpdateTextureInfo)(ColorChangeDw * _this)
        = (void (FASTCALL*)(ColorChangeDw * _this))0x00000001402C2C60;
    ColorChangeDw__UpdateTextureInfo(_this);

    _this->GetSetColorTone();

    ::texture* org_tex = _this->GetOrgTex();
    ::texture* chg_tex = _this->GetChgTex();
    ::color_tone* col_tone = _this->GetColorTone();

    //if (org_tex && chg_tex && col_tone)
    //    texture_apply_color_tone(org_tex, chg_tex, col_tone);

    // Added
    if (org_tex && chg_tex && col_tone
        && chg_tex->internal_format == org_tex->internal_format
        && chg_tex->width == org_tex->width
        && chg_tex->height == org_tex->height
        && chg_tex->max_mipmap_level == org_tex->max_mipmap_level) {
        prj::vector<void*>& vec_data = color_change_dw_textures->data()[_this->color_change_index];

        for (int32_t i = 0; i <= org_tex->max_mipmap_level; i++) {
            int32_t size = org_tex->get_size_mip_level(i);
            void* tex_data = vec_data.data()[i];
            if (!tex_data)
                continue;

            void* data = prj::HeapCMallocAllocate(prj::HeapCMallocTemp, size, "imgf_color_tone_cpu()");
            if (!data)
                break;

            memcpy(data, tex_data, size);

            int32_t width_align = chg_tex->get_width_align_mip_level(i);
            int32_t height_align = chg_tex->get_height_align_mip_level(i);
            if (chg_tex->flags & TEXTURE_BLOCK_COMPRESSION) {
                if (chg_tex->internal_format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
                    dxt1_image_apply_color_tone(width_align, height_align, size, (dxt1_block*)data, col_tone);
                else
                    dxt5_image_apply_color_tone(width_align, height_align, size, (dxt5_block*)data, col_tone);

                gl_state_bind_texture_2d(org_tex->glid);
                int32_t width = chg_tex->get_width_mip_level(i);
                int32_t height = chg_tex->get_height_mip_level(i);
                glCompressedTexSubImage2D(org_tex->target, i, 0, 0, width, height,
                    org_tex->internal_format, size, data);
            }
            else if (chg_tex->internal_format == GL_RGB5) {
                rgb565_image_apply_color_tone(width_align, height_align, size, (rgb565*)data, col_tone);

                gl_state_bind_texture_2d(org_tex->glid);
                int32_t width = chg_tex->get_width_mip_level(i);
                int32_t height = chg_tex->get_height_mip_level(i);
                glTexSubImage2DDLL(org_tex->target, i, 0, 0, width, height,
                    GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
            }
            gl_state_get_error();
            prj::HeapCMallocFree(prj::HeapCMallocTemp, data);
        }
        gl_state_bind_texture_2d(0);
    }

    if (_this->show) {
        if (org_tex) {
            spr::SprArgs args;
            args.texture = org_tex;
            if (_this->auto_resize)
                args.SetSpriteSize({ 256.0f, 256.0f });
            spr::put_sprite(&args);
        }

        if (chg_tex) {
            spr::SprArgs args;
            args.texture = chg_tex;
            args.trans.x = (float_t)chg_tex->width;
            if (_this->auto_resize) {
                args.SetSpriteSize({ 256.0f, 256.0f });
                args.trans.x = 266.0f;
            }
            spr::put_sprite(&args);
        }
    }

    _this->dw::Shell::Draw();
}

HOOK(void, FASTCALL, ColorChangeDw__ResetData, 0x00000001402C4130, ColorChangeDw* _this) {
    _this->ResetData();
}

void color_change_dw_patch() {
    color_change_dw_textures = new prj::vector<prj::vector<void*>>(); // Added

    INSTALL_HOOK(ColorChangeDw__ReloadData);
    INSTALL_HOOK(ColorChangeDw__Draw);
    INSTALL_HOOK(ColorChangeDw__ResetData);
}

::texture* ColorChangeDw::GetChgTex() {
    if (color_change_index >= 0 && color_change_index < texture.size())
        return texture.data()[color_change_index];
    return 0;
}

::color_tone* ColorChangeDw::GetColorTone() {
    if (color_change_index >= 0 && color_change_index < color_tone.size())
        return &color_tone.data()[color_change_index];
    return 0;
}

void ColorChangeDw::GetColorToneDefault(::color_tone& value) {
    value = {};
}

::texture* ColorChangeDw::GetOrgTex() {
    ::texture** textures = objset_info_storage_get_set_textures(obj_set_id);
    if (textures && color_change_index < objset_info_storage_get_set_tex_num(obj_set_id))
        return textures[color_change_index];
    return 0;
}

void ColorChangeDw::GetSetColorTone() {
    ::color_tone* col_tone = GetColorTone();
    if (col_tone) {
        inverse->SetValue(col_tone->inverse);
        blend_r->scroll_bar->SetValue(col_tone->blend.x);
        blend_g->scroll_bar->SetValue(col_tone->blend.y);
        blend_b->scroll_bar->SetValue(col_tone->blend.z);
        offset_r->scroll_bar->SetValue(col_tone->offset.x);
        offset_g->scroll_bar->SetValue(col_tone->offset.y);
        offset_b->scroll_bar->SetValue(col_tone->offset.z);
        hue->scroll_bar->SetValue(col_tone->hue);
        saturation->scroll_bar->SetValue(col_tone->saturation);
        value->scroll_bar->SetValue(col_tone->value);
        contrast->scroll_bar->SetValue(col_tone->contrast);
    }
}

bool ColorChangeDw::LoadColorTone() {
    ResetColorTone();

    if (!texture.size())
        return false;

    ::color_tone col_tone;
    GetColorToneDefault(col_tone);
    color_tone.resize(texture.size(), col_tone);
    return true;
}

bool ColorChangeDw::LoadTexture() {
    ResetTexture();

    ::texture** textures = objset_info_storage_get_set_textures(obj_set_id);
    if (!textures)
        return false;

    int32_t tex_num = objset_info_storage_get_set_tex_num(obj_set_id);
    
    /*for (int32_t i = 0; i < tex_num; i++) {
        ::texture* chg_tex = 0;
        ::texture* org_tex = textures[i];
        if (org_tex->target == GL_TEXTURE_2D) {
            chg_tex = texture_create_copy_texture(texture_id(0x22, i), org_tex);
            if (!chg_tex)
                break;
        }

        texture.push_back(chg_tex);
    }*/

    // Added
    texture.reserve(tex_num);
    color_change_dw_textures->reserve(tex_num);
    for (int32_t i = 0; i < tex_num; i++) {
        color_change_dw_textures->push_back({});

        ::texture* chg_tex = 0;
        ::texture* org_tex = textures[i];
        if (org_tex->target == GL_TEXTURE_2D) {
            prj::vector<void*>& vec_data = color_change_dw_textures->back();
            vec_data.reserve((size_t)org_tex->max_mipmap_level + 1);
            for (int32_t i = 0; i <= org_tex->max_mipmap_level; i++) {
                int32_t size = org_tex->get_size_mip_level(i);
                void* data = prj::HeapCMallocAllocate(prj::HeapCMallocTemp, size, "ColorChangeDw::LoadTexture()");
                if (!data)
                    break;

                int32_t width_align = org_tex->get_width_align_mip_level(i);
                int32_t height_align = org_tex->get_height_align_mip_level(i);
                gl_state_bind_texture_2d(org_tex->glid);
                if (org_tex->flags & TEXTURE_BLOCK_COMPRESSION)
                    glGetCompressedTexImage(org_tex->target, i, data);
                else
                    glGetTexImageDLL(org_tex->target, i, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
                gl_state_get_error();
                vec_data.push_back(data);
            }

            chg_tex = texture_load_tex_2d(texture_id(0x22, i), org_tex->internal_format,
                org_tex->width, org_tex->height, org_tex->max_mipmap_level, (const void**)vec_data.data(), true);

            if (!chg_tex)
                break;
        }

        texture.push_back(chg_tex);
    }
    return true;
}

void ColorChangeDw::ReloadData() {
    ResetData();
    if (LoadTexture() && LoadColorTone())
        set = true;
    else
        ResetData();
}

void ColorChangeDw::ResetColorTone() {
    color_tone.clear();
}

void ColorChangeDw::ResetData() {
    set = false;
    ResetTexture();
    ResetColorTone();
}

void ColorChangeDw::ResetTexture() {
    for (::texture*& i : texture) {
        texture_release(i);
        i = 0;
    }
    texture.clear();

    // Added
    for (prj::vector<void*>& i : *color_change_dw_textures)
        for (void*& j : i)
            if (j) {
                prj::HeapCMallocFree(prj::HeapCMallocTemp, j);
                j = 0;
            }

    color_change_dw_textures->clear();
}

void ColorChangeDw::UpdateData() {
    if (!set)
        ReloadData();
    else if (!objset_info_storage_get_set_textures(obj_set_id))
        ResetData();

}