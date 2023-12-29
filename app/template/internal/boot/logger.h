//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_LOGGER_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_LOGGER_H_

#include "singleton.h"
#include "template/internal/conf/logger.h"
#include "config.h"

namespace boot {
class LoggerLoad : public Logger, public Singleton<LoggerLoad> {
 public:
  LoggerLoad() = default;

  inline void Load(const ConfigFiles &files) {
    for (auto &f : files) {
      std::string filename = f.path().filename().string();
      if (filename == configName) {
        parse(f.path());
        break;
      }
    }
  }

 private:
  inline void parse(const std::string &filePath) {
    try {
      YAML::Node config = YAML::LoadFile(filePath);
      Development = config["Development"].as<bool>();
      DisableCaller = config["DisableCaller"].as<bool>();
      DisableStacktrace = config["DisableStacktrace"].as<bool>();
      Encoding = config["Encoding"].as<std::string>();
      Level = config["Level"].as<std::string>();
      Name = config["Name"].as<std::string>();
      Writers = config["Writers"].as<std::string>();
      LoggerDir = config["LoggerDir"].as<std::string>();
      LogRollingPolicy = config["LogRollingPolicy"].as<std::string>();
      LogRotateDate = config["LogRotateDate"].as<uint32_t>();
      LogRotateSize = config["LogRotateSize"].as<uint32_t>();
      LogBackupCount = config["LogBackupCount"].as<uint32_t>();
    } catch (const std::exception &e) {
      std::cerr << "Error parsing YAML file " << filePath << ": " << e.what() << std::endl;
      exit(-1);
    }
  };
};
}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_LOGGER_H_
