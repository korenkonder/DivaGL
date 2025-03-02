/*
    by korenkonder
    GitHub/GitLab: korenkonder

    Original: Allocator.h, Types.h by blueskythlikesclouds from DivaModLoader
*/

#pragma once

#include "../KKdLib/default.hpp"
#include "../KKdLib/prj/vector_pair.hpp"
#include "../KKdLib/prj/vector_pair_combine.hpp"
#include <list>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>

typedef uint32_t AFTGLuint;
typedef uint32_t AFTGLenum;
typedef ssize_t AFTGLsizeiptr;

namespace prj {
    typedef enum HeapCMallocType {
        HeapCMallocSystem = 0x00,
        HeapCMallocTemp   = 0x01,
        HeapCMallocMode   = 0x02,
        HeapCMallocEvent  = 0x03,
        HeapCMallocDebug  = 0x04,
        HeapCMallocMax    = 0x05,
    } HeapCMallocType;

    static void* (*HeapCMallocAllocate)(HeapCMallocType type, size_t size, const char* name)
        = (void* (*)(HeapCMallocType, size_t, const char*))0x00000001403F2A00;
    static void* (*HeapCMallocAllocateByType)(HeapCMallocType type, size_t size, const char* name)
        = (void* (*)(HeapCMallocType, size_t, const char*))0x0000000140181C30;
    static void(*HeapCMallocFree)(HeapCMallocType type, void* data)
        = (void(*)(HeapCMallocType, void*))0x00000001403F2960;
    static void(*HeapCMallocFreeByType)(HeapCMallocType type, void* data)
        = (void(*)(HeapCMallocType, void*))0x0000000140182DB0;
};

static void* (*_operator_new)(size_t) = (void* (*)(size_t))0x000000014084530C;
static void(*_operator_delete)(void*) = (void(*)(void*))0x0000000140845378;

static float_t(*get_delta_frame)() = (float_t(*)())0x0000000140192D50;

template <class T>
class Allocator {
public:
    using value_type = T;

    Allocator() noexcept {}
    template <class U> Allocator(Allocator<U> const&) noexcept {}

    value_type* allocate(std::size_t n) {
        return reinterpret_cast<value_type*>(_operator_new(n * sizeof(value_type)));
    }

    void deallocate(value_type* p, std::size_t) noexcept {
        _operator_delete(reinterpret_cast<void*>(p));
    }
};

template <class T, class U>
bool operator==(Allocator<T> const&, Allocator<U> const&) noexcept {
    return true;
}

template <class T, class U>
bool operator!=(Allocator<T> const& x, Allocator<U> const& y) noexcept {
    return !(x == y);
}

namespace prj {
    template<typename T, typename U>
    using pair = std::pair<T, U>;

    using string = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

    using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t>>;

    template<typename T>
    using vector = std::vector<T, Allocator<T>>;

    template<typename T, typename U>
    using vector_pair_alloc = prj::vector_pair<T, U, Allocator<prj::pair<T, U>>>;

    template<typename T, typename U>
    using vector_pair_combine_alloc = prj::vector_pair_combine<T, U, Allocator<prj::pair<T, U>>>;

    template<typename T>
    using list = std::list<T, Allocator<T>>;

    template<typename T, typename U>
    using map = std::map<T, U, std::less<T>, Allocator<pair<T, U>>>;

    template<typename T>
    using set = std::set<T, std::less<T>, Allocator<T>>;

    template<typename T, typename U>
    using unordered_map = std::unordered_map<T, U, std::hash<T>, std::equal_to<T>, Allocator<pair<T, U>>>;

    template<typename T>
    using unordered_set = std::unordered_set<T, std::hash<T>, std::equal_to<T>, Allocator<T>>;

    template<typename T>
    class ref_count {
    private:
        int32_t uses;
        int32_t weaks;
        T* ptr;
        void(*delete_this_func)(ref_count* ref);
        void(*destroy_func)(ref_count* ref, T* ptr);
    };

    template<class T>
    class shared_ptr;

    template<class T>
    class ptr_base {
    public:
        T* get() const {
            return ptr;
        }

    private:
        T* ptr;
        ref_count<T>* ref;

        template<class U>
        friend class ptr_base;
    };

