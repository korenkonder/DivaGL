/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "dw.hpp"
#include "print_work.hpp"

class PrintWorkDebug : public PrintWork {
public:
    PrintWorkDebug();
};

namespace dw {
    struct Print {
        p_Font* font;
        PrintWorkDebug* print_work;
        color4u8 field_10;
        color4u8 field_14;
        rectangle field_18;
        rectangle clip_data;
        float_t field_38;

        Print(float_t width, float_t height);
        ~Print();

        void FillRectangle(rectangle rect);
        vec2 GetTextSize(prj::string& str);
        vec2 GetTextSize(prj::wstring& str);
        vec2 GetTextSize(const wchar_t* str, size_t length);
        void PrintText(prj::string& str, float_t pos_x, float_t pos_y);
        void PrintText(prj::wstring& str, float_t pos_x, float_t pos_y);
        void SetClipData(rectangle rect);
        void SetFillColor(color4u8 value);
        void SetFont(p_Font* value);
        void SetColor(color4u8 value);

        void sub_140301390(rectangle rect, float_t a3);
        void sub_140302800();

        inline vec2 GetTextSize(prj::string&& str) {
            return GetTextSize(str);
        }

        inline vec2 GetTextSize(prj::wstring&& str) {
            return GetTextSize(str);
        }

        inline void PrintText(prj::string&& str, float_t pos_x, float_t pos_y) {
            PrintText(str, pos_x, pos_y);
        }

        inline void PrintText(prj::wstring&& str, float_t pos_x, float_t pos_y) {
            PrintText(str, pos_x, pos_y);
        }
    };

    struct Colors {
        color4u8 background;
        color4u8 foreground;
        color4u8 selection_background;
        color4u8 selection_foreground;
        color4u8 selection_inactive_background;
        color4u8 selection_inactive_foreground;
        color4u8 disable_background;
        color4u8 disable_foreground;
        color4u8 title_background;
        color4u8 title_foreground;
        color4u8 title_inactive_background;
        color4u8 title_inactive_foreground;
        color4u8 active_border_color;
        color4u8 inactive_border_color;
        color4u8 focus_border_color;
        color4u8 border_color;
        color4u8 border_light_color;
        color4u8 border_dark_color;
        color4u8 popup_background;
        color4u8 popup_foreground;
        color4u8 popup_selection_background;
        color4u8 popup_selection_foreground;
        color4u8 menu_background;
        color4u8 menu_foreground;
        color4u8 menu_selection_background;
        color4u8 menu_selection_foreground;
        color4u8 button_background;
        color4u8 button_foreground;
        color4u8 button_disable_background;
        color4u8 button_disable_foreground;
    };
}

struct struc_751 {
    vec2 mouse_pos;
    vec2 mouse_delta;
    int32_t mouse_input;
    int32_t modifier;
    int32_t field_18;
    int8_t field_1C;
    int8_t field_1D;
    int64_t field_20;
    uint8_t field_28;
    int32_t key_input[2];
};

namespace dw_gui_detail {
    struct Display {
        class RootKeySelection : public dw::KeyAdapter {
        public:
        };

        void* __vftable;
        dw::Shell* selected_shell;
        dw::Menu* opened_menu;
        dw::Widget* howered_shell;
        dw::Widget* move_resize_shell;
        dw::Widget* howered_widget;
        dw::Widget* selected_widget;
        dw::Shell* focused_shell;
        bool set_focus;
        bool find_focus;
        prj::vector<dw::Shell*> shells;
        prj::vector<dw::Menu*> menus;
        dw::Widget::MouseCallbackData mouse_callback_data;
        dw::Widget::KeyCallbackData key_callback_data;
        prj::vector<dw::Widget*> free_widgets;
        struc_751 field_C0;
        dw::init_data init_data;
        int64_t field_118;
        vec2 field_120;
        float_t field_128;
        struct DragBoundsControl* drag_bounds_control;
        prj::wstring name;
        RootKeySelection root_key_selection;

        dw::Shell* GetHoveredShell();
        void MakeShellFirst(dw::Shell* value);
        void MakeShellLast(dw::Shell* value);
        void RemoveShell(dw::Shell* value);
    };
}

static_assert(sizeof(dw_gui_detail::Display) == 0x160, "\"dw_gui_detail::Display\" struct should have a size of 0x160");

dw_gui_detail::Display*& dw_gui_detail_display = *(dw_gui_detail::Display**)0x0000000141190108;

int32_t& dword_140EDA6B8 = *(int32_t*)0x0000000140EDA6B8;

static void dw_gui_detail_display_set_selected_widget(dw::Widget* widget, bool set);

static void sub_1402EE3C0(rectangle rect, float_t border, int32_t type, color4u8* fill_color);

namespace dw {
    const dw::Colors& colors_default = *(dw::Colors*)0x000000141190130;

    static RowLayout* row_layout_vertical = new RowLayout(VERTICAL);

    DropDownListScrollBarSelectionListener& drop_down_list_scroll_bar_selection_listener
        = *(DropDownListScrollBarSelectionListener*)0x0000000140C987E8;
    ScrollBarTestSelectionListener& scroll_bar_test_Selection_listener
        = *(ScrollBarTestSelectionListener*)0x0000000140C987F0;

    Print*& print = *(Print**)0x00000001411900C8;

    dw::Colors& colors_current = *(dw::Colors*)0x00000001411901B0;

    p_Font& current_font = *(p_Font*)0x00000141190128;

    Font*& font_type_6x12 = *(Font**)0x00000001411902A0;
    Font*& font_type_8x12 = *(Font**)0x00000001411902A8;
    Font*& font_type_8x16 = *(Font**)0x00000001411902B0;
    Font*& font_type_10x16 = *(Font**)0x00000001411902B8;
    Font*& font_type_10x20 = *(Font**)0x00000001411902C0;
    Font*& font_type_12x24 = *(Font**)0x00000001411902C8;
    Font*& font_type_scroll_bar = *(Font**)0x00000001411902D0;

    p_Font& p_font_type_6x12 = *(p_Font*)0x00000001411901A8;
    p_Font& p_font_type_8x12 = *(p_Font*)0x0000000141190238;
    p_Font& p_font_type_8x16 = *(p_Font*)0x0000000141190240;
    p_Font& p_font_type_10x16 = *(p_Font*)0x0000000141190248;
    p_Font& p_font_type_10x20 = *(p_Font*)0x0000000141190250;
    p_Font& p_font_type_12x24 = *(p_Font*)0x0000000141190258;
    p_Font& p_font_type_scroll_bar = *(p_Font*)0x0000000141190260;

    prj::map<int32_t, p_Font*>& font_collection = *(prj::map<int32_t, p_Font*>*)0x00000001403024BE;

    Font::Font(int32_t font_handler_index, prj::string& name, bool half_width) {
        font = font_info(font_handler_index);
        this->name.assign(name);
        this->half_width = half_width;
    }

    Font::Font(int32_t font_handler_index, prj::string& name) {
        font = font_info(font_handler_index);
        this->name.assign(name);
        this->half_width = 0;
    }

    Font::~Font() {

    }

    float_t Font::GetFontGlyphHeight() {
        return font.glyph.y;
    }

    vec2 Font::GetFontGlyphSize() {
        vec2 size;
        size.x = GetFontGlyphWidth();
        size.y = GetFontGlyphHeight();
        return size;
    }

    float_t Font::GetFontGlyphWidth() {
        float_t glyph_height = font.glyph.x;
        if (half_width)
            return glyph_height * 0.5f;
        return glyph_height;
    }

    prj::string Font::GetName() {
        return name;
    }

    void Font::SetGlyphSize(float_t glyph_width, float_t glyph_height) {
        if (half_width)
            glyph_width *= 2.0f;
        font.set_glyph_size(glyph_width, glyph_height);
    }

    p_Font::p_Font() : ptr() {

    }

    p_Font::~p_Font() {

    }

    float_t p_Font::GetFontGlyphHeight() {
        return ptr->GetFontGlyphHeight();
    }

    vec2 p_Font::GetFontGlyphSize() {
        return ptr->GetFontGlyphSize();
    }

    float_t p_Font::GetFontGlyphWidth() {
        return ptr->GetFontGlyphWidth();
    }

    prj::string p_Font::GetName() {
        return ptr->GetName();
    }

    void p_Font::SetGlyphSize(float_t glyph_width, float_t glyph_height) {
        ptr->SetGlyphSize(glyph_width, glyph_height);
    }

    SelectionListener::SelectionListener() {

    }

    SelectionListener::~SelectionListener() {

    }

    SelectionAdapter::SelectionAdapter() {

    }

    SelectionAdapter::~SelectionAdapter() {

    }

    void SelectionAdapter::Callback(SelectionListener::CallbackData* data) {

    }

    void SelectionAdapter::Field_10(SelectionListener::CallbackData* data) {

    }

    SelectionListenerOnHook::SelectionListenerOnHook() : callback() {

    }

    SelectionListenerOnHook::SelectionListenerOnHook(SelectionListenerOnHook::CallbackFunc callback) {
        this->callback = callback;
    }

    SelectionListenerOnHook::~SelectionListenerOnHook() {

    }

    void SelectionListenerOnHook::Callback(SelectionListener::CallbackData* data) {
        callback(data->widget);
    }

    Widget::KeyCallbackData::KeyCallbackData() : widget(),
        modifier(), field_C(), field_D(), input() {
    }

    Widget::MouseCallbackData::MouseCallbackData() : widget(), input(), modifier() {

    }

    Widget::Widget(Widget* parent, Flags flags) : freed(),
        callback_data(), free_callback(), parent() {
        text.assign(L"NO_NAME");
        this->flags = flags;
        font = current_font;
    }

    Widget::~Widget() {
        Free();
    }

    void Widget::Draw() {

    }

    void Widget::Reset() {
        text.clear();
    }

    prj::wstring Widget::GetText() {
        return text;
    }

    void Widget::SetText(prj::wstring& str) {
        text.assign(str);
    }

    int32_t Widget::KeyCallback(const Widget::KeyCallbackData& data) {
        return 0;
    }

    int32_t Widget::MouseCallback(const Widget::MouseCallbackData& data) {
        return 0;
    }

    void Widget::SetSize(const vec2& value) {
        rect.size = value;
    }

    vec2 Widget::GetPos() {
        return rect.pos;
    }

    bool Widget::CheckHitPos(const vec2& hit_pos) {
        rectangle rect = GetRectangle();
        return rect.pos.x <= hit_pos.x && hit_pos.x < rect.pos.x + rect.size.x
            && rect.pos.y <= hit_pos.y && hit_pos.y < rect.size.y + rect.pos.y;
    }

    void Widget::Free() {
        if (freed)
            return;

        Reset();

        if (free_callback)
            free_callback(this);

        freed = true;

        if (dw_gui_detail_display)
            dw_gui_detail_display->free_widgets.push_back(this);
    }

    float_t Widget::GetFontGlyphHeight() {
        return font.GetFontGlyphHeight();
    }

    rectangle Widget::GetRectangle() {
        return { GetPos(), rect.size };
    }

    void Widget::SetText(prj::string& str) {
        std::string temp_utf8(str.data(), str.size());
        std::wstring temp_utf16(utf8_to_utf16(temp_utf8));
        SetText(prj::wstring(temp_utf16.data(), temp_utf16.size()));
    }

    void Widget::UpdateDraw() {
        print->sub_140302800();
        Draw();
    }

