//
// Created by bys on 2023/10/13.
//

#ifndef PTZ_MODULE_APP_PTZ_INTERNAL_PKG_UTILS_FILE_H_
#define PTZ_MODULE_APP_PTZ_INTERNAL_PKG_UTILS_FILE_H_

#include <string>
#include <filesystem>

int CreateParentFolder(const std::string &filePath) {
  std::error_code code;
  std::filesystem::path path(filePath);
  if (path.has_parent_path() && !std::filesystem::exists(path)) {
    std::filesystem::path folder_path = path.parent_path();
    std::filesystem::create_directories(folder_path, code);
  }
  return code.value();
}
#endif //PTZ_MODULE_APP_PTZ_INTERNAL_PKG_UTILS_FILE_H_
