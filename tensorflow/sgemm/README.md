This microbenchmark compares sgemm from MKL-DNN v0.21.3 with DNNL v1.2. To run:
```
git clone https://github.com/penpornk/tensorflow
cd tensorflow; git checkout sgemm
yes "" | ./configure
cd tensorflow/sgemm
./benchmark.sh
```

The results are in results.csv. By default, the script reports median and
standard deviation from 5 runs.

The number of trials and sizes of `M`, `N`, and `K` can be adjusted in benchmark.sh.