    template<class T>
    class shared_ptr : public ptr_base<T> {
    public:
        T* operator->() const {
            return this->get();
        }

        operator bool() const {
            return !!this->get();
        }
    };

    struct stack_allocator {
        size_t size;
        uint8_t* begin;
        uint8_t* end;
        uint8_t* capacity_end;

        inline void* allocate(size_t size) {
            static void* (*prj__stack_allocator__allocate)(prj::stack_allocator * This, size_t size)
                = (void* (*)(prj::stack_allocator * This, size_t size))0x000000014009B110;
            return prj__stack_allocator__allocate(this, size);
        }

        template <typename T>
        inline T* allocate() {
            return new((T*)allocate(sizeof(T))) T;
        }

        template <typename T>
        inline T* allocate(size_t size) {
            if (!size)
                return 0;

            T* arr = (T*)allocate(sizeof(T) * size);
            for (size_t i = 0; i < size; i++)
                new(&arr[i]) T();
            return arr;
        }

        template <typename T>
        inline T* allocate(const T* src) {
            if (!src)
                return 0;

            return new((T*)allocate(sizeof(T))) T(*src);
        }

        template <typename T>
        inline T* allocate(const T* src, size_t size) {
            if (!src || !size)
                return 0;

            T* dst = (T*)allocate(sizeof(T) * size);
            for (size_t i = 0; i < size; i++)
                new(&dst[i]) T(src[i]);
            return dst;
        }
    };

    template<typename T, typename U>
    struct sorted_vector {
        vector<pair<T, U>> field_0;
        size_t field_18;
        vector<pair<T, U>> field_20;
        bool field_38;
    };

    struct string_range {
        char* begin;
        char* end;

        inline string_range() : begin(), end() {

        }

        inline string_range(char* str) {
            this->begin = str;
            this->end = str + utf8_length(str);
        }

        inline string_range(char* str, size_t length) {
            this->begin = str;
            this->end = str + length;
        }

        inline string_range(char* begin, char* end) {
            this->begin = begin;
            this->end = end;
        }

        inline string_range(prj::string& str) {
            begin = (char*)str.data();
            end = (char*)str.data() + str.size();
        }
    };

    static_assert(sizeof(string_range) == 0x10, "\"string_range\" struct should have a size of 0x10");

    struct string_range_capacity {
        string_range range;
        char* capacity_end;

        inline string_range_capacity() : capacity_end() {

        }

        inline string_range_capacity(char* str) {
            range = string_range(str);
            capacity_end = range.end;
        }

        inline string_range_capacity(char* str, size_t length) {
            range = string_range(str, length);
            capacity_end = range.end;
        }

        inline string_range_capacity(char* str, size_t length, size_t capacity) {
            range = string_range(str, length);
            capacity_end = str + capacity;
        }

        inline string_range_capacity(char* begin, char* end) {
            range = string_range(begin, end);
            capacity_end = range.end;
        }

        inline string_range_capacity(char* begin, char* end, char* capacity_end) {
            range = string_range(begin, end);
            this->capacity_end = capacity_end;
        }

        inline string_range_capacity(prj::string& str) {
            range = string_range(str);
            capacity_end = (char*)str.data() + str.capacity();
        }

        inline string_range_capacity(prj::string_range_capacity str_rng_cap, const char* str) {
            *this = str_rng_cap;

            char* end = range.end;
            while (end != capacity_end && *str)
                *end++ = *str++;
            range.end = end;
        }
    };

    static_assert(sizeof(string_range) == 0x10, "\"string_range\" struct should have a size of 0x10");

    struct wstring_range {
        const wchar_t* begin;
        const wchar_t* end;

        inline wstring_range() : begin(), end() {

        }

        inline wstring_range(const wchar_t* str) {
            this->begin = str;
            this->end = str + utf16_length(str);
        }
        
        inline wstring_range(const wchar_t* str, size_t length) {
            this->begin = str;
            this->end = str + length;
        }

        inline wstring_range(const wchar_t* begin, const wchar_t* end) {
            this->begin = begin;
            this->end = end;
        }

        inline wstring_range(const prj::wstring& str) {
            begin = str.c_str();
            end = str.c_str() + str.size();
        }
    };

    static_assert(sizeof(wstring_range) == 0x10, "\"wstring_range\" struct should have a size of 0x10");
}
