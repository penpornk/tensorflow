load("//tensorflow:tensorflow.bzl", "tf_cc_binary")

tf_cc_binary(
    name = "sgemm_v0",
    srcs = ["sgemm.cc"],
    copts = ["-DUSE_MKLDNN_V0"],
    deps = ["@mkl_dnn//:mkldnn_single_threaded"],
)

tf_cc_binary(
    name = "sgemm_v1",
    srcs = ["sgemm.cc"],
    deps = ["@mkl_dnn_v1//:dnnl_single_threaded"],
)
