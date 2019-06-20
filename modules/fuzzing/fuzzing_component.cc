/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include <memory>
#include <string>

#include "modules/fuzzing/fuzzing_component.h"

#include "modules/fuzzing/proto/routing_fuzzed.pb.h"

using apollo::cyber::common::GetProtoFromASCIIFile;
using apollo::cyber::common::GetProtoFromBinaryFile;
using apollo::cyber::common::SetProtoToASCIIFile;
using apollo::cyber::common::SetProtoToBinaryFile;
using apollo::fuzzing::FuzzMessage;
using apollo::routing::RoutingRequest;
using protobuf_mutator::Mutator;
using protobuf_mutator::RandomEngine;
using std::string;
using std::vector;

namespace apollo {
namespace fuzzing {

void FuzzingComponent::InitReaders() {
  chassis_reader_ = fuzzer_node_->CreateReader<Chassis>(
      FLAGS_chassis_topic, [this](const std::shared_ptr<Chassis>& chassis) {
        // AINFO << "Received chassis data:\n" << chassis->DebugString();
      });
  control_command_reader_ =
      node_->CreateReader<ControlCommand>(FLAGS_control_command_topic);
  gps_reader_ = node_->CreateReader<Gps>(FLAGS_gps_topic);
  localization_reader_ = fuzzer_node_->CreateReader<LocalizationEstimate>(
      FLAGS_localization_topic,
      [this](const std::shared_ptr<LocalizationEstimate>& localization) {
        // AINFO << "Received localization data:\n" <<
        // localization->DebugString();
      });
  localization_msf_reader_ = fuzzer_node_->CreateReader<LocalizationStatus>(
      FLAGS_localization_msf_status,
      [this](const std::shared_ptr<LocalizationStatus>& localization_msf) {
        // AINFO << "Received localization_msf data:\n"
        //       << localization_msf->DebugString();
      });
  navigation_reader_ =
      node_->CreateReader<NavigationInfo>(FLAGS_navigation_topic);
  perception_obstacles_reader_ =
      fuzzer_node_->CreateReader<PerceptionObstacles>(
          FLAGS_perception_obstacle_topic,
          [this](const std::shared_ptr<PerceptionObstacles>&
                     perception_obstacles) {
            // AINFO << "Received perception_obstacles data:\n"
            //       << perception_obstacles->DebugString();
          });
  perception_traffic_light_reader_ = node_->CreateReader<TrafficLightDetection>(
      FLAGS_traffic_light_detection_topic);
  planning_reader_ = fuzzer_node_->CreateReader<ADCTrajectory>(
      FLAGS_planning_trajectory_topic,
      [this](const std::shared_ptr<ADCTrajectory>& planning_trajectory) {
        // AINFO << "Received planning_trajectory data:\n"
        //       << planning_trajectory->DebugString();
      });
  prediction_obstacles_reader_ =
      fuzzer_node_->CreateReader<PredictionObstacles>(
          FLAGS_prediction_topic,
          [this](const std::shared_ptr<PredictionObstacles>&
                     prediction_obstacles) {
            // AINFO << "Received prediction_obstacles data:\n"
            //       << prediction_obstacles->DebugString();
          });
  relative_map_reader_ = fuzzer_node_->CreateReader<MapMsg>(
      FLAGS_relative_map_topic,
      [this](const std::shared_ptr<MapMsg>& relative_map) {
        // AINFO << "Received relative_map data:\n" <<
        // relative_map->DebugString();
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
}

void FuzzingComponent::InitWriters() {
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
}

bool FuzzingComponent::Init() {
  fuzzer_node_ = apollo::cyber::CreateNode("fuzzer");

  InitReaders();
  InitWriters();

  message_p_ = new apollo::routing::RoutingRequest013();
  GetProtoFromASCIIFile("modules/fuzzing/proto/routing013.ascii", message_p_);
  AINFO << "Initial data:\n" << message_p_->DebugString();

  srand(time(NULL));
  mutator_p_ = new Mutator(new RandomEngine(rand()));

  return true;
}

bool FuzzingComponent::Proc(
    const std::shared_ptr<localization::LocalizationEstimate>&
        localization_estimate) {
  //   Mutator mutator(new RandomEngine(1));
  mutator_p_->Mutate(message_p_, 4096);
  //   AINFO << "message_p_ data:\n" << message_p_->DebugString();
  auto now = message_p_->New();

  SetProtoToASCIIFile(*message_p_,
                      "/apollo/modules/fuzzing/proto/routing013_tmp.ascii");
  GetProtoFromASCIIFile("/apollo/modules/fuzzing/proto/routing013_tmp.ascii",
                        now);

  routing_request_writer_->Write(
      std::shared_ptr<RoutingRequest>(reinterpret_cast<RoutingRequest*>(now)));
  return true;
}

}  // namespace fuzzing
}  // namespace apollo
