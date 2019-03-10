/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include "modules/fuzzing/fuzzing_component.h"

namespace apollo {
namespace fuzzing {

bool FuzzingComponent::Init() {
  fuzzer_node_ = apollo::cyber::CreateNode("fuzzer");

  chassis_reader_ = fuzzer_node_->CreateReader<Chassis>(
      FLAGS_chassis_topic, [this](const std::shared_ptr<Chassis>& chassis) {
        AINFO << "Received chassis data:\n" << chassis->DebugString();
      });
  localization_reader_ = fuzzer_node_->CreateReader<LocalizationEstimate>(
      FLAGS_localization_topic,
      [this](const std::shared_ptr<LocalizationEstimate>& localization) {
        AINFO << "Received localization data:\n" << localization->DebugString();
      });
  localization_msf_reader_ = fuzzer_node_->CreateReader<LocalizationStatus>(
      FLAGS_localization_msf_status,
      [this](const std::shared_ptr<LocalizationStatus>& localization_msf) {
        AINFO << "Received localization_msf data:\n"
              << localization_msf->DebugString();
      });
  perception_obstacles_reader_ =
      fuzzer_node_->CreateReader<PerceptionObstacles>(
          FLAGS_perception_obstacle_topic,
          [this](const std::shared_ptr<PerceptionObstacles>&
                     perception_obstacles) {
            AINFO << "Received perception_obstacles data:\n"
                  << perception_obstacles->DebugString();
          });
  traffic_light_reader_ = fuzzer_node_->CreateReader<TrafficLightDetection>(
      FLAGS_traffic_light_detection_topic,
      [this](const std::shared_ptr<TrafficLightDetection>& traffic_light) {
        AINFO << "Received traffic_light data:\n"
              << traffic_light->DebugString();
      });
  planning_trajectory_reader_ = fuzzer_node_->CreateReader<ADCTrajectory>(
      FLAGS_planning_trajectory_topic,
      [this](const std::shared_ptr<ADCTrajectory>& planning_trajectory) {
        AINFO << "Received planning_trajectory data:\n"
              << planning_trajectory->DebugString();
      });
  prediction_obstacles_reader_ =
      fuzzer_node_->CreateReader<PredictionObstacles>(
          FLAGS_prediction_topic,
          [this](const std::shared_ptr<PredictionObstacles>&
                     prediction_obstacles) {
            AINFO << "Received prediction_obstacles data:\n"
                  << prediction_obstacles->DebugString();
          });
  relative_map_reader_ = fuzzer_node_->CreateReader<MapMsg>(
      FLAGS_relative_map_topic,
      [this](const std::shared_ptr<MapMsg>& relative_map) {
        AINFO << "Received relative_map data:\n" << relative_map->DebugString();
      });
  routing_request_reader_ = fuzzer_node_->CreateReader<RoutingRequest>(
      FLAGS_routing_request_topic,
      [this](const std::shared_ptr<RoutingRequest>& routing_request) {
        AINFO << "Received routing_request data:\n"
              << routing_request->DebugString();
      });
  routing_response_reader_ = fuzzer_node_->CreateReader<RoutingResponse>(
      FLAGS_routing_response_topic,
      [this](const std::shared_ptr<RoutingResponse>& routing_response) {
        AINFO << "Received routing_response data:\n"
              << routing_response->DebugString();
      });

  chassis_writer_ = fuzzer_node_->CreateWriter<Chassis>(FLAGS_chassis_topic);
  localization_writer_ = fuzzer_node_->CreateWriter<LocalizationEstimate>(
      FLAGS_localization_topic);
  localization_msf_writer_ = fuzzer_node_->CreateWriter<LocalizationStatus>(
      FLAGS_localization_msf_status);
  perception_obstacles_writer_ =
      fuzzer_node_->CreateWriter<PerceptionObstacles>(
          FLAGS_perception_obstacle_topic);
  traffic_light_writer_ = fuzzer_node_->CreateWriter<TrafficLightDetection>(
      FLAGS_traffic_light_detection_topic);
  planning_trajectory_writer_ = fuzzer_node_->CreateWriter<ADCTrajectory>(
      FLAGS_planning_trajectory_topic);
  prediction_obstacles_writer_ =
      fuzzer_node_->CreateWriter<PredictionObstacles>(FLAGS_prediction_topic);
  relative_map_writer_ =
      fuzzer_node_->CreateWriter<MapMsg>(FLAGS_relative_map_topic);
  routing_request_writer_ =
      fuzzer_node_->CreateWriter<RoutingRequest>(FLAGS_routing_request_topic);
  routing_response_writer_ =
      fuzzer_node_->CreateWriter<RoutingResponse>(FLAGS_routing_response_topic);

  return true;
}

bool FuzzingComponent::Proc(
    const std::shared_ptr<localization::LocalizationEstimate>&
        localization_estimate) {
  static uint32_t seq = 0;
  ++seq;
  auto chassis = std::make_shared<Chassis>();
  auto* header = chassis->mutable_header();
  header->set_timestamp_sec(Clock::NowInSeconds());
  header->set_module_name("hello, from the FUZZING modules!");
  header->set_sequence_num(seq);
  chassis->set_driving_mode(apollo::canbus::Chassis_DrivingMode_COMPLETE_AUTO_DRIVE);
  chassis_writer_->Write(chassis);
  return true;
}

}  // namespace fuzzing
}  // namespace apollo
