#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/redis/component.hpp>
#include <userver/storages/secdist/component.hpp>
#include <userver/storages/secdist/provider_component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "hello.hpp"
#include "v1/make-a-move/view.hpp"
#include "v1/get-last-move/view.hpp"
#include "v1/make-room/view.hpp"
#include "v1/join-room/view.hpp"
int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::Redis>("key-value-database")
          .Append<userver::components::Secdist>()
          .Append<userver::components::DefaultSecdistProvider>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>();

  chess_game::AppendHello(component_list);
  chess_game::AppendMakeMove(component_list);
  chess_game::AppendGetLastMove(component_list);
  chess_game::AppendJoinRoom(component_list);
  chess_game::AppendMakeRoom(component_list);
  return userver::utils::DaemonMain(argc, argv, component_list);
}
