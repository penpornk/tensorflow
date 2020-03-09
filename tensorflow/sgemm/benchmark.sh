#!/bin/bash

set -e

bazel build --config=opt :sgemm_v0
bazel build --config=opt :sgemm_v1

RUN_SIZES=(32 64 128 256 512)

run () {
  NAME=$1
  echo "Benchmarking ${NAME}..."

  # Create an empty folder
  FOLDER_PATH=/tmp/${NAME}
  rm -rf ${FOLDER_PATH}  
  mkdir -p ${FOLDER_PATH}

  # Run 5 times.
  for T in 1 2 3 4 5; do 
    echo "Run #$T"
    for M in ${RUN_SIZES[@]}; do
      for N in ${RUN_SIZES[@]}; do
        for K in ${RUN_SIZES[@]}; do
          ../../bazel-bin/tensorflow/sgemm/${NAME} $M $N $K >> ${FOLDER_PATH}/$T
        done
      done
    done
  done
}

run sgemm_v0
run sgemm_v1

echo "Parsing results."
python parse.py | tee results.csv

echo "Done. Results are in results.csv."

