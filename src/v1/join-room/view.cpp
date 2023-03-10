
#include "view.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/redis/client.hpp>
#include <userver/storages/redis/component.hpp>
#include "../util/move.hpp"
namespace chess_game {
namespace {
namespace uredis = userver::storages::redis;
class JoinRoom final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-join-room";

  JoinRoom(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_contex)
      : userver::server::handlers::HttpHandlerBase(config, component_contex),
        redis_client_{
            component_contex
                .FindComponent<userver::components::Redis>("key-value-database")
                .GetClient("taxi-tmp")} {}
  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto id = PlayersId(request);
    auto& response = request.GetHttpResponse();
    auto len = redis_client_->Llen(id, redis_cc_).Get();
    if (len != 1) {
      response.SetStatus(userver::server::http::HttpStatus::kForbidden);
      return {};
    }
    auto blacks_name = request.GetArg("name");
    auto whites_name = redis_client_->Lindex(id, 0, redis_cc_).Get().value();
    if (whites_name == blacks_name) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return {};
    }
    redis_client_->Rpush(id, blacks_name, redis_cc_).Wait();
    return {};
  }
  uredis::ClientPtr redis_client_;
  uredis::CommandControl redis_cc_;
};

}  // namespace
void AppendJoinRoom(userver::components::ComponentList& component_list) {
  component_list.Append<JoinRoom>();
}
}  // namespace chess_game