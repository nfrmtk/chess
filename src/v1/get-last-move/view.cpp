
#include "view.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/redis/client.hpp>
#include <userver/storages/redis/component.hpp>
#include "../util/move.hpp"
namespace chess_game {
namespace {
namespace uredis = userver::storages::redis;
class LastMove final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-last-move";

  LastMove(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_contex)
      : userver::server::handlers::HttpHandlerBase(config, component_contex),
        redis_client_{
            component_contex
                .FindComponent<userver::components::Redis>("key-value-database")
                .GetClient("taxi-tmp")} {}
  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto& response = request.GetHttpResponse();
    auto id = GameId(request);
    if (redis_client_->Type(id, redis_cc_).Get() != userver::storages::redis::KeyType::kList) {
      response.SetStatus(userver::server::http::HttpStatus::kNotFound);
      return {};
    }
    auto move = redis_client_->Lindex(id, 0, redis_cc_).Get().value();
    userver::formats::json::ValueBuilder builder;
    builder["last-move"] = move;
    return userver::formats::json::ToString(builder.ExtractValue());
  }
  uredis::ClientPtr redis_client_;
  uredis::CommandControl redis_cc_;
};

}  // namespace
void AppendGetLastMove(userver::components::ComponentList& component_list) {
  component_list.Append<LastMove>();
}
}  // namespace chess_game