    void Widget::sub_1402F3770(Widget* widget) {
        if (!widget)
            return;

        rectangle v10 = widget->GetRectangle();
        rectangle v11 = dw_gui_detail_display->init_data.field_0;
        if (v11.pos.x > v10.pos.x || v10.pos.x + v10.size.x > v11.pos.x + v11.size.x
            || v11.pos.y > v10.pos.y || v10.pos.y + v10.size.y > v11.pos.y + v11.size.y) {
            vec2 v7 = v10.pos;
            if (v10.pos.x + v10.size.x >= v11.pos.x + v11.size.x)
                v7.x = v11.pos.x + v11.size.x - widget->rect.size.x;
            if (v10.pos.x < v11.pos.x || v7.x < 0.0f)
                v7.x = 0.0f;

            if (v10.pos.y + v10.size.y >= v11.pos.y + v11.size.y)
                v7.y = v11.pos.y + v11.size.y - widget->rect.size.y;
            if (v10.pos.y < v11.pos.y || v7.y < 0.0f)
                v7.y = 0.0f;
            widget->rect.pos = v7;
        }
    }

    Control::Control(Composite* parent, Flags flags) : Widget(parent, flags),
        field_68(), parent_comp(), parent_shell(), parent_menu() {
        parent_comp = parent;
        foreground_color = colors_current.foreground;
        background_color = colors_current.background;

        if (parent) {
            parent->controls.push_back(this);
            parent_shell = parent->parent_shell;
        }
    }

    Control::~Control() {

    }

    void Control::Draw() {

    }

    void Control::Reset() {
        parent_comp = 0;
        Widget::Reset();
    }

    int32_t Control::KeyCallback(const Widget::KeyCallbackData& data) {
        if (!Field_A0())
            return 0;

        /*if ((data.field_8 & 0x20000) && data.field_10 == 0x1000013)
            if (parent_menu)
                parent_menu->sub_1402F35E0();*/

        if (data.input[0] || (data.input[1] & 0x40000000)) {
            Widget::KeyCallbackData v8 = data;
            v8.widget = parent;
            for (KeyListener*& i : key_listener)
                i->Field_8(v8);
        }
        return 0;
    }

