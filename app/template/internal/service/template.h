//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_SERVICE_TEMPLATE_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_SERVICE_TEMPLATE_H_

#include <wfrest/BluePrint.h>
#include <wfrest/HttpServer.h>
#include "template/internal/biz/template.h"

namespace service {
using namespace wfrest;
using namespace biz;

//! 服务层
//! 1. 继承biz层的UseCase
//! 2. 在Server层从下到上初始化data层 基础biz层 Service层
//! 3. 在server层完成路由注册
class TemplateSvc : public TemplateUseCase {
 public:
  TemplateSvc() = default;
  ~TemplateSvc() override = default;
  void set_bp_template(BluePrint &bp);
  inline void registerSvc(HttpServer &server) {
    BluePrint bp;
    set_bp_template(bp);
    server.register_blueprint(bp, "");
  }
 private:
};

inline TemplateSvc *NewTemplateSvc() {
  return new TemplateSvc();
}
}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_SERVICE_TEMPLATE_H_
