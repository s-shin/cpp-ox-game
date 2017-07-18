#ifndef OX_H_
#define OX_H_

#include <cstdlib>
#include <sstream>
#include <vector>

namespace ox {

enum class Mark { _, O, X };

const char* ToString(Mark m) {
  switch (m) {
    case Mark::_: {
      return " ";
    }
    case Mark::O: {
      return "o";
    }
    case Mark::X: {
      return "x";
    }
  }
}

class Table {
 public:
  Table(int num_cols, int num_rows)
      : num_cols_(num_cols),
        num_rows_(num_rows),
        table_(num_cols * num_rows, Mark::_) {}

  Mark Get(int x, int y) const { return table_.at(Index(x, y)); }
  void Set(int x, int y, Mark m) { table_.at(Index(x, y)) = m; }

  bool IsSet(int x, int y) const { return Get(x, y) != Mark::_; }

  bool IsValidPosition(int x, int y) const {
    return 0 <= x && x < num_cols_ && 0 <= y && y < num_rows_;
  }

  bool IsFilled() const {
    for (int y = 0; y < num_rows_; ++y) {
      for (int x = 0; x < num_cols_; ++x) {
        if (Get(x, y) == Mark::_) {
          return false;
        }
      }
    }
    return true;
  }

  void Reset() {
    std::fill(table_.begin(), table_.end(), Mark::_);
  }

  int num_cols() const { return num_cols_; }
  int num_rows() const { return num_rows_; }

 private:
  int Index(int x, int y) const { return x + y * num_cols_; }

  int num_cols_;
  int num_rows_;
  std::vector<Mark> table_;
};

std::string ToString(const Table& t) {
  std::stringstream ss;
  for (int y = 0; y < t.num_rows(); ++y) {
    for (int x = 0; x < t.num_cols(); ++x) {
      auto s = ::ox::ToString(t.Get(x, y));
      if (x == 0) {
        ss << " " << s;
      } else {
        ss << " | " << s;
      }
    }
    if (y < t.num_rows() - 1) {
      ss << std::endl;
      for (int x = 0; x < t.num_cols(); ++x) {
        if (x == 0) {
          ss << "---";
        } else {
          ss << "+---";
        }
      }
      ss << std::endl;
    }
  }
  return ss.str();
}

class StandardGame {
 public:
  static const int kTableSize = 3;

  enum class State {
    kBattle,
    kResult,
  };

  StandardGame()
      : table_(kTableSize, kTableSize),
        turn_(Mark::O),
        winner_(Mark::_),
        state_(State::kBattle) {}

  void Reset() {
    table_.Reset();
    turn_ = Mark::O;
    winner_ = Mark::_;
    state_ = State::kBattle;
  }

  bool DoMark(int x, int y) {
    if (state_ != State::kBattle) {
      return false;
    }

    if (!table_.IsValidPosition(x, y) || table_.IsSet(x, y)) {
      return false;
    }
    table_.Set(x, y, turn_);
    ToggleTurn();

    winner_ = CheckVictory();
    if (winner_ != Mark::_ || table_.IsFilled()) {
      state_ = State::kResult;
    }

    return true;
  }

  const Table& table() const { return table_; }
  Mark turn() const { return turn_; }
  State state() const { return state_; }

  /**
   * Available if state is State::kResult.
   * Mark::_ means draw.
   */
  Mark winner() const { return winner_; }

 private:
  void ToggleTurn() {
    switch (turn_) {
      case Mark::O: {
        turn_ = Mark::X;
        break;
      }
      case Mark::X: {
        turn_ = Mark::O;
        break;
      }
      default: { std::abort(); }
    }
  }

  Mark CheckVictory() {
    for (int y = 0; y < kTableSize; ++y) {
      for (int x = 0; x < kTableSize; ++x) {
        Mark mark = table_.Get(x, y);
        if (mark == Mark::_) {
          continue;
        }
        // vertical
        {
          int count = 1;
          count += CheckVictoryRec(x, y, 0, -1, mark, 0);
          count += CheckVictoryRec(x, y, 0, 1, mark, 0);
          if (count == kTableSize) {
            return mark;
          }
        }
        // left-up to right-down
        {
          int count = 1;
          count += CheckVictoryRec(x, y, -1, -1, mark, 0);
          count += CheckVictoryRec(x, y, 1, 1, mark, 0);
          if (count == kTableSize) {
            return mark;
          }
        }
        // horizontal
        {
          int count = 1;
          count += CheckVictoryRec(x, y, -1, 0, mark, 0);
          count += CheckVictoryRec(x, y, 1, 0, mark, 0);
          if (count == kTableSize) {
            return mark;
          }
        }
        // right-up to left-down
        {
          int count = 1;
          count += CheckVictoryRec(x, y, 1, -1, mark, 0);
          count += CheckVictoryRec(x, y, -1, 1, mark, 0);
          if (count == kTableSize) {
            return mark;
          }
        }
      }
    }
    return Mark::_;
  }

  int CheckVictoryRec(int x, int y, int dx, int dy, Mark mark, int count) {
    int nx = x + dx;
    int ny = y + dy;
    if (!table_.IsValidPosition(nx, ny)) {
      return count;
    }
    if (table_.Get(nx, ny) != mark) {
      return count;
    }
    return CheckVictoryRec(nx, ny, dx, dy, mark, ++count);
  }

  Table table_;
  Mark turn_;
  Mark winner_;
  State state_;
};

std::string ToString(const StandardGame& game) {
  std::stringstream ss;
  ss << ToString(game.table()) << std::endl;
  switch (game.state()) {
    case StandardGame::State::kBattle: {
      ss << "Next: " << ox::ToString(game.turn());
      break;
    }
    case StandardGame::State::kResult: {
      Mark winner = game.winner();
      if (winner == Mark::_) {
        ss << "Draw!";
      } else {
        ss << ToString(game.winner()) << " wins!";
      }
      break;
    }
  }
  return ss.str();
}

}  // namespace ox

#endif  // OX_H_
