#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>


template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    using key_value_t = typename std::pair<KeyType, ValueType>;
    size_t initial_size = 1;

    enum class Status {
        OK, DELETED, EMPTY
    };

public:
    class iterator {
    private:
        size_t index;
        HashMap* map;

    public:
        explicit iterator(size_t index = 0, HashMap* map = nullptr)
                : index(index)
                , map(map) {}

        iterator& operator++() {
            while (++index != map->data.size() && map->status[index] != Status::OK) {}
            return *this;
        }

        iterator operator++(int) {
            iterator copied = *this;
            ++*this;
            return copied;
        }

        bool operator ==(const iterator& other) const {
            return (index == other.index) && (map == other.map);
        }

        bool operator !=(const iterator& other) const {
            return !(*this == other);
        }

        key_value_t& operator*() const {
            return reinterpret_cast<key_value_t&>(map->data[index]);
        }

        key_value_t* operator->() const {
            return reinterpret_cast<key_value_t*>(&map->data[index]);
        }
    };

    class const_iterator {
    private:
        size_t index;
        const HashMap* map;

    public:
        explicit const_iterator(size_t index = 0, const HashMap* map = nullptr)
                : index(index)
                , map(map) {}

        const_iterator& operator++() {
            while (++index != map->data.size() && map->status[index] != Status::OK) {}
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator copied = *this;
            ++*this;
            return copied;
        }

        bool operator ==(const const_iterator& other) const {
            return (index == other.index) && (map == other.map);
        }

        bool operator !=(const const_iterator& other) const {
            return !(*this == other);
        }

        const key_value_t& operator*() const {
            return reinterpret_cast<const key_value_t&>(map->data[index]);
        }

        const key_value_t* operator->() const {
            return reinterpret_cast<const key_value_t*>(&map->data[index]);
        }
    };

public:
    std::vector<key_value_t> data;
    std::vector<Status> status;
    size_t sz = 0, real_sz = initial_size;
    Hash hasher;

    size_t get_hash(const KeyType& key, size_t mod) const {
        return hasher(key) % mod;
    }

    size_t get_index(
            const std::vector<key_value_t>& target_data,
            const std::vector<Status>& target_status,
            const KeyType& key) const {
        size_t pos = get_hash(key, target_data.size());
        for (size_t i = pos; i < target_data.size(); ++i) {
            if (target_data[i].first == key || target_status[i] == Status::EMPTY) {
                return i;
            }
        }
        for (size_t i = 0; i < pos; ++i) {
            if (target_data[i].first == key || target_status[i] == Status::EMPTY) {
                return i;
            }
        }
        return sz;
    }

    void rehash(size_t new_size) {
        std::vector<key_value_t> new_data(new_size);
        std::vector<Status> new_status(new_size, Status::EMPTY);
        for (size_t i = 0; i < data.size(); ++i) {
            if (status[i] == Status::OK) {
                size_t new_i = get_index(new_data, new_status, data[i].first);
                new_data[new_i] = std::move(data[i]);
                new_status[new_i] = Status::OK;
            }
        }
        data = std::move(new_data);
        status = std::move(new_status);
    }

public:
    explicit HashMap(const Hash& hasher = Hash())
            : hasher(hasher) {
        clear();
    }

    template<typename Iter>
    HashMap(Iter begin, Iter end, const Hash& hasher = Hash())
            : HashMap(hasher) {
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    HashMap (const std::initializer_list<key_value_t>& l, const Hash& hasher = Hash())
            : HashMap(l.begin(), l.end(), hasher) {}

    iterator begin() {
        if (status[0] == Status::OK) {
            return iterator(0, this);
        } else {
            return ++iterator(0, this);
        }
    }
    const_iterator begin() const {
        if (status[0] == Status::OK) {
            return const_iterator(0, this);
        } else {
            return ++const_iterator(0, this);
        }
    }
    iterator end() {
        return iterator(data.size(), this);
    }
    const_iterator end() const {
        return const_iterator(data.size(), this);
    }

    [[nodiscard]] size_t size() const {
        return sz;
    }
    [[nodiscard]] bool empty() const {
        return data.empty();
    }
    const Hash& hash_function() const {
        return hasher;
    }

    void insert(const key_value_t& p) {
        size_t index = get_index(data, status, p.first);
        if (status[index] == Status::EMPTY) {
            ++real_sz;
        }
        if (status[index] != Status::OK) {
            ++sz;
            data[index] = p;
            status[index] = Status::OK;
        }
        if (2 * real_sz > data.size()) {
            rehash(2 * data.size());
        }
    }

    void erase(const KeyType& key) {
        size_t index = get_index(data, status, key);
        if (status[index] == Status::OK) {
            --sz;
            status[index] = Status::DELETED;
        }
    }

    iterator find(const KeyType& key) {
        size_t index = get_index(data, status, key);
        if (status[index] == Status::OK) {
            return iterator(index, this);
        }
        return end();
    }

    const_iterator find(const KeyType& key) const {
        size_t index = get_index(data, status, key);
        if (status[index] == Status::OK) {
            return const_iterator(index, this);
        }
        return end();
    }

    ValueType& operator[] (const KeyType& key) {
        insert({key, ValueType()});
        return find(key)->second;
    }

    const ValueType& at(const KeyType& key) const {
        size_t index = get_index(data, status, key);
        if (status[index] == Status::OK) {
            return data[index].second;
        }
        throw std::out_of_range("");
    }

    void clear() {
        data.resize(initial_size);
        status.assign(initial_size, Status::EMPTY);
        real_sz = 0;
        sz = 0;
    }
};
