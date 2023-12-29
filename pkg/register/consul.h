//
// Created by souffle on 23-12-28.
//

#ifndef TEMPLATE_PKG_REGISTER_CONSUL_H_
#define TEMPLATE_PKG_REGISTER_CONSUL_H_

#include <utility>
#include <workflow/WFConsulClient.h>
#include <workflow/WFFacilities.h>
#include <spdlog/fmt/fmt.h>
#include "singleton.h"

#include "node.h"

namespace consul {
using namespace protocol;

struct RegisterConfig {
  std::string service_namespace;
  std::string service_name;
  std::string service_id;

  std::string address;
  std::string scheme;
  bool health_check{};
  std::string health_check_url; //  完整地址
  int health_check_timeout{};
};
class Api : public Singleton<Api> {
 public:
  explicit Api() = default;
  ~Api() override { DeRegister(); };

  int init(RegisterConfig c) {
    register_config_ = std::move(c);

    if (register_config_.health_check) {
      config_.set_health_check(register_config_.health_check);
      config_.set_check_http_url(register_config_.health_check_url);
      config_.set_check_timeout(register_config_.health_check_timeout);
//      config_.set_token("cd125427-3fd1-f326-bf46-fbce06cc9003");
      config_.set_blocking_query(true);
    }

    auto url = fmt::format("{}://{}", register_config_.scheme, register_config_.address);
    return client_.init(url, config_);
  }

  //! 注册服务
  //! \param url  服务端口地址 ex:127.0.0.1:8080
  //! \param tags tags 信息
  //! \param metas metas {"key", "value"}
  //! \return
  bool Register(const std::string &url,
                const std::vector<std::string> &tags = {},
                const std::map<std::string, std::string> &metas = {}) {
    bool ok;
    WFFacilities::WaitGroup wait_group(1);
    auto task = client_.create_register_task(register_config_.service_namespace,
                                             register_config_.service_name,
                                             register_config_.service_id, 3,
                                             [&wait_group, &ok](WFConsulTask *task) {
                                               register_callback(task,
                                                                 wait_group,
                                                                 ok);
                                             });

    std::istringstream stream(url);
    std::string host;
    int port{};

    std::getline(stream, host, ':');
    stream >> port;

    protocol::ConsulService service;
    service.tags = tags;
    service.service_address.first = host;
    service.service_address.second = port;
    service.meta = metas;
    service.tag_override = true;
    task->set_service(&service);
    task->start();
    wait_group.wait();
    return ok;
  }

  bool DeRegister() {
    bool ok;
    WFFacilities::WaitGroup wait_group(1);
    auto task = client_.create_deregister_task(register_config_.service_namespace,
                                               register_config_.service_id, 3,
                                               [&wait_group, &ok](WFConsulTask *task) {
                                                 deregister_callback(task,
                                                                     wait_group,
                                                                     ok);
                                               });
    task->start();
    wait_group.wait();
    return ok;
  }

  bool Discovery(const std::string &service_namespace, const std::string &service_name, Instances &instance) {
    bool ok;
    WFFacilities::WaitGroup wait_group(1);
    auto task = client_.create_discover_task(service_namespace,
                                             service_name, 3,
                                             [&wait_group, &ok, &instance](WFConsulTask *task) {
                                               discovery_callback(task,
                                                                  wait_group,
                                                                  ok,
                                                                  instance);
                                             });
    task->start();
    wait_group.wait();
    return ok;
  }

  bool ListService(const std::string &service_namespace, std::vector<protocol::ConsulServiceTags> &services) {
    bool ok;
    WFFacilities::WaitGroup wait_group(1);
    auto task = client_.create_list_service_task(service_namespace, 3,
                                                 [&wait_group, &ok, &services](WFConsulTask *task) {
                                                   list_service_callback(task,
                                                                         wait_group,
                                                                         ok,
                                                                         services);
                                                 });
    task->start();
    wait_group.wait();
    return ok;
  }

 private:
  RegisterConfig register_config_;

  WFConsulClient client_;
  ConsulConfig config_;

  static void register_callback(WFConsulTask *task, WFFacilities::WaitGroup &wait_group, bool &ok) {
    int state = task->get_state();
    int error = task->get_error();

    if (state != WFT_STATE_SUCCESS) {
      spdlog::error("[consul] code:{} msg:{}", error, WFGlobal::get_error_string(state, error));
      wait_group.done();
      ok = false;
      return;
    }

    int api_type = task->get_api_type();
    if (api_type == CONSUL_API_TYPE_REGISTER) {
      spdlog::info("[consul] register success");
      ok = true;
    }

    wait_group.done();
  }
  static void deregister_callback(WFConsulTask *task, WFFacilities::WaitGroup &wait_group, bool &ok) {
    int state = task->get_state();
    int error = task->get_error();

    if (state != WFT_STATE_SUCCESS) {
      spdlog::error("[consul] code:{} msg:{}", error, WFGlobal::get_error_string(state, error));
      wait_group.done();
      ok = false;
      return;
    }

    int api_type = task->get_api_type();
    if (api_type == CONSUL_API_TYPE_DEREGISTER) {
      spdlog::info("[consul] deregister success");
      ok = true;
    }
    wait_group.done();
  }
  static void discovery_callback(WFConsulTask *task, WFFacilities::WaitGroup &wait_group, bool &ok,
                                 Instances &instances) {
    int state = task->get_state();
    int error = task->get_error();

    if (state != WFT_STATE_SUCCESS) {
      spdlog::error("[consul] code:{} msg:{}", error, WFGlobal::get_error_string(state, error));
      wait_group.done();
      ok = false;
      return;
    }

    int api_type = task->get_api_type();
    if (api_type == CONSUL_API_TYPE_DISCOVER) {
      spdlog::info("[consul] discover success");
      spdlog::info("[consul] discover consul-index:{}", task->get_consul_index());
      std::string body = HttpUtil::decode_chunked_body(task->get_http_resp());
      nlohmann::json j = nlohmann::json::parse(body, nullptr, false);
      if (!j.is_discarded() && j.is_array()) {
        for (auto &obj : j) {
          Instance i{};
          i.node_from_json(obj);
          instances.emplace_back(i);
        }
        ok = true;
      }

    }
    wait_group.done();
  }
  static void list_service_callback(WFConsulTask *task, WFFacilities::WaitGroup &wait_group, bool &ok,
                                    std::vector<struct protocol::ConsulServiceTags> &services) {
    int state = task->get_state();
    int error = task->get_error();

    if (state != WFT_STATE_SUCCESS) {
      spdlog::error("[consul] code:{} msg:{}", error, WFGlobal::get_error_string(state, error));
      wait_group.done();
      ok = false;
      return;
    }

    int api_type = task->get_api_type();
    if (api_type == CONSUL_API_TYPE_LIST_SERVICE) {
      spdlog::info("[consul] list service success");
      spdlog::info("[consul] list service consul-index:{}", task->get_consul_index());
      if (task->get_list_service_result(services)) {
        ok = true;
      }
    }
    wait_group.done();
  }

};

}

#endif //TEMPLATE_PKG_REGISTER_CONSUL_H_
