#include "core.hpp"
#include <unordered_set>
#include <iostream>
#include <vector>
#include <future>
#include <cmath>

/**
 * Check if Complex c is infinite.
 * If c is infinite, infinite - infinite != 0
 * If c is nan, nan - nan != 0
 */

bool infinite(Complex c) {
  return c - c != Complex{};
}


/**
 * A class that can add members to a datastructure that only stores
 * unique values. Created to benchmark different data structures for
 * this particular use case. 
 */

class unique_set {
  std::unordered_set<Complex, complex_hasher> set{};

public:

  unique_set(){}

    
  /**
   * Add element to data structure if not already exists. 
   * Return if the element was inserted. False otherwise
   */
    
  bool add(Complex c) {
    auto [_, inserted] = set.insert(c);
    return inserted;
  }
};


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
					int max_iterations) {
  Complex x{};
  
  unique_set outputs;
    
  int iterations = 0;
  
  while (iterations < max_iterations) {
    iterations++;
    x = f(x);

    bool inserted = outputs.add(x);
    bool repeated = !inserted;

    if (repeated) {
      return iterations;
    }

    if (infinite(x)) {
	return {};
    }
  }

  return {};
}


/**
 * Returns a map of complex number and an optional int 
 * indicating the number of iterations it took to reach 
 * infinity. If the function does not reach infinity within
 * max_iterations, the function is deemed infinite and empty
 * optional is returned as the value
 */

MandelbrotSet _mandelbrot_set(InclusiveRange real_range,
			     InclusiveRange imag_range,
			     T step,
			     int max_iterations) {
    MandelbrotSet result;

    double real = real_range.start;

    while (real <= real_range.end) {
	double imag = imag_range.start;

	while (imag <= imag_range.end) {
	    const Complex c(real, imag);
	    
	    auto mandelbrot = [&](Complex x) {
		return x * x + c;
	    };
	    
	    auto iterations = iterations_to_repeat(mandelbrot, max_iterations);
	    result[c] = iterations;
	    
	    imag += step;
	}

	real += step;
    }

    return result;
}


MandelbrotSet mandelbrot_set(InclusiveRange real_range,
			     InclusiveRange imag_range,
			     T step,
			     int max_iterations,
			     int thread_count) {
    /// Thread count may be 0 if implementation cannot
    /// find thread count. So use one thread in that case.
    
    if (thread_count < 1)
	thread_count = 1;

    std::vector<std::future<MandelbrotSet>> futures;
    futures.reserve(thread_count);

    MandelbrotSet result;

    T real_step = real_range.end - real_range.start / thread_count;
    T imag_step = imag_range.end - imag_range.start / thread_count;

    T current_real_start = real_range.start;
    T current_imag_start = imag_range.start;
    
    for (int i = 0; i < thread_count; i++) {
	InclusiveRange real(current_real_start, current_real_start + real_step);
	InclusiveRange imag(current_imag_start, current_imag_start + imag_step);
	
	auto future = std::async(std::launch::async, _mandelbrot_set, real,
								      imag,
								      step,
								      max_iterations);

       	futures.push_back(std::move(future));

	current_real_start += real_step;
	current_imag_start += imag_step;
    }


    for (auto& f : futures) {
	auto mandelbrot = f.get();

	result.insert(mandelbrot.begin(), mandelbrot.end());
    }

    return result;
}
