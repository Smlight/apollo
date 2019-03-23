/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include "gflags/gflags.h"

#include "modules/common/adapters/adapter_gflags.h"

#include "cyber/common/global_data.h"
#include "cyber/common/log.h"
#include "cyber/cyber.h"
#include "cyber/init.h"
#include "cyber/state.h"

#include "modules/planning/mainboard/module_argument.h"
#include "modules/planning/mainboard/module_controller.h"
#include "modules/routing/proto/routing.pb.h"

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
  apollo::cyber::Init("cyber");

  // start module
  ModuleController controller(module_args);
  if (!controller.Init()) {
    controller.Clear();
    AERROR << "module start error.";
    return -1;
  }

  auto component_list = controller.GetComponentList();
  int cnt = 0;
  for (auto sp : component_list) {
    AINFO << "cnt: " << ++cnt << "\n";
  }

  auto fuzzer_node = apollo::cyber::CreateNode("fuzzer");
  auto routing_response_writer =
      fuzzer_node->CreateWriter<RoutingResponse>(FLAGS_routing_response_topic);
  // Measurement measurement;
  // measurement.set_distance(100.0);
  RoutingResponse routing_response;
  // routing_response.set_measurement(measurement);
  routing_response.set_map_version("3");
  routing_response_writer->Write(routing_response);

  apollo::cyber::WaitForShutdown();
  controller.Clear();

  return 0;
}
