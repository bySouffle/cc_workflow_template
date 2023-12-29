//
// Created by souffle on 23-12-22.
//

#ifndef TEMPLATE_PKG_RESPONSE_H_
#define TEMPLATE_PKG_RESPONSE_H_

#include <wfrest/json.hpp>

inline nlohmann::json RESP_OK(const nlohmann::json &data) {
  return nlohmann::json{
      {"code", 200},
      {"message", "OK"},
      {"data", data}
  };
}

inline nlohmann::json RESP_Fail(int code, const std::string &msg, const nlohmann::json &data) {
  return nlohmann::json{
      {"code", code},
      {"message", msg},
      {"data", data}
  };
}

#endif //TEMPLATE_PKG_RESPONSE_H_
