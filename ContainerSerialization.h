#include <map>
#include <iostream>
#include <string>
#include <vector>

void Serialize(const std::string& str, std::ostream& out);
template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out);
template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);

void Deserialize(std::istream& in, std::string& str);
template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data);
template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);

template <typename T>
void Serialize(const T& pod, std::ostream& out) {
    out.write(reinterpret_cast<const char *>(&pod), sizeof(pod));
}

void Serialize(const std::string& str, std::ostream& out) {
    size_t sz = str.size();
    out.write(reinterpret_cast<char *>(&sz), sizeof(sz));
    for (const auto& ch : str) {
        Serialize(ch, out);
    }
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    size_t sz = data.size();
    out.write(reinterpret_cast<char *>(&sz), sizeof(sz));
    for (const auto& x : data) {
        Serialize(x, out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    size_t sz = data.size();
    out.write(reinterpret_cast<char *>(&sz), sizeof(sz));
    for (const auto& [x, y] : data) {
        Serialize(x, out);
        Serialize(y, out);
    }
}



template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char *>(&pod), sizeof(pod));
}

void Deserialize(std::istream& in, std::string& str) {
    size_t sz;
    in.read(reinterpret_cast<char *>(&sz), sizeof(sz));
    str.resize(sz);
    for (auto& ch : str) {
        Deserialize(in, ch);
    }
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t sz;
    in.read(reinterpret_cast<char *>(&sz), sizeof(sz));
    data.resize(sz);
    for (auto& x : data) {
        Deserialize(in, x);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t sz;
    in.read(reinterpret_cast<char *>(&sz), sizeof(sz));
    T1 x;
    T2 y;
    for (int i = 0; i < sz; ++i) {
        Deserialize(in, x);
        Deserialize(in, y);
        data[x] = y;
    }
}