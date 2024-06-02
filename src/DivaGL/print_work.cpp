/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "print_work.hpp"
#include "shader_ft.hpp"

static const font_character font_character_null = {};

const font_character* font::get_character(wchar_t c) const {
    if (c >= 0 && c < characters.size())
        return &characters[c];
    return &font_character_null;
}

font_char::font_char() : glyph_offset(), glyph_width(), field_18() {

}

font_info::font_info() : font_ptr(), type() {
    font_handler_index = -1;
    static font_info* (FASTCALL * font_info__font_info)(font_info * _this)
        = (font_info * (FASTCALL*)(font_info * _this))0x00000001401965C0;
    font_info__font_info(this);
}

font_info::font_info(int32_t font_handler_index) : font_ptr(), type() {
    this->font_handler_index = -1;
    static font_info* (FASTCALL * font_info__font_info)(font_info * _this, int32_t font_handler_index)
        = (font_info * (FASTCALL*)(font_info * _this, int32_t font_handler_index))0x0000000140196510;
    font_info__font_info(this, font_handler_index);
}

font_char* font_info::get_char_data(wchar_t c, font_char* font_c) const {
    const font_character* fc = font_ptr->get_character(c);
    if (!fc->init)
        return 0;

    vec2 glyph_size = font_glyph;
    if (fc->halfwidth)
        glyph_size.x *= 0.5f;

    float_t glyph_offset = 0.0f;
    float_t glyph_width = glyph_size.x;
    if (type == 2) {
        glyph_offset = (float_t)fc->glyph_offset;
        glyph_width = (float_t)fc->glyph_width;
    }

    font_c->tex.pos = vec2((float_t)fc->tex_column, (float_t)fc->tex_row) * font_glyph_box;
    font_c->tex.size = glyph_size;
    font_c->glyph_offset = glyph_offset;
    font_c->glyph_width = glyph_width;
    font_c->field_18 = 0;
    return font_c;
}

void font_info::set_glyph_size(float_t glyph_width, float_t glyph_height) {
    glyph.x = glyph_width;
    glyph.y = glyph_height;
    glyph_ratio.x = glyph_width / font_glyph.x;
    glyph_ratio.y = glyph_height / font_glyph.y;
}

bool font_info::sub_140199170() const {
    return font_glyph != glyph && font_glyph == font_glyph_box;
}

sprite_text_mesh::sprite_text_mesh() {
    sprite_id = -1;
}

sprite_text_mesh::~sprite_text_mesh() {

}

spr::SpriteVertex* sprite_text_mesh::add_char() {
    size_t size = vertices.size();
    vertices.resize(size + 4);
    return &vertices.data()[size];
}

void sprite_text_mesh::add_set_char(app::text_flags flags, rectangle pos, rectangle uv, color4u8 color) {
    sprite_text_mesh::set_char(add_char(), flags, pos, uv, color);
}

void sprite_text_mesh::apply_scale_offset(vec2 scale, vec2 offset) {
    for (spr::SpriteVertex& i : vertices)
        *(vec2*)&i.pos.x = *(vec2*)&i.pos.x * scale + offset;
}

void sprite_text_mesh::set_char(spr::SpriteVertex* vtx, app::text_flags flags, rectangle pos, rectangle uv, color4u8 color) {
    vec2 uv_size = uv.size;
    vec2 uv_pos = uv.pos;
    if (flags & app::TEXT_FLAG_11) {
        uv_pos += 0.5f;
        uv_size += -1.0f;
    }

    vtx[0].pos.x = pos.pos.x;
    vtx[0].pos.y = pos.pos.y;
    vtx[0].pos.z = 0.0f;
    vtx[0].uv.x = uv_pos.x;
    vtx[0].uv.y = uv_pos.y;
    vtx[0].color = color;

    vtx[1].pos.x = pos.pos.x + pos.size.x;
    vtx[1].pos.y = pos.pos.y;
    vtx[1].pos.z = 0.0;
    vtx[1].uv.x = uv_pos.x + uv_size.x;
    vtx[1].uv.y = uv_pos.y;
    vtx[1].color = color;

    vtx[2].pos.x = pos.pos.x + pos.size.x;
    vtx[2].pos.y = pos.pos.y + pos.size.y;
    vtx[2].pos.z = 0.0f;
    vtx[2].uv.x = uv_pos.x + uv_size.x;
    vtx[2].uv.y = uv_pos.y + uv_size.y;
    vtx[2].color = color;

    vtx[3].pos.x = pos.pos.x;
    vtx[3].pos.y = pos.pos.y + pos.size.y;
    vtx[3].pos.z = 0.0f;
    vtx[3].uv.x = uv_pos.x;
    vtx[3].uv.y = uv_pos.y + uv_size.y;
    vtx[3].color = color;
}

