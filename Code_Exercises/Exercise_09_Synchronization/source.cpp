/*
 SYCL Academy (c)

 SYCL Academy is licensed under a Creative Commons
 Attribution-ShareAlike 4.0 International License.

 You should have received a copy of the license along with this
 work.  If not, see <http://creativecommons.org/licenses/by-sa/4.0/>.

 * SYCL Quick Reference
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * // Default construct a queue
 * auto q = sycl::queue{};
 *
 * // Declare a buffer pointing to ptr
 * auto buf = sycl::buffer{ptr, sycl::range{n}};
 *
 * // Do a USM malloc_device
 * auto ptr = sycl::malloc_device<T>(n, q);
 *
 * // Do a USM memcpy
 * q.memcpy(dst_ptr, src_ptr, sizeof(T)*n);
 *
 * // Wait on a queue
 * q.wait();
 *
 * // Submit work to the queue
 * q.submit([&](sycl::handler &cgh) {
 *   // COMMAND GROUP
 * });
 *
 *
 * // Within the command group you can
 * //    1. Declare an accessor to a buffer
 *          auto read_write_acc = sycl::accessor{buf, cgh};
 *          auto read_acc = sycl::accessor{buf, cgh, sycl::read_only};
 *          auto write_acc = sycl::accessor{buf, cgh, sycl::write_only};
 *          auto no_init_acc = sycl::accessor{buf, cgh, sycl::no_init};
 * //    2. Enqueue a parallel for:
 *              cgh.parallel_for<class mykernel>(sycl::range{n}, 
 *                    [=](sycl::id<1> i) { // Do something });
 *
 *
*/

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "sycl/sycl.hpp"

/*
TEST_CASE("synchronization_usm", "synchronization_source") {
  // Use your code from Exercise 3 to start
  REQUIRE(true);
}
*/

TEST_CASE("synchronization_buffer_acc", "synchronization_source") {
  // Use your code from Exercise 3 to start

  // Using exercise 6
  constexpr size_t dataSize = 1024;

  float a[dataSize], b[dataSize], r[dataSize];
  for (int i = 0; i < dataSize; ++i) {
    a[i] = static_cast<float>(i);
    b[i] = static_cast<float>(i);
    r[i] = 0.0f;
  }

  // Task: Compute r[i] = a[i] + b[i] in parallel on the SYCL device
  auto q = sycl::queue{};

  {
    auto bufA = sycl::buffer{a, sycl::range{dataSize}};
    auto bufB = sycl::buffer{b, sycl::range{dataSize}};
    auto bufR = sycl::buffer{r, sycl::range{dataSize}};

    q.submit([&](sycl::handler &cgh)
             {
               auto accA = sycl::accessor{bufA, cgh, sycl::read_only};
               auto accB = sycl::accessor{bufB, cgh, sycl::read_only};
               auto accR = sycl::accessor{bufR, cgh, sycl::no_init};

               cgh.parallel_for<>(sycl::range{dataSize},
                                  [=](sycl::id<1> i)
                                  {
                                    accR[i] = accA[i] + accB[i];
                                  });
             });

    q.wait();
  }

  REQUIRE(true);
}
