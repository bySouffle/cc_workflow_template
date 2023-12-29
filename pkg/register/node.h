//
// Created by souffle on 23-12-29.
//

#ifndef TEMPLATE_PKG_REGISTER_NODE_H_
#define TEMPLATE_PKG_REGISTER_NODE_H_
#include <string>
#include <vector>
#include <map>
#include <wfrest/json.hpp>

//  Parse URL
//http://127.0.0.1:8500/v1/health/service/go.Template.Service?dc=&ns=&passing=false&token=&filter=&index=0&wait=5m

namespace consul {

using ConsulAddress = std::pair<std::string, unsigned short>;
//using namespace
struct Node {
  std::string ID;
  std::string Node;
  std::string Address;
  std::string Datacenter;
  struct TaggedAddresses {
    std::string lan;
    std::string lan_ipv4;
    std::string wan;
    std::string wan_ipv4;
  };
  TaggedAddresses TaggedAddresses{};
  struct Meta {
    std::string consul_network_segment;
    std::string consul_version;
  };
  Meta Meta{};
  int CreateIndex{};
  int ModifyIndex{};
};
struct Service {
  std::string ID;
  std::string CreateIndex;
  std::string Service;
  std::vector<std::string> Tags;
  std::string Address;
  struct TaggedAddresses {
    ConsulAddress grpc;
    ConsulAddress http;
    ConsulAddress lan_ipv4;
    ConsulAddress wan_ipv4;
  };
  TaggedAddresses TaggedAddresses;
  std::map<std::string, std::string> Meta;
  int Port{};
  struct Weights {
    int Passing{};
    int Warning{};
  };
  Weights Weights{};

  std::string Namespace;
};

//  TODO 添加check解析
struct Check {

};

struct Instance {
  Node node{};
  Service service{};
  Check check{};

  void node_from_json(const nlohmann::json &j) {
    if (j.contains("Node") && j.at("Node").is_object()) {
      node.ID = j["Node"]["ID"];
      node.Node = j["Node"]["Node"];
      node.Address = j["Node"]["Address"];
      node.Datacenter = j["Node"]["Datacenter"];
      node.TaggedAddresses.lan = j["Node"]["TaggedAddresses"]["lan"];
      node.TaggedAddresses.lan_ipv4 = j["Node"]["TaggedAddresses"]["lan_ipv4"];
      node.TaggedAddresses.wan = j["Node"]["TaggedAddresses"]["wan"];
      node.TaggedAddresses.wan_ipv4 = j["Node"]["TaggedAddresses"]["wan_ipv4"];

      node.Meta.consul_network_segment = j["Node"]["Meta"]["consul-network-segment"];
      node.Meta.consul_version = j["Node"]["Meta"]["consul-version"];
      node.CreateIndex = j["Node"]["CreateIndex"];
      node.ModifyIndex = j["Node"]["ModifyIndex"];
    }
    if (j.contains("Service") && j.at("Service").is_object()) {
      service.ID = j["Service"]["ID"];
      service.Service = j["Service"]["Service"];
      service.Address = j["Service"]["Address"];
      if (j["Service"].contains("Tags")) {
        for (auto &tag : j["Service"]["Tags"]) {
          service.Tags.emplace_back(tag);
        }
      }

      if (j["Service"]["TaggedAddresses"].contains("grpc")) {
        service.TaggedAddresses.grpc.first = j["Service"]["TaggedAddresses"]["grpc"]["Address"];
        service.TaggedAddresses.grpc.second = j["Service"]["TaggedAddresses"]["grpc"]["Port"];
      }
      if (j["Service"]["TaggedAddresses"].contains("http")) {
        service.TaggedAddresses.http.first = j["Service"]["TaggedAddresses"]["http"]["Address"];
        service.TaggedAddresses.http.second = j["Service"]["TaggedAddresses"]["http"]["Port"];
      }
      if (j["Service"]["TaggedAddresses"].contains("lan_ipv4")) {
        service.TaggedAddresses.lan_ipv4.first = j["Service"]["TaggedAddresses"]["lan_ipv4"]["Address"];
        service.TaggedAddresses.lan_ipv4.second = j["Service"]["TaggedAddresses"]["lan_ipv4"]["Port"];
      }
      if (j["Service"]["TaggedAddresses"].contains("wan_ipv4")) {
        service.TaggedAddresses.wan_ipv4.first = j["Service"]["TaggedAddresses"]["wan_ipv4"]["Address"];
        service.TaggedAddresses.wan_ipv4.second = j["Service"]["TaggedAddresses"]["wan_ipv4"]["Port"];
      }

      if (j["Service"]["Meta"].size() > 0) {
        service.Meta = j["Service"]["Meta"].get<std::map<std::string, std::string>>();
      }
      service.Port = j["Service"]["Port"];

      service.Weights.Passing = j["Service"]["Weights"]["Passing"];
      service.Weights.Warning = j["Service"]["Weights"]["Warning"];

      //  Notice: Current Namespace no exist
//        service.Namespace = j["Service"]["Namespace"];
    }
  }
};

using Instances = std::vector<Instance>;

};

#endif //TEMPLATE_PKG_REGISTER_NODE_H_