PrintWork::PrintWork() : clip(), layer(), field_28(), line_length() {
    color = 0xFFFFFFFF;
    fill_color = 0xFF808080;
    prio = spr::SPR_PRIO_DEFAULT;
    resolution_mode = RESOLUTION_MODE_HD;
    font = (font_info*)0x0000000140EDA860;
    empty_char = L'\x25A1';
}

void PrintWork::DrawLine(vec2 pos[2]) {
    spr::put_sprite_line(pos[0], pos[1], resolution_mode, prio, color, layer);
}

void PrintWork::DrawPolyLine(vec2* points, size_t count) {
    spr::put_sprite_line_list(points, count, resolution_mode, prio, color, layer);
}

void PrintWork::DrawRectangle(rectangle rect) {
    spr::put_sprite_multi(rect, resolution_mode, prio, color, layer);
}

void PrintWork::DrawTextMesh(app::text_flags flags, sprite_text_mesh& mesh) {
    if (!mesh.vertices.size())
        return;

    spr::SprArgs args;
    args.blend = (field_28 >> 3) & 0x02;
    args.resolution_mode_screen = resolution_mode;
    args.resolution_mode_sprite = args.resolution_mode_screen;
    args.prio = prio;
    args.color = color;
    args.layer = layer;

    if (field_28 & 0x02) {
        for (spr::SpriteVertex& i : mesh.vertices) {
            i.color.r = 0x40;
            i.color.g = 0x40;
            i.color.b = 0x40;
            i.pos += 2.0f;
        }
        args.SetVertexArray(mesh.vertices.data(), mesh.vertices.size());
        args.id.index = mesh.sprite_id;
        spr::put_sprite(&args);

        for (spr::SpriteVertex& i : mesh.vertices) {
            i.color.r = color.r;
            i.color.g = color.g;
            i.color.b = color.b;
            i.pos.x -= 2.0f;
            i.pos.y -= 2.0f;
        }
    }

    args.SetVertexArray(mesh.vertices.data(), mesh.vertices.size());
    if (flags & app::TEXT_FLAG_FONT)
        args.shader = SHADER_FT_FONT;
    args.id.index = mesh.sprite_id;
    spr::put_sprite(&args);
}

void PrintWork::FillRectangle(rectangle rect) {
    if (clip) {
        float_t clip_max_pos_x;
        PrintWork::ClampPosToClipBox(rect.pos.x, rect.pos.x + rect.size.x,
            clip_data.pos.x, clip_data.pos.x + clip_data.size.x, rect.pos.x, clip_max_pos_x);
        rect.size.x = clip_max_pos_x - rect.pos.x;

        float_t clip_max_pos_y;
        PrintWork::ClampPosToClipBox(rect.pos.y, rect.pos.y + rect.size.y,
            clip_data.pos.y, clip_data.pos.y + clip_data.size.y, rect.pos.y, clip_max_pos_y);
        rect.size.y = clip_max_pos_y - rect.pos.y;
    }

    spr::put_sprite_rect(rect, resolution_mode, prio, fill_color, layer);
}

font_char PrintWork::GetCharData(wchar_t c) {
    font_char font_c = {};
    if (!font->get_char_data(c, &font_c))
        font->get_char_data(empty_char, &font_c);
    return font_c;
}

