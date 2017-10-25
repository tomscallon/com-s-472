#include "AlphaBetaPlayer.hpp"

#include "Game.hpp"
#include <map>
#include <string>
#include <vector>

AlphaBetaPlayer::AlphaBetaPlayer(
  std::string name,
  unsigned int depth,
  AlphaBetaPlayer::EvalFn eval
) : Player(name), depth(depth), eval(eval) {}

double AlphaBetaPlayer::maxValue(
  const Game& game,
  double a,
  double b,
  unsigned int d
) const {
  if (d == 0 || game.movesetForCurrentPlayer().empty()) {
    return this->eval(game);
  }

  double value = MIN_DOUBLE;
  for (Game::Location l : game.movesetForCurrentPlayer()) {
    value = std::max(value, this->minValue(game + l, a, b, d - 1));
    if (value >= b) {
      return value;
    }
    a = std::max(a, value);
  }

  return value;
}

double AlphaBetaPlayer::minValue(
  const Game& game,
  double a,
  double b,
  unsigned int d
) const {
  if (d == 0 || game.movesetForCurrentPlayer().empty()) {
    return this->eval(game);
  }

  double value = MAX_DOUBLE;
  for (Game::Location l : game.movesetForCurrentPlayer()) {
    value = std::min(value, this->maxValue(game + l, a, b, d - 1));
    if (value <= a) {
      return value;
    }
    b = std::min(b, value);
  }

  return value;
}

Game::Location AlphaBetaPlayer::getNextMove(const Game& game) const {
  Game::Location result;
  double resultValue = MIN_DOUBLE;
  for (Game::Location l : game.movesetForCurrentPlayer()) {
    double value = this->minValue(
      game + l,
      MIN_DOUBLE,
      MAX_DOUBLE,
      this->depth
    );
    if (value > resultValue) {
      result = l;
      resultValue = value;
    }
  }

  return result;
}

// Simple heuristic. Counts how many cells are captured by the active player.
AlphaBetaPlayer* AlphaBetaPlayer::evalFnA(
  std::string name,
  bool forPlayer,
  unsigned int depth
) {
  Game::Space pro = forPlayer == Game::P_WHITE ? Game::WHITE : Game::BLACK;

  return new AlphaBetaPlayer(name, depth, [pro](const Game& game) {
    return game.count(pro);
  });
}

AlphaBetaPlayer* AlphaBetaPlayer::evalFnB(
  std::string name,
  bool forPlayer,
  unsigned int depth
) {
  Game::Space pro = forPlayer == Game::P_WHITE ? Game::WHITE : Game::BLACK;
  Game::Space ant = forPlayer == Game::P_WHITE ? Game::BLACK : Game::WHITE;

  return new AlphaBetaPlayer(name, depth, [pro, ant](const Game& game) {
    int result = game.count(pro);

    int CORNER_BONUS = 4;
    int CORNER_ADJ_PENALTY = 2;

    std::map<Game::Location, std::vector<Game::Direction>> corners = {
      {{0,0}, {Game::E, Game::SE, Game::S}},
      {{0,7}, {Game::W, Game::SW, Game::S}},
      {{7,7}, {Game::W, Game::NW, Game::N}},
      {{7,0}, {Game::E, Game::NE, Game::N}},
    };

    for (auto pair : corners) {
      auto corner = pair.first;
      auto cornerSpace = game[corner];
      auto adjacent = pair.second;

      if (cornerSpace == Game::EMPTY) {
        // corner is empty
        // for each adjacent space, add or subtract
        // based on contents
        for (auto dir : adjacent) {
          auto loc = corner + dir;
          if (game[loc] == pro) {
            // Subtract if player is in adjacent spots (bad)
            result -= CORNER_ADJ_PENALTY;
          } else if (game[loc] == ant) {
            // Add if opponent is in adjacent spots (good)
            result += CORNER_ADJ_PENALTY;
          }
        }
      } else if(cornerSpace == pro) {
        // player has corner
        result += CORNER_BONUS;
      } else {
        // opponent has corner
        result -= CORNER_BONUS;
      }
    }

    return result;
  });
}
