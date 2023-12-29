//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_CMD_TEMPLATE_ENVIRONMENT_H_
#define TEMPLATE_APP_TEMPLATE_CMD_TEMPLATE_ENVIRONMENT_H_
#include <spdlog/spdlog.h>

#include "template/internal/boot/config.h"
#include "template/internal/boot/bootstrap.h"
#include "template/internal/boot/device.h"
#include "template/internal/boot/folder.h"
#include "template/internal/boot/logger.h"
#include "template/internal/boot/registry.h"

#include "template/internal/server/server.h"

#include "otel/http.h"
#include "otel/grpc.h"
#include "register/consul.h"

using namespace boot;

class Environment {
 public:
  static void NewBootConf(const std::string &path) {
    auto files = Boot::LoadConfigDirectory(path);
    auto &boot_strap_load = BootStrapLoad::GetInstance();
    boot_strap_load.Load(files);

    auto &device_load = DeviceLoad::GetInstance();
    device_load.Load(files);

    auto &folder_load = FolderLoad::GetInstance();
    folder_load.Load(files);

    auto &logger_load = LoggerLoad::GetInstance();
    logger_load.Load(files);

    auto &registry_load = RegistryLoad::GetInstance();
    registry_load.Load(files);
  };

  static server::Server *NewServer() {
    auto Srv = new server::Server;
    Srv->InitDataLayer(BootStrapLoad::GetInstance());
    Srv->InitBizLayer();
    Srv->InitSvcLayer();
    Srv->InitSrvLayer(BootStrapLoad::GetInstance());

    return Srv;
  }

  static void Logger(const Logger &conf) {}

  static void NewTrace(const Trace &conf) {

    if (conf.Exporter == pkg::otel::GRPCOtel::Exporter) {
      auto c = pkg::otel::GRPCOtel::Config{
          .ServiceName = conf.ServiceName,
          .HostName = conf.HostName,
          .CollectorEndpoint = conf.CollectorEndpoint,
          .Exporter = conf.Exporter,
          .TraceFilePath = conf.TraceFilePath,
          .GrpcToken = conf.GrpcToken,
      };
      pkg::otel::GRPCOtel::InitTracer(c);
    } else if (conf.Exporter == pkg::otel::HTTPOtel::Exporter) {
      auto c = pkg::otel::HTTPOtel::Config{
          .ServiceName = conf.ServiceName,
          .HostName = conf.HostName,
          .CollectorEndpoint = conf.CollectorEndpoint,
          .Exporter = conf.Exporter,
          .TraceFilePath = conf.TraceFilePath,
          .GrpcToken = conf.GrpcToken,
      };
      pkg::otel::HTTPOtel::InitTracer(c);
    } else {
      spdlog::warn("[Env] otlp Exporter no exist");
    }

  }

  static void NewRegister(const Consul &conf, const App &app, const Http &http) {
    auto c = consul::RegisterConfig{
        .service_namespace="",
        .service_name=app.Name,
        .service_id=app.Name,
        .address=conf.address,
        .scheme=conf.scheme,
        .health_check=conf.health_check,
        .health_check_url = fmt::format("{}://{}{}", http.scheme, http.addr, conf.health_check_url),
        .health_check_timeout=conf.health_check_timeout,
    };
    auto &r = consul::Api::GetInstance();
    r.init(c);
    r.Register(http.addr, {"version=v1.0"}, {{"k1", "v1"}, {"k2", "v2"}});
  }

 private:

};

#endif //TEMPLATE_APP_TEMPLATE_CMD_TEMPLATE_ENVIRONMENT_H_
