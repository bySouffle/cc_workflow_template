//
// Created by souffle on 23-12-27.
//

#ifndef TEMPLATE_PKG_OTEL_OTEL_H_
#define TEMPLATE_PKG_OTEL_OTEL_H_
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/exporters/otlp/otlp_grpc_exporter_factory.h>
#include <opentelemetry/sdk/common/global_log_handler.h>
#include <opentelemetry/sdk/trace/processor.h>
#include <opentelemetry/sdk/resource/semantic_conventions.h>
#include <opentelemetry/sdk/trace/simple_processor_factory.h>
#include <opentelemetry/sdk/trace/tracer_provider_factory.h>
#include <opentelemetry/trace/provider.h>

namespace trace = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace otlp = opentelemetry::exporter::otlp;
namespace internal_log = opentelemetry::sdk::common::internal_log;
namespace resource = opentelemetry::sdk::resource;

namespace nostd = opentelemetry::nostd;

namespace pkg {
namespace otel {
inline static nostd::shared_ptr<trace::Tracer> get_tracer(std::string_view name) {
  auto provider = trace::Provider::GetTracerProvider();
  return provider->GetTracer(name.data(), OPENTELEMETRY_SDK_VERSION);
}
inline static trace::Scope defer(nostd::shared_ptr<opentelemetry::trace::Span> span) {
  return trace::Scope(span);
}
inline static void CleanupTracer() {
  std::shared_ptr<opentelemetry::trace::TracerProvider> none;
  trace::Provider::SetTracerProvider(none);
}

} // ns pkg
} //ns otel

#endif //TEMPLATE_PKG_OTEL_OTEL_H_
