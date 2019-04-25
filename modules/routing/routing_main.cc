/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include <iostream>
#include <random>
#include <vector>

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
#include "modules/localization/proto/localization.pb.h"
#include "modules/perception/proto/perception_obstacle.pb.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"
#include "modules/routing/mainboard/module_argument.h"
#include "modules/routing/mainboard/module_controller.h"
#include "modules/routing/proto/routing.pb.h"

#include "modules/fuzzing/libprotobuf-mutator/src/mutator.h"
#include "modules/fuzzing/proto/routing_fuzzed.pb.h"

using apollo::canbus::Chassis;
using apollo::cyber::Rate;
using apollo::cyber::common::GetProtoFromASCIIFile;
using apollo::cyber::common::GetProtoFromBinaryFile;
using apollo::cyber::common::SetProtoToASCIIFile;
using apollo::localization::LocalizationEstimate;
using apollo::perception::PerceptionObstacles;
using apollo::prediction::PredictionObstacles;
using apollo::routing::ModuleArgument;
using apollo::routing::ModuleController;
using apollo::routing::RoutingRequest;
using apollo::routing::RoutingResponse;
using protobuf_mutator::Mutator;
using std::vector;

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

  vector<::google::protobuf::Message*> routing_vec;
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest001());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest002());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest003());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest004());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest005());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest006());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest007());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest008());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest009());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest010());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest011());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest012());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest013());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest014());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest015());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest016());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest017());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest018());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest019());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest020());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest021());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest022());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest023());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest024());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest025());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest026());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest027());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest028());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest029());
  routing_vec.push_back(new apollo::fuzzing::RoutingRequest030());

  auto fuzzer_node = apollo::cyber::CreateNode("fuzzer");
  auto routing_request_writer =
      fuzzer_node->CreateWriter<RoutingRequest>(FLAGS_routing_request_topic);

  srand(time(NULL));
  Mutator mutator(1);

  for (int i = 0; i < 30; i++) {
    GetProtoFromASCIIFile("modules/routing/routing.ascii", routing_vec[i]);
  }

  Rate rate(1.0);
  while (apollo::cyber::OK()) {
    int idx = rand() % 30;
    mutator.Mutate(routing_vec[idx], 4096);
    routing_request_writer->Write(static_cast<std::shared_ptr<RoutingRequest>>(
          reinterpret_cast<RoutingRequest*>(routing_vec[idx]));
    rate.Sleep();
  }

  apollo::cyber::WaitForShutdown();
  controller.Clear();
  return 0;
}
