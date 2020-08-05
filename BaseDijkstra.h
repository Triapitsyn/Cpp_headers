#include <vector>
#include <unordered_map>
#include <set>

struct Edge {
    size_t start, finish, weight;
};

using Vertex = decltype(Edge::start);
using Distance = decltype(Edge::weight);

std::unordered_map<Vertex, Distance> dijkstra(
        const std::unordered_map<Vertex,
        std::vector<Edge>>& friends, const Vertex& start) {
    std::unordered_map<Vertex, Distance> result;
    result[start] = Distance(0);

    std::set<std::pair<Distance, Vertex>> queue;
    queue.insert({Distance(0), start});

    while (!queue.empty()) {
        auto it = queue.begin();
        const auto& [distance, vertex] = *it;

        if (friends.find(vertex) != friends.end()) {
            for (const Edge &edge: friends.at(vertex)) {
                Distance new_distance = distance + edge.weight;
                if (result.find(edge.finish) == result.end()) {
                    result[edge.finish] = new_distance;
                    queue.insert({new_distance, edge.finish});
                } else if (new_distance < result[edge.finish]) {
                    queue.erase({result[edge.finish], edge.finish});
                    result[edge.finish] = new_distance;
                    queue.insert({new_distance, edge.finish});
                }
            }
        }

        queue.erase(it);
    }

    return result;
}