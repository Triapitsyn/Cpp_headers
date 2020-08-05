#include <vector>

class DSU {
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;

    size_t get_rep(size_t n) {
        return n == parent[n] ? n : parent[n] = get_rep(parent[n]);
    }

public:
    explicit DSU(size_t n) {
        rank.resize(n);
        parent.reserve(n);
        for (size_t i = 0; i < n; ++i) parent.push_back(i);
    }

    void merge(size_t first, size_t second) {
        size_t first_rep = get_rep(first), second_rep = get_rep(second);
        if (first_rep == second_rep) return;
        if (rank[first_rep] < rank[second_rep]) std::swap(first_rep, second_rep);
        parent[second_rep] = first_rep;
        rank[first_rep] += rank[second_rep];
    }

    bool are_equal(size_t first, size_t second) {
        return get_rep(first) == get_rep(second);
    }
};