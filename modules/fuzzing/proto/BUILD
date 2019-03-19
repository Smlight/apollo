load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "fuzzing_proto",
    deps = [
        ":fuzzing_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_proto_lib",
    srcs = [
        "fuzzing.proto",
    ],
    deps = [
    ],
)
