/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#pragma once

#include <memory>

#include "cyber/class_loader/class_loader.h"
#include "cyber/component/component.h"
#include "cyber/message/raw_message.h"

#include "modules/canbus/proto/chassis.pb.h"
#include "modules/localization/proto/localization.pb.h"
#include "modules/perception/proto/traffic_light_detection.pb.h"
#include "modules/planning/common/planning_gflags.h"
#include "modules/planning/proto/pad_msg.pb.h"
#include "modules/planning/proto/planning.pb.h"
#include "modules/planning/proto/planning_config.pb.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"
#include "modules/routing/proto/routing.pb.h"

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

  // Readers
  std::shared_ptr<cyber::Reader<perception::TrafficLightDetection>> traffic_light_reader_;
  std::shared_ptr<cyber::Reader<localization::LocalizationEstimate>> localization_reader_;
  std::shared_ptr<cyber::Reader<routing::RoutingRequest>> routing_request_reader_;
  std::shared_ptr<cyber::Reader<routing::RoutingResponse>> routing_response_reader_;
  std::shared_ptr<cyber::Reader<planning::PadMessage>> pad_message_reader_;
  std::shared_ptr<cyber::Reader<relative_map::MapMsg>> relative_map_reader_;

  // Writers
  std::shared_ptr<cyber::Writer<planning::ADCTrajectory>> planning_writer_;
  std::shared_ptr<cyber::Writer<routing::RoutingRequest>> rerouting_writer_;

  std::mutex mutex_;
  perception::TrafficLightDetection traffic_light_;
  routing::RoutingResponse routing_response_;
  planning::PadMessage pad_message_;
  relative_map::MapMsg relative_map_;

//   LocalView local_view_;

//   std::unique_ptr<FuzzingBase> planning_base_;

//   FuzzingConfig config_;
};

CYBER_REGISTER_COMPONENT(FuzzingComponent)

}  // namespace fuzzing
}  // namespace apollo