rectangle PrintWork::GetClipBox(app::text_flags flags, vec2 glyph_size, vec2 glyph_ratio) {
    glyph_ratio = 1.0f / glyph_ratio;
    rectangle clip_box;
    clip_box.pos = (clip_data.pos - text_current_loc) * glyph_ratio;
    clip_box.size = clip_data.size * glyph_ratio;

    if (flags & app::TEXT_FLAG_ALIGN_FLAG_RIGHT)
        clip_box.pos.x += glyph_size.x;
    else if (flags & app::TEXT_FLAG_ALIGN_FLAG_H_CENTER)
        clip_box.pos.x = (glyph_size.x - clip_data.size.x) * 0.5f * glyph_ratio.x;
    else if (flags & app::TEXT_FLAG_ALIGN_FLAG_LOCATE_H_CENTER)
        clip_box.pos.x += glyph_size.x * 0.5f;

    if (flags & app::TEXT_FLAG_ALIGN_FLAG_V_CENTER)
        clip_box.pos.y = (glyph_size.y - clip_data.size.y) * 0.5f * glyph_ratio.y;
    else if (flags & app::TEXT_FLAG_ALIGN_FLAG_LOCATE_V_CENTER)
        clip_box.pos.y += glyph_size.y * 0.5f;
    return clip_box;
}

std::vector<font_char> PrintWork::GetStringData(const wchar_t* str_begin, const wchar_t* str_end) {
    std::vector<font_char> vec(str_end - str_begin);
    font_char* fc = vec.data();
    for (const wchar_t* i = str_begin; i != str_end; i++)
        *fc++ = PrintWork::GetCharData(*i);
    return vec;
}

vec2 PrintWork::GetStringSize(const wchar_t* str_begin, const wchar_t* str_end) {
    std::vector<font_char> vec = GetStringData(str_begin, str_end);
    float_t spacing = font->spacing.x / font->glyph_ratio.x;
    vec2 size;
    for (font_char& i : vec) {
        if (&i - vec.data())
            size.x += spacing;
        size.x += i.glyph_width;
    }

    size.x *= font->glyph_ratio.x;
    size.y = font->glyph.y;
    return size;
}

vec2 PrintWork::GetTextSize(const wchar_t* str, size_t length) {
    vec2* (FASTCALL * PrintWork__GetTextSize)(PrintWork * _this, vec2 * size, const prj::wstring_range& str)
        = (vec2 * (FASTCALL*)(PrintWork * _this, vec2 * size, const prj::wstring_range & str))0x000000014019A0A0;

    vec2 size;
    return *PrintWork__GetTextSize(this, &size, prj::wstring_range(str, length));
}

vec2 PrintWork::GetTextSize(const wchar_t* str_begin, const wchar_t* str_end) {
    return GetTextSize(str_begin, str_end - str_begin);
}

vec2 PrintWork::GetTextSize(const char* str, size_t length) {
    return GetTextSize(str, str + length);
}

vec2 PrintWork::GetTextSize(const char* str_begin, const char* str_end) {
    wchar_t* wstr = utf8_to_utf16(str_begin, str_end - str_begin);
    vec2 size = GetTextSize(wstr, wstr + utf16_length(wstr));
    free_def(wstr);
    return size;
}

vec2 PrintWork::GetTextSize(const std::wstring& str) {
    return GetTextSize(str.c_str(), str.c_str() + str.size());
}

void PrintWork::NewLine() {
    text_current_loc.x = line_origin_loc.x;
    float_t loc_y = text_current_loc.y + font->spacing.y + font->glyph.y;
    text_current_loc.y = loc_y;
    line_origin_loc.y = loc_y;
}

void PrintWork::PrintText(app::text_flags flags, const char* str) {
    wchar_t* wstr = utf8_to_utf16(str);
    PrintText(flags, wstr, wstr + utf16_length(wstr));
    free_def(wstr);
}

void PrintWork::PrintText(app::text_flags flags, const char* str, size_t length) {
    wchar_t* wstr = utf8_to_utf16(str, length);
    PrintText(flags, wstr, wstr + utf16_length(wstr));
    free_def(wstr);
}

void PrintWork::PrintText(app::text_flags flags, const wchar_t* str_begin, const wchar_t* str_end) {
    void (FASTCALL * PrintWork__PrintText)(PrintWork * _this,
        app::text_flags flags, const prj::wstring_range& str)
        = (void (FASTCALL*)(PrintWork * _this,
            app::text_flags flags, const prj::wstring_range & str))0x0000000140198380;
    PrintWork__PrintText(this, flags, prj::wstring_range(str_begin, str_end));
}

