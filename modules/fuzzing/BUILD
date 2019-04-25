load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "protobuf_mutator_lib",
    srcs = [
        "libprotobuf-mutator/mutator.cc",
    ],
    hdrs = [
        "libprotobuf-mutator/mutator.h",
    ],
    deps = [
    ]
)

cc_library(
    name = "fuzzing_component_lib",
    srcs = [
        "fuzzing_component.cc",
    ],
    hdrs = [
        "fuzzing_component.h",
    ],
    copts = [
        "-DMODULE_NAME=\\\"fuzzing\\\"",
    ],
    deps = [
        ":fuzzing_lib",
        "//modules/canbus/proto:canbus_proto",
        "//modules/common/proto:pnc_point_proto",
        "//modules/control/proto:control_proto",
        "//modules/localization/proto:gps_proto",
        "//modules/localization/proto:localization_proto",
        "//modules/map/relative_map/proto:navigation_proto",
        "//modules/perception/proto:perception_proto",
        "//modules/planning/proto:planning_config_proto",
        "//modules/planning/proto:planning_proto",
        "//modules/planning/proto:traffic_rule_config_proto",
        "//modules/prediction/proto:prediction_proto",
    ],
)

cc_binary(
    name = "libfuzzing_component.so",
    linkshared = True,
    linkstatic = False,
    deps = [
        ":fuzzing_component_lib"
    ],
)

cc_binary(
    name = "fuzzing_stage1",
    srcs = [
        "fuzzing_stage1.cc",
    ],
    deps = [
    ],
)

cc_library(
    name = "fuzzing_lib",
    srcs = [
    ],
    hdrs = [
    ],
    copts = ["-DMODULE_NAME=\\\"fuzzing\\\""],
    deps = [
        "//cyber",
        "//external:gflags",
        "//modules/common",
        "//modules/common/util:message_util",
        "//modules/common/adapters:adapter_gflags",
        "//modules/common/configs:config_gflags",
        "//modules/common/math:quaternion",
        "//modules/common/vehicle_state:vehicle_state_provider",
        "//modules/control/common",
        "//modules/control/controller",
        "//modules/map/hdmap:hdmap_util",
        "//modules/planning/common:planning_common",
        "//modules/planning/common:planning_gflags",
        "//modules/planning/common/trajectory:trajectory_stitcher",
        "//modules/planning/planner",
        "//modules/planning/planner:planner_dispatcher",
        "//modules/planning/tasks:task_factory",
        "//modules/planning/tasks/smoothers:smoother",
        "//modules/planning/traffic_rules:traffic_decider",
    ],
)

filegroup(
    name = "fuzzing_testdata",
    srcs = glob([
        "testdata/**",
    ]),
)

filegroup(
    name = "fuzzing_conf",
    srcs = glob([
        "conf/**",
    ]),
)

cpplint()
