# C++ Workflow Template

## 简介

本项目基于`sogou`[workflow](https://github.com/sogou/workflow)的REST API框架[wfrest](https://github.com/wfrest/wfrest)进行了常用功能集成，包含发现注册服务(consul)，链路追踪(Opentelemetry)
等，采用分层的设计,从而帮助使用者更好的使用workflow项目。

## 环境搭建

* 安装wfrest

```shell
apt-get install build-essential cmake zlib1g-dev libssl-dev libgtest-dev -y
git clone --recursive https://github.com/wfrest/wfrest
cd wfrest/workflow
# workflow
mkdir build && cd build
cmake ..
make -j4
sudo make install 
# wfrest
cd .. && mkdir build && cd build
cmake ..
make -j4
sudo make install 
```

* [安装gRPC](https://grpc.io/docs/languages/cpp/quickstart/)

* 安装 opentelemetry-cpp

```
git clone --recursive https://github.com/open-telemetry/opentelemetry-cpp
cd opentelemetry-cpp
mkdir build && cd build
cmake -DBUILD_TESTING=OFF \
      -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DBUILD_SHARED_LIBS=ON \
      -DWITH_ABSEIL=ON -DWITH_OTLP_GRPC=ON -DWITH_OTLP_HTTP=ON -DWITH_PROMETHEUS=ON  ..
```

* 启动 consul docker

```
docker pull hashicorp/consul
docker run -d --restart=always --name=consul -p 8500:8500 -e CONSUL_BIND_INTER \
       hashicorp/consul agent -server -bootstrap -ui -client='0.0.0.0'

```

* 启动 jaeger docker

```
docker pull jaegertracing/all-in-one:latest
docker run --d --restart=always --name jaeger   -e COLLECTOR_ZIPKIN_HOST_PORT=:9411 \
           -p 6831:6831/udp   -p 6832:6832/udp   -p 5778:5778   -p 16686:16686   -p 4317:4317 \
           -p 4318:4318   -p 14250:14250   -p 14268:14268   -p 14269:14269   -p 9411:9411  \
           jaegertracing/all-in-one
```

## 测试

* 编译运行template

```shell
mkdir build && cd build
cmake ..
make -j4
./template --conf ../app/template/configs/
```

* 访问测试地址
  注意修改配置文件`bootstrap.server.http.addr`地址为网口地址或`docker bridge Gateway`
  地址,`consul`以`docker`环境运行,若使用`localhost`, `consul`健康检查无法访问服务
* 请求路由
  http://127.0.0.1:18000/v1/consul/discovery?ns=&sn=Bys.Template.Service
* 访问jaeger http://127.0.0.1:16686/ `Service`选择`template`,可以看到`template: discovery`的链路追踪
