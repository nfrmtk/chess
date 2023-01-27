#include "hello.hpp"

#include <userver/utest/utest.hpp>

UTEST(SayHelloTo, Basic) {
  EXPECT_EQ(chess_game::SayHelloTo("Developer"), "Hello, Developer!\n");
  EXPECT_EQ(chess_game::SayHelloTo({}), "Hello, unknown user!\n");
}
