#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <memory>
#include <vector>


template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    using key_value_pair = std::pair<KeyType, ValueType>;

public:
    using iterator = typename std::vector<key_value_pair>::iterator;
    using const_iterator = typename std::vector<key_value_pair>::const_iterator;

private:
    std::vector<key_value_pair> data;
    std::vector<std::vector<size_t>> index_data;
    Hash hasher;

    size_t get_hash(const KeyType& key) const {
        return hasher(key) % index_data.size();
    }

    void reallocate() {
        if (data.size() >= index_data.size() * 2) {
            size_t new_size = index_data.size() * 2;
            index_data.clear();
            index_data.resize(new_size);
            for (size_t i = 0; i < data.size(); ++i) {
                index_data[get_hash(data[i].first)].push_back(i);
            }
        }
    }

public:
    explicit HashMap(const Hash& hasher = Hash())
            : data(0)
            , index_data(1)
            , hasher(hasher) {}

    template<class Forward_Iter>
    HashMap(Forward_Iter begin, Forward_Iter end, const Hash& hasher = Hash())
            : HashMap(hasher) {
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> l, const Hash& hasher = Hash())
            :HashMap(l.begin(), l.end(), hasher) {}

    [[nodiscard]] size_t size() const {
        return data.size();
    }
    [[nodiscard]] bool empty() const {
        return data.empty();
    }
    const Hash& hash_function() const {
        return hasher;
    }

    iterator begin() {
        return data.begin();
    }
    const_iterator begin() const {
        return data.cbegin();
    }
    iterator end() {
        return data.end();
    }
    const_iterator end() const {
        return data.cend();
    }

    iterator find(const KeyType& key) {
        for (auto it : index_data[get_hash(key)]) {
            if (data[it].first == key) {
                return data.begin() + it;
            }
        }
        return data.end();
    }

    const_iterator find(const KeyType& key) const {
        for (auto it : index_data[get_hash(key)]) {
            if (data[it].first == key) {
                return data.begin() + it;
            }
        }
        return data.cend();
    }

    void erase(const KeyType& key) {
        if (find(key) == end()) {
            return;
        }
        auto it = find(key);
        size_t hash_it = get_hash(key);
        size_t hash_end = get_hash(data.back().first);
        size_t index_it = std::distance(data.begin(), it);
        size_t index_end = data.size() - 1;

        swap(data[index_it], data[index_end]);
        data.pop_back();

        for (auto& index : index_data[hash_it]) {
            if (index == index_it) {
                std::swap(index, index_data[hash_it].back());
                break;
            }
        }
        index_data[hash_it].pop_back();
    }

    void insert(const key_value_pair& p) {
        if (find(p.first) == data.end()) {
            data.push_back(p);
            index_data[get_hash(p.first)].push_back(data.size() - 1);
            reallocate();
        }
    }

    const ValueType& at(const KeyType& key) const {
        if (find(key) != data.end()) {
            return find(key)->second;
        } else {
            throw std::out_of_range("");
        }
    }

    ValueType& operator[](const KeyType& key) {
        insert({key, ValueType()});
        return find(key)->second;
    }

    void clear() {
        data.clear();
        index_data.clear();
        index_data.resize(1);
    }
};
