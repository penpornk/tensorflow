#!/bin/bash

for M in 16 32 64 128 256; do
  for N in 16 32 64 128 256; do
    for K in 16 32 64 128 256; do
      ../../bazel-bin/tensorflow/sgemm/sgemm $M $N $K
    done
  done
done
