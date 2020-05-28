#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_

#include "MiniMax.hpp"

#include <map>
#include <array>

struct TicTacToeState : public State
{
  std::array<char, 9> board;

  TicTacToeState(const std::array<char, 9>& board) : board(board) {}

  bool operator==(const std::shared_ptr<State>& rhs) const override;
  std::size_t getHash() const override;
};

struct TicTacToeAction : public Action
{
  const int cell;

  TicTacToeAction(const int cell) : cell(cell) {}
};

class TicTacToe : public SearchableGame
{
public:

  class TicTacToeInvalidMoveException : public std::exception
  {
  public:
    virtual const char* what() const throw() override { return "Cannot move there - must move to empty square"; }
  };

  TicTacToe();

  void makeMove(int cell);
  bool checkEndOfGame() const { return checkEndOfGame(board); }
  bool checkWinner(Player player) const { return checkWinner(board, player); };

  //To implement SearchableGame
  std::vector<std::pair<std::shared_ptr<State>, std::shared_ptr<Action>>> successorStates(const std::shared_ptr<State>& state) const override ;
  bool terminalState(const std::shared_ptr<State>& state) const override;
  int getUtility(const std::shared_ptr<State>& state, const Player& player) const override;
  Player getPlayerFromState(const std::shared_ptr<State>& state) const override;
  std::shared_ptr<State> getState() const override;

  void printState(const std::shared_ptr<State>& state) const override;
  void printAction(const std::shared_ptr<Action>& action) const override;

  friend std::ostream& operator<< (std::ostream &out, const TicTacToe& ticTacToeGame);

private:
  std::array<char, 9> board;
  Player currentPlayer;

  std::map<Player, char> playerCounter;

  bool checkEndOfGame(const std::array<char, 9>& board) const;
  bool checkWinner(const std::array<char, 9>& board, Player player) const;
};

std::ostream& operator<< (std::ostream &out, const TicTacToe& ticTacToeGame);

#endif