    int32_t Control::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!Field_A0())
            return 0;

        /*if (dw_gui_detail_display->field_C0.field_18 == 9 && (data.field_10 & 0x40) && !data.field_14)
            if (parent_menu)
                parent_menu->sub_1402F35E0();*/

        if (!data.input)
            return 0;

        if (data.input & 0xF0) {
            Widget::MouseCallbackData v19 = data;
            v19.widget = parent;
            v19.input = data.input & 0xF0;

            for (MouseListener* i : mouse_listener)
                i->Field_10(v19);
        }

        if (data.input & 0xF000000) {
            Widget::MouseCallbackData v19 = data;
            v19.widget = parent;
            v19.input = data.input & 0xF000000;

            for (MouseListener* i : mouse_listener)
                i->Field_18(v19);
        }

        if (data.input & 0xF000) {
            Widget::MouseCallbackData v19 = data;
            v19.widget = parent;
            v19.input = data.input & 0xF000;

            for (MouseListener* i : mouse_listener)
                i->Field_8(v19);
        }

        if (data.input & 0xF00) {
            Widget::MouseCallbackData v19 = data;
            v19.widget = parent;
            v19.input = data.input & 0xF00;

            for (MouseListener* i : mouse_listener)
                i->Field_20(v19);
        }

        if (data.input & 0x40000000) {
            Widget::MouseCallbackData v19 = data;
            v19.widget = parent;

            for (MouseMoveListener* i : mouse_move_listener)
                i->Field_8(v19);
        }
        return 0;
    }

    vec2 Control::GetPos() {
        vec2 pos = rect.pos;
        if (parent_comp) {
            rectangle parent_comp_bb = parent_comp->GetBoundingBox();
            pos += parent_comp->GetPos() + parent_comp_bb.pos;
            if (parent_comp->v_bar)
                pos.y -= parent_comp->v_bar->value;
        }
        return pos;
    }

    void Control::UpdateLayout() {
        SetSize(GetSize());
    }

    vec2 Control::GetSize() {
        return rect.size;
    }

    bool Control::Field_58() {
        return parent_comp->SetCurrentControl(this);
    }

    bool Control::Field_60() {
        return parent_comp->sub_1402F1E60(this);
    }

    void Control::SetFont(p_Font& value) {
        font = value;
    }

    p_Font& Control::GetFont() {
        return font;
    }

    void Control::SetForegroundColor(color4u8 value) {
        foreground_color = value;
    }

    void Control::SetBackgroundColor(color4u8 value) {
        background_color = value;
    }

    color4u8 Control::GetForegroundColor() {
        return foreground_color;
    }

    color4u8 Control::GetBackgroundColor() {
        return background_color;
    }

    bool Control::Field_98() {
        return !field_68;
    }

    bool Control::Field_A0() {
        if (parent_comp)
            return parent_comp->Field_A0() && Field_98();
        return Field_98();
    }

    void Control::Field_A8(bool value) {
        field_68 = !value;
    }

    Widget* Control::GetHitWidget(const vec2& hit_pos) {
        if (CheckHitPos(hit_pos))
            return this;
        return 0;
    }

    void Control::SetParentMenu(Menu* menu) {
        if (menu) {
            //menu->Field_60();
            parent_menu = menu;
        }
        else
            parent_menu = 0;
    }

    Scrollable::Scrollable(Composite* parent, Flags flags) : Control(parent, flags), h_bar(), v_bar() {
        if (flags & HORIZONTAL) {
            h_bar = new ScrollBar(this, HORIZONTAL);
            h_bar->SetText(L"h_bar");
        }

        if (flags & VERTICAL) {
            v_bar = new ScrollBar(this, VERTICAL);
            v_bar->SetText(L"v_bar");
        }
    }

    Scrollable::~Scrollable() {

    }

    void Scrollable::Draw() {
        if (v_bar)
            v_bar->UpdateDraw();
    }

    void Scrollable::Reset() {
        if (h_bar) {
            h_bar->Free();
            h_bar = 0;
        }

        if (v_bar) {
            v_bar->Free();
            v_bar = 0;
        }

        Control::Reset();
    }

    int32_t Scrollable::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!v_bar)
            return Control::MouseCallback(data);

        bool v4 = v_bar->field_A6;
        int32_t ret = v_bar->MouseCallback(data);
        if (v4 != v_bar->field_A6)
            dw_gui_detail_display_set_selected_widget(v_bar, v_bar->field_A6);
        return ret;
    }

    void Scrollable::SetSize(const vec2& value) {
        Widget::SetSize(value);

        vec2 size = value;
        if (flags & FLAG_800)
            size -= 2.0f * 2.0f;

        if (v_bar) {
            v_bar->rect.pos.x = size.x - v_bar->rect.size.x;
            v_bar->rect.pos.y = 0.0f;
            v_bar->SetWidth(size.y);
        }
    }

    void Scrollable::SetFont(p_Font& value) {
        font = value;
        if (h_bar)
            h_bar->SetFont(value);
        if (v_bar)
            v_bar->SetFont(value);
    }

    Widget* Scrollable::GetHitWidget(const vec2& hit_pos) {
        if (!CheckHitPos(hit_pos))
            return 0;

        if (v_bar && v_bar->CheckHitPos(hit_pos))
            return v_bar;
        return this;
    }

    rectangle Scrollable::GetBoundingBox() {
        rectangle rect;
        rect.size = this->rect.size;

        if (flags & FLAG_800) {
            rect.pos = 2.0f;
            rect.size -= 2.0f * 2.0f;
        }

        if (v_bar)
            rect.size.x -= v_bar->rect.size.x;
        return rect;
    }

    rectangle Scrollable::GetScrollableRectangle() {
        vec2 pos = GetPos();
        rectangle rect = GetBoundingBox();
        rect.pos += pos;
        return rect;
    }

    Composite::Composite(Composite* parent, Flags flags) : Scrollable(parent, flags) {
        layout = row_layout_vertical;
        current_control = -1;
    }

    Composite::~Composite() {

    }

    void Composite::Draw() {
        rectangle rect = GetBoundingBox();
        rect.pos = 0.0f;

        if (v_bar)
            rect.pos.y = v_bar->value;

        for (Control*& i : controls)
            if ((i->rect.pos.x + i->rect.size.x) > rect.pos.x && (rect.pos.x + rect.size.x) > i->rect.pos.x
                && (i->rect.pos.y + i->rect.size.y) > rect.pos.y && (rect.pos.y + rect.size.y) > i->rect.pos.y)
                i->UpdateDraw();

        Scrollable::Draw();
    }

    void Composite::Reset() {
        for (Control*& i : controls)
            i->Free();

        controls.clear();

        Scrollable::Reset();
    }

    int32_t Composite::KeyCallback(const Widget::KeyCallbackData& data) {
        if (current_control != -1 && current_control < controls.size())
            return controls[current_control]->KeyCallback(data);
        else
            return 0;
    }

    int32_t Composite::MouseCallback(const Widget::MouseCallbackData& data) {
        for (Control*& i : controls)
            if (i->CheckHitPos(data.pos))
                return i->MouseCallback(data);

        return Scrollable::MouseCallback(data);
    }

    void Composite::SetSize(const vec2& size) {
        Scrollable::SetSize(size);
        layout->SetSize(this);
    }

    void Composite::UpdateLayout() {
        SetSize(GetSize());
    }

    vec2 Composite::GetSize() {
        return layout->GetSize(this);
    }

    bool Composite::Field_58() {
        current_control = -1;
        return Control::Field_58();
    }

    bool Composite::Field_60() {
        return current_control == -1 && Field_C0();
    }

    Widget* Composite::GetHitWidget(const vec2& hit_pos) {
        if (!CheckHitPos(hit_pos))
            return Scrollable::GetHitWidget(hit_pos);

        for (Control*& i : controls) {
            Widget* widget = i->GetHitWidget(hit_pos);
            if (widget)
                return widget;
        }

        return this;
    }

    bool Composite::Field_C0() {
        return parent_comp->sub_1402F1E60(this);
    }

    bool Composite::SetCurrentControl(Control* control) {
        current_control = GetControlIndex(control);
        return parent_comp->SetCurrentControl(this);
    }

    size_t Composite::GetControlIndex(Control* control) {
        for (Control*& i : controls)
            if (i == control)
                return &i - controls.data();
        return -1;
    }

    void Composite::SetLayout(Layout* value) {
        layout = value;
    }

    bool Composite::sub_1402F1E60(Control* control) {
        return GetControlIndex(control) == current_control && Field_C0();
    }

    Shell::Shell(Shell* parent, Flags flags) : Composite(parent, flags), disp(), disp_callback(),
        hide_callback(), field_128(), destroy(), close_button(), field_170(), field_178() {
        parent_shell = this;

        if (flags & CLOSE_BUTTON) {
            close_button = new ShellCloseButton(this);
            if (controls.size())
                controls.pop_back();
        }

        if (parent) {
            parent_comp = parent;
            parent->children.push_back(this);
        }

        dw_gui_detail_display->shells.push_back(this);
    }

    Shell::~Shell() {

    }

    void Shell::Draw() {
        if (!GetDisp())
            return;

        rectangle v12 = GetRectangle();
        bool active = dw_gui_detail_display->selected_shell == this;

        print->SetColor(active
            ? colors_current.active_border_color
            : colors_current.inactive_border_color);
        print->sub_140301390(v12, 2.0f);

        if (flags & CHECKBOX) {
            rectangle v11 = v12;
            v11.size.y = current_font.GetFontGlyphHeight() + 2.0f + 2.0f;
            print->sub_140301390(v11, 2.0f);

            rectangle v10;
            v10.pos = v11.pos + 2.0f;
            v10.size = v11.size - 2.0f * 2.0f;

            print->SetFillColor(active
                ? colors_current.title_background
                : colors_current.title_inactive_background);
            print->FillRectangle(v10);

            print->SetColor(active
                ? colors_current.title_foreground
                : colors_current.title_inactive_foreground);
            print->SetFont(&current_font);
            print->SetClipData(v10);

            print->PrintText(GetText(), v10.pos.x, v10.pos.y);

            print->sub_140302800();
            if (close_button)
                close_button->UpdateDraw();
        }

        print->SetFillColor(colors_current.background);
        print->FillRectangle(GetScrollableRectangle());
        print->SetColor(colors_current.foreground);
        Composite::Draw();

        if (field_178) {
            field_170 = true;
            field_178->UpdateDraw();
            field_170 = false;
        }
    }

    void Shell::Reset() {
        prj::vector<Widget*> vec;
        vec.assign(children.begin(), children.end());
        children.clear();

        for (Widget* i : vec)
            i->Free();

        for (Menu* i : menus)
            ;//i->Free();
        menus.clear();

        if (close_button) {
            close_button->Free();
            close_button = 0;
        }

        if (parent_comp) {
            Shell* parent_shell = dynamic_cast<Shell*>(parent_comp);
            if (parent_shell) {
                auto i = parent_shell->children.begin();
                auto i_end = parent_shell->children.end();
                while (i != i_end)
                    if (*i == this) {
                        parent_shell->children.erase(i);
                        break;
                    }
            }
        }

        dw_gui_detail_display->RemoveShell(this);

        Composite::Reset();
    }

    int32_t Shell::KeyCallback(const Widget::KeyCallbackData& data) {
        if (!GetDisp())
            return 0;

        if ((data.modifier & 0x10000) && data.input[0] == 0x100000D || (data.input[0] & 0x2000)) {
            Hide();
            return 0;
        }

        return Composite::KeyCallback(data);
    }

    int32_t Shell::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!GetDisp())
            return 0;

        bool (FASTCALL * sub_140192D00)() = (bool (FASTCALL*)())0x0000000140192D00;

        if (data.input & 0xF0) {
            if (!sub_140192D00())
                return 0;

            dw_gui_detail_display->MakeShellFirst(this);
        }

        if ((uint16_t)dw_gui_detail_display->field_C0.field_18 >= 1 && (uint16_t)dw_gui_detail_display->field_C0.field_18 <= 8) {
            if (data.input & 0x10) {
                if (sub_140192D00()) {
                    dw_gui_detail_display->field_C0.field_18 |= 0x20000000;
                    dw_gui_detail_display->move_resize_shell = dw_gui_detail_display->selected_shell;
                }
                return 0;
            }
        }
        else if ((uint16_t)dw_gui_detail_display->field_C0.field_18 == 10) {
            if (close_button && close_button->CheckHitPos(data.pos))
                return close_button->MouseCallback(data);

            if (data.input & 0x10) {
                if (sub_140192D00()) {
                    dw_gui_detail_display->field_C0.field_18 |= 0x40000000;
                    dw_gui_detail_display->move_resize_shell = dw_gui_detail_display->selected_shell;
                }
                return 0;
            }
        }

        return Composite::MouseCallback(data);
    }

    void Shell::SetSize(const vec2& size) {
        Widget::SetSize(size);
        if (flags & CHECKBOX) {
            if (close_button) {
                vec2 size = close_button->GetSize();
                close_button->rect.pos.x = rect.size.x - (size.x + 2.0f);
                close_button->rect.pos.y = 2.0f;
            }
        }

        rectangle bounding_box = GetBoundingBox();
        if (v_bar) {
            v_bar->rect.pos.x = bounding_box.pos.x + bounding_box.size.x - v_bar->rect.size.x;
            v_bar->rect.pos.y = bounding_box.pos.y;
            v_bar->SetWidth(bounding_box.size.y);
        }

        layout->SetSize(this);
    }

    vec2 Shell::GetPos() {
        return rect.pos;
    }

    vec2 Shell::GetSize() {
        vec2 size = Composite::GetSize() + 2.0f * 2.0f;
        if (flags & CHECKBOX)
            size.y += current_font.GetFontGlyphHeight() + 2.0f;
        return size;
    }

    bool Shell::Field_58() {
        return this == dw_gui_detail_display->selected_shell && GetDisp();
    }

    Widget* Shell::GetHitWidget(const vec2& hit_pos) {
        rectangle rect = GetRectangle();
        if (rect.pos.x > hit_pos.x || hit_pos.x >= rect.pos.x + rect.size.x
            || rect.pos.y > hit_pos.y || hit_pos.y >= rect.size.y + rect.pos.y)
            return 0;

        if (close_button && close_button->CheckHitPos(hit_pos))
            return close_button;

        Widget* widget = Composite::GetHitWidget(hit_pos);
        if (widget)
            return widget;

        return this;
    }

    rectangle Shell::GetBoundingBox() {
        rectangle rect;
        rect.pos = 2.0f;
        rect.size = this->rect.size - 2.0f * 2.0f;

        if (flags & CHECKBOX) {
            rect.size.y -= current_font.GetFontGlyphHeight() + 2.0f;
            rect.pos.y += current_font.GetFontGlyphHeight() + 2.0f;
        }

        if (v_bar)
            rect.size.x -= v_bar->rect.size.x;
        return rect;
    }

    bool Shell::Field_C0() {
        return Field_58();
    }

    bool Shell::SetCurrentControl(Control* control) {
        current_control = GetControlIndex(control);
        return Field_58();
    }

    void Shell::Hide() {
        destroy = true;
    }

    void Shell::Disp() {
        SetDisp(true);
        dw_gui_detail_display->MakeShellFirst(this);
        dw_gui_detail_display->focused_shell = this;
        dw_gui_detail_display->set_focus = true;
    }

    bool Shell::GetDisp() {
        return disp;
    }

    void Shell::SetDisp(bool value) {
        if (disp == value)
            return;

        disp = value;
        if (disp) {
            dw_gui_detail_display->MakeShellFirst(this);
            dw_gui_detail_display->focused_shell = this;
            dw_gui_detail_display->set_focus = true;
            if (disp_callback)
                disp_callback(this);
        }
        else {
            dw_gui_detail_display->find_focus = true;
            if (hide_callback)
                hide_callback(this);
        }
    }

    void Shell::sub_1402E43C0(Widget* widget) {
        if (!field_178)
            field_178 = widget;
    }

    void Shell::sub_1402E61F0(Widget* widget) {
        if (field_178 && field_178 == widget)
            field_178 = 0;
    }

    void Shell::sub_1402F38B0() {
        sub_1402F3770(this);
        Disp();
    }

    KeyListener::KeyListener() {

    }

    KeyListener::~KeyListener() {

    }

    KeyAdapter::KeyAdapter() {

    }

    KeyAdapter::~KeyAdapter() {

    }

    void KeyAdapter::Field_8(const Widget::KeyCallbackData& data) {

    }

    void KeyAdapter::Field_10(const Widget::KeyCallbackData& data) {

    }

    void KeyAdapter::Field_18(const Widget::KeyCallbackData& data) {

    }

    MouseListener::MouseListener() {

    }

    MouseListener::~MouseListener() {

    }

    MouseAdapter::MouseAdapter() {

    }

    MouseAdapter::~MouseAdapter() {

    }

    void MouseAdapter::Field_8(const Widget::MouseCallbackData& data) {

    }

    void MouseAdapter::Field_10(const Widget::MouseCallbackData& data) {

    }

    void MouseAdapter::Field_18(const Widget::MouseCallbackData& data) {

    }

    void MouseAdapter::Field_20(const Widget::MouseCallbackData& data) {

    }

    MouseMoveListener::MouseMoveListener() {

    }

    MouseMoveListener::~MouseMoveListener() {

    }

    Layout::Layout() {

    }

    Layout::~Layout() {

    }

    FillLayout::FillLayout(Flags flags) {
        this->flags = flags;
    }

    FillLayout::~FillLayout() {

    }

    vec2 FillLayout::GetSize(Composite* comp) {
        if (!comp->controls.size()) {
            vec2 size = comp->rect.size;
            if (comp->v_bar)
                size.x += comp->v_bar->rect.size.x;
            return size;
        }

        vec2 size = 0.0f;
        if (flags == VERTICAL)
            for (Control* i : comp->controls) {
                vec2 _size = i->GetSize();
                size.x = max_def(_size.x, size.x);
            }
        else
            for (Control* i : comp->controls) {
                vec2 _size = i->GetSize();
                size.y = max_def(_size.y, size.y);
            }

        if (comp->v_bar)
            size.x += comp->v_bar->rect.size.x;
        return size;
    }

    void FillLayout::SetSize(Composite* comp) {
        rectangle comp_bb = comp->GetBoundingBox();
        if (!comp->controls.size())
            return;

        float_t size = (float_t)(int64_t)comp->controls.size();
        if (size < 0.0f)
            size += (float_t)UINT64_MAX;

        if (flags == VERTICAL) {
            float_t pos = 0.0f;
            size = comp_bb.size.y / size;
            for (Control* i : comp->controls) {
                i->rect.pos = { 0.0f, pos };
                i->SetSize({ comp_bb.size.x, size });
                pos += size;
            }
        }
        else {
            float_t pos = 0.0f;
            size = comp_bb.size.x / size;
            for (Control* i : comp->controls) {
                i->rect.pos = { pos, 0.0f };
                i->SetSize({ size,  comp_bb.size.y });
                pos += size;
            }
        }
    }

    GraphLayout::GraphLayout(Flags flags) {
        this->flags = flags;
    }

    GraphLayout::~GraphLayout() {

    }

    vec2 GraphLayout::GetSize(Composite* comp) {
        return comp->rect.size;
    }

    void GraphLayout::SetSize(Composite* comp) {
        const rectangle rect = comp->rect;
        for (Control* i : comp->controls) {
            i->rect.pos = rect.pos;
            i->SetSize(rect.size);
        }
    }

    GridLayout::GridLayout(size_t columns) : rows() {
        this->columns = columns;
        spacing = 2.0f;
    }

    GridLayout::~GridLayout() {

    }

    vec2 GridLayout::GetSize(Composite* comp) {
        prj::vector<Control*>& controls = comp->controls;
        size_t controls_count = controls.size();
        if (!controls_count) {
            vec2 size = comp->rect.size;
            if (comp->v_bar)
                size.x += comp->v_bar->rect.size.x;
            return size;
        }

        rows = (controls_count + columns - 1) / columns;
        rows = rows ? rows : 1;

        column_size.resize(columns);
        row_size.resize(rows);

        size_t control = 0;
        for (size_t row = 0; row < rows && control < controls_count; row++)
            for (size_t column = 0; column < columns && control < controls_count; column++, control++) {
                vec2 size = controls.data()[control]->GetSize();
                column_size.data()[column] = max_def(size.x, column_size.data()[column]);
                row_size.data()[row] = max_def(size.y, row_size.data()[row]);
            }

        vec2 size;
        size.x = (float_t)(int64_t)(columns - 1);
        if (size.x < 0.0f)
            size.x += (float_t)UINT64_MAX;

        size.y = (float_t)(int64_t)(rows - 1);
        if (size.y < 0.0f)
            size.y += (float_t)UINT64_MAX;

        size *= spacing;

        float_t size_x = size.x;
        for (float_t i : column_size)
            size_x += i;
        size.x = size_x;

        float_t size_y = size.y;
        for (float_t i : row_size)
            size_y += i;
        size.y = size_y;

        if (comp->v_bar)
            size.x += comp->v_bar->rect.size.x;
        return size;
    }

    void GridLayout::SetSize(Composite* comp) {
        prj::vector<Control*>& controls = comp->controls;
        size_t controls_count = controls.size();

        rectangle comp_bb = comp->GetBoundingBox();
        if (!controls_count)
            return;

        if (controls_count == 1) {
            Scrollable* scrollable = dynamic_cast<Scrollable*>(comp->controls.front());
            if (scrollable) {
                scrollable->rect.pos = 0.0f;
                scrollable->SetSize(comp_bb.size);
                return;
            }
        }

        size_t control = 0;
        vec2 pos = 0.0f;
        for (size_t row = 0; row < rows && control < controls_count; row++) {
            vec2 _pos = pos;
            for (size_t column = 0; column < columns && control < controls_count; column++, control++) {
                comp->controls.data()[control]->rect.pos = _pos;
                comp->controls.data()[control]->SetSize({ column_size.data()[column], row_size.data()[row] });
                _pos.x += column_size.data()[column] + spacing.x;
            }
            pos.y += row_size.data()[row] + spacing.y;
        }
    }

    RowLayout::RowLayout(Flags flags) {
        this->flags = flags;
        spacing = 2.0f;
    }

    RowLayout::~RowLayout() {

    }

    vec2 RowLayout::GetSize(Composite* comp) {
        if (!comp->controls.size()) {
            vec2 size = comp->rect.size;
            if (comp->v_bar)
                size.x += comp->v_bar->rect.size.x;
            return size;
        }

        vec2 size = 0.0f;
        if (flags == VERTICAL) {
            for (Control* i : comp->controls) {
                vec2 _size = i->GetSize();
                size.x = max_def(_size.x, size.x);
                size.y += _size.y + spacing;
            }
            size.y -= spacing;
        }
        else {
            for (Control* i : comp->controls) {
                vec2 _size = i->GetSize();
                size.x += _size.x + spacing;
                size.y = max_def(_size.y, size.y);
            }
            size.x -= spacing;
        }

        if (comp->v_bar)
            size.x += comp->v_bar->rect.size.x;
        return size;
    }

    void RowLayout::SetSize(Composite* comp) {
        rectangle comp_bb = comp->GetBoundingBox();
        if (!comp->controls.size())
            return;

        if (comp->controls.size() == 1) {
            Scrollable* scrollable = dynamic_cast<Scrollable*>(comp->controls.front());
            if (scrollable) {
                scrollable->rect.pos = 0.0f;
                scrollable->SetSize(comp_bb.size);
                return;
            }
        }

        float_t pos = 0.0f;
        if (flags == VERTICAL)
            for (Control*& i : comp->controls) {
                i->rect.pos = { 0.0f, pos };
                float_t size_y = i->GetSize().y;
                i->SetSize({ comp_bb.size.x, size_y });
                pos += size_y + spacing;
            }
        else
            for (Control*& i : comp->controls) {
                i->rect.pos = { pos, 0.0f };
                float_t size_x = i->GetSize().x;
                i->SetSize({ size_x,  comp_bb.size.y });
                pos += size_x + spacing;
            }
    }

    Group::Group(Composite* parent, Flags flag) : Composite(parent, (Flags)(flags & ~(VERTICAL | HORIZONTAL))) {

    }

    Group::~Group() {

    }

    void Group::Draw() {
        print->SetFont(&font);

        rectangle rect = GetRectangle();

        float_t half_glyph_height = prj::floorf(font.GetFontGlyphHeight() * 0.5f);
        rect.pos.y += half_glyph_height;
        rect.size.y -= half_glyph_height;

        print->SetColor(colors_current.border_color);
        print->sub_140301390(rect, 2.0f);

        rect.size += -1.0f;
        print->SetColor(color_black);
        print->sub_140301390(rect, 1.0f);

        if (GetText().size()) {
            rectangle rect = GetRectangle();

            rect.pos.x += 2.0f;
            rect.pos.y = rect.pos.y;
            rect.size.x += 2.0f * -2.0f;
            rect.size.y = font.GetFontGlyphHeight();

            vec2 text_size = print->GetTextSize(GetText());
            rect.size.x = min_def(text_size.x, rect.size.x);

            print->SetFillColor(colors_current.background);
            print->FillRectangle(rect);

            print->SetColor(colors_current.foreground);
            print->SetClipData(rect);
            print->PrintText(GetText(), rect.pos.x, rect.pos.y);
        }

        Composite::Draw();
    }

    vec2 Group::GetSize() {
        vec2 size = Composite::GetSize();
        size.x += 2.0f * 2.0f;
        size.y += font.GetFontGlyphHeight() + 2.0f;
        return size;
    }

    rectangle Group::GetBoundingBox() {
        float_t glyph_height = font.GetFontGlyphHeight();

        rectangle rect = Scrollable::GetBoundingBox();
        rect.pos.x += 2.0f;
        rect.pos.y += glyph_height;
        rect.size.x -= 2.0f * 2.0f;
        rect.size.y -= glyph_height + 2.0f;
        return rect;
    }

    Label::Label(Composite* parent, Flags flags) : Control(parent, flags) {

    }

    Label::~Label() {

    }

    void Label::Draw() {
        rectangle rect = GetRectangle();
        print->SetFont(&font);
        vec2 text_size = print->GetTextSize(GetText());

        float_t pos_x;
        if (flags & FLAG_4000)
            pos_x = rect.pos.x;
        else if (flags & FLAG_20000)
            pos_x = rect.pos.x + rect.size.x - text_size.x;
        else
            pos_x = rect.pos.x + (rect.size.x - text_size.x) * 0.5f;
        float_t pos_y = rect.pos.y + (rect.size.y - text_size.y) * 0.5f;

        print->SetColor(Field_A0()
            ? colors_current.foreground
            : colors_current.disable_foreground);
        print->SetClipData(rect);
        print->SetFont(&font);
        print->PrintText(GetText(), pos_x, pos_y);
    }

    vec2 Label::GetSize() {
        print->SetFont(&font);
        return  print->GetTextSize(text) + 2.0f * 2.0f;
    }

    Button::Button(Composite* parent, Flags flags) : Control(parent, flags), value(), callback(), field_100() {
        field_104 = 1;
        SetSize(GetSize());
    }

    Button::~Button() {

    }

    void Button::Draw() {
        print->SetFont(&this->font);
        vec2 checkbox_size = print->GetTextSize(L"[X]");

        rectangle v6 = GetRectangle();
        rectangle v8 = v6;

        if (flags & (FLAG_8 | MULTISELECT)) {
            int32_t v3;
            if ((field_100 & 0x01) && (field_100 & 0x02)) {
                v3 = 4;
                v8.pos.x += 2.0f;
                v8.size.x += -2.0f;
            }
            else
                v3 = 2;

            sub_1402EE3C0(v6, 2.0f, v3, &colors_current.button_background);

            v8.pos += 2.0f;
            v8.size -= 2.0f * 2.0f;
        }
        else if (flags & CHECKBOX) {
            print->SetColor(colors_current.button_foreground);
            print->SetClipData(v8);
            print->PrintText(value ? L"[X]" : L"[ ]", v8.pos.x, v8.pos.y);

            v8.size.x -= checkbox_size.x;
            v8.pos.x += checkbox_size.x;
        }
        else if (flags & RADIOBUTTON) {
            print->SetColor(colors_current.button_foreground);
            print->SetClipData(v8);
            print->PrintText(value ? L"(*)" : L"( )", v8.pos.x, v8.pos.y);

            v8.size.x -= checkbox_size.x;
            v8.pos.x += checkbox_size.x;
        }

        prj::wstring text = GetText();

        print->SetFont(&font);
        vec2 text_size = print->GetTextSize(text);

        if (!(flags & (CHECKBOX | RADIOBUTTON))) {
            float_t v5 = (v8.size.x - text_size.x) * 0.5f;
            v8.pos.x = v8.pos.x + v5;
            v8.size.x = v8.size.x + (v5 * -2.0f);
        }

        if (Field_A0()) {
            print->SetColor(colors_current.button_foreground);
            print->SetClipData(v8);
            print->PrintText(text, v8.pos.x, v8.pos.y);
            if (Field_60()) {
                print->SetColor(colors_current.focus_border_color);
                print->sub_140301390(v8, 1.0);
            }
        }
        else {
            print->SetColor(colors_current.button_disable_foreground);
            print->SetClipData(v8);
            print->PrintText(text, v8.pos.x, v8.pos.y);
        }
    }

    int32_t Button::KeyCallback(const Widget::KeyCallbackData& data) {
        if (!Field_A0())
            return 0;

        if (data.input[0] != 0x0D && data.input[0] != 0x20)
            return Control::KeyCallback(data);

        if (flags & CHECKBOX)
            SetValue(!value);
        else if (flags & RADIOBUTTON)
            SetValue(true);

        if (callback)
            callback(this);

        SelectionListener::CallbackData callback_data = this;
        for (SelectionListener*& i : selection_listeners)
            i->Callback(&callback_data);

        field_100 &= ~0x102;
        return Control::KeyCallback(data);
    }

    int32_t Button::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!Field_A0())
            return 0;

        field_100 &= ~0x01;
        if (CheckHitPos(data.pos))
            field_100 |= 0x01;

        if (field_100 & 0x01) {
            if ((data.input & 0xF0) && !Field_60())
                Field_58();

            if (data.input & 0x100) {
                if (field_100 & 0x02)
                    field_100 = (field_100 & ~0x02) | 0x100;
            }
            else if (data.input & 0x10)
                field_100 |= 0x2;
        }

        if (field_100 & 0x100) {
            field_104--;

            if (flags & CHECKBOX)
                SetValue(!value);
            else if (flags & RADIOBUTTON)
                SetValue(true);

            if (callback)
                callback(this);

            SelectionListener::CallbackData callback_data = this;
            for (SelectionListener*& i : selection_listeners)
                i->Callback(&callback_data);

            field_100 &= ~0x102;
            field_104 = 1;
        }

        return Control::MouseCallback(data);
    }

    vec2 Button::GetSize() {
        print->SetFont(&font);
        vec2 size = print->GetTextSize(text);

        if (flags & FLAG_8) {
            size.x += 2.0f;
            size.x += 2.0f * 2.0f;
            size.y += 2.0f * 2.0f;
        }
        else if (flags & RADIOBUTTON) {
            print->SetFont(&font);
            size.x += print->GetTextSize("(X)").x;
        }
        else if (flags & CHECKBOX) {
            print->SetFont(&font);
            size.x += print->GetTextSize("[X]").x;
        }
        return size;
    }

    void Button::AddSelectionListener(SelectionListener* value) {
        selection_listeners.push_back(value);
    }

    void Button::SetValue(bool value) {
        this->value = value;
        if (!(flags & RADIOBUTTON))
            return;

        for (Control* i : parent_comp->controls) {
            Button* button = (Button*)dynamic_cast<Button*>(i);
            if (button && button != this && button->flags & RADIOBUTTON)
                button->value = false;
        }
    }

    List::List(Composite* parent, Flags flags) : Scrollable(parent, flags), field_148() {
        hovered_item = -1;
        selected_item = -1;

        max_items = -1;
        if (flags & VERTICAL)
            max_items = 10;
    }

    List::~List() {

    }

    void List::Draw() {
        if (flags & FLAG_800) {
            rectangle rect;
            color4u8* v3;
            if (Field_A0()) {
                rect = GetRectangle();
                v3 = 0;
            }
            else {
                rect = GetRectangle();
                v3 = &colors_current.disable_background;
            }
            sub_1402EE3C0(rect, 2.0f, 2, v3);
        }

        Scrollable::Draw();

        float_t glyph_height = GetFontGlyphHeight();

        float_t pos_y = 0.0f;
        if (v_bar)
            pos_y = prj::floorf(v_bar->value);
        print->SetFont(&font);

        rectangle v21 = GetScrollableRectangle();

        pos_y = -pos_y;
        print->SetClipData(v21);

        size_t items_count = items.size();
        for (size_t i = 0; i < items_count; i++, pos_y += glyph_height) {
            rectangle v22;
            v22.pos.x = v21.pos.x;
            v22.pos.y = v21.pos.y + pos_y;
            v22.size.x = v21.size.x;
            v22.size.y = glyph_height;
            if (v21.pos.x + v21.size.x <= v21.pos.x
                || v21.pos.y + pos_y + glyph_height <= v21.pos.y
                || v21.size.y + v21.pos.y <= v21.pos.y + pos_y)
                continue;

            bool selected = false;
            bool hovered = false;

            color4u8 color;
            if (!field_148) {
                if ((flags & MULTISELECT) && selected_item == i || (flags & FLAG_2) && selections[i])
                    selected = true;

                if (Field_60())
                    hovered = hovered_item == i;

                if (selected) {
                    print->SetFillColor(colors_current.selection_background);
                    print->FillRectangle(v22);
                    color = colors_current.selection_foreground;
                }
                else if (Field_A0())
                    color = colors_current.foreground;
                else
                    color = colors_current.disable_foreground;
            }
            else {
                if (hovered_item == i) {
                    print->SetFillColor(colors_current.popup_selection_background);
                    print->FillRectangle(v22);
                    color = colors_current.popup_selection_foreground;
                }
                else if (field_148)
                    color = colors_current.popup_foreground;
                else if (Field_A0())
                    color = colors_current.foreground;
                else
                    color = colors_current.disable_foreground;
            }

            print->SetColor(color);
            print->SetFont(&font);
            print->PrintText(items[i], v21.pos.x, pos_y + v21.pos.y);

            if (hovered) {
                print->SetColor(colors_current.focus_border_color);
                print->sub_140301390(v22, 1.0f);
            }
        }
    }

    void List::Reset() {
        items.clear();
        items.shrink_to_fit();
        selections.clear();
        Scrollable::Reset();
    }

    int32_t List::KeyCallback(const Widget::KeyCallbackData& data) {
        size_t items_count = items.size();
        if (!data.input[0] || !items_count)
            return Control::KeyCallback(data);

        size_t hovered_item = this->hovered_item;

        SelectionListener::CallbackData v12 = v_bar;

        size_t max_items_visible = GetMaxItemsVisible();
        switch (data.input[0]) {
        case 0x1000001:
            if (hovered_item == -1)
                hovered_item = 0;
            else if (hovered_item)
                hovered_item--;

            if (sub_1402F1F20(hovered_item) && !sub_1402F1F20(hovered_item))
                v12.field_8 = data.input[0];
            break;
        case 0x1000002:
            if (hovered_item == -1)
                hovered_item = 0;
            else if (hovered_item < items_count - 1)
                hovered_item++;

            if (sub_1402F1F20(hovered_item) && !sub_1402F1F20(hovered_item))
                v12.field_8 = data.input[0];
            break;
        case 0x1000005:
            if (hovered_item == -1 || hovered_item < max_items_visible)
                hovered_item = 0;
            else
                hovered_item -= max_items_visible;
            v12.field_8 = data.input[0];
            break;
        case 0x1000006:
            if (hovered_item == -1)
                hovered_item = 0;
            else if (max_items_visible >= items_count
                || hovered_item > items_count - max_items_visible - 1)
                hovered_item = items_count - 1;
            else
                hovered_item += max_items_visible;
            v12.field_8 = data.input[0];
            break;
        case 0x1000007:
            hovered_item = 0;
            v12.field_8 = data.input[0];
            break;
        case 0x1000008:
            hovered_item = items_count - 1;
            v12.field_8 = data.input[0];
            break;
        }

        if (hovered_item < items_count && hovered_item != -1 && this->hovered_item != hovered_item) {
            SetItemIndex(hovered_item);

            SelectionListener::CallbackData callback_data = parent;
            for (SelectionListener*& i : selection_listeners)
                i->Callback(&callback_data);

            if (v12.field_8 && v_bar)
                ScrollBar::sub_1402E6CC0(v12);

            sub_1402F9C90(hovered_item);
        }

        return Control::KeyCallback(data);
    }

    int32_t List::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!Field_A0() || !CheckHitPos(data.pos))
            return 0;

        if (!field_148 && (data.input & 0xF0) && !Field_60())
            Field_58();

        rectangle v27 = GetScrollableRectangle();
        if (v27.pos.x > data.pos.x || data.pos.x >= v27.size.x + v27.pos.x
            || v27.pos.y > data.pos.y || data.pos.y >= v27.size.y + v27.pos.y)
            return Scrollable::MouseCallback(data);

        size_t hovered_item = this->hovered_item;
        if (field_148 && (data.input & 0x40000000) || (data.input & 0x10)) {
            float_t glyph_height = GetFontGlyphHeight();
            float_t v10 = (float_t)sub_1402EF570();
            if (v10 < 0.0f)
                v10 += (float_t)UINT64_MAX;

            size_t v11 = 0;
            float_t v12 = (data.pos.y - (v27.pos.y - v10 * glyph_height)) / glyph_height;

            if (v12 >= (float_t)INT64_MAX) {
                v12 -= (float_t)INT64_MAX;
                if (v12 < (float_t)INT64_MAX)
                    v11 = 0x8000000000000000u;
            }

            size_t hovered_item = v11 + (int64_t)v12;
            if (hovered_item < items.size())
                this->hovered_item = hovered_item;
        }

        if (hovered_item < this->items.size()) {
            if (data.input & 0x10) {
                if (data.modifier & 0x20000) {
                    ResetSelectedItem();
                    size_t end = this->hovered_item;
                    size_t begin = hovered_item;
                    if (end < begin) {
                        begin = this->hovered_item;
                        end = hovered_item;
                    }

                    SetSelectedItems(begin, end);
                }
                else if (data.modifier & 0x40000) {
                    if (CheckItemSelected(hovered_item))
                        UnsetSelectedItem(hovered_item);
                    else
                        SetSelectedItem(hovered_item);
                }
                else {
                    ResetSelectedItem();
                    SetSelectedItem(this->hovered_item);
                }

                SelectionListener::CallbackData v26 = parent;
                for (SelectionListener*& i : selection_listeners)
                    i->Callback(&v26);
            }

            if (data.input & 0x1000) {
                SelectionListener::CallbackData v26 = parent;
                for (SelectionListener*& i : selection_listeners)
                    i->Field_10(&v26);
            }
        }

        if ((data.input & 0x100000) || (data.input & 0x200000))
            return Scrollable::MouseCallback(data);
        return 0;
    }

    void List::SetSize(const vec2& size) {
        Scrollable::SetSize(size);
        SetScrollBarParams();
    }

    vec2 List::GetSize() {
        print->SetFont(&font);

        float_t max_text_size = 0.0f;
        for (prj::wstring& i : items) {
            vec2 text_size = print->GetTextSize(i);
            max_text_size = max_def(text_size.x, max_text_size);
        }

        size_t max_items_visible = GetMaxItemsVisible();
        size_t items_count = items.size();
        float_t glyph_height = font.GetFontGlyphHeight();

        float_t v14 = (float_t)(int64_t)min_def(max_items_visible, items_count);
        if (v14 < 0.0f)
            v14 += (float_t)UINT64_MAX;

        vec2 size = { max_text_size, glyph_height * v14 };
        if (v_bar)
            size.x += v_bar->rect.size.x;

        if (flags & FLAG_800)
            size += 2.0f * 2.0f;
        return size;
    }

    void List::AddItem(const prj::string& str) {
        std::string temp_utf8(str.data(), str.size());
        std::wstring temp_utf16(utf8_to_utf16(temp_utf8));
        items.push_back(prj::wstring(temp_utf16.data(), temp_utf16.size()));
    }

    void List::AddItem(const prj::wstring& str) {
        items.push_back(str);
    }

    void List::AddSelectionListener(SelectionListener* value) {
        selection_listeners.push_back(value);
    }

    void List::Callback(SelectionListener::CallbackData* data) {
        if (data->widget)
            for (SelectionListener*& i : selection_listeners)
                i->Callback(data);
    }

    bool List::CheckItemSelected(size_t index) {
        if (index >= items.size())
            return false;
        else if (flags & MULTISELECT)
            return index == selected_item;
        return selections[index] != 0;
    }

    void List::ClearItems() {
        items.clear();
        selections.clear();
        hovered_item = -1;
        selected_item = -1;
    }

    prj::string List::GetItemStr(size_t index) const {
        std::wstring temp_utf16(items[index].data(), items[index].size());
        std::string temp_utf8(utf16_to_utf8(temp_utf16));
        return prj::string(temp_utf8.data(), temp_utf8.size());
    }

    prj::wstring List::GetItem(size_t index) const {
        return items[index];
    }

    size_t List::GetItemCount() {
        return items.size();
    }

    size_t List::GetMaxItemsVisible() {
        if (!v_bar)
            return items.size();
        else if (max_items >= items.size())
            return items.size();
        return max_items;
    }

    void List::ResetSelectedItem() {
        if (flags & MULTISELECT)
            selected_item = -1;
        else
            for (int32_t& i : selections)
                i = 0;
    }

    void List::SetMaxItems(size_t value) {
        max_items = value;
    }

    void List::SetScrollBarParams() {
        rectangle bb = GetBoundingBox();
        if (v_bar)
            v_bar->SetParams(bb.size.y, font.GetFontGlyphHeight(), items.size());
    }

    void List::SetSelectedItem(size_t index) {
        if (index < items.size())
            if (flags & MULTISELECT)
                selected_item = index;
            else
                selections[index] = 1;
    }

    void List::SetSelectedItems(size_t begin, size_t end) {
        if (end < begin || end >= items.size())
            return;

        if (flags & MULTISELECT) {
            if (begin == end)
                selected_item = begin;
        }
        else
            for (size_t i = begin; i <= end; i++)
                selections[i] = 1;
    }

    void List::UnsetSelectedItem(size_t index) {
        if (index < items.size())
            if (flags & MULTISELECT)
                selected_item = -1;
            else
                selections[index] = 0;
    }

    int64_t List::sub_1402EF570() {
        float_t glyph_height = GetFontGlyphHeight();

        float_t v4 = 0.0f;
        if (v_bar)
            v4 = prj::floorf(v_bar->value / glyph_height);

        int64_t v6 = 0;
        if (v4 >= (float_t)INT64_MAX) {
            v4 -= (float_t)INT64_MAX;
            if (v4 < (float_t)INT64_MAX)
                v6 = 0x8000000000000000u;
        }
        return v6 + (int64_t)v4;
    }

    bool List::sub_1402F1F20(size_t index) {
        size_t max_items_visible = GetMaxItemsVisible();
        size_t v5 = sub_1402EF570();
        return max_items_visible >= items.size()
            || v5 <= index && index < max_items_visible + v5;
    }

    void List::sub_1402F9680(int64_t index) {
        if (!v_bar)
            return;

        float_t glyph_height = GetFontGlyphHeight();

        size_t max_items_visible = GetMaxItemsVisible();
        size_t items_count = items.size();
        if (items_count <= max_items_visible)
            return;

        float_t value = 0.0f;
        if (index != -1) {
            if (items_count - index >= max_items_visible)
                value = (float_t)index;
            else
                value = (float_t)(items_count - max_items_visible);

            if (value < 0.0f)
                value += (float_t)UINT64_MAX;

            value *= glyph_height;
        }

        v_bar->SetValue(value);
    }

    void List::sub_1402F9C90(size_t index) {
        if (!sub_1402F1F20(index))
            sub_1402F9680(index);
    }

    ListBox::ListBox(Composite* parent, Flags flags) : Composite(parent, flags), list(), field_118() {
        list = new List(this, flags);
        list->field_148 = true;
        list->SetText(L"ddl");
        list->parent = this;

        if (list->v_bar) {
            list->v_bar->parent = this;
            list->v_bar->AddSelectionListener(&drop_down_list_scroll_bar_selection_listener);
        }
    }

    ListBox::~ListBox() {

    }

    void ListBox::Draw() {
        if (!parent_shell->field_170) {
            rectangle v13 = GetRectangle();
            sub_1402EE3C0(v13, 2.0, 4, Field_A0()
                ? &colors_current.background
                : &colors_current.disable_background);
            v13.pos = v13.pos + 2.0f;
            v13.size = v13.size - 2.0f * 2.0f;

            rectangle v14 = v13;

            vec2 glyph_size = font.GetFontGlyphSize();

            rectangle v15;
            v15.pos = v13.pos + v13.size - glyph_size;
            v15.size = glyph_size;

            print->SetFont(&font);
            print->SetColor(colors_current.foreground);
            print->sub_140301390(v15, 1.0f);
            print->SetClipData(v15);
            print->PrintText(L"v", v15.pos.x, v15.pos.y);

            v14.size.x -= glyph_size.x;

            if (Field_60()) {
                print->SetColor(colors_current.focus_border_color);
                print->sub_140301390(v14, 1.0f);
            }

            if (list->selected_item < list->items.size()) {
                print->SetFont(&font);
                print->SetColor(Field_A0()
                    ? colors_current.foreground
                    : colors_current.disable_foreground);
                print->SetClipData(v14);
                print->PrintText(GetItem(list->selected_item), v14.pos.x, v14.pos.y);
            }
        }

        if (parent_shell->field_170 && field_118) {
            rectangle rect = GetListRectangle();
            print->SetFillColor(colors_current.popup_background);
            print->FillRectangle(rect);
            list->UpdateDraw();
            print->SetColor(color_black);
            print->sub_140301390(rect, 1.0f);
        }
    }

    void ListBox::Reset() {
        Composite::Reset();
    }

    int32_t ListBox::KeyCallback(const Widget::KeyCallbackData& data) {
        if (!Field_A0())
            return 0;

        list->KeyCallback(data);
        if (field_118 && data.input[0] == 0x0D)
            sub_1402E4F40();
        return 0;
    }

    int32_t ListBox::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!Field_A0())
            return 0;

        if ((data.input & 0xF0) && !Field_60())
            Field_58();

        if (field_118) {
            list->MouseCallback(data);

            if ((data.input & 0x10) && (!list->CheckHitPos(data.pos)
                || ListBox::sub_1402EC8C0(list->GetScrollableRectangle(), data.pos))) {
                sub_1402E4F40();
                return 0;
            }
        }
        else if (CheckHitPos(data.pos) && (data.input & 0x10))
           sub_1402E4F90();
        return 0;
    }

    vec2 ListBox::GetSize() {
        vec2 size = list->GetSize();
        size.x += font.GetFontGlyphWidth();
        size.y = font.GetFontGlyphHeight();
        return size + 2.0f * 2.0f;
    }

    void ListBox::SetFont(p_Font& value) {
        Scrollable::SetFont(value);

        if (list)
            list->SetFont(value);
    }

    rectangle ListBox::GetListRectangle() {
        return list->GetRectangle();
    }

    void ListBox::sub_1402E4440() {
        rectangle rect = GetRectangle();
        vec2 size = list->GetSize();

        vec2 pos;
        if (dw_gui_detail_display->init_data.field_0.pos.y
            + dw_gui_detail_display->init_data.field_0.size.y > rect.pos.y + rect.size.y + size.y)
            pos.y = rect.size.y;
        else
            pos.y = -size.y;

        list->rect.pos = pos;
        list->SetSize({ max_def(rect.size.x, size.x), size.y });
    }

    void ListBox::sub_1402E4F40() {
        parent_shell->sub_1402E61F0(this);
        dw_gui_detail_display_set_selected_widget(this, false);

        list->hovered_item = list->selected_item;
        field_118 = false;
    }

    void ListBox::sub_1402E4F90() {
        field_118 = true;
        sub_1402E4440();

        list->hovered_item = list->selected_item;
        list->sub_1402F9C90(list->selected_item);

        dw_gui_detail_display_set_selected_widget(this, true);
        parent_shell->sub_1402E43C0(this);
    }

    bool ListBox::sub_1402EC8C0(rectangle rect, vec2 pos) {
        return (rect.pos.x <= pos.x && pos.x < rect.pos.x + rect.size.x)
            && (rect.pos.y <= pos.y && pos.y < rect.pos.y + rect.size.y);
    }

    ScrollBar::ScrollBar(Control* parent, Flags flags) : Widget(parent, flags),
        value(), min(), field_A0(), field_A5(), field_A6() {
        parent_control = parent;
        max = 100.0f;
        grab = 10.0f;
        step = 1.0f;
        step_fast = 10.0f;
        round = true;
        font = p_font_type_scroll_bar;
        SetSize({ 8.0f, 8.0f });
    }

    ScrollBar::~ScrollBar() {

    }

    void ScrollBar::Draw() {
        rectangle v14 = GetRectangle();
        print->SetColor(colors_current.border_color);
        print->sub_140301390(v14, 1.0f);
        print->SetClipData(v14);
        print->SetColor(Field_60()
            ? colors_current.foreground
            : colors_current.disable_foreground);
        print->SetFont(&font);

        vec2 glyph_size = font.GetFontGlyphSize();
        rectangle v16;
        v16.pos = v14.pos;
        v16.size = glyph_size;

        if (flags & HORIZONTAL) {
            print->sub_140301390(v16, 1.0f);
            print->PrintText(L"<", v14.pos.x, v14.pos.y);

            v16.pos.x = v14.size.x + v14.pos.x - glyph_size.x;
            print->sub_140301390(v16, 1.0f);
            print->PrintText(L">", v14.size.x + v14.pos.x - v16.size.x, v14.pos.y);

            rectangle v18 = v14;
            v18.pos.x = v14.pos.x + glyph_size.x;
            v18.size.x = v14.size.x - v16.size.x * 2.0f;
            print->SetFillColor(color_grey);
            print->FillRectangle(v18);

            vec2 v11 = sub_1402E4790();
            rectangle v17 = v18;
            v17.pos.x = v11.x + v18.pos.x;
            v17.size.x = v11.y;
            print->SetFillColor(colors_current.background);
            print->FillRectangle(v17);
            print->sub_140301390(v17, 1.0f);
            print->PrintText(L"#", v17.size.x * 0.5f + v17.pos.x - glyph_size.x * 0.5f, v17.pos.y);
        }
        else {
            print->sub_140301390(v16, 1.0f);
            print->PrintText(L"A", v14.pos.x, v14.pos.y);

            v16.pos.y = v14.size.y + v14.pos.y - glyph_size.y;
            print->sub_140301390(v16, 1.0f);
            print->PrintText(L"V", v14.pos.x, v14.size.y + v14.pos.y - v16.size.y);

            rectangle v18 = v14;
            v18.pos.y = v14.pos.y + glyph_size.y;
            v18.size.y = v14.size.y - v16.size.y * 2.0f;
            print->SetFillColor(color_grey);
            print->FillRectangle(v18);

            vec2 v11 = sub_1402E4790();
            rectangle v17 = v18;
            v17.pos.y = v11.x + v18.pos.y;
            v17.size.y = v11.y;
            print->SetFillColor(colors_current.background);
            print->FillRectangle(v17);
            print->sub_140301390(v17, 1.0f);
            print->PrintText(L"#", v17.pos.x, v17.size.y * 0.5f + v17.pos.y - glyph_size.y * 0.5f);
        }

    }

    int32_t ScrollBar::KeyCallback(const Widget::KeyCallbackData& data) {
        if (!Field_60())
            return 0;

        SelectionListener::CallbackData v6 = sub_1402E5140(data);
        if (v6.widget)
            for (SelectionListener*& i : selection_listeners)
                i->Callback(&v6);
        return 0;
    }

    int32_t ScrollBar::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!Field_60())
            return 0;

        bool v4 = field_A6;
        SelectionListener::CallbackData v8 = sub_1402E5380(data);
        if (v4 != field_A6)
            dw_gui_detail_display_set_selected_widget(this, field_A6);

        if (v8.widget)
            for (SelectionListener*& i : selection_listeners)
                i->Callback(&v8);

        return 0;
    }

    void ScrollBar::SetSize(const vec2& size) {
        if (flags & HORIZONTAL) {
            rect.size.x = size.x;
            rect.size.y = font.GetFontGlyphHeight();
        }
        else {
            rect.size.x = font.GetFontGlyphWidth();
            rect.size.y = size.y;
        }
    }

    vec2 ScrollBar::GetPos() {
        vec2 pos = rect.pos;
        Control* control = GetParentControl();
        if (control) {
            pos += control->GetPos();
            Scrollable* scrollable = dynamic_cast<Scrollable*>(control);
            if (scrollable && scrollable->flags & FLAG_800)
                pos += 2.0f;
        }
        return pos;
    }

    Control* ScrollBar::GetParentControl() {
        return parent_control;
    }

    void ScrollBar::SetFont(p_Font& value) {
        font = value;
    }

    bool ScrollBar::Field_58() {
        return !field_A5;
    }

    bool ScrollBar::Field_60() {
        return GetParentControl()->Field_A0() && Field_58();
    }

    void ScrollBar::Field_68(bool value) {
        field_A5 = !value;
    }

    void ScrollBar::SetGrab(float_t value) {
        grab = value;
    }

    void ScrollBar::SetMax(float_t value) {
        max = value;
    }

    void ScrollBar::SetMin(float_t value) {
        min = value;
    }

    void ScrollBar::SetParams(float_t value, float_t min,
        float_t max, float_t grab, float_t step, float_t step_fast) {
        this->value = value;
        this->grab = grab;
        this->min = min;
        this->step_fast = step_fast;
        this->max = max;
        this->step = step;
    }

    void ScrollBar::SetParams(float_t size, float_t step, size_t items_count) {
        if (!items_count)
            return;

        float_t v5 = (float_t)(int64_t)items_count;
        if (v5 < 0.0f)
            v5 += (float_t)UINT64_MAX;

        float_t v6 = v5 * step;
        if (v6 > size) {
            min = 0.0f;
            max = v6 - size;
            grab = (size / v6) * (v6 - size);
            ScrollBar::SetValue(value);
            this->step = step;

            size_t v8 = 0;
            float_t v9 = size / step;
            if (v9 >= (float_t)INT64_MAX) {
                v9 -= (float_t)INT64_MAX;
                if (v9 < (float_t)INT64_MAX)
                    v8 = INT64_MIN;
            }

            float_t v11 = (float_t)(int64_t)(v8 + (int64_t)v9);
            if (v11 < 0.0f)
                v11 += (float_t)UINT64_MAX;
            step_fast = v11 * step;
        }
        else {
            min = 0.0f;
            max = 0.0f;
            grab = v6;
            SetValue(value);
            this->step = step;
            step_fast = v6;
        }
    }

    void ScrollBar::SetRound(bool value) {
        round = value;
    }

    void ScrollBar::SetValue(float_t value) {
        value = clamp_def(value, min, max);
        if (round)
            this->value = prj::roundf(value / step) * step;
        else
            this->value = value;
    }

    void ScrollBar::SetWidth(float_t value) {
        vec2 glyph_size = font.GetFontGlyphSize();
        if (flags & HORIZONTAL) {
            rect.size.x = value;
            rect.size.y = glyph_size.y;
        }
        else {
            rect.size.x = glyph_size.x;
            rect.size.y = value;
        }
    }

    void ScrollBar::AddSelectionListener(SelectionListener* value) {
        selection_listeners.push_back(value);
    }

    vec2 ScrollBar::sub_1402E4790() {
        vec2 glyph_size = font.GetFontGlyphSize();

        float_t _glyph_size;
        float_t size;
        if (flags & HORIZONTAL) {
            _glyph_size = glyph_size.x;
            size = rect.size.x;
        }
        else {
            _glyph_size = glyph_size.y;
            size = rect.size.y;
        }

        size -= _glyph_size * 2.0f;
        float_t range = max - min;
        if (range > 0.0) {
            float_t v10 = max_def((grab * (1.0f / range)) * size, 20.0f);
            float_t v11 = ((value - min) * (1.0f / range)) * (size - v10);
            return { v11, v10 };
        }

        return { 0.0f, size };
    }

    SelectionListener::CallbackData ScrollBar::sub_1402E5140(const Widget::KeyCallbackData& key_callback_data) {
        SelectionListener::CallbackData callback_data;
        switch (key_callback_data.input[0]) {
        case 0x1000001:
        case 0x1000002:
        case 0x1000005:
        case 0x1000006:
        case 0x1000007:
        case 0x1000008:
            callback_data.field_8 = key_callback_data.input[0];
            break;
        case 0x1000003:
            callback_data.field_8 = 0x1000001;
            break;
        case 0x1000004:
            callback_data.field_8 = 0x1000002;
            break;
        default:
            break;
        }

        if (callback_data.field_8 != -1) {
            callback_data.widget = this;
            ScrollBar::sub_1402E6CC0(callback_data);
            callback_data.widget = parent;
        }

        return callback_data;
    }

    SelectionListener::CallbackData ScrollBar::sub_1402E5380(const Widget::MouseCallbackData& mouse_callback_data) {
        if (field_A6) {
            SelectionListener::CallbackData callback_data;
            if ((mouse_callback_data.input & 1) != 0) {
                callback_data.field_8 = 1;
                callback_data.mouse_pos = mouse_callback_data.pos;
            }
            else {
                callback_data.field_8 = 0;
                field_A6 = false;
            }

            if (callback_data.field_8 != -1) {
                callback_data.widget = this;
                ScrollBar::sub_1402E6CC0(callback_data);
                callback_data.widget = this->parent;
            }
            return callback_data;
        }

        SelectionListener::CallbackData callback_data;
        if (mouse_callback_data.input & 0x100000)
            callback_data.field_8 = 0x1000071;
        else if (mouse_callback_data.input & 0x200000)
            callback_data.field_8 = 0x1000072;

        if (callback_data.field_8 == -1 && !CheckHitPos(mouse_callback_data.pos))
            return callback_data;

        vec2 glyph_size = font.GetFontGlyphSize();
        rectangle rect = GetRectangle();

        float_t v8;
        float_t v9;
        float_t v10;
        float_t v11;
        if (flags & HORIZONTAL) {
            v8 = glyph_size.x;
            v9 = mouse_callback_data.pos.x;
            v10 = glyph_size.x + rect.pos.x;
            v11 = rect.size.x + rect.pos.x;
        }
        else {
            v8 = glyph_size.y;
            v9 = mouse_callback_data.pos.y;
            v10 = glyph_size.y + rect.pos.y;
            v11 = rect.size.y + rect.pos.y;
        }

        if (mouse_callback_data.input & 0x1000000) {
            if (v9 < v10)
                callback_data.field_8 = 0x1000001;
            else if (v11 - v8 < v9)
                callback_data.field_8 = 0x1000002;
            else {
                vec2 v23 = sub_1402E4790();
                if (v23.x + v10 > v9)
                    callback_data.field_8 = 0x1000005;
                else if (v9 > v23.x + v10 + v23.y)
                    callback_data.field_8 = 0x1000006;
            }
        }

        if (callback_data.field_8 == -1 && (mouse_callback_data.input & 0x01) && (mouse_callback_data.input & 0x40000000)) {
            vec2 v23 = sub_1402E4790();
            rectangle rect = GetRectangle();

            float_t v14;
            float_t v15;
            float_t v17;
            float_t v18;
            if (flags & HORIZONTAL) {
                v14 = rect.pos.x + glyph_size.x + v23.x;
                v15 = rect.pos.y;
                v17 = v23.y;
                v18 = rect.size.y;
            }
            else {
                v14 = rect.pos.x;
                v15 = rect.pos.y + glyph_size.y + v23.x;
                v17 = rect.size.x;
                v18 = v23.y;
            }

            if (v14 <= mouse_callback_data.pos.x && mouse_callback_data.pos.x < v17 + v14
                && v15 <= mouse_callback_data.pos.y && mouse_callback_data.pos.y < v18 + v15) {
                field_A6 = true;
                if (flags & HORIZONTAL)
                    field_A0 = mouse_callback_data.pos.x - (v17 * 0.5f + v14);
                else
                    field_A0 = mouse_callback_data.pos.y - (v18 * 0.5f + v15);
                callback_data.field_8 = 1;
                callback_data.mouse_pos = mouse_callback_data.pos;
            }
        }

        if (callback_data.field_8 != -1) {
            callback_data.widget = this;
            ScrollBar::sub_1402E6CC0(callback_data);
            callback_data.widget = parent;
        }
        return callback_data;
    }

    void ScrollBar::sub_1402E6CC0(SelectionListener::CallbackData data) {
        if (!data.field_8)
            return;

        ScrollBar* scroll_bar = dynamic_cast<ScrollBar*>(data.widget);
        if (!scroll_bar)
            return;

        float_t value = scroll_bar->value;
        switch (data.field_8) {
        case 1: {
            vec2 glyph_size = scroll_bar->font.GetFontGlyphSize();
            rectangle v14 = scroll_bar->GetRectangle();
            float_t v6;
            float_t v7;
            float_t v8;
            float_t v9;
            if (scroll_bar->flags & HORIZONTAL) {
                v6 = glyph_size.x;
                v7 = data.mouse_pos.x;
                v8 = v14.pos.x + glyph_size.x;
                v9 = v14.pos.x + v14.size.x;
            }
            else {
                v6 = glyph_size.y;
                v7 = data.mouse_pos.y;
                v8 = v14.pos.y + glyph_size.y;
                v9 = v14.pos.y + v14.size.y;
            }

            float_t v10 = v7 - scroll_bar->field_A0;
            vec2 size = scroll_bar->sub_1402E4790();
            float_t v11 = v9 - v6 - v8 - size.y;
            float_t v12 = v8 + size.y * 0.5f;
            if (v12 >= v10)
                value = scroll_bar->min;
            else if (v10 >= v12 + v11)
                value = scroll_bar->max;
            else
                value = scroll_bar->min + (v10 - v8 - size.y * 0.5f) / v11 * (scroll_bar->max - scroll_bar->min);
        } break;
        case 0x1000001:
            value -= scroll_bar->step;
            break;
        case 0x1000002:
            value += scroll_bar->step;
            break;
        case 0x1000005:
            value -= scroll_bar->step_fast;
            break;
        case 0x1000006:
            value += scroll_bar->step_fast;
            break;
        case 0x1000007:
            value = scroll_bar->min;
            break;
        case 0x1000008:
            value = scroll_bar->max;
            break;
        case 0x1000071:
            if (scroll_bar->flags & VERTICAL)
                value += scroll_bar->step * -3.0f;
            break;
        case 0x1000072:
            if (scroll_bar->flags & VERTICAL)
                value += scroll_bar->step * 3.0f;
            break;
        }
        scroll_bar->SetValue(value);
    }

    ShellCloseButton::ShellCloseButton(Shell* parent) : Button(parent) {
        SetSize({ 14.0f, 14.0f });
        SetText(L"Close Box");
        callback = Callback;
        field_100 = 0;
        field_104 = 4;
    }

    ShellCloseButton::~ShellCloseButton() {

    }

    void ShellCloseButton::Draw() {
        print->SetFont(&current_font);

        vec2 pos = GetPos();
        print->PrintText(!(field_100 & 2) || !(field_100 & 1) ? "x" : "*", pos.x, pos.y);
    }

    vec2 ShellCloseButton::GetPos() {
        return parent_comp->GetPos() + rect.pos;
    }

    vec2 ShellCloseButton::GetSize() {
        return current_font.GetFontGlyphSize();
    }

    void ShellCloseButton::Callback(Widget* data) {
        ShellCloseButton* close_button = dynamic_cast<ShellCloseButton*>(data);
        if (close_button)
            close_button->parent_shell->Hide();
    }

    Slider::Slider(Composite* parent, Flags flags) : Control(parent, flags) {
        format = "%f";
        scroll_bar = new ScrollBar(this, (Flags)(flags & (HORIZONTAL | VERTICAL)));
        scroll_bar->parent = this;
    }

    Slider::~Slider() {

    }

    void Slider::Draw() {
        rectangle rect = GetRectangle();
        print->SetClipData(rect);

        float_t pos_x = 0.0f;
        if (GetText().size()) {
            print->SetFont(&font);
            print->SetColor(Field_A0()
                ? colors_current.foreground
                : colors_current.disable_foreground);
            print->PrintText(GetText(), rect.pos.x, rect.pos.y);
            pos_x = font.GetFontGlyphWidth() + print->GetTextSize(GetText()).x;
        }

        print->SetFont(&font);
        print->SetColor(Field_A0()
            ? colors_current.foreground
            : colors_current.disable_foreground);

        char buf[256];
        sprintf_s(buf, sizeof(buf), format, scroll_bar->value);
        print->PrintText(buf, rect.pos.x + pos_x, rect.pos.y);

        scroll_bar->UpdateDraw();
        if (Field_A0() && Field_60()) {
            print->SetColor(colors_current.focus_border_color);
            print->sub_140301390(GetRectangle(), 1.0f);
        }
    }

    void Slider::Reset() {
        if (scroll_bar) {
            scroll_bar->Free();
            scroll_bar = 0;
        }

        Control::Reset();
    }

    int32_t Slider::KeyCallback(const Widget::KeyCallbackData& data) {
        if (!Field_A0())
            return 0;

        SelectionListener::CallbackData v6 = scroll_bar->sub_1402E5140(data);
        if (v6.widget)
            for (SelectionListener*& i : selection_listeners)
                i->Callback(&v6);
        return 0;
    }

    int32_t Slider::MouseCallback(const Widget::MouseCallbackData& data) {
        if (!Field_A0())
            return 0;

        if ((data.input & 0xF0) && !Field_60())
            Field_58();

        bool v5 = scroll_bar->field_A6;
        SelectionListener::CallbackData v9 = scroll_bar->sub_1402E5380(data);
        if (v5 != scroll_bar->field_A6)
            dw_gui_detail_display_set_selected_widget(this, scroll_bar->field_A6);

        if (v9.widget)
            for (SelectionListener*& i : selection_listeners)
                i->Callback(&v9);
        return 0;
    }

    void Slider::SetSize(const vec2& value) {
        Widget::SetSize(value);
        scroll_bar->rect.pos.x = value.x - scroll_bar->rect.size.x;
        scroll_bar->rect.pos.y = 0.0f;
    }

    void Slider::UpdateLayout() {
        scroll_bar->rect.pos = { GetTextSize().x, 0.0f };
        SetSize(GetSize());
    }

    vec2 Slider::GetSize() {
        vec2 v10 = Slider::GetTextSize();
        vec2 v11 = scroll_bar->rect.size;

        vec2 size;
        size.x = v11.x + v10.x;
        size.y = max_def(v11.y, v10.y);
        return size;
    }

    void Slider::AddSelectionListener(SelectionListener* value) {
        selection_listeners.push_back(value);
    }

    vec2 Slider::GetTextSize() {
        vec2 size;
        if (GetText().size()) {
            print->SetFont(&font);
            size.x += font.GetFontGlyphWidth() + print->GetTextSize(GetText()).x;
        }

        char buf[0x100];
        sprintf_s(buf, sizeof(buf), format, scroll_bar->value);
        size.x += font.GetFontGlyphWidth() + print->GetTextSize(buf).x;
        return size;
    }

    Slider* Slider::Create(Composite* parent, Flags flags, const char* name, const char* format, float_t width) {
        Slider* slider = new Slider(parent, flags);
        slider->scroll_bar->SetText(L"slider's bar");
        slider->SetText(name);
        slider->format = format;
        slider->scroll_bar->SetWidth(width);
        return slider;
    }

    Slider* Slider::Create(Composite* parent, Flags flags,
        float_t pos_x, float_t pos_y, float_t width, float_t height, const char* text) {
        Slider* slider = new Slider(parent, flags);
        ScrollBar* scroll_bar = slider->scroll_bar;
        slider->SetText(text);
        scroll_bar->SetText(L"slider's bar");
        scroll_bar->SetSize({ width, height });
        print->SetFont(&slider->font);
        vec2 size;
        if (flags & HORIZONTAL) {
            vec2 v21 = print->GetTextSize(slider->text);
            vec2 v22 = print->GetTextSize(L"0000");
            scroll_bar->rect.pos.x = v21.x + v22.x;
            scroll_bar->rect.pos.y = 0.0f;
            size.x = v21.x + v22.x + scroll_bar->rect.size.x;
            size.y = slider->font.GetFontGlyphHeight();
        }
        else {
            vec2 v21 = print->GetTextSize(slider->text);
            vec2 v22 = print->GetTextSize(L"0000");
            size.x = slider->font.GetFontGlyphWidth();
            size.y = v21.x + v22.x + scroll_bar->rect.size.y;
        }
        slider->rect.pos = { pos_x, pos_y };
        slider->SetSize(size);
        return slider;
    }

    init_data::init_data() {

    }

    int32_t font_get_index(prj::string& name) {
        for (auto& i : font_collection)
            if (!i.second->GetName().compare(name)) {
                current_font = *i.second;
                return i.first;
            }

        return -1;
    }
}

