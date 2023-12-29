# Service 介绍

* 本层主要描述路由逻辑,不进行复杂的逻辑处理
* 继承biz层的UseCase
* 在server层从下到上初始化data层 基础biz层 Service层
* 在server层完成路由注册