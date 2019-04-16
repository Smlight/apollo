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

#include <list>
#include <string>

#include "cyber/common/global_data.h"
#include "cyber/common/log.h"
#include "cyber/common/types.h"

namespace apollo {
namespace routing {

static const char DEFAULT_process_group_[] = "mainboard_default";
static const char DEFAULT_sched_name_[] = "CYBER_DEFAULT";

class ModuleArgument {
 public:
  ModuleArgument();
  virtual ~ModuleArgument();

  inline std::string GetBinaryName() const { return binary_name_; }
  inline std::string GetProcessGroup() const { return process_group_; }
  inline std::string GetSchedName() const { return sched_name_; }
  inline std::list<std::string> GetDAGConfList() const {
    return dag_conf_list_;
  }

  inline void SetBinaryName(std::string binary_name) {
    binary_name_ = binary_name;
  }
  inline void SetProcessGroup(std::string process_group) {
    process_group_ = process_group;
  }
  inline void SetSchedName(std::string sched_name) { sched_name_ = sched_name; }
  inline void SetDAGConfList(std::list<std::string> dag_conf_list) {
    dag_conf_list_ = dag_conf_list;
  }

 private:
  std::list<std::string> dag_conf_list_;
  std::string binary_name_;
  std::string process_group_;
  std::string sched_name_;
};

}  // namespace routing
}  // namespace apollo
