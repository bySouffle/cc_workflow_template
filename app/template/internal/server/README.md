# server 介绍

* 本层主要对HTTP服务初始化以及管理整个服务的生命周期
* `InitDataLayer` 初始化数据层
* `InitBizLayer`  初始化biz层
* `InitSvcLayer`  初始化service层
* `InitSrvLayer`  初始化server层
* `Start` 同步启动
* `StartASync` 异步启动
* `Stop` 停止服务