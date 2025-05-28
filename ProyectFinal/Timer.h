// Timer.h
#pragma once

#include <chrono>
#include <iostream>
#include <vector>
#include <numeric>
#include <type_traits>
#include <utility>

struct Timer {
    std::vector<double> measurements;

    // Sobrecarga para funciones que devuelven algo (R != void)
    template<typename F, typename... Args,
        typename R = std::result_of_t<F && (Args&&...)>,
        typename = typename std::enable_if<!std::is_void<R>::value>::type>
    R run(const std::string& label, F&& func, Args&&... args) {
        using namespace std::chrono;
        auto t0 = high_resolution_clock::now();
        R result = std::forward<F>(func)(std::forward<Args>(args)...);
        auto t1 = high_resolution_clock::now();
        double ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;
        measurements.push_back(ms);
        std::cout << "[" << label << "] " << ms << " ms\n";
        return result;
    }

    // Sobrecarga para funciones void
    template<typename F, typename... Args,
        typename R = std::result_of_t<F && (Args&&...)>,
        typename = typename std::enable_if<std::is_void<R>::value>::type,
        typename = void>  // desempate de firma
    void run(const std::string& label, F&& func, Args&&... args) {
        using namespace std::chrono;
        auto t0 = high_resolution_clock::now();
        std::forward<F>(func)(std::forward<Args>(args)...);
        auto t1 = high_resolution_clock::now();
        double ms = duration_cast<microseconds>(t1 - t0).count() / 1000.0;
        measurements.push_back(ms);
        std::cout << "[" << label << "] " << ms << " ms\n";
    }

    double average() const {
        if (measurements.empty()) return 0.0;
        return std::accumulate(measurements.begin(), measurements.end(), 0.0)
            / measurements.size();
    }
};