PrintWorkDebug::PrintWorkDebug() {
    prio = spr::SPR_PRIO_29;
    SetResolutionMode(RESOLUTION_MODE_MAX);
}

namespace dw {
    Print::Print(float_t width, float_t height) {
        font = &current_font;
        print_work = new PrintWorkDebug;
        field_10 = color_black;
        field_14 = color_white;
        field_18 = { 0.0f, 0.0f, width, height };
        clip_data = field_18;
        field_38 = 1.0f;
    }

    Print::~Print() {
        delete print_work;
    }

    void Print::FillRectangle(rectangle rect) {
        if (rect.size.x > 0.0f && rect.size.y > 0.0f)
            print_work->FillRectangle(rect);
    }

    vec2 Print::GetTextSize(prj::string& str) {
        std::string temp_utf8(str.data(), str.size());
        std::wstring temp_utf16(utf8_to_utf16(temp_utf8));
        return GetTextSize(prj::wstring(temp_utf16.data(), temp_utf16.size()));
    }

    vec2 Print::GetTextSize(prj::wstring& str) {
        return print_work->GetTextSize(str.c_str(), str.c_str() + str.size());
    }

    vec2 Print::GetTextSize(const wchar_t* str, size_t length) {
        return print_work->GetTextSize(str, str + length);
    }

