//
// Created by souffle on 23-12-25.
//

#ifndef TEMPLATE_PKG_OTLP_HTTP_H_
#define TEMPLATE_PKG_OTLP_HTTP_H_
#include <spdlog/fmt/fmt.h>
#include <opentelemetry/exporters/otlp/otlp_http_exporter_factory.h>
#include <opentelemetry/exporters/otlp/otlp_http_exporter_options.h>
#include <opentelemetry/sdk/common/global_log_handler.h>
#include <opentelemetry/sdk/trace/processor.h>
#include <opentelemetry/sdk/trace/simple_processor_factory.h>
#include <opentelemetry/sdk/trace/tracer_provider_factory.h>
#include <opentelemetry/trace/provider.h>

#include <string>

namespace trace = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace otlp = opentelemetry::exporter::otlp;
namespace internal_log = opentelemetry::sdk::common::internal_log;
namespace resource = opentelemetry::sdk::resource;

namespace nostd = opentelemetry::nostd;

namespace pkg::otel {

class Config {
 public:
  std::string ServiceName{};
  std::string LocalAgentHostPort{};
  std::string CollectorEndpoint{};
  std::string Exporter{};
  std::string TraceFilePath{};
  std::string Token{};
};

class HTTPOtel {
 public:
  inline const static std::string Exporter = "otlp.http";

  class Config {
   public:
    std::string ServiceName;
    std::string HostName;
    std::string CollectorEndpoint;
    std::string Exporter;
    std::string TraceFilePath;
    std::string GrpcToken;
  };

  inline static void InitTracer(const Config &c) {
    try {
      opentelemetry::exporter::otlp::OtlpHttpExporterOptions opts{};

      opts.url = c.CollectorEndpoint;


      // 创建OTLP exporter
      auto exporter = otlp::OtlpHttpExporterFactory::Create(opts);
      auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));

      resource::ResourceAttributes attributes = {
          {resource::SemanticConventions::kServiceName, c.ServiceName},
          {resource::SemanticConventions::kHostName, c.HostName}
      };
      auto resource = opentelemetry::sdk::resource::Resource::Create(attributes);

      std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
          trace_sdk::TracerProviderFactory::Create(std::move(processor), resource);

      // 设置全局trace provider
      trace::Provider::SetTracerProvider(provider);
    } catch (const std::exception &e) {
      // 在这里处理异常
      std::cerr << "[otlp] HTTP Caught exception: " << e.what() << std::endl;
      exit(-1);
    }
  }
};
}  // namespace otlp
//  ns pkg

#endif //TEMPLATE_PKG_OTLP_HTTP_H_
