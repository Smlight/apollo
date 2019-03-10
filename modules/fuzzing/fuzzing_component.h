/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#pragma once

#include <memory>

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
#include "modules/localization/proto/localization.pb.h"
#include "modules/perception/proto/perception_obstacle.pb.h"
#include "modules/perception/proto/traffic_light_detection.pb.h"
#include "modules/planning/proto/planning.pb.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"
#include "modules/routing/proto/routing.pb.h"

using apollo::canbus::Chassis;
using apollo::common::time::Clock;
using apollo::hdmap::HDMapUtil;
using apollo::localization::LocalizationEstimate;
using apollo::localization::LocalizationStatus;
using apollo::perception::PerceptionObstacles;
using apollo::perception::TrafficLightDetection;
using apollo::planning::ADCTrajectory;
using apollo::prediction::PredictionObstacles;
using apollo::relative_map::MapMsg;
using apollo::routing::RoutingRequest;
using apollo::routing::RoutingResponse;

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
  void CheckRerouting();
  bool CheckInput();

  std::shared_ptr<apollo::cyber::Node> fuzzer_node_;
  std::mutex mutex_;

  // Readers
  std::shared_ptr<cyber::Reader<Chassis>> chassis_reader_;
  std::shared_ptr<cyber::Reader<LocalizationEstimate>> localization_reader_;
  std::shared_ptr<cyber::Reader<LocalizationStatus>> localization_msf_reader_;
  std::shared_ptr<cyber::Reader<PerceptionObstacles>> perception_obstacles_reader_;
  std::shared_ptr<cyber::Reader<TrafficLightDetection>> traffic_light_reader_;
  std::shared_ptr<cyber::Reader<ADCTrajectory>> planning_trajectory_reader_;
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

};

CYBER_REGISTER_COMPONENT(FuzzingComponent)

}  // namespace fuzzing
}  // namespace apollo