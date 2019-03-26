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

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "cyber/class_loader/class_loader_manager.h"
#include "cyber/component/component.h"
#include "cyber/proto/dag_conf.pb.h"
#include "modules/planning/mainboard/module_argument.h"

namespace apollo {
namespace planning {

using apollo::cyber::proto::DagConfig;

class ModuleController {
 public:
  explicit ModuleController(const ModuleArgument& args);
  virtual ~ModuleController();

  bool Init();
  bool LoadAll();
  void Clear();

 private:
  bool LoadModule(const std::string& path);
  bool LoadModule(const DagConfig& dag_config);

  ModuleArgument args_;
  apollo::cyber::class_loader::ClassLoaderManager class_loader_manager_;
  std::vector<std::shared_ptr<apollo::cyber::ComponentBase>> component_list_;
};

}  // namespace planning
}  // namespace apollo
