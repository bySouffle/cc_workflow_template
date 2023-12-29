//
// Created by souffle on 23-12-20.
//

#ifndef TEMPLATE_PKG_SINGLETON_SINGLETON_H_
#define TEMPLATE_PKG_SINGLETON_SINGLETON_H_

template<typename T>
class Singleton {
 public:
  static T &GetInstance() {
    static T instance;
    return instance;
  }

  Singleton(T &&) = delete;
  Singleton(const T &) = delete;
  void operator=(const T &) = delete;

 protected:
  Singleton() = default;
  virtual ~Singleton() = default;
};

#endif //TEMPLATE_PKG_SINGLETON_SINGLETON_H_
