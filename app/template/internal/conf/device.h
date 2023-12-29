//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_DEVICE_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_DEVICE_H_

#include "conf.h"

namespace conf {
class Ptz {
 public:
  std::string IP{};
  int32 Port{};
  std::string username{};
  std::string passwd{};

};

class Device {
 public:
  Ptz ptz;

 protected:
  const inline static std::string configName = "device.yaml";

};
}
#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_DEVICE_H_
