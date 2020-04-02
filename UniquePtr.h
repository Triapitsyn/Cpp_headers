#include <cstddef>
#include <iostream>

template <typename T>
class UniquePtr {
private:
    T * ptr;

public:
    UniquePtr() {
        ptr = nullptr;
    }
    explicit UniquePtr(T * other) noexcept : ptr(other) {}
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator= (std::nullptr_t null_ptr) noexcept {
        ptr = null_ptr;
        return *this;
    }
    UniquePtr& operator= (UniquePtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete this->ptr;
        this->ptr = other.ptr;
        other = nullptr;
        return *this;
    }

    ~UniquePtr() noexcept {
        delete ptr;
    }

    T& operator * () const noexcept {
        return *ptr;
    }

    T * operator -> () const noexcept {
        return ptr;
    }

    T * release() noexcept {
        T * tmp = ptr;
        *this = nullptr;
        return tmp;
    }

    void reset(T * other) noexcept {
        delete this->ptr;
        this->ptr = other;
    }

    void swap(UniquePtr& other) {
        std::swap(ptr, other.ptr);
    }

    [[nodiscard]] T * get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return (ptr != nullptr);
    }

    UniquePtr& operator= (UniquePtr& other) = delete;
    UniquePtr(UniquePtr& other) = delete;
};