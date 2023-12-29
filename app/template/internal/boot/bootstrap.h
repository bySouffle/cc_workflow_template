//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_CONF_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_CONF_H_

#include "singleton.h"
#include "template/internal/conf/config.h"
#include "config.h"
using namespace conf;

namespace boot {

class BootStrapLoad : public BootStrap, public Singleton<BootStrapLoad> {
 public:
  BootStrapLoad() = default;

  void Load(const ConfigFiles &files) {
    for (auto &f : files) {
      std::string filename = f.path().filename().string();
      if (filename == configName) {
        parse(f.path());
        break;
      }
    }
  }

 private:

  void parse(const std::string &filePath) {
    try {
      YAML::Node config = YAML::LoadFile(filePath);
      // Parse the YAML nodes and assign values to the corresponding class members
      // 解析 App 字段
      app.ID = config["app"]["ID"].as<std::string>();
      app.Name = config["app"]["Name"].as<std::string>();
      app.Version = config["app"]["Version"].as<std::string>();
      app.Environment = config["app"]["Environment"].as<std::string>();

      // 解析 General 字段
      general.SSL = config["general"]["SSL"].as<bool>();
      general.Mode = config["general"]["Mode"].as<std::string>();
      general.CSRF = config["general"]["CSRF"].as<bool>();
      general.Debug = config["general"]["Debug"].as<bool>();
      general.Cert = config["general"]["Cert"].as<std::string>();
      general.Key = config["general"]["Key"].as<std::string>();

      // 解析 Security 字段
      security.CookieName = config["security"]["CookieName"].as<std::string>();
      security.JwtSecret = config["security"]["JwtSecret"].as<std::string>();
      security.JwtTimeout = config["security"]["JwtTimeout"].as<std::string>();

      // 解析 Experiment 字段
      experiment.EnableTrace = config["experiment"]["EnableTrace"].as<bool>();
      experiment.EnablePprof = config["experiment"]["EnablePprof"].as<bool>();

      // 解析 Experiment::Trace 字段
      experiment.trace.ServiceName = config["experiment"]["trace"]["ServiceName"].as<std::string>();
      experiment.trace.HostName = config["experiment"]["trace"]["HostName"].as<std::string>();
      experiment.trace.CollectorEndpoint = config["experiment"]["trace"]["CollectorEndpoint"].as<std::string>();
      experiment.trace.Exporter = config["experiment"]["trace"]["Exporter"].as<std::string>();
      experiment.trace.TraceFilePath = config["experiment"]["trace"]["TraceFilePath"].as<std::string>();
      experiment.trace.GrpcToken = config["experiment"]["trace"]["GrpcToken"].as<std::string>();

      // 解析 Server 字段
      server.http.addr = config["server"]["http"]["addr"].as<std::string>();
      server.http.scheme = config["server"]["http"]["scheme"].as<std::string>();
      server.http.timeout = config["server"]["http"]["timeout"].as<std::string>();

      server.grpc.addr = config["server"]["grpc"]["addr"].as<std::string>();
      server.grpc.scheme = config["server"]["grpc"]["scheme"].as<std::string>();
      server.grpc.timeout = config["server"]["grpc"]["timeout"].as<std::string>();

      server.cron.addr = config["server"]["cron"]["addr"].as<std::string>();
      server.cron.db = config["server"]["cron"]["db"].as<uint32>();
      server.cron.password = config["server"]["cron"]["password"].as<std::string>();
      server.cron.dial_timeout = config["server"]["cron"]["dial_timeout"].as<std::string>();
      server.cron.read_timeout = config["server"]["cron"]["read_timeout"].as<std::string>();
      server.cron.write_timeout = config["server"]["cron"]["write_timeout"].as<std::string>();
      server.cron.MinIdleConn = config["server"]["cron"]["MinIdleConn"].as<uint32>();
      server.cron.PoolSize = config["server"]["cron"]["PoolSize"].as<uint32>();
      server.cron.PoolTimeout = config["server"]["cron"]["PoolTimeout"].as<std::string>();
      server.cron.Concurrency = config["server"]["cron"]["Concurrency"].as<uint32>();

      server.mqtt.addr = config["server"]["mqtt"]["addr"].as<std::string>();
      server.mqtt.ClientID = config["server"]["mqtt"]["ClientID"].as<std::string>();
      server.mqtt.Username = config["server"]["mqtt"]["Username"].as<std::string>();
      server.mqtt.Password = config["server"]["mqtt"]["Password"].as<std::string>();
      server.mqtt.AutoReconnect = config["server"]["mqtt"]["AutoReconnect"].as<bool>();
      server.mqtt.MaxReconnectInterval = config["server"]["mqtt"]["MaxReconnectInterval"].as<std::string>();


      // 解析 Database 字段
      data.database.driver = config["data"]["database"]["driver"].as<std::string>();
      data.database.source = config["data"]["database"]["source"].as<std::string>();
      data.database.Name = config["data"]["database"]["Name"].as<std::string>();
      data.database.Addr = config["data"]["database"]["Addr"].as<std::string>();
      data.database.UserName = config["data"]["database"]["UserName"].as<std::string>();
      data.database.Password = config["data"]["database"]["Password"].as<std::string>();
      data.database.ShowLog = config["data"]["database"]["ShowLog"].as<bool>();
      data.database.MaxIdleConn = config["data"]["database"]["MaxIdleConn"].as<uint32_t>();
      data.database.MaxOpenConn = config["data"]["database"]["MaxOpenConn"].as<uint32_t>();
      data.database.ConnMaxLifeTime = config["data"]["database"]["ConnMaxLifeTime"].as<std::string>();
      data.database.SlowThreshold = config["data"]["database"]["SlowThreshold"].as<std::string>();

      // 解析 Redis 字段
      data.redis.network = config["data"]["redis"]["network"].as<std::string>();
      data.redis.addr = config["data"]["redis"]["addr"].as<std::string>();
      data.redis.db = config["data"]["redis"]["db"].as<uint32_t>();
      data.redis.password = config["data"]["redis"]["password"].as<std::string>();
      data.redis.dial_timeout = config["data"]["redis"]["dial_timeout"].as<std::string>();
      data.redis.read_timeout = config["data"]["redis"]["read_timeout"].as<std::string>();
      data.redis.write_timeout = config["data"]["redis"]["write_timeout"].as<std::string>();

      return;
    } catch (const std::exception &e) {
      std::cerr << "Error parsing YAML file " << filePath << ": " << e.what() << std::endl;
      exit(-1);
    }
  }
};
}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BOOT_CONF_H_
