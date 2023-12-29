//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_BIZ_BIZ_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_BIZ_BIZ_H_

#include "singleton.h"
#include "template/internal/data/data.h"
#include "template/internal/boot/bootstrap.h"
#include "template/internal/boot/registry.h"

using namespace data;

namespace biz {
class UseCaseBase : public Singleton<UseCaseBase> {
  friend class TemplateUseCase;
 public:
  UseCaseBase() = default;
  ~UseCaseBase() override {
    delete repo_;
    repo_ = nullptr;
  };

  void SetRepo(Repo *repo) {
    repo_ = repo;
  }

 protected:
  Repo *repo_ = nullptr;
};

}

#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_BIZ_BIZ_H_
