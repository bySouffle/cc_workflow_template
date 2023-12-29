//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_REGISTRY_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_REGISTRY_H_
#include "conf.h"

namespace conf {
class Consul {
 public:
  std::string address{};
  std::string scheme{};
  bool health_check{};
  std::string health_check_url{};
  int health_check_timeout{};
};

class Endpoint {
 public:
  std::string address{};
  std::string scheme{};
  std::string discovery{};
};

class Registry {
 public:
  Consul consul{};
  Endpoint endpoint{};
 protected:
  const inline static std::string configName = "registry.yaml";
};
}
#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_REGISTRY_H_
