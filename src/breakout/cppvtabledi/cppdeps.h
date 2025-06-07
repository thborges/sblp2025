
#pragma once

typedef char int8_t;
typedef unsigned char uint8_t;

typedef int int16_t;
typedef unsigned int uint16_t;
typedef unsigned int size_t;

typedef long int int32_t;
typedef unsigned long int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef unsigned int uintptr_t;

#define assert(cond) ((void)0)
#define NULL nullptr

extern "C" {
    size_t strlen(const char* s);
};

struct false_type {
    static constexpr bool value = false;
    using type = false_type;
    constexpr operator bool() const noexcept { return value; }
};

struct true_type {
    static constexpr bool value = true;
    using type = true_type;
    constexpr operator bool() const noexcept { return value; }
};

/*
namespace std {
    template <typename From, typename To>
    struct is_convertible {
        static constexpr bool value = __is_convertible_to(From, To);
    };

    template<class T>
    class sharedptr {
    private:
        T *ptr = nullptr;
        uint8_t *refcount = nullptr;
        
        template <class U>
        friend class sharedptr;
    public:
        sharedptr() : ptr(nullptr), refcount(new uint8_t(0)) {}
        
        sharedptr(T *ptr) : ptr(ptr), refcount(new uint8_t(1)) {}

        sharedptr(const sharedptr &s) {
            this->ptr = s.ptr;
            this->refcount = s.refcount;
            if (s.ptr != nullptr)
                (*this->refcount)++;
        }

        sharedptr& operator=(const sharedptr &s) {
            this->ptr = s.ptr;
            this->refcount = s.refcount;
            if (s.ptr != nullptr)
                (*this->refcount)++;
            return *this;
        }

        T* operator->() const {
            return this->ptr;
        }

        T& operator*() const {
            return *this->ptr;
        }

        T* get() const {
            return this->ptr;
        }

        ~sharedptr() {
            (*refcount)--;
            if (*refcount == 0) {
                if (ptr != nullptr)
                    delete ptr;
                delete refcount;
            }
        }

        template <class U>
        sharedptr(const sharedptr<U>& s) {
            static_assert(is_convertible<U*, T*>::value, "Incompatible pointer types");
            this->ptr = s.get();
            this->refcount = s.refcount;
            if (this->ptr != nullptr)
                (*this->refcount)++;
        }
    };

    template <typename T>
    struct remove_reference {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&> {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&&> {
        using type = T;
    };

    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type; // C++14+

    template<typename T>
    typename remove_reference<T>::type&& move(T&& t) noexcept;
};
*/