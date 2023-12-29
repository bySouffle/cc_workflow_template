//
// Created by souffle on 23-12-19.
//

#include "template.h"
#include "template/internal/biz/template.h"
#include "middleware/header_handle.h"
#include "register/consul.h"
#include "response.h"
#include "otel/otel.h"
#include "workflow/ConsulDataTypes.h"

namespace service {

void TemplateSvc::set_bp_template(BluePrint &bp) {
  bp.GET("v1/template/set",
         [this](const HttpReq *req, HttpResp *resp, SeriesWork *series_work) {
           const auto r = req->json<nlohmann::json>();

           biz::Template reqJson{};
           reqJson.from_json(r);

           if (service::TemplateSvc::Create(reqJson)) {
             resp->Json(RESP_OK({}));
           } else {
             resp->Json(RESP_Fail(500, "执行失败", {}));
           }
         });

  bp.GET("v1/template/get",
         [this](const HttpReq *req, HttpResp *resp, SeriesWork *series_work) {
           auto span = pkg::otel::get_tracer("TemplateSvc")->StartSpan("get");
           auto defer = pkg::otel::defer(span);

           const auto r = req->json<nlohmann::json>();

           biz::Template reqJson{};
           reqJson.from_json(r);

           nlohmann::json respJson;
           reqJson.to_json(respJson);

           std::string traceID;
           if (!middleware::Trace::findValue(resp->user_data, "traceID", traceID)) {
             resp->Json(RESP_Fail(500, "traceID 不存在", {}));
             return;
           }

           span->SetAttribute("req", r.dump());

           if (service::TemplateSvc::Query(reqJson, reqJson)) {
             resp->Json(RESP_OK({respJson}));
           } else {
             resp->Json(RESP_Fail(500, "执行失败", {}));
           }
           return;
         });
  bp.GET("v1/health_check",
         [this](const HttpReq *req, HttpResp *resp, SeriesWork *series_work) {
           resp->Json(RESP_OK({}));
         });
  bp.GET("v1/consul/discovery",
         [this](const HttpReq *req, HttpResp *resp, SeriesWork *series_work) {
           auto span = pkg::otel::get_tracer("TemplateSvc")->StartSpan("discovery");
           auto defer = pkg::otel::defer(span);

           std::string ns;
           std::string sn;
           consul::Instances instances;

           if (req->has_query("ns")) {
             ns = req->query("ns");
             span->SetAttribute("ns", ns);
           }
           if (req->has_query("sn")) {
             sn = req->query("sn");
             span->SetAttribute("sn", sn);
           }
           if (!consul::Api::GetInstance().Discovery(ns, sn, instances)) {
             resp->Json(RESP_Fail(500, "查询失败", {}));
             return;
           }
           nlohmann::json j_instances;
           for (auto &instance : instances) {
             nlohmann::json j_instance{
                 {"ID", instance.service.ID},
                 {"Service", instance.service.Service},
                 {"Address", instance.service.Address},
                 {"Port", instance.service.Port},
                 {"http", instance.service.TaggedAddresses.http},
                 {"grpc", instance.service.TaggedAddresses.grpc},
                 {"lan_ipv4", instance.service.TaggedAddresses.lan_ipv4},
                 {"wan_ipv4", instance.service.TaggedAddresses.wan_ipv4},

                 {"Tags", instance.service.Tags},
                 {"Meta", instance.service.Meta},
             };
             j_instances.push_back(j_instance);
           }
           span->SetAttribute("instance", j_instances.dump());

           resp->Json(RESP_OK(j_instances));
         });

  bp.GET("v1/consul/list_services",
         [this](const HttpReq *req, HttpResp *resp, SeriesWork *series_work) {
           std::vector<protocol::ConsulServiceTags> services;
           if (!consul::Api::GetInstance().ListService("", services)) {
             resp->Json(RESP_Fail(500, "查询失败", {}));
             return;
           }
           for (const auto &service : services) {
             spdlog::info(service.service_name);
           }
           resp->Json(RESP_OK({}));
         });
}
}