#include <cstddef>
#include <memory>
#include <new>
#include <utility>

template <typename T>
struct RawMemory {
    T* buf = nullptr;
    size_t cp = 0;

    static T* Allocate(size_t n) {
        return static_cast<T*>(operator new(n * sizeof(T)));
    }

    static void Deallocate(T* buf) {
        operator delete(buf);
    }

    void Swap(RawMemory& other) noexcept {
        std::swap(buf, other.buf);
        std::swap(cp, other.cp);
    }

    RawMemory() = default;

    explicit RawMemory(size_t n) {
        buf = Allocate(n);
        cp = n;
    }

    RawMemory(const RawMemory&) = delete;

    RawMemory(RawMemory&& other) noexcept {
        Swap(other);
    }

    RawMemory& operator=(const RawMemory&) = delete;

    RawMemory& operator=(RawMemory&& other) noexcept {
        Swap(other);
        return *this;
    }

    T * operator + (size_t shift) {
        return buf + shift;
    }
    const T * operator + (size_t shift) const {
        return buf + shift;
    }

    T& operator[] (size_t i) {
        return buf[i];
    }
    const T& operator[] (size_t i) const {
        return buf[i];
    }

    ~RawMemory() {
        Deallocate(buf);
    }
};

template <typename T>
class Vector {
private:
    RawMemory<T> data;
    size_t sz = 0;

public:
    Vector() = default;

    explicit Vector(size_t n): data(n) {
        std::uninitialized_value_construct_n(data.buf, n);
        sz = n;
    }

    Vector(const Vector& other): data(other.sz) {
        std::uninitialized_copy_n(other.data.buf, other.sz, data.buf);
        sz = other.sz;
    }

    void swap(Vector& other) noexcept {
        data.Swap(other.data);
        std::swap(sz, other.sz);
    }

    Vector(Vector&& other) noexcept {
        swap(other);
    }

    ~Vector() {
        std::destroy_n(data.buf, sz);
    }

    void reserve(size_t n) {
        if (n > capacity()) {
            RawMemory<T> data2(n);
            std::uninitialized_move_n(data.buf, sz, data2.buf);
            std::destroy_n(data.buf, sz);
            data.Swap(data2);
        }
    }

    [[nodiscard]] size_t size() const {
        return sz;
    }

    [[nodiscard]] size_t capacity() const {
        return data.cp;
    }

    const T& operator[](size_t i) const {
        return data[i];
    }

    T& operator[](size_t i) {
        return data[i];
    }

    Vector& operator=(const Vector& other) {
        if (other.sz > capacity()) {
            Vector tmp(other);
            swap(tmp);
        } else {
            for (size_t i = 0; i < sz && i < other.sz; ++i) {
                data[i] = other[i];
            }
            if (sz < other.sz) {
                std::uninitialized_copy_n(other.data.buf + sz, other.sz - sz, data.buf + sz);
            } else if (sz > other.sz) {
                std::destroy_n(data.buf + other.sz, sz - other.sz);
            }
            sz = other.sz;
        }
        return *this;
    }
    Vector& operator=(Vector&& other) noexcept {
        swap(other);
        return *this;
    }

    void resize(size_t n) {
        reserve(n);
        if (sz < n) {
            std::uninitialized_value_construct_n(data + sz, n - sz);
        } else if (sz > n) {
            std::destroy_n(data + n, sz - n);
        }
        sz = n;
    }

    void clear() {
        resize(0);
    }
    void push_back(const T& elem) {
        if (sz == capacity()) {
            reserve(sz == 0 ? 1 : sz * 2);
        }
        new (data + sz) T(elem);
        ++sz;
    }
    void push_back(T&& elem) {
        if (sz == capacity()) {
            reserve(sz == 0 ? 1 : sz * 2);
        }
        new (data + sz) T(std::move(elem));
        ++sz;
    }
    void pop_back() {
        std::destroy_at(data + sz - 1);
        --sz;
    }

    inline auto begin() {
        return data.buf;
    }
    inline auto begin() const {
        return data.buf;
    }
    inline auto end() {
        return data.buf + sz;
    }
    inline auto end() const {
        return data.buf + sz;
    }
};

