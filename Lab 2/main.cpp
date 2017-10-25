#include "Game.hpp"
#include "HumanPlayer.hpp"
#include "AlphaBetaPlayer.hpp"

#include <iostream>
#include <set>
#include <sstream>

int readLineAsInt();
Player* determinePlayer(bool);
int runGame(Player*, Player*);
std::array<std::array<short, 6>, 6> testFnA();
std::array<std::array<short, 6>, 6> testFnAvB();

int main(int argc, char** argv) {
  Player* white = determinePlayer(Game::P_WHITE);
  Player* black = determinePlayer(Game::P_BLACK);

  runGame(white, black);

  delete white;
  delete black;

  /*auto results = testFnA();

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      std::cout << (i+1) << "/" << (j+1) << " = " << results[i][j] << std::endl;
    }
  }*/

  /*auto results = testFnAvB();

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      std::cout << (i+1) << "/" << (j+1) << " = " << results[i][j] << std::endl;
    }
  }

  return 0;*/
}

std::array<std::array<short, 6>, 6> testFnA() {
  std::array<std::array<short, 6>, 6> results;

  Player* w;
  Player* b;
  for (unsigned int i = 0; i < 6; i++) {
    for (unsigned int j = 0; j < 6; j++) {
      std::cout << "Testing depths " << i + 1 << " / " << j + 1 << std::endl;
      results[i][j] = runGame(
        w = AlphaBetaPlayer::evalFnA("White", Game::P_WHITE, i + 1),
        b = AlphaBetaPlayer::evalFnA("Black", Game::P_BLACK, j + 1)
      );
      delete b;
      delete w;
    }
  }

  return results;
}

std::array<std::array<short, 6>, 6> testFnAvB() {
  std::array<std::array<short, 6>, 6> results;

  Player* w;
  Player* b;
  for (unsigned int i = 0; i < 6; i++) {
    for (unsigned int j = 0; j < 6; j++) {
      std::cout << "Testing depths " << i + 1 << " / " << j + 1 << std::endl;
      results[i][j] = runGame(
        w = AlphaBetaPlayer::evalFnA("White", Game::P_WHITE, i + 1),
        b = AlphaBetaPlayer::evalFnB("Black", Game::P_BLACK, j + 1)
      );
      delete b;
      delete w;
    }
  }

  return results;
}

int readLineAsInt() {
  std::string line;
  getline(std::cin, line);
  return std::stoi(line);
}

Player* determinePlayer(bool player) {
  std::stringstream namePromptStream;
  namePromptStream << "\nWhat's "
                   << (player == Game::P_WHITE ? "WHITE" : "BLACK")
                   << "'s name?";
  std::string namePrompt = namePromptStream.str();
  std::string name = "";

  while (name.empty()) {
    std::cout << namePrompt << std::endl;
    getline(std::cin, name);
  }

  std::stringstream promptStream;
  promptStream << "\nWhat type of player should "
               << (player == Game::P_WHITE ? "WHITE" : "BLACK")
               << " be?\n"
               << "  1. Human\n"
               << "  2. CPU - Alpha-Beta (evaluation 1)\n"
               << "  3. CPU - Alpha-Beta (evaluation 2)\n";
  std::string prompt = promptStream.str();

  int which = -1;
  do {
    std::cout << prompt << std::endl;
    try {
      which = readLineAsInt();
    } catch (...) {}

    if (which < 1 || which > 3) {
      std::cout << "Type a number between 1 and 3, inclusive." << std::endl;
    }
  } while (which < 1 || which > 3);

  int depth = 0;

  if (which == 2 || which == 3) {
    while (depth <= 0 || depth > 10) {
      std::cout << "\nHow deep should the agent search?\n"
                << "Enter an integer from 1 - 10." << std::endl;

      try {
        depth = readLineAsInt();
      } catch(...) {}

      std::cout << "Type a number between 1 and 10, inclusive." << std::endl;
    }
  }

  switch (which) {
    case 1:
      return new HumanPlayer(name);
    case 2:
      return AlphaBetaPlayer::evalFnA(name, player, depth);
    case 3:
      return AlphaBetaPlayer::evalFnB(name, player, depth);
    default:
      return nullptr;
  }
}

int runGame(Player* white, Player* black) {
  Game g(white, black);

  while (!g.isFinished()) {
    std::cout
      << "\n==========================================\n"
      << g.getCurrentPlayer()->getName() << "'s Turn\n"
      << "==========================================" << std::endl;

    if (g.movesetForCurrentPlayer().empty()) {
      std::cout << "There are no possible moves, so "
                << g.getCurrentPlayer()->getName()
                << " must pass." << std::endl;
    }

    g.nextTurn();
    std::cout << "\n" << g.boardAsString() << std::endl;
  }

  std::string winner = g.isTie()
    ? "Tie Game!"
    : g.getWinningPlayer()->getName() + " wins!";

  int whiteCount = g.count(Game::WHITE);
  int blackCount = g.count(Game::BLACK);

  std::cout
    << "\n==========================================\n"
    << "Final Result: " << winner
    << " (" << whiteCount << " - " << blackCount << ")" << "\n"
    << "==========================================\n\n"
    << g.boardAsString() << "\n" << std::endl;

  return g.isTie()
    ? 0
    : g.getWinner() == Game::P_WHITE
      ? 1
      : -1;
}
