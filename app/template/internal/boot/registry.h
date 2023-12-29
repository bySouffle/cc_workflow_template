//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_REGISTRY_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_REGISTRY_H_

#include "singleton.h"
#include "template/internal/conf/registry.h"
#include "config.h"

namespace boot {
class RegistryLoad : public Registry, public Singleton<RegistryLoad> {
 public:
  void Load(const ConfigFiles &files) {
    for (auto &f : files) {
      std::string filename = f.path().filename().string();
      if (filename == configName) {
        parse(f.path());
        break;
      }
    }
  }

 private:
  void parse(const std::string &filePath) {
    try {
      YAML::Node config = YAML::LoadFile(filePath);
      consul.address = config["consul"]["address"].as<std::string>();
      consul.scheme = config["consul"]["scheme"].as<std::string>();
      consul.health_check = config["consul"]["health_check"].as<bool>();
      consul.health_check_url = config["consul"]["health_check_url"].as<std::string>();
      consul.health_check_timeout = config["consul"]["health_check_timeout"].as<int>();
      // 解析 Endpoint 字段
      endpoint.address = config["endpoint"]["address"].as<std::string>();
      endpoint.scheme = config["endpoint"]["scheme"].as<std::string>();
      endpoint.discovery = config["endpoint"]["discovery"].as<std::string>();

    } catch (const std::exception &e) {
      std::cerr << "Error parsing YAML file " << filePath << ": " << e.what() << std::endl;
      exit(-1);
    }
  };
};
}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_REGISTRY_H_
