//
// Created by bys on 2023/10/12.
//

#ifndef PTZ_MODULE_APP_PTZ_INTERNAL_PKG_UTILS_UUID_H_
#define PTZ_MODULE_APP_PTZ_INTERNAL_PKG_UTILS_UUID_H_

#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>

inline std::string generateUUID() {
  std::random_device rd;
  std::default_random_engine rng(rd());
  std::uniform_int_distribution<int> dist(0, 15);

  std::stringstream ss;
  ss << std::hex;
  for (int i = 0; i < 32; ++i) {
    if (i == 8 || i == 12 || i == 16 || i == 20) {
      ss << '-';
    }
    ss << dist(rng);
  }

  return ss.str();
}

#endif //PTZ_MODULE_APP_PTZ_INTERNAL_PKG_UTILS_UUID_H_
