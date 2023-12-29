# opentelemetry-cpp

## gRPC

### Install

[步骤](https://grpc.io/docs/languages/cpp/quickstart/)

## Building

Get the opentelemetry-cpp source:

```
git clone --recursive https://github.com/open-telemetry/opentelemetry-cpp
```

Navigate to the repository cloned above, and create the CMake build configuration:

```
cd opentelemetry-cpp
mkdir build && cd build
cmake -DBUILD_TESTING=OFF \
-DCMAKE_POSITION_INDEPENDENT_CODE=ON -DBUILD_SHARED_LIBS=ON \
-DWITH_ABSEIL=ON -DWITH_OTLP_GRPC=ON -DWITH_OTLP_HTTP=ON -DWITH_PROMETHEUS=ON  ..
```

Once build configuration is created, build the CMake targets http_client and http_server:

```
cmake --build . --target http_client http_server
```

https://help.aliyun.com/zh/sls/user-guide/import-trace-data-from-cpp-applications-to-log-service?spm=a2c4g.11186623.0.0.558c5d81Op6Dh4
https://opentelemetry-cpp.readthedocs.io/en/v1.2.0/sdk/GettingStarted.html#tracerprovider
https://www.alibabacloud.com/help/zh/opentelemetry/user-guide/use-managed-service-for-opentelemetry-to-report-cpp-application-data#p-fnl-5m8-i4z