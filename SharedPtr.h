#include <cstddef>

template <typename T>
class SharedPtr {
public:
    T * ptr = nullptr;
    size_t * cnt = new size_t(1);

    void suicide() {
        --*cnt;
        if (*cnt == 0) {
            delete ptr;
            delete cnt;
        }
    }

public:
    SharedPtr() = default;
    explicit SharedPtr(T * new_ptr) noexcept: ptr(new_ptr) {}
    explicit SharedPtr(std::nullptr_t null_ptr) noexcept: ptr(null_ptr) {}
    SharedPtr(const SharedPtr& other) noexcept: ptr(other.ptr), cnt(other.cnt) {
        ++*cnt;
    }
    SharedPtr(SharedPtr&& other) noexcept: ptr(other.ptr), cnt(other.cnt) {
        other.ptr = nullptr;
        other.cnt = new size_t(1);
    }

    SharedPtr& operator= (const SharedPtr& other) noexcept {
        if (this == &other) {
            return *this;
        }
        suicide();
        this->cnt = other.cnt;
        ++*cnt;
        this->ptr = other.ptr;
        return *this;
    }
    SharedPtr& operator= (SharedPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        suicide();
        this->cnt = other.cnt;
        this->ptr = other.ptr;
        other.cnt = new size_t(1);
        other.ptr = nullptr;
        return *this;
    }
    SharedPtr& operator= (T * new_ptr) noexcept {
        *this = SharedPtr(new_ptr);
        return *this;
    }
    SharedPtr& operator= (std::nullptr_t null_ptr) noexcept {
        *this = SharedPtr(null_ptr);
        return *this;
    }

    ~SharedPtr() noexcept {
        suicide();
    }

    T& operator * () noexcept {
        return *ptr;
    }
    const T& operator * () const noexcept {
        return *ptr;
    }
    const T * operator -> () const noexcept {
        return ptr;
    }

    void reset(T * other) noexcept {
        *this = SharedPtr(other);
    }

    void swap(SharedPtr& other) {
        std::swap(ptr, other.ptr);
        std::swap(cnt, other.cnt);
    }

    T * get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
};