// It was intended to be DrawText but there's WinAPI macro for that
void PrintWork::PutText(app::text_flags flags, const wchar_t* str_begin, const wchar_t* str_end) {
    if (str_begin == str_end)
        return;

    app::text_flags v5 = (app::text_flags)(flags
        & (app::TEXT_FLAG_ALIGN_FLAG_LOCATE_V_CENTER | app::TEXT_FLAG_ALIGN_FLAG_V_CENTER
            | app::TEXT_FLAG_ALIGN_FLAG_LOCATE_H_CENTER | app::TEXT_FLAG_ALIGN_FLAG_H_CENTER
            | app::TEXT_FLAG_ALIGN_FLAG_RIGHT | app::TEXT_FLAG_ALIGN_FLAG_LEFT));
    app::text_flags v6 = (app::text_flags)(flags & (app::TEXT_FLAG_11 | app::TEXT_FLAG_CLIP));
    bool clip = !!(flags & app::TEXT_FLAG_CLIP);
    if (clip && font->glyph.y + text_current_loc.y <= clip_data.pos.y
        && clip_data.pos.y + clip_data.size.y < text_current_loc.y)
        return;

    std::vector<font_char> v43 = PrintWork::GetStringData(str_begin, str_end);
    float_t spacing = font->spacing.x / font->glyph_ratio.x;

    vec2 glyph_ratio = font->glyph_ratio;
    rectangle clip_box;
    if (clip) {
        float_t glyph_size_x = 0.0f;
        for (font_char& i : v43)
            glyph_size_x += i.glyph_width + spacing;

        vec2 glyph_size;
        glyph_size.x = glyph_size_x - spacing;
        glyph_size.y = font->glyph.y;
        clip_box = GetClipBox(v5, glyph_size, glyph_ratio);
    }
    else
        clip_box = {};

    std::vector<sprite_text_mesh> v44;
    sprite_text_mesh v45;
    v45.sprite_id = font->font_ptr->sprite_id;
    v44.push_back(v45);
    v44.back().vertices.reserve(str_end - str_begin);

    float_t glyph_x = 0.0f;
    if (str_end - str_begin) {
        for (size_t i = 0; i != str_end - str_begin; i++) {
            font_char* fc = &v43.data()[i];
            rectangle v37;
            v37.pos.x = glyph_x;
            v37.pos.y = 0.0f;
            v37.size.x = fc->glyph_width;
            v37.size.y = fc->tex.size.y;
            rectangle v42;
            v42.pos.x = fc->tex.pos.x + fc->glyph_offset;
            v42.pos.y = fc->tex.pos.y;
            v42.size.x = fc->glyph_width;
            v42.size.y = fc->tex.size.y;
            if (!clip || PrintWork::sub_140197D60(clip_box, v37, v42))
                v44.data()[fc->field_18].add_set_char(v6, v37, v42, color);
            glyph_x += fc->glyph_width + spacing;
        }
    }
    else
        glyph_x -= spacing;

    vec2 v35;
    v35.x = glyph_x * glyph_ratio.x;
    v35.y = font->glyph.y;

    vec2 v41 = PrintWork::sub_140197B80((app::text_flags)(v5 | v6), v35);
    if (v5 & app::TEXT_FLAG_ALIGN_FLAG_LEFT)
        text_current_loc.x += font->spacing.x + v35.x;

    for (sprite_text_mesh& i : v44) {
        i.apply_scale_offset(glyph_ratio, v41);
        DrawTextMesh(flags, i);
    }
}

void PrintWork::SetFont(const font_info* value) {
    font = value;
}

void PrintWork::SetResolutionMode(::resolution_mode value) {
    resolution_mode = value;
}

void PrintWork::set_text_position(float_t column, float_t line) {
    line_origin_loc.x = (font->spacing.x + font->glyph.x * font->font_ptr->column_scale) * column;
    line_origin_loc.y = (font->spacing.y + font->glyph.y) * line;
    text_current_loc = line_origin_loc;
}

void PrintWork::printf(app::text_flags flags,
    _In_z_ _Printf_format_string_ const char* const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(flags, fmt, args);
    va_end(args);
}

void PrintWork::printf_align_left(_In_z_ _Printf_format_string_ const char* const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(app::TEXT_FLAG_ALIGN_FLAG_LEFT, fmt, args);
    va_end(args);
}

