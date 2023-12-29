//
// Created by souffle on 23-12-20.
//

#ifndef TEMPLATE_PKG_WFREDIS_SET_H_
#define TEMPLATE_PKG_WFREDIS_SET_H_

#include "conf.h"

inline bool Set(const std::string &redisAddr,
                const std::string &key, const std::string &value) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] Set: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      } else {
                                                        ok = true;
                                                      }
                                                      wait_group.done();
                                                    }
  );
  redisTask->get_req()->set_request("SET", {key, value});
  redisTask->start();
  wait_group.wait();
  return ok;
}

inline bool SetEx(const std::string &redisAddr,
                  const std::string &key, const std::string &value, int seconds) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] SetEX: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      } else {
                                                        ok = true;
                                                      }
                                                      wait_group.done();
                                                    }
  );
  redisTask->get_req()->set_request("SetEX", {key, fmt::format("{}", seconds), value});
  redisTask->start();
  wait_group.wait();
  return ok;
}

inline bool PSetEx(const std::string &redisAddr,
                   const std::string &key, const std::string &value, uint64_t milliseconds) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] PSetEX: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      } else {
                                                        ok = true;
                                                      }
                                                      wait_group.done();
                                                    }
  );
  redisTask->get_req()->set_request("PSetEX", {key, fmt::format("{}", milliseconds), value});
  redisTask->start();
  wait_group.wait();
  return ok;
}

inline void SetAsync(const std::string &redisAddr,
                     const std::string &key, const std::string &value) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] Set: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("SET", {key, value});
  redisTask->start();
}

inline void SetExAsync(const std::string &redisAddr,
                       const std::string &key, const std::string &value, int64_t seconds = 0) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] SetEX: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("SetEX", {key, fmt::format("{}", seconds), value});
  redisTask->start();
}

inline void SetPxAsync(const std::string &redisAddr,
                       const std::string &key, const std::string &value, int64_t milliseconds = 0) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] SetPX: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("SetPX", {key, fmt::format("{}", milliseconds), value});
  redisTask->start();
}
#endif //TEMPLATE_PKG_WFREDIS_SET_H_
