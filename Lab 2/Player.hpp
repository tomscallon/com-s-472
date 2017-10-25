#ifndef PLAYER_H
#define PLAYER_H

#include "Game.hpp"

#include <string>

class Player {
protected:
  std::string name;

public:
  Player(std::string name);
  virtual ~Player();

  std::string getName() const;

  virtual Game::Location getNextMove(const Game& g) const = 0;
};

#endif
