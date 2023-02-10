#include <string>
#include <userver/server/http/http_request.hpp>
namespace chess_game {

class Move {
 public:
  [[nodiscard]] std::string Representation() const;
  explicit Move(const std::string& alternative_representation);
  Move(const std::string& previous, const std::string& next);

 private:
  std::uint8_t next;
  std::uint8_t previous;
};

std::string GameId(const userver::server::http::HttpRequest& request);
std::string PlayersId(const userver::server::http::HttpRequest& request);
}  // namespace chess_game