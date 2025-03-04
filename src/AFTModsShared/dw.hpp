/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/rectangle.hpp"
#include "../KKdLib/vec.hpp"
#include "color.hpp"
#include "print_work.hpp"
#include <vector>

namespace dw {
    enum Flags {
        FLAG_1       = 0x0000001,
        FLAG_2       = 0x0000002,
        MULTISELECT  = 0x0000004,
        FLAG_8       = 0x0000008,
        RADIOBUTTON  = 0x0000010,
        CHECKBOX     = 0x0000020,
        CLOSE_BUTTON = 0x0000040,
        FLAG_80      = 0x0000080,
        HORIZONTAL   = 0x0000100,
        VERTICAL     = 0x0000200,
        FLAG_400     = 0x0000400,
        FLAG_800     = 0x0000800,
        FLAG_1000    = 0x0001000,
        FLAG_2000    = 0x0002000,
        FLAG_4000    = 0x0004000,
        FLAG_8000    = 0x0008000,
        FLAG_10000   = 0x0010000,
        FLAG_20000   = 0x0020000,
        FLAG_40000   = 0x0040000,
        FLAG_80000   = 0x0080000,
        FLAG_100000  = 0x0100000,
        FLAG_200000  = 0x0200000,
        FLAG_400000  = 0x0400000,
        FLAG_800000  = 0x0800000,
        FLAG_1000000 = 0x1000000,
    };

    struct Font {
        font_info font;
        prj::string name;
        bool half_width;

        Font(int32_t font_handler_index, prj::string& name);
        Font(int32_t font_handler_index, prj::string& name, bool half_width);

        inline Font(int32_t font_handler_index, prj::string&& name)
            : Font(font_handler_index, name) {

        }

        inline Font(int32_t font_handler_index, prj::string&& name, bool half_width)
            : Font(font_handler_index, name, half_width) {

        }

        ~Font();

        float_t GetFontGlyphHeight();
        vec2 GetFontGlyphSize();
        float_t GetFontGlyphWidth();
        prj::string GetName();
        void SetGlyphSize(float_t glyph_width, float_t glyph_height);
    };

    static_assert(sizeof(dw::Font) == 0x68, "\"dw::Font\" struct should have a size of 0x68");

    struct p_Font {
        Font* ptr;

        p_Font();

        inline p_Font(Font* ptr) {
            this->ptr = ptr;
        }

        ~p_Font();

        float_t GetFontGlyphHeight();
        vec2 GetFontGlyphSize();
        float_t GetFontGlyphWidth();
        prj::string GetName();
        void SetGlyphSize(float_t glyph_width, float_t glyph_height);
    };

    static_assert(sizeof(dw::p_Font) == 0x08, "\"dw::p_Font\" struct should have a size of 0x08");

    class Widget;

    class SelectionListener {
    public:
        struct CallbackData {
            int64_t field_0;
            int32_t field_8;
            int8_t field_C;
            vec2 mouse_pos;
            int32_t field_18;
            Widget* widget;

            inline CallbackData() : field_0(), field_8(), field_C(), field_18(), widget() {
                field_8 = -1;
            }

            inline CallbackData(Widget* widget) : field_0(), field_8(), field_C(), field_18() {
                this->widget = widget;
            }
        };

        static_assert(sizeof(dw::SelectionListener::CallbackData) == 0x28, "\"dw::SelectionListener::CallbackData\" struct should have a size of 0x28");

        typedef void(*CallbackFunc)(SelectionListener::CallbackData* data);

        SelectionListener();
        virtual ~SelectionListener();

        virtual void Callback(CallbackData* data) = 0;
        virtual void Field_10(CallbackData* data) = 0;
    };

    static_assert(sizeof(dw::SelectionListener) == 0x08, "\"dw::SelectionListener\" struct should have a size of 0x08");

    class SelectionAdapter : public SelectionListener {
    public:
        SelectionAdapter();
        virtual ~SelectionAdapter() override;

        virtual void Callback(SelectionListener::CallbackData* data) override;
        virtual void Field_10(SelectionListener::CallbackData* data) override;
    };

    static_assert(sizeof(dw::SelectionAdapter) == 0x08, "\"dw::SelectionAdapter\" struct should have a size of 0x08");

    class SelectionListenerOnHook : public SelectionAdapter {
    public:
        typedef void(*CallbackFunc)(Widget* widget);

        CallbackFunc callback;

        SelectionListenerOnHook();
        SelectionListenerOnHook(SelectionListenerOnHook::CallbackFunc callback);
        virtual ~SelectionListenerOnHook() override;