    void Print::PrintText(prj::string& str, float_t pos_x, float_t pos_y) {
        std::string temp_utf8(str.data(), str.size());
        std::wstring temp_utf16(utf8_to_utf16(temp_utf8));
        PrintText(prj::wstring(temp_utf16.data(), temp_utf16.size()), pos_x, pos_y);
    }

    void Print::PrintText(prj::wstring& str, float_t pos_x, float_t pos_y) {
        PrintWorkDebug* print_work = this->print_work;
        print_work->line_origin_loc = { pos_x, pos_y };
        print_work->text_current_loc = print_work->line_origin_loc;
        print_work->PrintText(app::TEXT_FLAG_ALIGN_FLAG_LEFT, str.c_str(), str.c_str() + str.size());
    }

    void Print::SetColor(color4u8 value) {
        print_work->color = value;
    }

    void Print::SetFillColor(color4u8 value) {
        print_work->fill_color = value;
    }

    void Print::SetClipData(rectangle rect) {
        clip_data = rect;
        print_work->clip = true;
        print_work->clip_data = clip_data;
    }

    void Print::SetFont(p_Font* value) {
        this->font = value;
        print_work->SetFont(&value->ptr->font);
    }

    void Print::sub_140301390(rectangle rect, float_t a3) {
        color4u8 v3 = field_14;

        print_work->fill_color = print_work->color;

        rectangle v10;
        v10.pos.x = rect.pos.x;
        v10.pos.y = rect.pos.y;
        v10.size.x = a3;
        v10.size.y = rect.size.y;
        FillRectangle(v10);

        v10.pos.x = rect.size.x + rect.pos.x - a3;
        FillRectangle(v10);

        v10.pos.x = a3 + rect.pos.x;
        v10.pos.y = rect.pos.y;
        v10.size.x = rect.size.x - a3 - a3;
        v10.size.y = a3;
        FillRectangle(v10);

        v10.pos.y = rect.size.y + rect.pos.y - a3;
        FillRectangle(v10);

        print_work->fill_color = v3;
    }

