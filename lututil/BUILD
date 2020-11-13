cc_library(
    name = "lututil",
    srcs = [
        "lututil.cc",
    ],
    hdrs = [
        "lututil.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lutmask",
    ],
)

cc_test(
    name = "lututil_test",
    srcs = ["lututil_test.cc"],
    data = [
        "results_file",
    ],
    deps = [
        ":lututil",
        "//lutmask",
        "//parse",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
