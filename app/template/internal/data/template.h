//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_DATA_TEMPLATE_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_DATA_TEMPLATE_H_

#include <string_view>
#include <string>
#include <utility>
#include <workflow/Workflow.h>
#include <workflow/WFTaskFactory.h>
#include <wfredis/wfredis.h>
#include <spdlog/fmt/fmt.h>
#include "template/internal/boot/bootstrap.h"

using namespace boot;

namespace data {

class TemplateRepo {
 public:
  explicit TemplateRepo(const BootStrap &conf) {
    setRedisURL(conf);
  };
  void setRedisURL(const BootStrap &conf) {
//    conf.data.redis.addr
    redisURL = fmt::format("redis://:{}@{}/{}",
                           conf.data.redis.password,
                           conf.data.redis.addr,
                           conf.data.redis.db);
  };

  struct TemplateReq {
    std::string key;
    int keyID;
    std::string value;

    [[nodiscard]] inline std::string buildKey() const {
      return fmt::format("{}:{}", key, keyID);
    }
  };
  struct TemplateResp {
    std::string value;
  };

  bool CreateTemplate(const TemplateReq &req) {
    auto key = req.buildKey();
    return SetEx(redisURL, key, req.value, 1000);
  }
  bool UpdateTemplate(const TemplateReq &req) {
    auto key = req.buildKey();
    return SetEx(redisURL, key, req.value, 1000);
  }
  bool DeleteTemplate(const TemplateReq &req) {
    auto key = req.buildKey();
    return DEL(redisURL, key);
  }
  bool QueryTemplate(const TemplateReq &req, TemplateResp &resp) {
    auto key = req.buildKey();
    return Get(redisURL, key, resp.value);
  }

  static TemplateRepo *NewTemplateRepo(const BootStrap &conf) {
    return new TemplateRepo(conf);
  }

 protected:
 private:
  std::string redisURL;
};

}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_DATA_TEMPLATE_H_
