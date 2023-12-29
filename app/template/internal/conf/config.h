//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_CONFIG_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_CONFIG_H_

#include "conf.h"

namespace conf {
class App {
 public:
  std::string ID{};
  std::string Name{};
  std::string Version{};
  std::string Environment{};
};

class General {
 public:
  bool SSL{};
  std::string Mode{};
  bool CSRF{};
  bool Debug{};
  std::string Cert{};
  std::string Key{};
};

class Security {
 public:
  std::string CookieName{};
  std::string JwtSecret{};
  std::string JwtTimeout{};
};

class Trace {
 public:
  std::string ServiceName{};
  std::string HostName{};
  std::string CollectorEndpoint{};
  std::string Exporter{};
  std::string TraceFilePath{};
  std::string GrpcToken{};
};

class Experiment {
 public:
  bool EnableTrace{};
  bool EnablePprof{};
  Trace trace{};
};

class Http {
 public:
  std::string addr{};
  std::string scheme{};
  std::string timeout{};
};

class Grpc {
 public:
  std::string addr{};
  std::string scheme{};
  std::string timeout{};
};

class Cron {
 public:
  std::string addr{};
  uint32 db{};
  std::string password{};
  std::string dial_timeout{};
  std::string read_timeout{};
  std::string write_timeout{};
  uint32 MinIdleConn{};
  uint32 PoolSize{};
  std::string PoolTimeout{};
  uint32 Concurrency{};
};

class Mqtt {
 public:
  std::string addr{};
  std::string ClientID{};
  std::string Username{};
  std::string Password{};
  bool AutoReconnect{};
  std::string MaxReconnectInterval{};
};

class Server {
 public:
  Http http{};
  Grpc grpc{};
  Cron cron{};
  Mqtt mqtt{};
};

class Database {
 public:
  std::string driver{};
  std::string source{};
  std::string Name{};
  std::string Addr{};
  std::string UserName{};
  std::string Password{};
  bool ShowLog{};
  uint32 MaxIdleConn{};
  uint32 MaxOpenConn{};
  std::string ConnMaxLifeTime{};
  std::string SlowThreshold{};
};

class Redis {
 public:
  std::string network{};
  std::string addr{};
  uint32 db{};
  std::string password{};
  std::string dial_timeout{};
  std::string read_timeout{};
  std::string write_timeout{};
};

class Data {
 public:
  Database database{};
  Redis redis{};
};

class BootStrap {
 public:
  App app{};
  General general{};
  Security security{};
  Experiment experiment{};
  Server server{};
  Data data{};
  inline const static std::string configName = "bootstrap.yaml";
 protected:
};
}
#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_CONF_CONFIG_H_
