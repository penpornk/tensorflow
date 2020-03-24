#include <immintrin.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>

#ifdef __AVX512F__
#define REG_WIDTH 512
#define LOAD      _mm512_loadu_ps
#define STORE     _mm512_storeu_ps
#define MUL       _mm512_mul_ps
#define REG       __m512
#else
#define REG_WIDTH 256
#define LOAD      _mm256_loadu_ps
#define STORE     _mm256_storeu_ps
#define MUL       _mm256_mul_ps
#define REG       __m256
#endif  // __AVX512F__

using namespace std;

int main(int argc, char *argv[]) {
  const int n = 128 << 20;  // ~1.28e8.
  const int trials = 10;
  int size_in_bytes = n << 2;
  float *a = static_cast<float *>(aligned_alloc(512, size_in_bytes));
  float *b = static_cast<float *>(aligned_alloc(512, size_in_bytes));
  float *c = static_cast<float *>(aligned_alloc(512, size_in_bytes));

  // Initialization.
  for (int i = 0; i < n; ++i) {
    a[i] = float(rand() % 8);
    b[i] = float(rand() % 8);
    c[i] = 0.0f;
  }

  int stride = REG_WIDTH / 8 / sizeof(float);
  int unrolled_stride = stride * 4;
  auto start = chrono::steady_clock::now();
  for (int iter = 0; iter < trials; ++iter) {
    float *a_ptr = a;
    float *b_ptr = b;
    float *c_ptr = c;
    for (int i = 0; i < n; i += unrolled_stride) {
      REG a0 = LOAD(a_ptr);
      REG b0 = LOAD(b_ptr);
      REG a1 = LOAD(a_ptr + stride);
      REG b1 = LOAD(b_ptr + stride);
      REG a2 = LOAD(a_ptr + stride * 2);
      REG b2 = LOAD(b_ptr + stride * 2);
      REG a3 = LOAD(a_ptr + stride * 3);
      REG b3 = LOAD(b_ptr + stride * 3);
      REG c0 = MUL(a0, b0);
      REG c1 = MUL(a1, b1);
      REG c2 = MUL(a2, b2);
      REG c3 = MUL(a3, b3);
      STORE(c_ptr, c0);
      STORE(c_ptr + stride, c1);
      STORE(c_ptr + stride * 2, c2);
      STORE(c_ptr + stride * 3, c3);
      a_ptr += unrolled_stride;
      b_ptr += unrolled_stride;
      c_ptr += unrolled_stride;
    }
  }
  auto stop = chrono::steady_clock::now();

  // Uncomment the loop below to verify results.
  // for (int i = 0; i < n; ++i) {
  //   // The results will actually be exact (c[i] == a[i] * b[i]) 
  //   // because they are all small integer values by initialization.
  //   if (fabs(c[i] - a[i] * b[i]) > 1e-6) {
  //     std::cout << "WRONG!\n";
  //     break;
  //   }
  //  }

  double nanoseconds =
      chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
  double gflops = n * trials / nanoseconds;
  cout << gflops << " Gflops/s\n";

  delete[] a;
  delete[] b;
  delete[] c;
  return 0;
}