        virtual void Callback(SelectionListener::CallbackData* data) override;
    };

    static_assert(sizeof(dw::SelectionListenerOnHook) == 0x10, "\"dw::SelectionListenerOnHook\" struct should have a size of 0x10");

    class Widget {
    public:
        typedef void(*Callback)(Widget* data);

        struct KeyCallbackData {
            Widget* widget;
            int32_t modifier;
            int8_t field_C;
            int8_t field_D;
            int32_t input[2];

            KeyCallbackData();
        };

        static_assert(sizeof(dw::Widget::KeyCallbackData) == 0x18, "\"dw::Widget::KeyCallbackData\" struct should have a size of 0x18");

        struct MouseCallbackData {
            Widget* widget;
            vec2 pos;
            int32_t input;
            int32_t modifier;

            MouseCallbackData();
        };

        static_assert(sizeof(dw::Widget::MouseCallbackData) == 0x18, "\"dw::Widget::MouseCallbackData\" struct should have a size of 0x18");

        bool freed;

        union {
            int8_t i8;
            uint8_t u8;
            int16_t i16;
            uint16_t u16;
            int32_t i32;
            uint32_t u32;
            int64_t i64;
            uint64_t u64;
            void* v64;
        } callback_data;

        Callback free_callback;
        prj::wstring text;
        rectangle rect;
        Widget* parent;
        Flags flags;
        p_Font font;

        Widget(Widget* parent = 0, Flags flags = (Flags)0);
        virtual ~Widget();

