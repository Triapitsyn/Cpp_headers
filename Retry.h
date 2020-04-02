#include <exception>
#include <functional>
#include <optional>
#include <thread>
#include <chrono>

template <typename Result, typename Exception = std::exception>
std::optional<Result> DoWithRetry(std::function<Result()> func, int cnt, int t, bool throwLast) {
    for (int i = 0; i < cnt + 1; ++i) {
        try {
            return func();
        } catch (const Exception& err) {
            if (i == cnt) {
                if (throwLast) {
                    throw err;
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(t));
            }
        }
    }
    return {};
}
