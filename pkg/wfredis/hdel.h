//
// Created by souffle on 23-12-21.
//

#ifndef TEMPLATE_PKG_WFREDIS_HDEL_H_
#define TEMPLATE_PKG_WFREDIS_HDEL_H_
#include "conf.h"

using RedisHDelCallBack = std::function<void(void *)>;
inline void DefaultRedisHDelCallBack(void *) {}

inline bool HDEL(const std::string &redisAddr,
                 const std::string &key, const std::vector<std::string> &field,
                 RedisHDelCallBack cb = DefaultRedisHDelCallBack) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok, &cb](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (value.is_ok()) {
                                                        ok = true;
                                                        cb(nullptr);
                                                      }
                                                      wait_group.done();
                                                    }
  );
  std::vector<std::string> key_field;
  key_field.emplace_back(key);
  key_field.insert(key_field.end(), field.begin(), field.end());
  redisTask->get_req()->set_request("HDEL", key_field);
  redisTask->start();
  wait_group.wait();
  return ok;
}
#endif //TEMPLATE_PKG_WFREDIS_HDEL_H_
