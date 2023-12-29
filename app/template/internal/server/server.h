//
// Created by souffle on 23-12-21.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_SERVER_SERVER_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_SERVER_SERVER_H_

#include "http.h"
#include "template/internal/data/data.h"
namespace server {
class Server {
 public:
  Server() = default;
  ~Server() {
    delete dataLayer;
    dataLayer = nullptr;
    delete template_svc_;
    template_svc_ = nullptr;
  }

  void InitDataLayer(const BootStrap &conf) {
    dataLayer = NewRepo(conf);
  }

  void InitBizLayer() {
    UseCaseBase::GetInstance().SetRepo(dataLayer);
  }

  void InitSvcLayer() {
    template_svc_ = NewTemplateSvc();
  }

  void InitSrvLayer(const BootStrap &conf) {
    http_server = NewHttpServer(conf);
    http_server->RegisterSvc(template_svc_);
  }

  void StartASync() {
    http_server->StartASync();
  }

  void Start() {
    http_server->Start();
  }

  void Stop() {
    http_server->Stop();
  }

 private:
  Repo *dataLayer = nullptr;
  TemplateSvc *template_svc_ = nullptr;
  HttpServer *http_server = nullptr;
};

}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_SERVER_SERVER_H_