    void Print::sub_140302800() {
        SetClipData(field_18);
    }
}

dw::Shell* dw_gui_detail::Display::GetHoveredShell() {
    auto i_begin = shells.end();
    auto i_end = shells.begin();
    for (auto i = i_begin; i != i_end;) {
        i--;

        if (!(*i)->freed && (*i)->GetDisp()) {
            rectangle rect = (*i)->GetRectangle();
            if (rect.pos.x <= mouse_callback_data.pos.x && mouse_callback_data.pos.x < rect.pos.x + rect.size.x
                && rect.pos.y <= mouse_callback_data.pos.y && mouse_callback_data.pos.y < rect.pos.y + rect.size.y)
                return *i;
        }
    }

    return 0;
}

void dw_gui_detail::Display::MakeShellFirst(dw::Shell* value) {
    if (!value)
        return;

    auto i = shells.begin();
    auto i_end = shells.end();
    while (i != i_end)
        if (*i == value) {
            shells.erase(i);
            shells.push_back(value);
            break;
        }
        else
            i++;
}

void dw_gui_detail::Display::MakeShellLast(dw::Shell* value) {
    if (!value)
        return;

    auto i = shells.begin();
    auto i_end = shells.end();
    while (i != i_end)
        if (*i == value) {
            shells.erase(i);
            shells.insert(shells.begin(), value);
            break;
        }
        else
            i++;
}

