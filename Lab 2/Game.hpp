#ifndef GAME_H
#define GAME_H

#include <array>
#include <functional>
#include <set>
#include <vector>
#include <string>

class Player;

class Game {
public:
  static const int SIZE = 8;
  static const bool P_WHITE = true;
  static const bool P_BLACK = false;
  enum Space { EMPTY, WHITE, BLACK };
  enum Direction { E, SE, S, SW, W, NW, N, NE };
  typedef std::array<std::array<Space, SIZE>, SIZE> Board;
  struct Location {
    int x;
    int y;
  };

private:
  Board board;

  bool currentPlayer = P_WHITE;
  Player* whitePlayer;
  Player* blackPlayer;

  void makeMove(Location l);
  bool testRun(Space pro, Space ant, Location loc, Direction dir) const;
  void flipRun(Space pro, Space ant, Location loc, Direction dir);

  // Private overload for ::boardAsString
  static std::string displayCharFor(Space s, Location l);

public:
  Game(Player* whitePlayer, Player* blackPlayer);

  // Control
  void nextTurn();

  // Informational
  const Player* getWhitePlayer() const;
  const Player* getBlackPlayer() const;
  const Player* getCurrentPlayer() const;
  const Player* getWinningPlayer() const;

  int count(Space type) const;

  bool isWhiteTurn() const;
  bool isBlackTurn() const;
  bool isFinished() const;
  bool isTie() const;
  bool getWinner() const;

  bool isValidMove(bool who, Location l) const;

  // Operators
  Space operator[] (Location l) const;
  Game  operator + (Location l) const;

  // Utility
  std::vector<Location> movesetForWhite() const;
  std::vector<Location> movesetForBlack() const;
  std::vector<Location> movesetForCurrentPlayer() const;
  std::vector<Location> movesetFor(bool who) const;
  std::string boardAsString(
    std::function<std::string (Space, Location)> display = (std::string (*)(Space, Location)) &Game::displayCharFor
  ) const;

  // Static Utility
  static bool isValidLocation(Location l);
  static std::string displayCharFor(Space s);
};

bool operator == (const Game::Location& a, const Game::Location& b);
bool operator < (const Game::Location& a, const Game::Location& b);
Game::Location operator + (const Game::Location& l, const Game::Direction& d);

#endif
