/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#pragma once

#include "gflags/gflags.h"
#include "modules/common/adapters/adapter_gflags.h"
#include "modules/control/common/control_gflags.h"
#include "modules/common/configs/config_gflags.h"

#include "cyber/cyber.h"
#include "cyber/class_loader/class_loader.h"
#include "cyber/component/component.h"
#include "cyber/message/raw_message.h"

#include "modules/common/util/message_util.h"
#include "modules/map/hdmap/hdmap_util.h"
#include "modules/map/pnc_map/pnc_map.h"
#include "modules/planning/common/planning_context.h"

#include "modules/canbus/proto/chassis.pb.h"
#include "modules/control/proto/control_cmd.pb.h"
#include "modules/fuzzing/proto/fuzzing.pb.h"
#include "modules/localization/proto/gps.pb.h"
#include "modules/localization/proto/localization.pb.h"
#include "modules/perception/proto/perception_obstacle.pb.h"
#include "modules/perception/proto/traffic_light_detection.pb.h"
#include "modules/planning/proto/planning.pb.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"
#include "modules/routing/proto/routing.pb.h"

#include "modules/fuzzing/libprotobuf-mutator/src/mutator.h"

using apollo::canbus::Chassis;
using apollo::common::time::Clock;
using apollo::control::ControlCommand;
using apollo::hdmap::Curve;
using apollo::hdmap::Map;
using apollo::hdmap::Path;
using apollo::localization::Gps;
using apollo::localization::LocalizationEstimate;
using apollo::localization::LocalizationStatus;
using apollo::perception::PerceptionObstacles;
using apollo::perception::TrafficLightDetection;
using apollo::planning::ADCTrajectory;
using apollo::planning::DecisionResult;
using apollo::prediction::PredictionObstacles;
using apollo::relative_map::MapMsg;
using apollo::relative_map::NavigationInfo;
using apollo::routing::RoutingRequest;
using apollo::routing::RoutingResponse;
using protobuf_mutator::Mutator;
using protobuf_mutator::RandomEngine;

namespace apollo {
namespace fuzzing {

class FuzzingComponent final
    : public cyber::Component<localization::LocalizationEstimate> {
 public:
  FuzzingComponent() = default;
  ~FuzzingComponent() = default;
 public:
  bool Init() override;
  bool Proc(const std::shared_ptr<localization::LocalizationEstimate>&
                localization_estimate) override;

 private:
  void InitReaders();
  void InitWriters();
  void CheckRerouting();
  bool CheckInput();

  std::shared_ptr<apollo::cyber::Node> fuzzer_node_;
  std::mutex mutex_;

  // Readers
  std::shared_ptr<cyber::Reader<Chassis>> chassis_reader_;
  std::shared_ptr<cyber::Reader<ControlCommand>> control_command_reader_;
  std::shared_ptr<cyber::Reader<Gps>> gps_reader_;
  std::shared_ptr<cyber::Reader<LocalizationEstimate>> localization_reader_;
  std::shared_ptr<cyber::Reader<LocalizationStatus>> localization_msf_reader_;
  std::shared_ptr<cyber::Reader<NavigationInfo>> navigation_reader_;
  std::shared_ptr<cyber::Reader<PerceptionObstacles>> perception_obstacles_reader_;
  std::shared_ptr<cyber::Reader<TrafficLightDetection>> perception_traffic_light_reader_;
  std::shared_ptr<cyber::Reader<ADCTrajectory>> planning_reader_;
  std::shared_ptr<cyber::Reader<PredictionObstacles>> prediction_obstacles_reader_;
  std::shared_ptr<cyber::Reader<MapMsg>> relative_map_reader_;
  std::shared_ptr<cyber::Reader<RoutingRequest>> routing_request_reader_;
  std::shared_ptr<cyber::Reader<RoutingResponse>> routing_response_reader_;

  // Writers
  std::shared_ptr<cyber::Writer<Chassis>> chassis_writer_;
  std::shared_ptr<cyber::Writer<LocalizationEstimate>> localization_writer_;
  std::shared_ptr<cyber::Writer<LocalizationStatus>> localization_msf_writer_;
  std::shared_ptr<cyber::Writer<PerceptionObstacles>> perception_obstacles_writer_;
  std::shared_ptr<cyber::Writer<TrafficLightDetection>> traffic_light_writer_;
  std::shared_ptr<cyber::Writer<ADCTrajectory>> planning_trajectory_writer_;
  std::shared_ptr<cyber::Writer<PredictionObstacles>> prediction_obstacles_writer_;
  std::shared_ptr<cyber::Writer<MapMsg>> relative_map_writer_;
  std::shared_ptr<cyber::Writer<RoutingRequest>> routing_request_writer_;
  std::shared_ptr<cyber::Writer<RoutingResponse>> routing_response_writer_;

  google::protobuf::Message* message_p_;

};

CYBER_REGISTER_COMPONENT(FuzzingComponent)

}  // namespace fuzzing
}  // namespace apollo