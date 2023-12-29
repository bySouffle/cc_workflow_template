//
// Created by souffle on 23-12-21.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BIZ_TEMPLATE_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BIZ_TEMPLATE_H_
#include <wfrest/json.hpp>
#include "biz.h"
#include "otel/otel.h"

using namespace data;
using namespace nlohmann;

namespace biz {

#define repo (GetInstance().repo_)

struct Template {
  int64_t id;
  std::string Name;
  std::string Mobile;
  std::string Address;
  std::string PostCode;

  void to_json(nlohmann::json &j) {
    j["id"] = id;
    if (!Name.empty()) {
      j["Name"] = Name;
    }
    if (!Mobile.empty()) {
      j["Mobile"] = Mobile;
    }
    if (!Address.empty()) {
      j["Address"] = Address;
    }
    if (!PostCode.empty())
      j["PostCode"] = PostCode;
  }

  void from_json(const nlohmann::json &j) {
    if (j.contains("id") && j.at("id").is_number()) {
      j.at("id").get_to(id);
    }
    if (j.contains("Name") && j.at("Name").is_string()) {
      j.at("Name").get_to(Name);
    }
    if (j.contains("Mobile") && j.at("Mobile").is_string()) {
      j.at("Mobile").get_to(Mobile);
    }
    if (j.contains("Address") && j.at("Address").is_string()) {
      j.at("Address").get_to(Address);
    }
    if (j.contains("PostCode") && j.at("PostCode").is_string()) {
      j.at("PostCode").get_to(PostCode);
    }
  }
};

class TemplateUseCase : public UseCaseBase {
 public:
  TemplateUseCase() = default;
  ~TemplateUseCase() override = default;

  static bool Create(const Template &t) {
    return repo->template_repo_->CreateTemplate(
        TemplateRepo::TemplateReq{
            .key = t.Name,
            .keyID = static_cast<int>(t.id),
            .value = t.Mobile,
        });
  }
  static bool Query(const Template &req, Template &resp) {
    auto span = pkg::otel::get_tracer("TemplateUseCase")->StartSpan("Query");
    auto defer = pkg::otel::defer(span);

    TemplateRepo::TemplateResp template_resp{};
    bool ok = repo->template_repo_->QueryTemplate(
        TemplateRepo::TemplateReq{
            .key = req.Name,
            .keyID = static_cast<int>(req.id),
        }, template_resp);
    resp.Mobile = template_resp.value;

    span->SetAttribute("status", ok);

    return ok;
  }

};

}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BIZ_TEMPLATE_H_
