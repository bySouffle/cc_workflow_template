//
// Created by souffle on 23-12-21.
//

#ifndef TEMPLATE_PKG_WFREDIS_DEL_H_
#define TEMPLATE_PKG_WFREDIS_DEL_H_
#include "conf.h"

using RedisDelCallBack = std::function<void(void *)>;
inline void DefaultRedisDelCallBack(void *) {};

inline bool DEL(const std::string &redisAddr,
                const std::string &key) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (value.is_ok()) {
                                                        ok = true;
                                                      }
                                                      wait_group.done();
                                                    }
  );
  redisTask->get_req()->set_request("DEL", {key});
  redisTask->start();
  wait_group.wait();
  return ok;
}
#endif //TEMPLATE_PKG_WFREDIS_DEL_H_
