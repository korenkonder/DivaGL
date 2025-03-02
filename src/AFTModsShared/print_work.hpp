/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/vec.hpp"
#include "auth_2d.hpp"

namespace app {
    enum text_flags {
        TEXT_FLAG_ALIGN_FLAG_LEFT            = 0x00001,
        TEXT_FLAG_ALIGN_FLAG_RIGHT           = 0x00002,
        TEXT_FLAG_ALIGN_FLAG_H_CENTER        = 0x00004,
        TEXT_FLAG_ALIGN_FLAG_LOCATE_H_CENTER = 0x00008,
        TEXT_FLAG_ALIGN_FLAG_V_CENTER        = 0x00010,
        TEXT_FLAG_ALIGN_FLAG_LOCATE_V_CENTER = 0x00020,
        TEXT_FLAG_7                          = 0x00040,
        TEXT_FLAG_8                          = 0x00080,
        TEXT_FLAG_9                          = 0x00100,
        TEXT_FLAG_CLIP                       = 0x00200,
        TEXT_FLAG_11                         = 0x00400,
        TEXT_FLAG_12                         = 0x00800,
        TEXT_FLAG_13                         = 0x01000,
        TEXT_FLAG_14                         = 0x02000,
        TEXT_FLAG_15                         = 0x04000,
        TEXT_FLAG_16                         = 0x08000,
        TEXT_FLAG_FONT                       = 0x10000,
    };
}

struct font_char {
    rectangle tex;
    float_t glyph_offset;
    float_t glyph_width;
    uint8_t field_18;

    font_char();
};

struct font_character {
    bool init;
    bool halfwidth;
    uint8_t tex_column;
    uint8_t tex_row;
    uint8_t glyph_offset;
    uint8_t glyph_width;
};

struct font {
    int32_t sprite_id;
    uint8_t glyph_width;
    uint8_t glyph_height;
    uint8_t glyph_box_width;
    uint8_t glyph_box_height;
    uint8_t column_scale_num;
    uint8_t column_scale_denom;
    int32_t map_id;
    float_t column_scale;
    size_t characters_count_per_row;
    prj::vector<font_character> characters;
    bool disable_glyph_spacing;

    const font_character* get_character(wchar_t c) const;
};

struct font_info {
    int32_t font_handler_index;
    const struct font* font_ptr;
    int32_t type;
    vec2 font_glyph;
    vec2 font_glyph_box;
    vec2 glyph;
    vec2 glyph_ratio;
    vec2 spacing;

    font_info();
    font_info(int32_t font_handler_index);

    font_char* get_char_data(wchar_t c, font_char* font_c) const;
    void set_glyph_size(float_t glyph_width, float_t glyph_height);

    bool sub_140199170() const;
};

static_assert(sizeof(font_info) == 0x40, "\"font_info\" struct should have a size of 0x40");

struct sprite_text_mesh {
    int32_t sprite_id;
    std::vector<spr::SpriteVertex> vertices;

    sprite_text_mesh();
    ~sprite_text_mesh();

    spr::SpriteVertex* add_char();
    void add_set_char(app::text_flags flags,
        rectangle pos, rectangle uv, color4u8 color);
    void apply_scale_offset(vec2 scale, vec2 offset);

    static void set_char(spr::SpriteVertex* vtx, app::text_flags flags,
        rectangle pos, rectangle uv, color4u8 color);
};

struct PrintWork {
    color4u8 color;
    color4u8 fill_color;
    bool clip;
    rectangle clip_data;
    spr::SprPrio prio;
    uint32_t layer;
    resolution_mode resolution_mode;
    uint32_t field_28;
    vec2 text_current_loc;
    vec2 line_origin_loc;
    size_t line_length;
    const font_info* font;
    wchar_t empty_char;

    PrintWork();

    void DrawLine(vec2 pos[2]);
    void DrawPolyLine(vec2* points, size_t count);
    void DrawRectangle(rectangle rect);
    void DrawTextMesh(app::text_flags flags, sprite_text_mesh& mesh);
    void FillRectangle(rectangle rect);
    font_char GetCharData(wchar_t c);
    rectangle GetClipBox(app::text_flags flags, vec2 glyph_size, vec2 glyph_ratio);
    std::vector<font_char> GetStringData(const wchar_t* str_begin, const wchar_t* str_end);
    vec2 GetStringSize(const wchar_t* str_begin, const wchar_t* str_end);
    vec2 GetTextSize(const wchar_t* str, size_t length);
    vec2 GetTextSize(const wchar_t* str_begin, const wchar_t* str_end);
    vec2 GetTextSize(const char* str, size_t length);
    vec2 GetTextSize(const char* str_begin, const char* str_end);
    vec2 GetTextSize(const std::wstring& str);
    void NewLine();
    void PrintText(app::text_flags flags, const char* str);
    void PrintText(app::text_flags flags, const char* str, size_t length);
    void PrintText(app::text_flags flags, const wchar_t* str_begin, const wchar_t* str_end);
    void PutText(app::text_flags flags, const wchar_t* str_begin, const wchar_t* str_end);
    void SetFont(const font_info* value);
    void SetResolutionMode(::resolution_mode value);
    void set_text_position(float_t column, float_t line);

    void printf(app::text_flags flags,
        _In_z_ _Printf_format_string_ const char* const fmt, ...);
    void printf_align_left(_In_z_ _Printf_format_string_ const char* const fmt, ...);
    void wprintf(app::text_flags flags,
        _In_z_ _Printf_format_string_ const wchar_t* const fmt, ...);
    void wprintf_align_left(_In_z_ _Printf_format_string_ const wchar_t* const fmt, ...);
    void vprintf(app::text_flags flags,
        _In_z_ _Printf_format_string_ const char* const fmt, va_list args);
    void vwprintf(app::text_flags flags,
        _In_z_ _Printf_format_string_ const wchar_t* const fmt, va_list args);

    static void ClampPosToClipBox(float_t pos_min, float_t pos_max,
        float_t clip_box_min, float_t clip_box_max, float_t& clip_pos_min, float_t& clip_pos_max);

    vec2 sub_140197B80(app::text_flags flags, vec2 size);

    static int32_t sub_140197D60(rectangle clip_box, rectangle& pos, rectangle& uv);
};

static_assert(sizeof(PrintWork) == 0x58, "\"PrintWork\" struct should have a size of 0x58");
