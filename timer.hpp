#pragma once

#include <chrono>
#include <iostream>
#include <string>

class ScopedTimer {
public:
    explicit ScopedTimer(std::string name)
        : name_(std::move(name)),
          start_(std::chrono::steady_clock::now()) {}

    ~ScopedTimer() {
        auto end = std::chrono::steady_clock::now();

        double elapsed = std::chrono::duration<double, std::milli>(
                             end - start_).count();

        std::cout << name_ << ": " << elapsed << " ms\n";
    }

    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

private:
    std::string name_;
    std::chrono::steady_clock::time_point start_;
};