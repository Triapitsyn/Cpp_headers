#include <cstddef>
#include <iostream>

struct BadOptionalAccess: public std::exception {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char bytes[sizeof(T)]{};
    bool defined = false;

public:
    Optional() = default;

    explicit Optional(const T& val) {
        new (bytes) T(val);
        defined = true;
    }
    explicit Optional(T && val) {
        new (bytes) T(std::move(val));
        defined = true;
    }
    Optional(const Optional& opt) {
        if (opt.defined) {
            defined = true;
            new (bytes) T(*reinterpret_cast<const T *>(opt.bytes));
        }
    }

    Optional& operator= (const Optional& opt) {
        if (this == &opt) {
            return *this;
        }
        if (defined) {
            if (opt.defined) {
                *(reinterpret_cast<T *>(bytes)) = *(reinterpret_cast<const T *>(opt.bytes));
            } else {
                reinterpret_cast<T*>(bytes)->~T();
                defined = false;
            }
        } else if (opt.defined) {
            new (bytes) T(*reinterpret_cast<const T*>(opt.bytes));
            defined = true;
        }
        return *this;
    }
    Optional& operator= (const T& elem) {
        if (defined) {
            *(reinterpret_cast<T *>(bytes)) = elem;
        } else {
            new (bytes) T(elem);
            defined = true;
        }
        return *this;
    }
    Optional& operator= (T&& elem) {
        if (defined) {
            *(reinterpret_cast<T *>(bytes)) = std::move(elem);
        } else {
            new (bytes) T(std::move(elem));
            defined = true;
        }
        return *this;
    }

    [[nodiscard]] bool has_value() const {
        return defined;
    }

    T& operator * () {
        return *(reinterpret_cast<T *>(bytes));
    }

    const T& operator * () const {
        return *(reinterpret_cast<const T *>(bytes));
    }

    T * operator->() {
        return reinterpret_cast<T *>(bytes);
    }
    const T * operator->() const {
        return reinterpret_cast<const T *>(bytes);
    }

    T& value() {
        if (defined) {
            return *(reinterpret_cast<T *>(bytes));
        } else {
            throw BadOptionalAccess();
        }
    }

    [[nodiscard]] const T& value() const {
        if (defined) {
            return *(reinterpret_cast<const T *>(bytes));
        } else {
            throw BadOptionalAccess();
        }
    }

    void reset() {
        if (defined) reinterpret_cast<T *>(bytes)->~T();
        defined = false;
    }

    ~Optional() {
        if (defined) reinterpret_cast<T *>(bytes)->~T();
    }
};