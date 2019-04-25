load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "fuzzing_chassis_proto",
    deps = [
        ":fuzzing_chassis_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_chassis_proto_lib",
    srcs = [
        "chassis_fuzzed.proto",
    ],
    deps = [
        "//modules/common/proto:drive_state_proto_lib",
        "//modules/common/proto:geometry_proto_lib",
        "//modules/common/proto:header_proto_lib",
        "//modules/common/proto:vehicle_signal_proto_lib",
        "//modules/drivers/canbus/proto:canbus_proto_lib",
    ],
)

cc_proto_library(
    name = "fuzzing_localization_proto",
    deps = [
        ":fuzzing_localization_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_localization_proto_lib",
    srcs = [
        "localization_fuzzed.proto"
    ],
    deps = [
        "//modules/localization/proto:localization_status_proto_lib",
        "//modules/localization/proto:pose_proto_lib",
        "//modules/common/proto:geometry_proto_lib",
        "//modules/common/proto:header_proto_lib",
        "//modules/common/proto:pnc_point_proto_lib",
    ],
)


cc_proto_library(
    name = "fuzzing_perception_proto",
    deps = [
        ":fuzzing_perception_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_perception_proto_lib",
    srcs = [
        "perception_obstacle_fuzzed.proto",
        "traffic_light_detection_fuzzed.proto",
    ],
    deps = [
        "//modules/common/proto:error_code_proto_lib",
        "//modules/common/proto:geometry_proto_lib",
        "//modules/common/proto:header_proto_lib",
        "//modules/map/proto:map_proto_lib",
    ],
)

cc_proto_library(
    name = "fuzzing_planning_proto",
    deps = [
        ":fuzzing_planning_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_planning_proto_lib",
    srcs = [
        "planning_fuzzed.proto",
    ],
    deps = [
        "//modules/planning/proto:decision_proto_lib",
        "//modules/planning/proto:planning_internal_proto_lib",
        "//modules/canbus/proto:canbus_proto_lib",
        "//modules/common/proto:drive_state_proto_lib",
        "//modules/common/proto:error_code_proto_lib",
        "//modules/common/proto:geometry_proto_lib",
        "//modules/common/proto:header_proto_lib",
        "//modules/common/proto:pnc_point_proto_lib",
        "//modules/common/proto:vehicle_signal_proto_lib",
        "//modules/localization/proto:localization_proto_lib",
        "//modules/localization/proto:pose_proto_lib",
        "//modules/map/proto:map_proto_lib",
        "//modules/perception/proto:perception_proto_lib",
        "//modules/prediction/proto:prediction_proto_lib",
        "//modules/routing/proto:routing_proto_lib",
    ],
)

cc_proto_library(
    name = "fuzzing_pose_proto",
    deps = [
        ":fuzzing_pose_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_pose_proto_lib",
    srcs = [
        "pose_fuzzed.proto",
    ],
    deps = [
        "//modules/common/proto:geometry_proto_lib",
    ],
)

cc_proto_library(
    name = "fuzzing_prediction_proto",
    deps = [
        ":fuzzing_prediction_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_prediction_proto_lib",
    srcs = [
        "prediction_obstacle_fuzzed.proto",
    ],
    deps = [
        "//modules/common/proto:error_code_proto_lib",
        "//modules/common/proto:header_proto_lib",
        "//modules/common/proto:pnc_point_proto_lib",
        "//modules/perception/proto:perception_proto_lib",
        "//modules/prediction/proto:feature_proto_lib",
        "//modules/prediction/proto:scenario_proto_lib",
    ],
)

cc_proto_library(
    name = "fuzzing_routing_proto",
    deps = [
        ":fuzzing_routing_proto_lib",
    ],
)

proto_library(
    name = "fuzzing_routing_proto_lib",
    srcs = [
        "routing_fuzzed.proto",
    ],
    deps = [
        "//modules/common/proto:error_code_proto_lib",
        "//modules/common/proto:geometry_proto_lib",
        "//modules/common/proto:header_proto_lib",
        "//modules/map/proto:map_proto_lib",
    ],
)

