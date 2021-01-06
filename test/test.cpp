#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "core.hpp"

TEST_CASE("Iterations to repeat test cases") {
    SECTION("f(x) = x^2 + (-1 - 1i) takes 3 iterations to repeat") {
        const Complex c(-1);
        auto mandelbrot_set = [=](Complex x) {
            return x * x + c;
        };

        int iterations = iterations_to_repeat(mandelbrot_set, 1000).value();

        /**
	 * In iteration 1: x = f(x) => -1
	 * In iteration 2: x = f(x) => 0
	 * In iteration 3: x = f(x) => -1
	 */
	REQUIRE(iterations == 3);
    }

    SECTION("f(x) = 1 takes 2 iterations to repeat") {
      auto unity = [](Complex _) {
	  return 1;
      };

      int iterations = iterations_to_repeat(unity, 1000).value();

      REQUIRE(iterations == 2);
    }

    SECTION("f(x) = x^2 + 1 returns empty optional because the elements never repeat") {
      const Complex c(1);
      auto mandelbrot_set = [=](Complex x) {
	return x * x + c;
      };

      bool has_value = iterations_to_repeat(mandelbrot_set, 100).has_value();

      REQUIRE(has_value == false);
    }
}


TEST_CASE("Mandelbrot set test cases") {
  SECTION("For Mandelbrot set between -1 and 0 with step 1 "
	  "-1 and 0 repeat and 1 does not repeat ") {
      InclusiveRange real_range(-1, 1);
      InclusiveRange imag_range(0, 0);

      auto result = mandelbrot_set(real_range, imag_range, 1);

      /// result should have 3 elements
      REQUIRE(result.size() == 3);
      
      /// f(x) = x^2 + (-1) is repeatitive in 3rd iteration
      REQUIRE(result[Complex(-1, 0)].value() == 3);

      /// f(x) = x^2 + 0 repeats in 2nd iteration
      REQUIRE(result[Complex(0, 0)].value() == 2);

      /// f(x) = x^2 + 1 is infinite
      REQUIRE(result[Complex(1, 0)].has_value() == false);
  }
}
