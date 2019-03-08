/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include "modules/fuzzing/fuzzing_component.h"

#include "modules/common/adapters/adapter_gflags.h"
#include "modules/control/common/control_gflags.h"
#include "modules/common/configs/config_gflags.h"
#include "modules/common/util/message_util.h"
#include "modules/map/hdmap/hdmap_util.h"
#include "modules/map/pnc_map/pnc_map.h"
#include "modules/planning/common/planning_context.h"

namespace apollo {
namespace fuzzing {

using apollo::canbus::Chassis;
using apollo::common::time::Clock;
using apollo::hdmap::HDMapUtil;
using apollo::perception::TrafficLightDetection;
using apollo::relative_map::MapMsg;
using apollo::routing::RoutingRequest;
using apollo::routing::RoutingResponse;
using apollo::planning::PadMessage;
using apollo::localization::LocalizationEstimate;

bool FuzzingComponent::Init() {

  cyber::ReaderConfig localization_reader_config;
  localization_reader_config.channel_name = FLAGS_localization_topic;
  localization_reader_config.pending_queue_size =
      FLAGS_localization_pending_queue_size;
  localization_reader_ = node_->CreateReader<LocalizationEstimate>(
      localization_reader_config,
      [this](const std::shared_ptr<LocalizationEstimate>& localization) {
        AINFO << "Received localization data: "
              << localization->DebugString();
      });
  // CHECK(localization_reader_ != nullptr);

  routing_response_reader_ = node_->CreateReader<RoutingResponse>(
      "/apollo/routing_response",
      [this](const std::shared_ptr<RoutingResponse>& routing) {
        AINFO << "Received routing data: "
              << routing->DebugString();
        std::lock_guard<std::mutex> lock(mutex_);
        routing_response_.CopyFrom(*routing);
      });
  traffic_light_reader_ = node_->CreateReader<TrafficLightDetection>(
      FLAGS_traffic_light_detection_topic,
      [this](const std::shared_ptr<TrafficLightDetection>& traffic_light) {
        ADEBUG << "Received traffic light data: "
               << traffic_light->DebugString();
        std::lock_guard<std::mutex> lock(mutex_);
        traffic_light_.CopyFrom(*traffic_light);
      });


  return true;
}

bool FuzzingComponent::Proc(const std::shared_ptr<localization::LocalizationEstimate>&
                localization_estimate) {
  return true;
}

}  // namespace fuzzing
}  // namespace apollo
