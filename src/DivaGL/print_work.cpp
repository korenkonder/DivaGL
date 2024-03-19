/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "print_work.hpp"

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

void font_info::set_glyph_size(float_t glyph_width, float_t glyph_height) {
    glyph.x = glyph_width;
    glyph.y = glyph_height;
    glyph_ratio.x = glyph_width / font_glyph.x;
    glyph_ratio.y = glyph_height / font_glyph.y;
}

PrintWork::PrintWork() : clip(), layer(), field_28(), line_length() {
    color = 0xFFFFFFFF;
    fill_color = 0xFF808080;
    prio = spr::SPR_PRIO_DEFAULT;
    resolution_mode = RESOLUTION_MODE_HD;
    font = (font_info*)0x0000000140EDA860;
    empty_char = L'\x25A1';
}

void PrintWork::SetFont(const font_info* value) {
    font = value;
}

void PrintWork::printf(app::text_flags flags, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(flags, fmt, args);
    va_end(args);
}

void PrintWork::printf_align_left(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(app::TEXT_FLAG_ALIGN_FLAG_LEFT, fmt, args);
    va_end(args);
}

void PrintWork::wprintf(app::text_flags flags, const wchar_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vwprintf(flags, fmt, args);
    va_end(args);
}

void PrintWork::wprintf_align_left(const wchar_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vwprintf(app::TEXT_FLAG_ALIGN_FLAG_LEFT, fmt, args);
    va_end(args);
}

void PrintWork::vprintf(app::text_flags flags, const char* fmt, va_list args) {
    static void(FASTCALL * sub_1400CA8E0)(prj::string * buf, const char* fmt, va_list args)
        = (void (FASTCALL*)(prj::string * buf, const char* fmt, va_list args))0x00000001400CA8E0;
    static void(FASTCALL * PrintWork__PrintText)(PrintWork * _this, app::text_flags flags, const char* str, size_t length)
        = (void (FASTCALL*)(PrintWork * _this, app::text_flags flags, const char* str, size_t length))0x0000000140198500;

    prj::string buf;
    sub_1400CA8E0(&buf, fmt, args);
    PrintWork__PrintText(this, flags, buf.c_str(), buf.size());
}

void PrintWork::vwprintf(app::text_flags flags, const wchar_t* fmt, va_list args) {
    static void(FASTCALL * sub_1400DDC30)(prj::wstring * buf, const wchar_t* fmt, va_list args)
        = (void (FASTCALL*)(prj::wstring * buf, const wchar_t* fmt, va_list args))0x00000001400DDC30;
    static void(FASTCALL * PrintWork__PrintText)(PrintWork * _this, app::text_flags flags, const wchar_t* str, size_t length)
        = (void (FASTCALL*)(PrintWork * _this, app::text_flags flags, const wchar_t* str, size_t length))0x0000000140198380;

    prj::wstring buf;
    sub_1400DDC30(&buf, fmt, args);
    PrintWork__PrintText(this, flags, buf.c_str(), buf.size());
}
