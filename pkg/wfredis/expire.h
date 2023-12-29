//
// Created by souffle on 23-12-21.
//

#ifndef TEMPLATE_PKG_WFREDIS_EXPIRE_H_
#define TEMPLATE_PKG_WFREDIS_EXPIRE_H_

#include "conf.h"

using RedisEXPIREOptCallBack = std::function<void(void *)>;
inline void DefaultCallBack(void *) {}
//! 同步设置键超时(s)
//! \param redisAddr
//! \param key
//! \param seconds
inline bool EXPIRE(const std::string &redisAddr,
                   const std::string &key,
                   int seconds) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] EXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      } else {
                                                        ok = true;
                                                      }
                                                      wait_group.done();
                                                    }
  );
  redisTask->get_req()->set_request("EXPIRE", {key, fmt::format("{}", seconds)});
  redisTask->start();
  wait_group.wait();
  return ok;
}

//! 异步设置key超时(s)
//! \param redisAddr
//! \param key
//! \param seconds
inline void EXPIREASync(const std::string &redisAddr,
                        const std::string &key,
                        int seconds,
                        RedisEXPIREOptCallBack cb = DefaultCallBack
) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&cb](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] EXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                        cb(nullptr);
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("EXPIRE", {key, fmt::format("{}", seconds)});
  redisTask->start();
}

//! 同步设置键超时(s) 返回未执行的任务
//! \param redisAddr
//! \param key
//! \param seconds
inline WFRedisTask *EXPIRETask(const std::string &redisAddr,
                               const std::string &key,
                               int seconds) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] EXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("EXPIRE", {key, fmt::format("{}", seconds)});
  return redisTask;
}

//! 同步设置键超时(ms)
//! \param redisAddr
//! \param key
//! \param milliseconds
//! \return
inline bool PEXPIRE(const std::string &redisAddr,
                    const std::string &key,
                    int64_t milliseconds) {
  WFFacilities::WaitGroup wait_group(1);
  bool ok = false;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&wait_group, &ok](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] PEXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      } else {
                                                        ok = true;
                                                      }
                                                      wait_group.done();
                                                    }
  );
  redisTask->get_req()->set_request("PEXPIRE", {key, fmt::format("{}", milliseconds)});
  redisTask->start();
  wait_group.wait();
  return ok;
}

//! 异步设置key超时(ms)
//! \param redisAddr
//! \param key
//! \param milliseconds
inline void PEXPIREASync(const std::string &redisAddr,
                         const std::string &key,
                         int64_t milliseconds,
                         RedisEXPIREOptCallBack cb = DefaultCallBack) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&cb](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] PEXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                        cb(nullptr);
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("PEXPIRE", {key, fmt::format("{}", milliseconds)});
  redisTask->start();
}

//! 同步设置键超时(ms) 返回未执行的任务
//! \param redisAddr
//! \param key
//! \param milliseconds
inline WFRedisTask *PEXPIRETask(const std::string &redisAddr,
                                const std::string &key,
                                int64_t milliseconds) {
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] PEXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("PEXPIRE", {key, fmt::format("{}", milliseconds)});
  return redisTask;
}

inline int TTL(const std::string &redisAddr,
               const std::string &key) {
  int ttl = 0;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&ttl](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] PEXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                        ttl = -1;
                                                      } else {
                                                        ttl = value.int_value();
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("TTL", {key});
  redisTask->start();
  return ttl;
}

inline int64_t PTTL(const std::string &redisAddr,
                    const std::string &key) {
  int ttl = 0;
  auto redisTask = WFTaskFactory::create_redis_task(redisAddr,
                                                    RetryMax,
                                                    [&ttl](WFRedisTask *task) {
                                                      protocol::RedisValue value;
                                                      task->get_resp()->get_result(value);
                                                      if (!value.is_ok()) {
                                                        std::string cmd;
                                                        spdlog::error("[redis] PEXPIRE: {}",
                                                                      task->get_req()->get_command(cmd));
                                                        ttl = -1;
                                                      } else {
                                                        ttl = value.int_value();
                                                      }
                                                    }
  );
  redisTask->get_req()->set_request("PTTL", {key});
  redisTask->start();
  return ttl;
}

#endif //TEMPLATE_PKG_WFREDIS_EXPIRE_H_
