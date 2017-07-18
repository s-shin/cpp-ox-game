#include <iostream>
#include "cxxopts.hpp"

#include "ox.h"

class CommandReader {
 public:
  bool NextLine(std::istream &is, bool prompt = false) {
    if (prompt) {
      std::cout << "> ";
    }
    std::string line;
    std::getline(is, line);
    ss_.str(line);
    ss_.clear(std::stringstream::goodbit);
    return true;
  }

  bool NextToken(std::string &out) {
    out.clear();
    while (std::getline(ss_, out, ' ')) {
      if (!out.empty()) {
        return true;
      }
    }
    return false;
  }

  bool NextTokenAsInt(int &out) {
    std::string s;
    if (!NextToken(s)) {
      return false;
    }
    out = std::atoi(s.c_str());
    return true;
  }

 private:
  std::stringstream ss_;
};

bool StartOfflineGame() {
  CommandReader r;
  ox::StandardGame game;
  while (r.NextLine(std::cin, true)) {
    std::string cmd;
    if (!r.NextToken(cmd)) {
      continue;
    }

    if (cmd == "mark") {
      if (game.state() != ox::StandardGame::State::kBattle) {
        std::cout << "not in battle." << std::endl;
        continue;
      }
      int x, y;
      if (!r.NextTokenAsInt(x) || !r.NextTokenAsInt(y)) {
        std::cout << "invalid length of arguments for mark" << std::endl;
        continue;
      }
      if (game.DoMark(x, y)) {
        std::cout << ToString(game) << std::endl;
      } else {
        std::cout << "cannot mark to (" << x << ", " << y << ")" << std::endl;
      }
    } else if (cmd == "show") {
      std::cout << ToString(game) << std::endl;
    } else if (cmd == "reset") {
      game.Reset();
      std::cout << ToString(game) << std::endl;
    } else if (cmd == "help") {
      std::cout << "mark <x> <y>" << std::endl
                << "show" << std::endl
                << "reset" << std::endl
                << "help" << std::endl;
    } else {
      std::cout << "unknown command: " << cmd << std::endl;
      continue;
    }
  }
  return true;
}

int main(int argc, char **argv) {
  cxxopts::Options options("ox", "OX game\n");
  options.add_options()("h,help", "Print help text");
  options.parse(argc, argv);
  if (options["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 1;
  }
  return StartOfflineGame() ? 0 : 1;
}
