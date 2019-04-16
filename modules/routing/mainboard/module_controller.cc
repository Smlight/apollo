/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/routing/mainboard/module_controller.h"

#include <utility>

#include "cyber/common/environment.h"
#include "cyber/common/file.h"
#include "cyber/component/component_base.h"

namespace apollo {
namespace routing {

ModuleController::ModuleController(const ModuleArgument& args) { args_ = args; }

ModuleController::~ModuleController() {}

bool ModuleController::Init() { return LoadAll(); }

void ModuleController::Clear() {
  for (auto& component : component_list_) {
    component->Shutdown();
  }
  component_list_.clear();  // keep alive
  class_loader_manager_.UnloadAllLibrary();
}

bool ModuleController::LoadAll() {
  const std::string work_root = apollo::cyber::common::WorkRoot();
  const std::string current_path = apollo::cyber::common::GetCurrentPath();
  const std::string dag_root_path =
      apollo::cyber::common::GetAbsolutePath(work_root, "dag");

  for (auto& dag_conf : args_.GetDAGConfList()) {
    std::string module_path = "";
    if (dag_conf == apollo::cyber::common::GetFileName(dag_conf)) {
      // case dag conf argument var is a filename
      module_path =
          apollo::cyber::common::GetAbsolutePath(dag_root_path, dag_conf);
    } else if (dag_conf[0] == '/') {
      // case dag conf argument var is an absolute path
      module_path = dag_conf;
    } else {
      // case dag conf argument var is a relative path
      module_path =
          apollo::cyber::common::GetAbsolutePath(current_path, dag_conf);
      if (!apollo::cyber::common::PathExists(module_path)) {
        module_path =
            apollo::cyber::common::GetAbsolutePath(work_root, dag_conf);
      }
    }
    AINFO << "Start initialize dag: " << module_path;
    if (!LoadModule(module_path)) {
      AERROR << "Failed to load module: " << module_path;
      return false;
    }
  }
  return true;
}

bool ModuleController::LoadModule(const DagConfig& dag_config) {
  const std::string work_root = apollo::cyber::common::WorkRoot();

  for (auto module_config : dag_config.module_config()) {
    std::string load_path;
    if (module_config.module_library().front() == '/') {
      load_path = module_config.module_library();
    } else {
      load_path = apollo::cyber::common::GetAbsolutePath(
          work_root, module_config.module_library());
    }

    if (!apollo::cyber::common::PathExists(load_path)) {
      AERROR << "Path not exist: " << load_path;
      return false;
    }

    class_loader_manager_.LoadLibrary(load_path);

    for (auto& component : module_config.components()) {
      const std::string& class_name = component.class_name();
      std::shared_ptr<apollo::cyber::ComponentBase> base =
          class_loader_manager_.CreateClassObj<apollo::cyber::ComponentBase>(
              class_name);
      if (base == nullptr) {
        return false;
      }

      if (!base->Initialize(component.config())) {
        return false;
      }
      component_list_.emplace_back(std::move(base));
    }

    for (auto& component : module_config.timer_components()) {
      const std::string& class_name = component.class_name();
      std::shared_ptr<apollo::cyber::ComponentBase> base =
          class_loader_manager_.CreateClassObj<apollo::cyber::ComponentBase>(
              class_name);
      if (base == nullptr) {
        return false;
      }

      if (!base->Initialize(component.config())) {
        return false;
      }
      component_list_.emplace_back(std::move(base));
    }
  }
  return true;
}

bool ModuleController::LoadModule(const std::string& path) {
  DagConfig dag_config;
  if (!apollo::cyber::common::GetProtoFromFile(path, &dag_config)) {
    AERROR << "Get proto failed, file: " << path;
    return false;
  }
  return LoadModule(dag_config);
}

}  // namespace routing
}  // namespace apollo
