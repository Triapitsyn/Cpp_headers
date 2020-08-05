#include <vector>

template <typename T>
class Heap {
private:
    std::vector<T> data;

    static size_t left(size_t i) {
        return 2 * i + 1;
    }

    static size_t right(size_t i) {
        return 2 * i + 2;
    }

    static size_t parent(size_t i) {
        return i != 0 ? (i - 1) / 2 : i;
    }

    size_t greater_son(size_t i) {
        return data[left(i)] > data[right(i)] ? left(i) : right(i);
    }

public:
    void Insert(T item) {
        size_t i = data.size();
        data.push_back(item);

        while (data[i] > data[parent(i)]) {
            std::swap(data[i], data[parent(i)]);
            i = parent(i);
        }
    }

    int Extract() {
        int res = data.front();
        std::swap(data.front(), data.back());
        data.pop_back();
        size_t i = 0;
        while (left(i) < data.size()) {
            i = (right(i) < data.size() ? greater_son(i) : left(i));
            if (data[parent(i)] > data[i]) break;
            std::swap(data[parent(i)], data[i]);
        }
        return res;
    }
};
