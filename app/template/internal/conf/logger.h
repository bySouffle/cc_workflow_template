//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_LOGGER_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_LOGGER_H_
#include "conf.h"

namespace conf {
class Logger {
 public:
  bool Development{};
  bool DisableCaller{};
  bool DisableStacktrace{};
  std::string Encoding{};
  std::string Level{};
  std::string Name{};
  std::string Writers{};
  std::string LoggerDir{};
  std::string LogRollingPolicy{};
  uint32 LogRotateDate{};
  uint32 LogRotateSize{};
  uint32 LogBackupCount{};
 protected:
  const inline static std::string configName = "logger.yaml";

};
}
#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_LOGGER_H_
