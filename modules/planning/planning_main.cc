/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include <iostream>

#include "gflags/gflags.h"

#include "cyber/common/file.h"
#include "cyber/common/global_data.h"
#include "cyber/common/log.h"
#include "cyber/cyber.h"
#include "cyber/init.h"
#include "cyber/state.h"
#include "cyber/time/rate.h"
#include "cyber/time/time.h"

#include "modules/canbus/proto/chassis.pb.h"
#include "modules/common/adapters/adapter_gflags.h"
#include "modules/fuzzing/proto/fuzzing.pb.h"
#include "modules/localization/proto/localization.pb.h"
#include "modules/planning/mainboard/module_argument.h"
#include "modules/planning/mainboard/module_controller.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"
#include "modules/routing/proto/routing.pb.h"

using apollo::canbus::Chassis;
using apollo::cyber::Rate;
using apollo::cyber::common::GetProtoFromASCIIFile;
using apollo::localization::LocalizationEstimate;
using apollo::planning::ModuleArgument;
using apollo::planning::ModuleController;
using apollo::prediction::PredictionObstacles;
using apollo::routing::RoutingResponse;

int main(int argc, char** argv) {
  // parse the argument
  ModuleArgument module_args;
  module_args.SetBinaryName("planning_main");
  module_args.SetDAGConfList({"/apollo/modules/planning/dag/planning.dag"});

  // initialize cyber
  apollo::cyber::Init("planning");

  // start module
  ModuleController controller(module_args);
  if (!controller.Init()) {
    controller.Clear();
    AERROR << "module start error.";
    return -1;
  }

  auto fuzzer_node = apollo::cyber::CreateNode("fuzzer");
  auto routing_response_writer =
      fuzzer_node->CreateWriter<RoutingResponse>(FLAGS_routing_response_topic);
  auto prediction_obstacles_writer =
      fuzzer_node->CreateWriter<PredictionObstacles>(FLAGS_prediction_topic);
  auto chassis_writer = fuzzer_node->CreateWriter<Chassis>(FLAGS_chassis_topic);
  auto localization_writer =
      fuzzer_node->CreateWriter<LocalizationEstimate>(FLAGS_localization_topic);

  RoutingResponse routing_response;
  Chassis chassis;
  PredictionObstacles prediction_obstacles;
  LocalizationEstimate localization_estimate;

  Rate rate(1.0);
  while (apollo::cyber::OK()) {
    GetProtoFromASCIIFile("./routing.ascii", &routing_response);
    GetProtoFromASCIIFile("./chassis.ascii", &chassis);
    GetProtoFromASCIIFile("./prediction.ascii", &prediction_obstacles);
    GetProtoFromASCIIFile("./localization.ascii", &localization_estimate);
    routing_response_writer->Write(routing_response);
    chassis_writer->Write(chassis);
    prediction_obstacles_writer->Write(prediction_obstacles);
    localization_writer->Write(localization_estimate);
    rate.Sleep();
  }

  apollo::cyber::WaitForShutdown();
  controller.Clear();

  return 0;
}
