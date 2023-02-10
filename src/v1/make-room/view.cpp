
#include "view.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/redis/client.hpp>
#include <userver/storages/redis/component.hpp>
#include "../util/move.hpp"
namespace chess_game {
namespace {
namespace uredis = userver::storages::redis;
class MakeRoom final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-make-room";

  MakeRoom(const userver::components::ComponentConfig& config,
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
    auto type = redis_client_->Type(id, redis_cc_).Get();

    if (type != userver::storages::redis::KeyType::kNone) {
      response.SetStatus(userver::server::http::HttpStatus::kForbidden);
      return {};
    }
    redis_client_->Lpush(id, request.GetArg("name"), redis_cc_).Wait();
    return {};
  }
  uredis::ClientPtr redis_client_;
  uredis::CommandControl redis_cc_;
};

}  // namespace
void AppendMakeRoom(userver::components::ComponentList& component_list) {
  component_list.Append<MakeRoom>();
}
}  // namespace chess_game