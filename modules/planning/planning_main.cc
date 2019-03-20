/******************************************************************************
 * Created by Smlight
 *****************************************************************************/

#include "cyber/common/global_data.h"
#include "cyber/common/log.h"
#include "cyber/init.h"
#include "cyber/mainboard/module_argument.h"
#include "cyber/mainboard/module_controller.h"
#include "cyber/state.h"

#include "gflags/gflags.h"

using apollo::cyber::mainboard::ModuleArgument;
using apollo::cyber::mainboard::ModuleController;

int main(int argc, char** argv) {

  // parse the argument
  ModuleArgument module_args;
  module_args.ParseArgument(argc, argv);

  // initialize cyber
  apollo::cyber::Init(argv[0]);

  // start module
  ModuleController controller(module_args);
  if (!controller.Init()) {
    controller.Clear();
    AERROR << "module start error.";
    return -1;
  }

  apollo::cyber::WaitForShutdown();
  controller.Clear();

  return 0;
}
