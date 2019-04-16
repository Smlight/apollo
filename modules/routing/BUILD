load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "routing_main",
    srcs = [
        "routing_main.cc",
        "mainboard/module_argument.h",
        "mainboard/module_argument.cc",
        "mainboard/module_controller.h",
        "mainboard/module_controller.cc",
    ],
    linkstatic = False,
    deps = [
        ":routing_lib",
        "//cyber",
        "//cyber:cyber_core",
        "//cyber/proto:dag_conf_cc_proto",
        "//modules/common/adapters:adapter_gflags",
        "//modules/common/util:message_util",
        "//modules/localization/proto:localization_proto",
        "//modules/map/relative_map/proto:navigation_proto",
        "//modules/perception/proto:perception_proto",
        "//modules/canbus/proto:canbus_proto",
        "//modules/routing/proto:routing_proto",
        "//modules/prediction/proto:prediction_proto",
        "//modules/fuzzing/proto:fuzzing_proto",
    ],
)

cc_library(
    name = "routing_lib",
    srcs = [
        "routing.cc",
    ],
    hdrs = [
        "routing.h",
    ],
    deps = [
        "//modules/common/adapters:adapter_gflags",
        "//modules/common/monitor_log",
        "//modules/map/hdmap:hdmap_util",
        "//modules/routing/core",
    ],
)

cc_library(
    name = "routing_component_lib",
    srcs = [
        "routing_component.cc",
    ],
    hdrs = [
        "routing_component.h",
    ],
    copts = ['-DMODULE_NAME=\\"routing\\"'],
    deps = [
        ":routing_lib",
    ],
)

cc_binary(
    name = "librouting_component.so",
    linkshared = True,
    linkstatic = False,
    deps = [":routing_component_lib"],
)

filegroup(
    name = "routing_testdata",
    srcs = glob([
        "testdata/**",
    ]),
)

cpplint()
