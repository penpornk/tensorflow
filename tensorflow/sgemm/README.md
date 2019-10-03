To run the sgemm benchmark with MKL-DNN v0.20.3 (the current default):
```
git clone https://github.com/penpornk/tensorflow
cd tensorflow; git checkout sgemm
yes "" | ./configure
cd tensorflow/sgemm
bazel build --config=opt :sgemm
./benchmark.sh
```

To try with MKL-DNN v0.18 or v0.20.4, rename the appropriate `tf_http_archive` in
`${TENSORFLOW_ROOT}/tensorflow/workspace.bzl` to `mkl_dnn` (and rename the current `mkl_dnn` to something else).
* v0.18: change line 149
* v0.20.4: change line 162
Then, run
```
cd ${TENSORFLOW_ROOT}/tensorflow/sgemm
bazel build --config=opt :sgemm
./benchmark.sh
```
