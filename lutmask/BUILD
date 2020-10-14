cc_library(
    name = "lutmask",
    srcs = ["lutmask.cc"],
    hdrs = ["lutmask.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "lutmask_test",
    srcs = ["lutmask_test.cc"],
    deps = [
        ":lutmask",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
