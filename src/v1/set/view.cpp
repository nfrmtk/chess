
#include "view.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/redis/client.hpp>
#include <userver/storages/redis/component.hpp>

namespace chess_game {
namespace {
namespace uredis = userver::storages::redis;
class SetValue final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-set-value";

  SetValue(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_contex)
      : userver::server::handlers::HttpHandlerBase(config, component_contex),
        redis_client_{
            component_contex
                .FindComponent<userver::components::Redis>("key-value-database")
                .GetClient("taxi-tmp")} {}
  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    redis_client_->Set("answer", "42", redis_cc_).Wait();
    return "OK";
  }
  uredis::ClientPtr redis_client_;
  uredis::CommandControl redis_cc_;
};

}  // namespace
void AppendSetValue(userver::components::ComponentList& component_list) {
  component_list.Append<SetValue>();
}
}  // namespace chess_game