void PrintWork::wprintf(app::text_flags flags,
    _In_z_ _Printf_format_string_ const wchar_t* const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vwprintf(flags, fmt, args);
    va_end(args);
}

void PrintWork::wprintf_align_left(_In_z_ _Printf_format_string_ const wchar_t* const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vwprintf(app::TEXT_FLAG_ALIGN_FLAG_LEFT, fmt, args);
    va_end(args);
}

void PrintWork::vprintf(app::text_flags flags,
    _In_z_ _Printf_format_string_ const char* const fmt, va_list args) {
    std::string buf = vsprintf_s_string(fmt, args);
    PrintText(flags, buf.data(), buf.size());
}

void PrintWork::vwprintf(app::text_flags flags,
    _In_z_ _Printf_format_string_ const wchar_t* const fmt, va_list args) {
    std::wstring buf = vswprintf_s_string(fmt, args);
    PrintText(flags, buf.data(), buf.data() + buf.size());
}

void PrintWork::ClampPosToClipBox(float_t pos_min, float_t pos_max,
    float_t clip_box_min, float_t clip_box_max, float_t& clip_pos_min, float_t& clip_pos_max) {
    float_t v8 = max_def(pos_min, clip_box_min);
    float_t v10 = min_def(pos_max, clip_box_max);

    if (v8 < v10) {
        clip_pos_min = v8;
        clip_pos_max = v10;
    }
    else {
        clip_pos_min = pos_min;
        clip_pos_max = pos_min;
    }
}

vec2 PrintWork::sub_140197B80(app::text_flags flags, vec2 size) {
    vec2 pos = text_current_loc;
    if (flags & app::TEXT_FLAG_ALIGN_FLAG_RIGHT)
        pos.x -= size.x;
    else if (flags & app::TEXT_FLAG_ALIGN_FLAG_H_CENTER) {
        if (flags & app::TEXT_FLAG_CLIP)
            pos.x = clip_data.pos.x + clip_data.size.x * 0.5f;
        else {
            resolution_struct v10(resolution_mode);
            pos.x = (float_t)v10.width * 0.5f;
        }
        pos.x -= size.x * 0.5f;
    }
    else if (flags & app::TEXT_FLAG_ALIGN_FLAG_LOCATE_H_CENTER)
        pos.x -= size.x * 0.5f;

    if (flags & app::TEXT_FLAG_ALIGN_FLAG_V_CENTER) {
        if (flags & app::TEXT_FLAG_CLIP)
            pos.y = clip_data.pos.y + clip_data.size.y * 0.5f;
        else {
            resolution_struct v10(resolution_mode);
            pos.y = (float_t)v10.height * 0.5f;
        }
        pos.y -= size.y * 0.5f;
    }
    else if (flags & app::TEXT_FLAG_ALIGN_FLAG_LOCATE_V_CENTER)
        pos.y -= size.y * 0.5f;
    return pos;
}

int32_t PrintWork::sub_140197D60(rectangle clip_box, rectangle& pos, rectangle& uv) {
    vec2 pos_pos = pos.pos;
    vec2 pos_size = pos.size;
    vec2 uv_pos = uv.pos;
    vec2 uv_size = uv.size;

    vec2 clip_max_pos;
    vec2 clip_min_pos;
    PrintWork::ClampPosToClipBox(pos_pos.x, pos_pos.x + pos_size.x,
        clip_box.pos.x, clip_box.pos.x + clip_box.size.x, clip_min_pos.x, clip_max_pos.x);
    PrintWork::ClampPosToClipBox(pos_pos.y, pos_pos.y + pos_size.y,
        clip_box.pos.y, clip_box.pos.y + clip_box.size.y, clip_min_pos.y, clip_max_pos.y);

    vec2 clip_size = clip_max_pos - clip_min_pos;

    if (clip_size.x == pos_size.x && clip_size.y == pos_size.y)
        return 2;
    else if (clip_size.x <= 0.0f || clip_size.y <= 0.0f)
        return 0;

    vec2 uv_scale = uv_size / pos_size;
    pos.pos = clip_min_pos;
    pos.size = clip_size;
    uv.pos = (clip_min_pos - pos_pos) * uv_scale + uv_pos;
    uv.size = clip_size * uv_scale;
    return 1;
}
