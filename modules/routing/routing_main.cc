/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include <iostream>
#include <random>

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
#include "modules/perception/proto/perception_obstacle.pb.h"
#include "modules/planning/mainboard/module_argument.h"
#include "modules/planning/mainboard/module_controller.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"
#include "modules/routing/proto/routing.pb.h"

using apollo::canbus::Chassis;
using apollo::cyber::Rate;
using apollo::cyber::common::GetProtoFromASCIIFile;
using apollo::cyber::common::GetProtoFromBinaryFile;
using apollo::cyber::common::SetProtoToASCIIFile;
using apollo::localization::LocalizationEstimate;
using apollo::perception::PerceptionObstacles;
using apollo::planning::ModuleArgument;
using apollo::planning::ModuleController;
using apollo::prediction::PredictionObstacles;
using apollo::routing::RoutingResponse;

int main(int argc, char** argv) {
  // parse the argument
  ModuleArgument module_args;
  module_args.SetBinaryName("routing_main");
  module_args.SetDAGConfList({"/apollo/modules/routing/dag/routing.dag"});

  // initialize cyber
  apollo::cyber::Init("routing");

  // start module
  ModuleController controller(module_args);
  if (!controller.Init()) {
    controller.Clear();
    AERROR << "module start error.";
    return -1;
  }

  auto fuzzer_node = apollo::cyber::CreateNode("fuzzer");
  auto routing_request_writer =
      fuzzer_node->CreateWriter<RoutingRequest>(FLAGS_routing_request_topic);
  RoutingRequest routing_request;
  GetProtoFromASCIIFile("modules/routing/routing.ascii", &routing_request);
  routing_request_writer->Write(routing_request);

  apollo::cyber::WaitForShutdown();
  controller.Clear();
  return 0;
}
