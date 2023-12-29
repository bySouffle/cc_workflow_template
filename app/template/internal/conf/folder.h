//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_FOLDER_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_FOLDER_H_

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_FOLDER_H_

#include "conf.h"

namespace conf {
class SavePath {
 public:
  std::string image{};
};

class Folder {
 public:
  SavePath savePath;

 protected:
  const inline static std::string configName = "folder.yaml";

};
}