void dw_gui_detail::Display::RemoveShell(dw::Shell* value) {
    auto i = shells.begin();
    auto i_end = shells.end();
    while (i != i_end)
        if (*i == value) {
            shells.erase(i);
            break;
        }
        else
            i++;

    if (selected_shell == value) {
        selected_shell = 0;
        find_focus = true;
    }

    if (howered_shell == value)
        howered_shell = 0;

    if (move_resize_shell == value)
        move_resize_shell = 0;
}

static void dw_gui_detail_display_set_selected_widget(dw::Widget* widget, bool set) {
    dw_gui_detail_display->selected_widget = set ? widget : 0;
}

static void sub_1402EE3C0(rectangle rect, float_t border, int32_t type, color4u8* fill_color) {
    if (type != 2 && type != 4)
        return;

    color4u8 v26 = type == 2 ? dw::colors_current.border_light_color : dw::colors_current.border_dark_color;
    color4u8 v25 = type == 2 ? dw::colors_current.border_dark_color : dw::colors_current.border_light_color;

    rectangle v24a;
    v24a.pos.x = rect.pos.x;
    v24a.pos.y = rect.pos.y;
    v24a.size.x = rect.size.x;
    v24a.size.y = border;
    dw::print->SetFillColor(v26);
    dw::print->FillRectangle(v24a);

    rectangle v24b;
    v24b.pos.x = rect.pos.x;
    v24b.pos.y = rect.pos.y + border;
    v24b.size.x = border;
    v24b.size.y = rect.size.y - border;
    dw::print->SetFillColor(v26);
    dw::print->FillRectangle(v24b);

    rectangle v24c;
    v24c.pos.x = rect.pos.x + rect.size.x - border;
    v24c.pos.y = rect.pos.y + border;
    v24c.size.x = border;
    v24c.size.y = rect.size.y - border;
    dw::print->SetFillColor(v25);
    dw::print->FillRectangle(v24c);

    rectangle v24d;
    v24d.pos.x = rect.pos.x + border;
    v24d.pos.y = rect.pos.y + rect.size.y - border;
    v24d.size.x = rect.size.x - border;
    v24d.size.y = border;
    dw::print->SetFillColor(v25);
    dw::print->FillRectangle(v24d);

    if (fill_color) {
        rectangle v24;
        v24.pos.x = border + rect.pos.x;
        v24.pos.y = border + rect.pos.y;
        v24.size.x = rect.size.x - border - border;
        v24.size.y = rect.size.y - border - border;
        dw::print->SetFillColor(*fill_color);
        dw::print->FillRectangle(v24);
    }
}
