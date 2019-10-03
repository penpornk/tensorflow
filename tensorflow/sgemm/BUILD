load("//tensorflow:tensorflow.bzl", "tf_cc_binary")

tf_cc_binary(   
    name = "sgemm",
    srcs = ["sgemm.cc"],
    deps = ["@mkl_dnn//:mkldnn_single_threaded"],
)
