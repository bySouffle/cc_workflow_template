//
// Created by souffle on 23-12-20.
//

#ifndef TEMPLATE_PKG_WFREDIS_GET_H_
#define TEMPLATE_PKG_WFREDIS_GET_H_

#include "conf.h"

using RedisGetCallBack = std::function<void(const std::string &)>;

inline bool Get(const std::string &redisAddr,
                const std::string &key, std::string &query_value) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok, &query_value](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (value.is_ok()) {
                                                        query_value = value.string_value();
                                                        ok = true;
                                                      }
                                                      wait_group.done();
                                                    }
  );
  redisTask->get_req()->set_request("GET", {key});
  redisTask->start();
  wait_group.wait();
  return ok;
}

inline void GetAsync(const std::string &redisAddr,
                     const std::string &key, const RedisGetCallBack &cb) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [cb](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (value.is_ok()) {
                                                        cb(value.string_value());
                                                      } else {
                                                        std::string cmd;
                                                        spdlog::error("[redis] GET: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("GET", {key});
  redisTask->start();
}

#endif //TEMPLATE_PKG_WFREDIS_GET_H_
