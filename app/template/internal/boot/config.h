//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_CONFIG_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_CONFIG_H_
#include <iostream>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <functional>
namespace boot {

namespace fs = std::filesystem;
using ConfigFiles = std::vector<fs::directory_entry>;

class Boot {
 public:
  static ConfigFiles LoadConfigDirectory(const std::string &dir) {
    configFiles.clear();
    for (const auto &entry : fs::directory_iterator(dir)) {
      // 检查文件扩展名是否为 .yaml 或 .yml
      if (entry.path().extension() == ".yaml" || entry.path().extension() == ".yml") {
        configFiles.emplace_back(entry);
      }
    }
    return configFiles;
  };

  inline static ConfigFiles configFiles{};
};
}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_CONFIG_H_
