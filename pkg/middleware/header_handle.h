//
// Created by souffle on 23-12-27.
//

#ifndef TEMPLATE_PKG_MIDDLEWARE_HEAD_HANDLE_H_
#define TEMPLATE_PKG_MIDDLEWARE_HEAD_HANDLE_H_
#include <wfrest/HttpServer.h>
#include <wfrest/Aspect.h>
using namespace wfrest;

using kv = std::map<std::string, std::string>;

namespace middleware {

// Logging aspect
struct Trace : public Aspect {
  bool before(const HttpReq *req, HttpResp *resp) override {
    auto *ctx = new kv;
    if (req->has_header("traceID")) {
      ctx->insert(std::make_pair("traceID", req->header("traceID")));
    }
    resp->user_data = ctx;
    return true;
  }

  // 'after()' should be called after reply
  bool after(const HttpReq *req, HttpResp *resp) override {
    delete static_cast<kv *>(resp->user_data);
    return true;
  }

  static bool findValue(void *context, std::string_view key, std::string &value) {
    auto *ctx = static_cast<kv *>(context);
    auto v = ctx->find(key.data());
    if (v != ctx->end()) {
      value = v->second;
      return true;
    } else {
      return false;
    }
  }

};
}

#endif //TEMPLATE_PKG_MIDDLEWARE_HEAD_HANDLE_H_
