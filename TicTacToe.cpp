#include "TicTacToe.hpp"

#include <iostream>
#include <cstring>
#include <sstream>

bool TicTacToeState::operator==(const std::shared_ptr<State>& rhs) const
{
  const std::shared_ptr<TicTacToeState> ticTacState = std::dynamic_pointer_cast<TicTacToeState>(rhs);
  return board == ticTacState->board;
}

std::size_t TicTacToeState::getHash() const
{
  std::hash<std::string> hasher;
  std::ostringstream stringBuilder;
  for (char cell : board)
  {
    stringBuilder << cell;
  }
  return hasher(stringBuilder.str());
}

TicTacToe::TicTacToe()
{
  //Initialise player counters
  playerCounter[Player::Player1] = 'X';
  playerCounter[Player::Player2] = 'O';

  //initialise board
  for (int i = 0; i < 9; ++i)
    board[i] = '-';
  
  currentPlayer = Player::Player1;
}

void TicTacToe::makeMove(int cell)
{
  if (board[cell] != '-' || cell < 0 || cell > 8)
    throw TicTacToeInvalidMoveException();

  board[cell] = playerCounter[currentPlayer];
  currentPlayer = (currentPlayer == Player::Player1) ? Player::Player2 : Player::Player1;
}

bool TicTacToe::checkEndOfGame(const std::array<char, 9>& board) const
{
  if (checkWinner(board, Player::Player1) || checkWinner(board, Player::Player2))
    return true;
  
  for (int i = 0; i < 9; ++i)
  {
    if (board[i] == '-')
      return false;
  }

  return true;
}

bool TicTacToe::checkWinner(const std::array<char, 9>& board, const Player player) const
{
  char counter = playerCounter.at(player);

  //check rows
  bool completeRow = false;
  for (int y = 0; y < 3; ++y)
  {
    int  x;
    for (x = 0; x < 3; ++x)
    {
      if (board[y * 3 + x] != counter)
        break;
    }

    if (x == 3)
      return true;
  }

  //check columns
  bool completeColumn = false;
  for (int x = 0; x < 3; ++x)
  {
    int  y;
    for (y = 0; y < 3; ++y)
    {
      if (board[y * 3 + x] != counter)
        break;
    }

    if (y == 3)
      return true;
  }

  //check diagonals
  if (board[0] == board[4] && board[4] == board[8] && board[8] == counter)
    return true;
  if (board[2] == board[4] && board[4] == board[6] && board[6] == counter)
    return true;
  
  return false;
}

std::vector<std::pair<std::shared_ptr<State>, std::shared_ptr<Action>>> TicTacToe::successorStates(const std::shared_ptr<State>& state) const
{
  std::shared_ptr<TicTacToeState> ticTacToeState = std::dynamic_pointer_cast<TicTacToeState>(state);
  
  Player player = getPlayerFromState(state);
  
  std::vector<std::pair<std::shared_ptr<State>, std::shared_ptr<Action>>> stateActions;
  for (int i = 0; i < 9; ++i)
  {
    if (ticTacToeState->board[i] == '-')
    {
      std::shared_ptr<Action> possibleAction = std::make_shared<TicTacToeAction>(i);
      std::array<char, 9> board = ticTacToeState->board;
      board[i] = playerCounter.at(player);
      std::shared_ptr<State> possibleState = std::make_shared<TicTacToeState>(board);
      stateActions.emplace_back(possibleState, possibleAction);
    }
  }

  return stateActions;
}

bool TicTacToe::terminalState(const std::shared_ptr<State>& state) const
{
  std::shared_ptr<TicTacToeState> ticTacToeState = std::dynamic_pointer_cast<TicTacToeState>(state);
  return checkEndOfGame(ticTacToeState->board);
}

int TicTacToe::getUtility(const std::shared_ptr<State>& state, const Player& player) const
{
  std::shared_ptr<TicTacToeState> ticTacToeState = std::dynamic_pointer_cast<TicTacToeState>(state);
  Player oppositePlayer = (player == Player::Player1) ? Player::Player2 : Player::Player1;
  if (checkWinner(ticTacToeState->board, player))
    return 1;
  if (checkWinner(ticTacToeState->board, oppositePlayer))
    return -1;
  
  return 0;
}

Player TicTacToe::getPlayerFromState(const std::shared_ptr<State>& state) const
{
  std::shared_ptr<TicTacToeState> ticTacToeState = std::dynamic_pointer_cast<TicTacToeState>(state);
  
  //determine who the next player to place a counter is
  int player1Count = 0, player2Count = 0;
  for (int i = 0; i < 9; ++i)
  {
    if (ticTacToeState->board[i] == playerCounter.at(Player::Player1))
      player1Count++;
    else if (ticTacToeState->board[i] == playerCounter.at(Player::Player2))
      player2Count++;
  }

  return (player1Count == player2Count) ? Player::Player1 : Player::Player2;
}

std::shared_ptr<State> TicTacToe::getState() const
{
  std::shared_ptr<State> currentState = std::make_shared<TicTacToeState>(board);
  return currentState;
}

void TicTacToe::printState(const std::shared_ptr<State>& state) const
{
  std::shared_ptr<TicTacToeState> ticTacToeState = std::dynamic_pointer_cast<TicTacToeState>(state);
  for (int i = 0; i < 9; ++i)
  {
    if (i % 3 == 0)
      std::cout << std::endl;
    
    std::cout << ticTacToeState->board[i];
  }
  std::cout << std::endl;
}

void TicTacToe::printAction(const std::shared_ptr<Action>& action) const
{
  std::shared_ptr<TicTacToeAction> ticTacToeAction = std::dynamic_pointer_cast<TicTacToeAction>(action);
  std::cout << ticTacToeAction->cell << std::endl;
}

std::ostream& operator<< (std::ostream &out, const TicTacToe& ticTacToeGame)
{
  for (int i = 0; i < 9; ++i)
  {
    if (i % 3 == 0)
      out << std::endl;
    
    out << ticTacToeGame.board[i] << " ";
  }

  return out << std::endl;
}