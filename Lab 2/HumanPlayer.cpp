#include "HumanPlayer.hpp"

#include "Game.hpp"
#include <string>
#include <iostream>

const std::string HumanPlayer::LABELS = "abcdefghijklmnopqrstuvwxyz";

HumanPlayer::HumanPlayer(std::string name) : Player(name) {}

Game::Location HumanPlayer::getNextMove(const Game& g) const {
  auto moveSet = g.movesetForCurrentPlayer();

  // Create the board, marked with possible moves.
  std::string boardWithChoices = g.boardAsString([&moveSet](auto s, auto l) {
    int index = std::find(moveSet.begin(), moveSet.end(), l) - moveSet.begin();

    if (index < moveSet.size()) {
      return std::string(1, LABELS[index]);
    } else {
      return Game::displayCharFor(s);
    }
  });

  // Create the prompt for the player.
  std::string prompt = "Possible moves indicated on the grid above.";

  // For now just print.
  int decision = -1;
  std::string previousError = "";

  while (decision == -1) {
    std::string line;

    std::cout << "\n" << boardWithChoices << "\n\n" << prompt << std::endl;
    std::cout << previousError << std::endl;
    std::cout << "What's your move? ";
    getline(std::cin, line);

    if (line.length() != 1) {
      previousError = "Enter a single letter.";
    } else {
      int pos = LABELS.find(line);

      if (pos == std::string::npos || pos >= moveSet.size()) {
        previousError = "Enter a letter corresponding to one of the given options.";
      } else {
        decision = pos;
      }
    }
  }

  return moveSet[decision];
}
