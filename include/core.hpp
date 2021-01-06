#ifndef MANDELBROT_CORE_H
#define MANDELBROT_CORE_H

#include <complex>
#include <functional>
#include <unordered_map>
#include <optional>
#include <thread>

/**
 * Used to make it easy to change the underlying complex type
 * without affecting everything. T should be a type with 
 * std::hash specialized  for it. 
 */
using T = double;


/**
 * Shorthand. std::complex<T> is too long ;P
 */
using Complex = std::complex<T>;


/**
 * The number of iterations it takes function f to output
 * a value twice. F is successively called on its own 
 * output. Ex - f(x), f(f(x)), f(f(f(x))), etc. till max_iteration
 * if no value is repeated till max iterations, empty optional 
 * is retuned. Otherwise the number of iterations it took for a 
 * value to repeat is returned. 
 *
 * !!Note:!! The value will never be less than 2 if the function
 * repeats. This is because we need at least two values to check 
 * if a value is repeated. 
 */
std::optional<int> iterations_to_repeat(std::function<Complex(Complex)> f,
					int max_iterations = 1000);


/**
 * Hasher for Complex. Needed for std::unordered_set to work
 */
struct complex_hasher {
  inline size_t operator()(Complex x) const {
    return std::hash<T>{}(x.real()) ^ std::hash<T>{}(x.imag());
  }
};


/**
 * The result of mandelbrot set is a map of complex numbers
 * and an optional int indicating the number of iterations
 * it took for the function to repeat. 
 *
 * The type was too long. So using a using statement. 
 */
using MandelbrotSet = std::unordered_map<Complex, std::optional<int>, complex_hasher>;


/**
 * Inclusive Range is struct representing the start 
 * and end of includive range. 
 *
 * mandelbrot_set below had too many arguments. 
 * This struct reduces them
 */
struct InclusiveRange {
    T start, end;

    InclusiveRange(T start, T end)
	: start(start), end(end) {
	if (start > end) {
	    throw std::logic_error("Start should not be greater than end");
	}
    }
};


/**
 * Returns a map of complex number and an optional int 
 * indicating the number of iterations it took to reach 
 * infinity. If the function does not reach infinity within
 * max_iterations, the function is deemed infinite and empty
 * optional is returned as the value.
 *
 * !!Note:!! May launch multiple threads to speed up computation
 */
MandelbrotSet mandelbrot_set(InclusiveRange real_range,
			     InclusiveRange imag_range,
			     T step,
			     int max_iterations = 1000,
			     int thread_count = std::thread::hardware_concurrency()
			     );

#endif
