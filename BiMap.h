#include <stdexcept>
#include <optional>
#include <map>
#include <memory>

using std::optional;

template <typename Key1, typename Key2, typename Value>
class BiMap {
private:
    std::map<Key1, std::shared_ptr<Value>> map1;
    std::map<Key2, std::shared_ptr<Value>> map2;

public:
    // Insert value using either one or both keys
    // Raises std::invalid_argument("explanation") in case of
    // both keys emptiness or one of keys is already used
    void Insert(const optional<Key1>& key1, const optional<Key2>& key2, const Value& value) {
        if (!key1.has_value() && !key2.has_value()) {
            throw std::invalid_argument("empty keys");
        }
        if (key1.has_value() && map1.find(key1.value()) != map1.end()) {
            throw std::invalid_argument("key1 is already used");
        }
        if (key2.has_value() && map2.find(key2.value()) != map2.end()) {
            throw std::invalid_argument("key2 is already used");
        }
        auto x = std::make_shared<Value>(value);
        if (key1.has_value()) {
            map1[key1.value()] = x;
        }
        if (key2.has_value()) {
            map2[key2.value()] = x;
        }
    }

    // Get value by first key
    // Raises std::out_of_range("explanation") in case of
    // key emptiness (as std::map)
    Value& GetByPrimaryKey(const Key1& key) {
        return *map1.at(key);
    }
    [[nodiscard]] const Value& GetByPrimaryKey(const Key1& key) const {
        return *map1.at(key);
    }

    // Same function for second key
    Value& GetBySecondaryKey(const Key2& key) {
        return *map2.at(key);
    }
    [[nodiscard]] const Value& GetBySecondaryKey(const Key2& key) const {
        return *map2.at(key);
    }
};