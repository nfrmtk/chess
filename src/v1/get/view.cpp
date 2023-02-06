
#include "view.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/redis/client.hpp>
#include <userver/storages/redis/component.hpp>

namespace chess_game {
namespace {
namespace urdis = userver::storages::redis;
class GetValue final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-value";

  GetValue(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_contex)
      : userver::server::handlers::HttpHandlerBase(config, component_contex),
        redis_client_{
            component_contex
                .FindComponent<userver::components::Redis>("key-value-database")
                .GetClient("taxi-tmp")} {}
  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto response = redis_client_->Get("answer", redis_cc_).Get();
    return response.has_value()
                         ? response.value()
                         : "Not found";
  }
  urdis::ClientPtr redis_client_;
  urdis::CommandControl redis_cc_;
};

}  // namespace

void AppendGetValue(userver::components::ComponentList& component_list) {
  component_list.Append<GetValue>();
}
}  // namespace chess_game
