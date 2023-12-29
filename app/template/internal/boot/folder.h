//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_FOLDER_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_FOLDER_H_

#include "singleton.h"
#include "template/internal/conf/folder.h"
#include "config.h"

namespace boot {
class FolderLoad : public Folder, public Singleton<FolderLoad> {
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
      savePath.image = config["savePath"]["image"].as<std::string>();
    } catch (const std::exception &e) {
      std::cerr << "Error parsing YAML file " << filePath << ": " << e.what() << std::endl;
      exit(-1);
    }
  };
};
}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_FOLDER_H_
