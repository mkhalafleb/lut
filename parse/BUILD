cc_library(
    name = "parse",
    srcs = [
        "parse.cc",
        "readfile.cc",
    ],
    hdrs = [
        "parse.h",
        "readfile.h",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "parse_test",
    srcs = ["parse_test.cc"],
    data = [
        "results_file"
    ],
    deps = [
        ":parse",
        "@gtest",
        "@gtest//:gtest_main",
    ],
)
