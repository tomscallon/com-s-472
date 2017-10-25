#ifndef ALPHA_BETA_PLAYER_H
#define ALPHA_BETA_PLAYER_H

#include "Player.hpp"
#include "Game.hpp"
#include <functional>
#include <limits>
#include <string>

class AlphaBetaPlayer : public Player {
private:
  using EvalFn = std::function<double(const Game&)>;

  unsigned int depth;
  EvalFn eval;

  static constexpr double MIN_DOUBLE = std::numeric_limits<double>::lowest();
  static constexpr double MAX_DOUBLE = std::numeric_limits<double>::max();

  double maxValue(const Game& game, double a, double b, unsigned int d) const;
  double minValue(const Game& game, double a, double b, unsigned int d) const;

public:
  AlphaBetaPlayer(std::string name, unsigned int depth, EvalFn eval);

  virtual Game::Location getNextMove(const Game& g) const override;

  static AlphaBetaPlayer* evalFnA(
    std::string name,
    bool forPlayer,
    unsigned int depth = 2
  );

  static AlphaBetaPlayer* evalFnB(
    std::string name,
    bool forPlayer,
    unsigned int depth = 2
  );
};

#endif
