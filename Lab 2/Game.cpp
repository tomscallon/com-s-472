#include "Game.hpp"
#include "Player.hpp"

#include <string>
#include <sstream>
#include <vector>

#include <iostream>

Game::Game(Player* whitePlayer, Player* blackPlayer) :
  whitePlayer(whitePlayer), blackPlayer(blackPlayer) {
  // Initialize the grid.
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      this->board[i][j] = Space::EMPTY;
    }
  }

  // Set up the initial configuration.
  this->board[3][3] = Space::WHITE;
  this->board[4][4] = Space::WHITE;
  this->board[3][4] = Space::BLACK;
  this->board[4][3] = Space::BLACK;
}

// private helper
void Game::makeMove(Game::Location l) {
  Space pro = this->currentPlayer == P_WHITE ? WHITE : BLACK;
  Space ant = this->currentPlayer == P_WHITE ? BLACK : WHITE;

  // Change the selected space.
  this->board[l.x][l.y] = pro;

  // Then change any adjacent runs.
  Direction dirs[] = {E, SE, S, SW, W, NW, N, NE};
  for (int i = 0; i < 8; i++) {
    if (this->testRun(pro, ant, l, dirs[i])) {
      this->flipRun(pro, ant, l, dirs[i]);
    }
  }
}

// private helper
bool Game::testRun(
  Game::Space pro,
  Game::Space ant,
  Location loc,
  Direction dir
) const {
  // Move to the next space.
  loc = loc + dir;

  // If the space isn't valid or isn't 'ant', fail.
  if (!Game::isValidLocation(loc) || (*this)[loc] != ant) {
    return false;
  }

  // Otherwise, keep progressing.
  loc = loc + dir;
  while (Game::isValidLocation(loc)) {
    Space s = (*this)[loc];
    if (s == pro) {
      return true;
    } else if (s == EMPTY) {
      return false;
    }
    loc = loc + dir;
  }

  return false;
}

// private helper
// Assumes Game::testRun, invoked with the same arguments, returned 'true'
void Game::flipRun(
  Game::Space pro,
  Game::Space ant,
  Location loc,
  Direction dir
) {
  loc = loc + dir;
  while ((*this)[loc] == ant) {
    this->board[loc.x][loc.y] = pro;
    loc = loc + dir;
  }
}

void Game::nextTurn() {
  if (this->movesetForCurrentPlayer().empty()) {
    // If the current player can't move anywhere, pass.
    this->currentPlayer = !this->currentPlayer;
    return;
  }

  Location next = this->currentPlayer == P_WHITE
    ? this->whitePlayer->getNextMove(*this)
    : this->blackPlayer->getNextMove(*this);

  std::cout << "About to make move: " << next.x << "," << next.y << std::endl;

  this->makeMove(next);

  this->currentPlayer = !this->currentPlayer;
}

const Player* Game::getWhitePlayer() const {
  return this->whitePlayer;
}

const Player* Game::getBlackPlayer() const {
  return this->blackPlayer;
}

const Player* Game::getCurrentPlayer() const {
  return this->currentPlayer == P_WHITE
    ? this->whitePlayer
    : this->blackPlayer;
}

const Player* Game::getWinningPlayer() const {
  return this->isFinished()
    ? this->isTie()
      ? nullptr
      : this->getWinner() == P_WHITE
        ? this->whitePlayer
        : this->blackPlayer
    : nullptr;
}

int Game::count(Game::Space type) const {
  int total = 0;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (this->board[i][j] == type) {
        total++;
      }
    }
  }

  return total;
}

bool Game::isWhiteTurn() const {
  return this->currentPlayer == P_WHITE;
}

bool Game::isBlackTurn() const {
  return this->currentPlayer == P_BLACK;
}

bool Game::isFinished() const {
  return this->movesetForWhite().empty() && this->movesetForBlack().empty();
}

bool Game::isTie() const {
  return this->count(WHITE) == this->count(BLACK);
}

