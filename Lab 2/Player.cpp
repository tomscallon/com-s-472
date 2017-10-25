#include "Player.hpp"

#include <string>

Player::Player(std::string name) : name(name) {}

Player::~Player() {}

std::string Player::getName() const {
  return this->name;
}