        virtual void Draw();
        virtual void Reset();
        virtual prj::wstring GetText();
        virtual void SetText(prj::wstring& str);
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data);
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data);
        virtual void SetSize(const vec2& value);
        virtual vec2 GetPos();

        bool CheckHitPos(const vec2& hit_pos);
        void Free();
        float_t GetFontGlyphHeight();
        rectangle GetRectangle();
        void SetText(prj::string& str);
        void UpdateDraw();

        inline void SetText(prj::wstring&& str) {
            SetText(str);
        }

        inline void SetText(prj::string&& str) {
            SetText(str);
        }

        static void sub_1402F3770(Widget* widget);
    };

    static_assert(sizeof(dw::Widget) == 0x68, "\"dw::Widget\" struct should have a size of 0x68");

    class KeyListener;
    class MouseListener;
    class MouseMoveListener;
    class Composite;
    class Shell;
    class Menu;

    class Control : public Widget {
    public:
        bool field_68;
        color4u8 foreground_color;
        color4u8 background_color;
        prj::vector<KeyListener*> key_listener;
        prj::vector<MouseListener*> mouse_listener;
        prj::vector<MouseMoveListener*> mouse_move_listener;
        Composite* parent_comp;
        Shell* parent_shell;
        Menu* parent_menu;

        Control(Composite* parent = 0, Flags flags = (Flags)0);
        virtual ~Control() override;

        virtual void Draw() override;
        virtual void Reset() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;
        virtual vec2 GetPos() override;

        virtual void UpdateLayout();
        virtual vec2 GetSize();
        virtual bool Field_58();
        virtual bool Field_60();
        virtual void SetFont(p_Font& font);
        virtual p_Font& GetFont();
        virtual void SetForegroundColor(color4u8 value);
        virtual void SetBackgroundColor(color4u8 value);
        virtual color4u8 GetForegroundColor();
        virtual color4u8 GetBackgroundColor();
        virtual bool Field_98();
        virtual bool Field_A0();
        virtual void Field_A8(bool value);
        virtual Widget* GetHitWidget(const vec2& hit_pos);

        void SetParentMenu(Menu* menu);
    };

    static_assert(sizeof(dw::Control) == 0xD8, "\"dw::Control\" struct should have a size of 0xD8");

    class ScrollBar;

    class Scrollable : public Control {
    public:
        ScrollBar* h_bar;
        ScrollBar* v_bar;

        Scrollable(Composite* parent = 0, Flags flags = (Flags)0);
        virtual ~Scrollable() override;

        virtual void Draw() override;
        virtual void Reset() override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;
        virtual void SetSize(const vec2& value) override;
        virtual void SetFont(p_Font& font) override;
        virtual Widget* GetHitWidget(const vec2& hit_pos) override;

        virtual rectangle GetBoundingBox();

        rectangle GetScrollableRectangle();
    };

    static_assert(sizeof(dw::Scrollable) == 0xE8, "\"dw::Scrollable\" struct should have a size of 0xE8");

    class Layout;

    class Composite : public Scrollable {
    public:
        size_t current_control;
        Layout* layout;
        prj::vector<Control*> controls;

        Composite(Composite* parent = 0, Flags flags = (Flags)0);
        virtual ~Composite() override;

        virtual void Draw() override;
        virtual void Reset() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;
        virtual void SetSize(const vec2& size) override;

        virtual void UpdateLayout() override;
        virtual vec2 GetSize() override;
        virtual bool Field_58() override;
        virtual bool Field_60() override;
        virtual Widget* GetHitWidget(const vec2& hit_pos) override;

        virtual bool Field_C0();
        virtual bool SetCurrentControl(Control* control);

        size_t GetControlIndex(Control* control);
        void SetLayout(Layout* value);

        bool sub_1402F1E60(Control* control);
    };

    static_assert(sizeof(dw::Composite) == 0x110, "\"dw::Composite\" struct should have a size of 0x110");

    class ShellCloseButton;

    class Shell : public Composite {
    public:
        typedef void(*Callback)(Shell* data);

        bool disp;
        Callback disp_callback;
        Callback hide_callback;
        int64_t field_128;
        bool destroy;
        ShellCloseButton* close_button;
        prj::vector<Widget*> children;
        prj::vector<Menu*> menus;
        bool field_170;
        Widget* field_178;

        Shell(Shell* parent = 0, Flags flags
            = (Flags)(FLAG_400 | FLAG_80 | CLOSE_BUTTON | CHECKBOX | RADIOBUTTON));
        virtual ~Shell() override;

        virtual void Draw() override;
        virtual void Reset() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;
        virtual void SetSize(const vec2& size) override;
        virtual vec2 GetPos() override;

        virtual vec2 GetSize() override;
        virtual bool Field_58() override;
        virtual Widget* GetHitWidget(const vec2& hit_pos) override;

        virtual rectangle GetBoundingBox() override;

        virtual bool Field_C0() override;
        virtual bool SetCurrentControl(Control* control) override;

        virtual void Hide();

        void Disp();
        bool GetDisp();
        void SetDisp(bool value = false);

        void sub_1402E43C0(Widget* widget);
        void sub_1402E61F0(Widget* widget);
        void sub_1402F38B0();
    };

    static_assert(sizeof(dw::Shell) == 0x180, "\"dw::Shell\" struct should have a size of 0x180");

    class KeyListener {
    public:
        KeyListener();
        virtual ~KeyListener();

        virtual void Field_8(const Widget::KeyCallbackData& data) = 0;
        virtual void Field_10(const Widget::KeyCallbackData& data) = 0;
        virtual void Field_18(const Widget::KeyCallbackData& data) = 0;
    };

    static_assert(sizeof(dw::KeyListener) == 0x08, "\"dw::KeyListener\" struct should have a size of 0x08");

    class KeyAdapter : public KeyListener {
    public:
        KeyAdapter();
        virtual ~KeyAdapter() override;

        virtual void Field_8(const Widget::KeyCallbackData& data) override;
        virtual void Field_10(const Widget::KeyCallbackData& data) override;
        virtual void Field_18(const Widget::KeyCallbackData& data) override;
    };

    static_assert(sizeof(dw::KeyAdapter) == 0x08, "\"dw::KeyAdapter\" struct should have a size of 0x08");

    class MouseListener {
    public:
        MouseListener();
        virtual ~MouseListener();

        virtual void Field_8(const Widget::MouseCallbackData& data) = 0;
        virtual void Field_10(const Widget::MouseCallbackData& data) = 0;
        virtual void Field_18(const Widget::MouseCallbackData& data) = 0;
        virtual void Field_20(const Widget::MouseCallbackData& data) = 0;
    };

    static_assert(sizeof(dw::MouseListener) == 0x08, "\"dw::MouseListener\" struct should have a size of 0x08");

    class MouseAdapter : public MouseListener {
    public:
        MouseAdapter();
        virtual ~MouseAdapter() override;

        virtual void Field_8(const Widget::MouseCallbackData& data) override;
        virtual void Field_10(const Widget::MouseCallbackData& data) override;
        virtual void Field_18(const Widget::MouseCallbackData& data) override;
        virtual void Field_20(const Widget::MouseCallbackData& data) override;
    };

    static_assert(sizeof(dw::MouseAdapter) == 0x08, "\"dw::MouseAdapter\" struct should have a size of 0x08");

    class MouseMoveListener {
    public:
        MouseMoveListener();
        virtual ~MouseMoveListener();

        virtual void Field_8(const Widget::MouseCallbackData& data) = 0;
    };

    static_assert(sizeof(dw::MouseMoveListener) == 0x08, "\"dw::MouseMoveListener\" struct should have a size of 0x08");

    class Layout {
    public:
        Layout();
        virtual ~Layout();

        virtual vec2 GetSize(Composite* comp) = 0;
        virtual void SetSize(Composite* comp) = 0;
    };

    static_assert(sizeof(dw::Layout) == 0x08, "\"dw::Layout\" struct should have a size of 0x08");

    __declspec(align(8)) class FillLayout : public Layout {
    public:
        Flags flags;

        FillLayout(Flags flags = VERTICAL);
        virtual ~FillLayout() override;

        virtual vec2 GetSize(Composite* comp) override;
        virtual void SetSize(Composite* comp) override;
    };

    static_assert(sizeof(dw::FillLayout) == 0x10, "\"dw::FillLayout\" struct should have a size of 0x10");

    __declspec(align(8)) class GraphLayout : public Layout {
    public:
        Flags flags;

        GraphLayout(Flags flags = VERTICAL);
        virtual ~GraphLayout() override;

        virtual vec2 GetSize(Composite* comp) override;
        virtual void SetSize(Composite* comp) override;
    };

    static_assert(sizeof(dw::GraphLayout) == 0x10, "\"dw::GraphLayout\" struct should have a size of 0x10");

    class GridLayout : public Layout {
    public:
        size_t rows;
        prj::vector<float_t> column_size;
        prj::vector<float_t> row_size;
        size_t columns;
        vec2 spacing;

        GridLayout(size_t columns = 2);
        virtual ~GridLayout() override;

        virtual vec2 GetSize(Composite* comp) override;
        virtual void SetSize(Composite* comp) override;
    };

    static_assert(sizeof(dw::GridLayout) == 0x50, "\"dw::GridLayout\" struct should have a size of 0x50");

    class RowLayout : public Layout {
    public:
        Flags flags;
        float_t spacing;

        RowLayout(Flags flags = VERTICAL);
        virtual ~RowLayout() override;

        virtual vec2 GetSize(Composite* comp) override;
        virtual void SetSize(Composite* comp) override;
    };

    static_assert(sizeof(dw::RowLayout) == 0x10, "\"dw::RowLayout\" struct should have a size of 0x10");

    class Group : public Composite {
    public:
        Group(Composite* parent = 0, Flags flags = (Flags)0);
        virtual ~Group() override;

        virtual void Draw() override;

        virtual vec2 GetSize() override;

        virtual rectangle GetBoundingBox() override;
    };

    static_assert(sizeof(dw::Group) == 0x110, "\"dw::Group\" struct should have a size of 0x110");

    class Label : public Control {
    public:
        Label(Composite* parent = 0, Flags flags = (Flags)0);
        virtual ~Label() override;

        virtual void Draw() override;
        virtual vec2 GetSize() override;
    };

    static_assert(sizeof(dw::Label) == 0xD8, "\"dw::Label\" struct should have a size of 0xD8");

    class Button : public Control {
    public:
        bool value;
        prj::vector<SelectionListener*> selection_listeners;
        Widget::Callback callback;
        int32_t field_100;
        int32_t field_104;

        Button(Composite* parent = 0, Flags flags = (Flags)0);
        virtual ~Button() override;

        virtual void Draw() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;

        virtual vec2 GetSize() override;

        void AddSelectionListener(SelectionListener* value);
        void SetValue(bool value);
    };

    static_assert(sizeof(dw::Button) == 0x108, "\"dw::Button\" struct should have a size of 0x108");

    class List : public Scrollable {
    public:
        prj::vector<prj::wstring> items;
        prj::vector<int32_t> selections;
        size_t hovered_item;
        size_t selected_item;
        prj::vector<SelectionListener*> selection_listeners;
        size_t max_items;
        bool field_148;

        List(Composite* parent = 0, Flags flags = (Flags)0);
        virtual ~List() override;

        virtual void Draw() override;
        virtual void Reset() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;
        virtual void SetSize(const vec2& size) override;

        virtual vec2 GetSize() override;

        void AddItem(const prj::string& str);
        void AddItem(const prj::wstring& str);
        void AddSelectionListener(SelectionListener* value);
        void Callback(SelectionListener::CallbackData* data);
        bool CheckItemSelected(size_t index);
        void ClearItems();
        prj::string GetItemStr(size_t index) const;
        prj::wstring GetItem(size_t index) const;
        size_t GetItemCount();
        size_t GetMaxItemsVisible();
        void ResetSelectedItem();
        void SetMaxItems(size_t value);
        void SetScrollBarParams();
        void SetSelectedItem(size_t index);
        void SetSelectedItems(size_t begin, size_t end);
        void UnsetSelectedItem(size_t index);

        inline void AddItem(const prj::string&& str) {
            AddItem(str);
        }

        inline void AddItem(const prj::wstring&& str) {
            AddItem(str);
        }

        inline prj::string GetHoveredItemStr() {
            return GetItemStr(hovered_item);
        }

        inline prj::wstring GetHoveredItem() {
            return GetItem(hovered_item);
        }

        inline prj::string GetSelectedItemStr() {
            return GetItemStr(selected_item);
        }

        inline prj::wstring GetSelectedItem() {
            return GetItem(selected_item);
        }

        inline void ResetSetSelectedItem(size_t index) {
            ResetSelectedItem();
            SetSelectedItem(index);
        }

        inline void SetItemIndex(size_t index) {
            hovered_item = index;
            ResetSetSelectedItem(index);
        }

        int64_t sub_1402EF570();
        bool sub_1402F1F20(size_t index);
        void sub_1402F9680(int64_t index);
        void sub_1402F9C90(size_t index);
    };

    static_assert(sizeof(dw::List) == 0x150, "\"dw::List\" struct should have a size of 0x150");

    class ListBox : public Composite {
    public:
        List* list;
        bool field_118;

        ListBox(Composite* parent = 0, Flags flags = (Flags)(VERTICAL | MULTISELECT));
        virtual ~ListBox() override;

        virtual void Draw() override;
        virtual void Reset() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;

        virtual vec2 GetSize() override;
        virtual void SetFont(p_Font& value) override;

        rectangle GetListRectangle();

        inline void AddItem(const prj::string& str) {
            if (list)
                list->AddItem(str);
        }

        inline void AddItem(const prj::string&& str) {
            if (list)
                list->AddItem(str);
        }

        inline void AddSelectionListener(SelectionListener* value) {
            if (list)
                list->AddSelectionListener(value);
        }

        inline void ClearItems() {
            if (list)
                list->ClearItems();
        }

        inline prj::string GetItemStr(size_t index) {
            if (list)
                return list->GetItemStr(index);
            return {};
        }

        inline prj::wstring GetItem(size_t index) {
            if (list)
                return list->GetItem(index);
            return {};
        }

        inline size_t GetItemCount() {
            if (list)
                return list->GetItemCount();
            return 0;
        }

        inline prj::string GetHoveredItemStr() {
            if (list)
                return list->GetHoveredItemStr();
            return {};
        }

        inline prj::wstring GetHoveredItem() {
            if (list)
                return list->GetHoveredItem();
            return {};
        }

        inline prj::string GetSelectedItemStr() {
            if (list)
                return list->GetSelectedItemStr();
            return {};
        }

        inline prj::wstring GetSelectedItem() {
            if (list)
                return list->GetSelectedItem();
            return {};
        }

        inline void SetItemIndex(size_t index) {
            if (list)
                list->SetItemIndex(index);
        }

        inline void SetMaxItems(size_t value) {
            if (list)
                list->SetMaxItems(value);
        }

        void sub_1402E4440();
        void sub_1402E4F40();
        void sub_1402E4F90();

        static bool sub_1402EC8C0(rectangle rect, vec2 pos);
    };

    static_assert(sizeof(dw::ListBox) == 0x120, "\"dw::ListBox\" struct should have a size of 0x120");

    class ScrollBar : public Widget {
    public:
        Control* parent_control;
        prj::vector<SelectionListener*> selection_listeners;
        float_t value;
        float_t min;
        float_t max;
        float_t grab;
        float_t step;
        float_t step_fast;
        float_t field_A0;
        bool round;
        bool field_A5;
        bool field_A6;

        ScrollBar(Control* parent, Flags flags);
        virtual ~ScrollBar() override;

        virtual void Draw() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;
        virtual void SetSize(const vec2& value) override;
        virtual vec2 GetPos() override;

        virtual Control* GetParentControl();
        virtual void SetFont(p_Font& value);
        virtual bool Field_58();
        virtual bool Field_60();
        virtual void Field_68(bool value);

        void SetGrab(float_t value);
        void SetMax(float_t value);
        void SetMin(float_t value);
        void SetParams(float_t value, float_t min, float_t max,
            float_t grab, float_t step, float_t step_fast);
        void SetParams(float_t size, float_t step, size_t items_count);
        void SetRound(bool value);
        void SetValue(float_t value);
        void SetWidth(float_t value);

        void AddSelectionListener(SelectionListener* value);

        vec2 sub_1402E4790();
        SelectionListener::CallbackData sub_1402E5140(const Widget::KeyCallbackData& key_callback_data);
        SelectionListener::CallbackData sub_1402E5380(const Widget::MouseCallbackData& mouse_callback_data);

        static void sub_1402E6CC0(SelectionListener::CallbackData callback_data);

        inline float_t GetValue() const {
            return value;
        }
    };

    static_assert(sizeof(dw::ScrollBar) == 0xA8, "\"dw::ScrollBar\" struct should have a size of 0xA8");

    class ShellCloseButton : public Button {
    public:
        ShellCloseButton(Shell* parent);
        virtual ~ShellCloseButton() override;

        virtual void Draw() override;
        virtual vec2 GetPos() override;

        virtual vec2 GetSize() override;

        static void Callback(Widget* data);
    };

    static_assert(sizeof(dw::ShellCloseButton) == 0x108, "\"dw::ShellCloseButton\" struct should have a size of 0x108");

    class Slider : public Control {
    public:
        const char* format;
        prj::vector<SelectionListener*> selection_listeners;
        ScrollBar* scroll_bar;

        Slider(Composite* parent, Flags flags);
        virtual ~Slider();

        virtual void Draw() override;
        virtual void Reset() override;
        virtual int32_t KeyCallback(const Widget::KeyCallbackData& data) override;
        virtual int32_t MouseCallback(const Widget::MouseCallbackData& data) override;
        virtual void SetSize(const vec2& value) override;

        virtual void UpdateLayout() override;
        virtual vec2 GetSize() override;

        void AddSelectionListener(SelectionListener* value);
        vec2 GetTextSize();

        static Slider* Create(Composite* parent, Flags flags, const char* text,
            const char* format, float_t width = 128.f);
        static Slider* Create(Composite* parent = 0,
            Flags flags = (Flags)(FLAG_800 | HORIZONTAL),
            float_t pos_x = 0.0f, float_t pos_y = 0.0f,
            float_t width = 128.0f, float_t height = 20.0f, const char* text = "slider");

        inline float_t GetValue() const {
            return scroll_bar->GetValue();
        }

        inline void SetGrab(float_t value) {
            scroll_bar->SetGrab(value);
        }

        inline void SetMax(float_t value) {
            scroll_bar->SetMax(value);
        }

        inline void SetMin(float_t value) {
            scroll_bar->SetMin(value);
        }

        inline void SetParams(float_t value, float_t min, float_t max,
            float_t grab, float_t step, float_t step_fast) {
            scroll_bar->SetParams(value, min, max, grab, step, step_fast);
        }

        inline void SetRound(bool value) {
            scroll_bar->SetRound(value);
        }

        inline void SetValue(float_t value) {
            scroll_bar->SetValue(value);
        }
    };

    static_assert(sizeof(dw::Slider) == 0x100, "\"dw::Slider\" struct should have a size of 0x100");

    struct init_data {
        rectangle field_0;
        rectangle field_10;

        init_data();
    };

    static_assert(sizeof(dw::init_data) == 0x20, "\"dw::init_data\" struct should have a size of 0x20");

    class DropDownListScrollBarSelectionListener : public SelectionAdapter {
    public:
    };

    static_assert(sizeof(dw::DropDownListScrollBarSelectionListener) == 0x08, "\"dw::DropDownListScrollBarSelectionListener\" struct should have a size of 0x08");

    class ScrollBarTestSelectionListener : public SelectionAdapter {
    public:
    };

    static_assert(sizeof(dw::ScrollBarTestSelectionListener) == 0x08, "\"dw::ScrollBarTestSelectionListener\" struct should have a size of 0x08");

    extern p_Font& current_font;

    extern p_Font& p_font_type_6x12;
    extern p_Font& p_font_type_8x12;
    extern p_Font& p_font_type_8x16;
    extern p_Font& p_font_type_10x16;
    extern p_Font& p_font_type_10x20;
    extern p_Font& p_font_type_12x24;
    extern p_Font& p_font_type_scroll_bar;

    extern int32_t font_get_index(prj::string& name);
}
