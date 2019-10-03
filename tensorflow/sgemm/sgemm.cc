#include <stdio.h>
#include <stdlib.h>

#include <chrono>

#include "mkldnn.h"

#define FLOOD_SIZE 10000000

class CacheFlusher {
 public:
  CacheFlusher() : buffer_(NULL) { buffer_ = new int[FLOOD_SIZE]; }
  ~CacheFlusher() {
    if (buffer_ != NULL) delete[] buffer_;
  }
  void flush() {
    for (int i = 0; i < FLOOD_SIZE; ++i) buffer_[i] = rand();
  }

 private:
  int* buffer_;
};

void call_sgemm(float* a, float* b, float* c, const int m, const int n,
                const int k) {
  const char transpose_a = 'N';  // Not transposed.
  const char transpose_b = 'N';  // Not transposed.
  const float alpha = 1.0f;
  const float beta = 0.0f;
  const int lda = m;  // Column-major.
  const int ldb = k;  // Column-major.
  const int ldc = m;  // Column-major.

  mkldnn_status_t status =
      mkldnn_sgemm(&transpose_a, &transpose_b, &m, &n, &k, &alpha, a, &lda, b,
                   &ldb, &beta, c, &ldc);
}

void fill_matrix(float* mat, int m, int n) {
  for (int i = 0; i < m * n; ++i) {
    int sign = (rand() % 2)? 1 : -1;
    mat[i] = sign * ((rand() % 10) + 1);  // We don't want zeroes.
  }
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    printf("usage: %s <#rows of A> <#cols of B> <#cols of A or #rows of B>\n", argv[0]);
    return 0;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int k = atoi(argv[3]);

  const int kNumWarmups = 3;
  const int kNumTrials = 10;

  float* a = new float[m * k];
  float* b = new float[k * n];
  float* c = new float[m * n];

  fill_matrix(a, m, k);
  fill_matrix(b, k, n);

  for (int i = 0; i < kNumWarmups; ++i) {
    call_sgemm(a, b, c, m, n, k);
  }

  double elapsed_time = 0.0;
  CacheFlusher cache_flusher;
  for (int i = 0; i < kNumTrials; ++i) {
    cache_flusher.flush();
    auto start = std::chrono::system_clock::now();
    call_sgemm(a, b, c, m, n, k);
    auto stop = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = stop - start;
    elapsed_time += elapsed_seconds.count();
  }

  double gflops = 2e-9 * m * n * k * kNumTrials / elapsed_time;
  printf("m=%3d, n=%3d, k=%3d: %lf gflops\n", m, n, k, gflops);

  delete[] a;
  delete[] b;
  delete[] c;

  return 0;
}

