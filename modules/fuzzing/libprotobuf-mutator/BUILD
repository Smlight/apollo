load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "protobuf_mutator_lib",
    srcs = [
        "src/mutator.cc",
        "src/text-format.cc",
        "src/utf8_fix.cc",
    ],
    hdrs = [
        "src/mutator.h",
        "src/random.h",
        "src/text-format.h",
        "src/utf8_fix.h",
        "src/weighted_reservoir_sampler.h",
        "src/field_instance.h",
        "port/gtest.h",
        "port/protobuf.h",
    ],
    deps = [
    ]
)