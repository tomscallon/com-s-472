#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "Player.hpp"
#include "Game.hpp"
#include <string>

class HumanPlayer : public Player {
private:
  static const std::string LABELS;

public:
  HumanPlayer(std::string name);

  virtual Game::Location getNextMove(const Game& g) const override;
};

#endif
