/*
    by korenkonder
    GitHub/GitLab: korenkonder

    Taken from MSVC's VC/include/memory
*/

#pragma once

#include "../default.hpp"

namespace prj {
    template <typename T>
    class ref_count {
    private:
        uint32_t uses;
        uint32_t weaks;
        T* ptr;
        void(*delete_this_func)(ref_count<T>* ref);
        void(*destroy_func)(ref_count<T>* ref, T* ptr);

    protected:
        ref_count<T>() {
            uses = 1;
            weaks = 1;
            ptr = nullptr;
            delete_this_func = delete_this;
            destroy_func = destroy;
        }

    public:
        ~ref_count<T>() {

        }

        ref_count(T* ptr) {
            uses = 1;
            weaks = 1;
            this->ptr = ptr;
            delete_this_func = delete_this;
            destroy_func = destroy;
        }

        bool expired() const {
            return !use_count();
        }

        void decrement() {
            if (!--uses) {
                destroy_func(this, ptr);
                decrement_weaks();
            }
        }

        void decrement_weaks() {
            if (!--weaks)
                delete_this_func(this);
        }

        uint32_t use_count() const {
            return uses;
        }

        void increment() {
            uses++;
        }

        void increment_weaks() {
            weaks++;
        }

    private:
        static void delete_this(ref_count<T>* ref) {
            delete ref;
        }

        static void destroy(ref_count<T>* ref, T* ptr) {
            delete ptr;
        }
    };

    template<class T>
    class shared_ptr;

    template<class T>
    class ptr_base {
    public:
        typedef ptr_base<T> my_t;

        ptr_base() : ptr(0), ref(0) {

        }

        ptr_base(my_t&& right) noexcept : ptr(0), ref(0) {
            assign(std::forward<my_t>(right));
        }

        template<class U>
        ptr_base(ptr_base<U>&& right)
            : ptr(right.ptr), ref(right.ref) {
            right.ptr = 0;
            right.ref = 0;
        }

        my_t& operator=(my_t&& right) {
            assign(std::forward<my_t>(right));
            return *this;
        }

        void assign(my_t&& right) {
            swap(right);
        }

        uint32_t use_count() const {
            return ref ? ref->use_count() : 0;
        }

        void swap(ptr_base& right) {
            std::swap(ref, right.ref);
            std::swap(ptr, right.ptr);
        }

        template<class U>
        bool owner_before(const ptr_base<U>& right) const {
            return ref < right.ref;
        }

        T* get() const {
            return ptr;
        }

        bool expired() const {
            return !ref || ref->expired();
        }

        void decrement() {
            if (ref)
                ref->decrement();
        }

        void reset() {
            reset(0, 0);
        }

        template<class U>
        void reset(const ptr_base<U>& other) {
            reset(other.ptr, other.ref);
        }

        template<class U>
        void reset(T* ptr, const ptr_base<U>& other) {
            reset(ptr, other.ref);
        }

        void reset(T* other_ptr, ref_count<T>* other_ref) {
            if (other_ref)
                other_ref->increment();
            reset_base(other_ptr, other_ref);
        }

        void reset_base(T* other_ptr, ref_count<T>* other_ref) {
            if (ref)
                ref->decrement();
            ref = other_ref;
            ptr = other_ptr;
        }

        void decrement_weaks() {
            if (ref)
                ref->decrement_weaks();
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
        typedef shared_ptr<T> my_t;
        typedef ptr_base<T> my_base;

        shared_ptr() {

        }

        template<class U>
        explicit shared_ptr(U* ptr) {
            reset_ptr(ptr);
        }

        template<class U>
        shared_ptr(nullptr_t) {
            reset_ptr((T*)0);
        }

        shared_ptr(const my_t& other) {
            ptr_base<T>::reset(other);
        }

        shared_ptr(my_t&& right) noexcept : my_base(std::forward<my_t>(right)) {

        }

        my_t& operator=(my_t&& right) noexcept {
            shared_ptr(std::move(right)).swap(*this);
            return *this;
        }

        template<class U>
        my_t& operator=(shared_ptr<U>&& right) {
            shared_ptr(std::move(right)).swap(*this);
            return *this;
        }

        ~shared_ptr() {
            this->decrement();
        }

        my_t& operator=(const my_t& right) {
            shared_ptr(right).swap(*this);
            return *this;
        }

        template<class U>
        my_t& operator=(const shared_ptr<U>& right) {
            shared_ptr(right).swap(*this);
            return *this;
        }

        void reset() {
            shared_ptr().swap(*this);
        }

        template<class U>
        void reset(U* ptr) {
            shared_ptr(ptr).swap(*this);
        }

        T* operator->() const {
            return this->get();
        }

        bool unique() const {
            return this->use_count() == 1;
        }

        operator bool() const {
            return !!this->get();
        }

    private:
        template<class U>
        void reset_ptr(U* ptr) {
            try {
                this->reset_base(ptr, new ref_count<U>(ptr));
            }
            catch (...) {
                delete ptr;
                throw;
            }
        }
    };

    template<class T, class U>
    bool operator==(const shared_ptr<T>& left,
        const shared_ptr<U>& right) {
        return left.get() == right.get();
    }

    template<class T, class U>
    bool operator!=(const shared_ptr<T>& left,
        const shared_ptr<U>& right) {
        return !(left == right);
    }

    template<class T>
    void swap(shared_ptr<T>& left, shared_ptr<T>& right) {
        left.swap(right);
    }
}
