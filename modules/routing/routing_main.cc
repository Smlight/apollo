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
#include "modules/routing/proto/routing.pb.h"

#include "modules/routing/mainboard/module_argument.h"
#include "modules/routing/mainboard/module_controller.h"

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
using protobuf_mutator::RandomEngine;
using std::string;
using std::vector;

google::protobuf::Message* get_class_(int n)
{
  if (n==1) return new apollo::routing::RoutingRequest001();
  else if (n==2) return new apollo::routing::RoutingRequest002();
  else if (n==3) return new apollo::routing::RoutingRequest003();
  else if (n==4) return new apollo::routing::RoutingRequest004();
  else if (n==5) return new apollo::routing::RoutingRequest005();
  else if (n==6) return new apollo::routing::RoutingRequest006();
  else if (n==7) return new apollo::routing::RoutingRequest007();
  else if (n==8) return new apollo::routing::RoutingRequest008();
  else if (n==9) return new apollo::routing::RoutingRequest009();
  else if (n==10) return new apollo::routing::RoutingRequest010();
  else if (n==11) return new apollo::routing::RoutingRequest011();
  else if (n==12) return new apollo::routing::RoutingRequest012();
  else if (n==13) return new apollo::routing::RoutingRequest013();
  else if (n==14) return new apollo::routing::RoutingRequest014();
  else if (n==15) return new apollo::routing::RoutingRequest015();
  else if (n==16) return new apollo::routing::RoutingRequest016();
  else if (n==17) return new apollo::routing::RoutingRequest017();
  else if (n==18) return new apollo::routing::RoutingRequest018();
  else if (n==19) return new apollo::routing::RoutingRequest019();
  else if (n==20) return new apollo::routing::RoutingRequest020();
  else if (n==21) return new apollo::routing::RoutingRequest021();
  else if (n==22) return new apollo::routing::RoutingRequest022();
  else if (n==23) return new apollo::routing::RoutingRequest023();
  else if (n==24) return new apollo::routing::RoutingRequest024();
  else if (n==25) return new apollo::routing::RoutingRequest025();
  else if (n==26) return new apollo::routing::RoutingRequest026();
  else if (n==27) return new apollo::routing::RoutingRequest027();
  else if (n==28) return new apollo::routing::RoutingRequest028();
  else if (n==29) return new apollo::routing::RoutingRequest029();
  else return new apollo::routing::RoutingRequest030();
}

int main(int argc, char** argv) {
  
  ModuleArgument module_args;
  module_args.SetBinaryName("routing_main");
  module_args.SetDAGConfList({"/apollo/modules/routing/dag/routing.dag"});

  // initialize cyber
  apollo::cyber::Init("routing_main");

  // start module
  ModuleController controller(module_args);
  if (!controller.Init()) {
    controller.Clear();
    AERROR << "module start error.";
    return -1;
  }

  auto fuzzer_node = apollo::cyber::CreateNode("fuzzer");
  auto routing_request_writer = fuzzer_node->CreateWriter<RoutingRequest>(FLAGS_routing_request_topic);

  srand(time(NULL));
  Mutator mutator(new RandomEngine(rand()));

  vector<google::protobuf::Message*> routing_vec;
  char buf[16];
  for (int i = 0; i < 30; i++) {
    sprintf(buf, "%03d", i + 1);
    string i_leading0(buf);
    routing_vec.push_back(get_class_(i + 1));
    GetProtoFromASCIIFile("/apollo/modules/fuzzing/proto/routing" + i_leading0 + ".ascii", routing_vec[i]);
  }

  Rate rate(1.0);
  while (apollo::cyber::OK()) {
    int idx = rand() % 30;
    mutator.Mutate(routing_vec[idx], 4096);
    auto now = routing_vec[idx]->New();
    routing_request_writer->Write(std::shared_ptr<RoutingRequest>(reinterpret_cast<RoutingRequest*>(now)));
    rate.Sleep();
  }

  apollo::cyber::WaitForShutdown();
  controller.Clear();
  return 0;
}
