
#include "view.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/redis/client.hpp>
#include <userver/storages/redis/component.hpp>
#include "../util/move.hpp"
namespace chess_game {
namespace {
namespace uredis = userver::storages::redis;
class MakeMove final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-make-move";

  MakeMove(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_contex)
      : userver::server::handlers::HttpHandlerBase(config, component_contex),
        redis_client_{
            component_contex
                .FindComponent<userver::components::Redis>("key-value-database")
                .GetClient("taxi-tmp")} {}
  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto id = GameId(request);
    auto& response = request.GetHttpResponse();
    auto type = redis_client_->Type(id, redis_cc_).Get();

    if (type != userver::storages::redis::KeyType::kList) {
      response.SetStatus(userver::server::http::HttpStatus::kNotFound);
      return {};
    }
    auto name = request.GetHeader("name");
    auto move_count = redis_client_->Llen(id, redis_cc_).Get();
    auto whites_name =
        redis_client_->Lindex(id + "-players", 0, redis_cc_).Get().value();
    if ((whites_name == name) == (move_count % 2 == 1)) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return {};
    }
    auto body = userver::formats::json::FromString(request.RequestBody());
    auto move = chess_game::Move{body["previous"].As<std::string>(),
                                 body["next"].As<std::string>()};
    redis_client_->Lpush(id, move.Representation(), redis_cc_).Wait();
    return "OK";
  }
  uredis::ClientPtr redis_client_;
  uredis::CommandControl redis_cc_;
};

}  // namespace
void AppendMakeMove(userver::components::ComponentList& component_list) {
  component_list.Append<MakeMove>();
}
}  // namespace chess_game