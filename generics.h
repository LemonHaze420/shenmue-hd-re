#pragma once

#include "config.h"
#include "utilities.h"

#define DECLARE_VAR(type, name) \
    extern Var<type> name;

#define DEFINE_VAR(type, name) \
    Var<type> name{ O_##name };

template<class To, class From>
constexpr
typename std::enable_if_t<sizeof(To) == sizeof(From) && std::is_trivially_copyable_v<From>&&
    std::is_trivially_copyable_v<To>,
    To>
    // constexpr support needs compiler magic
    bit_cast(const From& src) noexcept {
    static_assert(
        std::is_trivially_constructible_v<To>,
        "This implementation additionally requires destination type to be trivially constructible");

    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

template<typename T>
inline auto& var(ptrdiff_t&& address) {
    return *bit_cast<T*>(address);
}

template<typename T>
struct Var {
    using value_type = T;
    using pointer_type = T*;

    Var() = delete;

    Var(const Var&) = delete;
    Var& operator=(const Var&) = delete;

    Var(Var&&) = delete;
    Var& operator=(Var&&) = delete;

    Var(ptrdiff_t&& address) : offset(address) {}

    ptrdiff_t offset;
    // T* pointer;

    inline T& operator()() {
        return *bit_cast<T*>(GetBaseAddress() + offset);
    }

    inline T& operator*() {
        return *bit_cast<T*>(GetBaseAddress() + offset);
    }

    inline const T& operator()() const {
        return *bit_cast<T*>(GetBaseAddress() + offset);
    }

    inline const T& operator*() const {
        return *bit_cast<T*>(GetBaseAddress() + offset);
    }
};
