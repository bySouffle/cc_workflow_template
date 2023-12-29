//
// Created by souffle on 23-12-21.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_SERVER_HTTP_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_SERVER_HTTP_H_
#include <utility>
#include <wfrest/HttpServer.h>
#include <workflow/WFFacilities.h>
#include <workflow/URIParser.h>
#include <spdlog/spdlog.h>
#include "template/internal/boot/bootstrap.h"
#include "template/internal/service/template.h"

#include "middleware/header_handle.h"

namespace server {
using namespace service;
class HttpServer {
 public:
  explicit HttpServer(const conf::BootStrap &c) :
      SSL_(c.general.SSL), cert_(c.general.Cert), key_(c.general.Key) {
    port_ = getPort(c.server.http.addr);
  }

  void RegisterSvc(TemplateSvc *template_svc) {
    if (template_svc == nullptr) {
      fprintf(stderr, "[http] server Register template_svc_=nil ");
      exit(-1);
    }
    template_svc->registerSvc(server_);
  }

  void Stop() {
    wait_group.done();
    server_.stop();
  }
  void Start() {
    int ret_val = 0;
    if (!SSL_) {
      ret_val = server_.start(port_);
    } else {
      ret_val = server_.start(port_, cert_.c_str(), key_.c_str());
    }

    middleWare();

    if (ret_val == 0) {
      server_.list_routes();
      server_.print_node_arch();
      spdlog::info("[http] port:{} cert:{} key:{} start", port_, cert_, key_);
      wait_group.wait();
    } else {
      spdlog::info("[http] port:{} cert:{} key:{} start failed", port_, cert_, key_);
      exit(1);
    }
  }
  void StartASync() {
    std::thread t([this]() {
      int ret_val = 0;
      if (!SSL_) {
        ret_val = server_.start(port_);
      } else {
        ret_val = server_.start(port_, cert_.c_str(), key_.c_str());
      }
      if (ret_val == 0) {
        server_.list_routes();
        server_.print_node_arch();
        spdlog::info("[http] port:{} cert:{} key:{} start async", port_, cert_, key_);
      } else {
        spdlog::info("[http] port:{} cert:{} key:{} start async failed", port_, cert_, key_);
        exit(-1);
      }
    });
    t.detach();
  }

  ~HttpServer() = default;

 private:
  int port_;
  bool SSL_{};
  std::string cert_;
  std::string key_;
  wfrest::HttpServer server_;
  WFFacilities::WaitGroup wait_group{1};

  static int getPort(const std::string &addr) {
    size_t colonPos = addr.find(':');
    if (colonPos != std::string::npos) {
      std::string portString = addr.substr(colonPos + 1);

      // 将提取的字符串转换为整数
      try {
        int port = std::stoi(portString);
        return port;
      } catch (const std::exception &e) {
        std::cerr << "Error converting port to integer: " << e.what() << std::endl;
      }
    } else {
      std::cerr << "Colon not found in the input string." << std::endl;
    }

    // 如果没有找到端口，返回一个错误值
    return -1;
  }

  //  注入中间层处理
  void middleWare() {
    server_.Use(middleware::Trace());
  }
};
HttpServer *NewHttpServer(const conf::BootStrap &c) {
  return new HttpServer(c);
}

}
#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_SERVER_HTTP_H_
