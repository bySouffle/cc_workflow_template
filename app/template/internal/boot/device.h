//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_DEVICE_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_DEVICE_H_

#include "singleton.h"
#include "template/internal/conf/device.h"
#include "config.h"

namespace boot {

class DeviceLoad : public Device, public Singleton<DeviceLoad> {
 public:
  DeviceLoad() = default;

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
      ptz.IP = config["ptz"]["IP"].as<std::string>();
      ptz.Port = config["ptz"]["Port"].as<int32_t>();
      ptz.username = config["ptz"]["username"].as<std::string>();
      ptz.passwd = config["ptz"]["passwd"].as<std::string>();

    } catch (const std::exception &e) {
      std::cerr << "Error parsing YAML file " << filePath << ": " << e.what() << std::endl;
      exit(-1);
    }
  };
};
}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_DEVICE_H_
