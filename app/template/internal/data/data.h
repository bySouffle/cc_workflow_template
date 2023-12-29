//
// Created by souffle on 23-12-19.
//

#ifndef TEMPLATE_APP_TEMPLATE_INTERNAL_DATA_DATA_H_
#define TEMPLATE_APP_TEMPLATE_INTERNAL_DATA_DATA_H_
/*
 *  数据层为biz层提供操作接口
 *  本层提供的对象为非静态对象，因为可能存在多个设备
 *  server层负责数据层对象生命周期管理
 *
*/

#include "template.h"
namespace data {

class Repo {
 public:
  explicit Repo(const BootStrap &conf) {
    template_repo_ = TemplateRepo::NewTemplateRepo(conf);
    template_repo_2 = TemplateRepo::NewTemplateRepo(conf);
  };

  TemplateRepo *template_repo_{};
  TemplateRepo *template_repo_2{};

 protected:
};

inline Repo *NewRepo(const BootStrap &conf) {
  return new Repo(conf);
}

}
#endif //TEMPLATE_APP_TEMPLATE_INTERNAL_DATA_DATA_H_
