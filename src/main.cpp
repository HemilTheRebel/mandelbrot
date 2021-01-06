#include <iostream>
#include <chrono>

#include "core.hpp"

int main() {
    InclusiveRange real_range(0, 4);
    InclusiveRange imag_range(0, 4);
    T step = 0.001;

    auto start = std::chrono::steady_clock::now();
    auto result = mandelbrot_set(real_range, imag_range, step);
    auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '\n';
}
