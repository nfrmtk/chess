
#include "move.hpp"
#include <stdexcept>
std::string chess_game::Move::Representation() const {
  std::string ans;
  ans += previous;
  ans += next;
  return ans;
}
chess_game::Move::Move(const std::string& alternative_representation) {
  previous = alternative_representation[0];
  next = alternative_representation[1];
}
chess_game::Move::Move(const std::string& previous, const std::string& next) {
  if (previous.size() != 2) {
    throw std::invalid_argument("previous has bad size");
  }
  if (next.size() != 2) {
    throw std::invalid_argument("next has bad size");
  }
  this->previous = (previous[0] - 'A') * 8 + (previous[1] - '1');
  this->next = (next[0] - 'A') * 8 + (next[1] - '1');
}
std::string chess_game::GameId(
    const userver::server::http::HttpRequest& request) {
  return request.GetPathArg("game-id");
}
std::string chess_game::PlayersId(
    const userver::server::http::HttpRequest& request) {
  return request.GetPathArg("game-id") + "-players";
}
