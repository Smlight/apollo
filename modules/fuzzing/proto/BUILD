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
        "//modules/common/proto:drive_state_proto_lib",
        "//modules/common/proto:geometry_proto_lib",
        "//modules/common/proto:header_proto_lib",
        "//modules/common/proto:vehicle_signal_proto_lib",
        "//modules/drivers/canbus/proto:canbus_proto_lib",
    ],
)

