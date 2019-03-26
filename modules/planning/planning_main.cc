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

#include "modules/common/adapters/adapter_gflags.h"
#include "modules/fuzzing/proto/fuzzing.pb.h"
#include "modules/planning/mainboard/module_argument.h"
#include "modules/planning/mainboard/module_controller.h"
#include "modules/routing/proto/routing.pb.h"

using apollo::cyber::common::GetProtoFromASCIIFile;
using apollo::planning::ModuleArgument;
using apollo::planning::ModuleController;
using apollo::routing::Measurement;
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
  RoutingResponse routing_response;
  GetProtoFromASCIIFile("./routing.ascii", &routing_response);
  routing_response_writer->Write(routing_response);

  apollo::cyber::WaitForShutdown();
  controller.Clear();

  return 0;
}