bool Game::getWinner() const {
  return this->count(WHITE) > this->count(BLACK) ? P_WHITE : P_BLACK;
}

bool Game::isValidMove(bool who, Game::Location l) const {
  // Must be a valid location.
  if (!Game::isValidLocation(l)) {
    return false;
  }

  // pro = "protagonist" -- the player whose is attempting
  // to make the move. ant = "antagonist" -- the other player
  Space pro = who == P_WHITE ? WHITE : BLACK;
  Space ant = who == P_WHITE ? BLACK : WHITE;

  // If the space isn't empty, this move isn't valid.
  if (this->board[l.x][l.y] != EMPTY) {
    return false;
  }

  // Otherwise, attempt to find a run adjacent to the tested location.
  return
    this->testRun(pro, ant, l, E ) ||
    this->testRun(pro, ant, l, SE) ||
    this->testRun(pro, ant, l, S ) ||
    this->testRun(pro, ant, l, SW) ||
    this->testRun(pro, ant, l, W ) ||
    this->testRun(pro, ant, l, NW) ||
    this->testRun(pro, ant, l, N ) ||
    this->testRun(pro, ant, l, NE);
}

Game::Space Game::operator[] (Game::Location l) const {
  if (Game::isValidLocation(l)) {
    return this->board[l.x][l.y];
  } else {
    return Space::EMPTY;
  }
}

Game Game::operator + (Game::Location l) const {
  if (Game::isValidLocation(l)) {
    Game newGame = *this;
    newGame.makeMove(l);
    newGame.currentPlayer = !newGame.currentPlayer;
    return newGame;
  } else throw "Invalid location";
}

std::vector<Game::Location> Game::movesetForWhite() const {
  return this->movesetFor(P_WHITE);
}

std::vector<Game::Location> Game::movesetForBlack() const {
  return this->movesetFor(P_BLACK);
}

std::vector<Game::Location> Game::movesetForCurrentPlayer() const {
  return this->movesetFor(this->currentPlayer);
}

std::vector<Game::Location> Game::movesetFor(bool who) const {
  std::vector<Location> moves;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (this->isValidMove(who, {i, j})) {
        moves.push_back({i, j});
      }
    }
  }

  return moves;
}

std::string Game::boardAsString(
  std::function<std::string (Space, Location)> display
) const {
  std::stringstream ss;

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      // ss << Game::displayCharFor(this->board[i][j]);
      ss << display(this->board[i][j], {i, j});

      if (j < SIZE - 1) {
        ss << ' ';
      }
    }

    if (i < SIZE - 1) {
      ss << '\n';
    }
  }

  return ss.str();
}

bool Game::isValidLocation(Game::Location l) {
  return l.x >= 0 && l.x < SIZE && l.y >= 0 && l.y < SIZE;
}

std::string Game::displayCharFor(Game::Space s, Game::Location l) {
  return Game::displayCharFor(s);
}

std::string Game::displayCharFor(Game::Space s) {
  switch (s) {
    case WHITE: return "\u25CF";
    case BLACK: return "\u25CB";
    default:    return "\u00B7";
  }
}

bool operator == (const Game::Location& a, const Game::Location& b) {
  return a.x == b.x && a.y == b.y;
}

bool operator < (const Game::Location& a, const Game::Location& b) {
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

Game::Location operator + (const Game::Location& l, const Game::Direction& d) {
  int x = 0, y = 0;

  if (
    d == Game::Direction::NE ||
    d == Game::Direction::E ||
    d == Game::Direction::SE
  ) {
    x = 1;
  } else if (
    d == Game::Direction::NW ||
    d == Game::Direction::W ||
    d == Game::Direction::SW
  ) {
    x = -1;
  }

  if (
    d == Game::Direction::NW ||
    d == Game::Direction::N ||
    d == Game::Direction::NE
  ) {
    y = -1;
  } else if (
    d == Game::Direction::SW ||
    d == Game::Direction::S ||
    d == Game::Direction::SE
  ) {
    y = 1;
  }

  return {l.x + x, l.y + y};
}
