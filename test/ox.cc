#include "ox.h"

#include "gtest/gtest.h"

namespace ox {

class OXTest : public ::testing::Test {};

TEST_F(OXTest, Table) {
  Table t(3, 3);
  ASSERT_EQ(3, t.num_cols());
  ASSERT_EQ(3, t.num_rows());

  ASSERT_EQ(Mark::_, t.Get(0, 0));
  ASSERT_FALSE(t.IsSet(0, 0));

  t.Set(0, 0, Mark::O);
  ASSERT_EQ(Mark::O, t.Get(0, 0));
  ASSERT_TRUE(t.IsSet(0, 0));

  t.Reset();
  ASSERT_EQ(Mark::_, t.Get(0, 0));
  ASSERT_FALSE(t.IsSet(0, 0));

  ASSERT_TRUE(t.IsValidPosition(0, 0));
  ASSERT_FALSE(t.IsValidPosition(0, 3));
}

TEST_F(OXTest, StandardGame) {
  StandardGame game;
  for (const auto &pos : std::vector<std::vector<int>>({
           {1, 1}, {0, 1}, {0, 0}, {2, 1}, {2, 2},
       })) {
    ASSERT_TRUE(game.DoMark(pos[0], pos[1]));
  }
  ASSERT_EQ(StandardGame::State::kResult, game.state());
  ASSERT_EQ(Mark::O, game.winner());

  game.Reset();
  for (const auto &pos : std::vector<std::vector<int>>({
           {1, 1}, {0, 1}, {1, 0}, {2, 1}, {1, 2},
       })) {
    ASSERT_TRUE(game.DoMark(pos[0], pos[1]));
  }
  ASSERT_EQ(StandardGame::State::kResult, game.state());
  ASSERT_EQ(Mark::O, game.winner());

  game.Reset();
  for (const auto &pos : std::vector<std::vector<int>>({
           {1, 1}, {0, 1}, {2, 0}, {2, 1}, {0, 2},
       })) {
    ASSERT_TRUE(game.DoMark(pos[0], pos[1]));
  }
  ASSERT_EQ(StandardGame::State::kResult, game.state());
  ASSERT_EQ(Mark::O, game.winner());
}

}  // namespace ox
