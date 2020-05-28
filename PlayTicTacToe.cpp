#include "PlayTicTacToe.hpp"

#include <chrono>

PlayTicTacToe::PlayTicTacToe(const PlayPolicy playPolicy)
  : playPolicy(playPolicy), ticTacGame(std::make_shared<TicTacToe>()), searchableGame(ticTacGame),
    search(searchableGame) {}

void PlayTicTacToe::play()
{
  char choice = 'y';
  if (playPolicy == PlayPolicy::SINGLEPLAYER)
  {
    std::cout << "Would you like to go first (y,n)? " << std::flush;
    std::cin >> choice;
  }

  std::map<int, std::string> assignedPlayers;
  if (choice == 'y')
  {
    assignedPlayers[1] = "PLAYER";
    if (playPolicy == PlayPolicy::SINGLEPLAYER)
      assignedPlayers[2] = "COMPUTER";
    else
      assignedPlayers[2] = "PLAYER";
  }
  else
  {
    assignedPlayers[2] = "PLAYER";
    if (playPolicy == PlayPolicy::SINGLEPLAYER)
      assignedPlayers[1] = "COMPUTER";
    else
      assignedPlayers[1] = "PLAYER";
  }

  for (int i = 0; i < 9; ++i)
  {
    if (i % 2 == 0)
    {
      if (assignedPlayers.at(1) == "PLAYER")
        playerMove();
      else
        computerMove();
    }
    else
    {
      if (assignedPlayers.at(2) == "PLAYER")
        playerMove();
      else
        computerMove();
    }

    if (ticTacGame->checkEndOfGame())
      break;
  }

  if (ticTacGame->checkWinner(Player::Player1))
    std::cout << "Player 1 wins!" << std::endl;
  else if (ticTacGame->checkWinner(Player::Player2))
    std::cout << "Player 2 wins" << std::endl;
  else
    std::cout << "Draw!" << std::endl;
}

void PlayTicTacToe::playerMove()
{
  int cell;
  bool validMove = false;
  while (!validMove)
  {
    validMove = true;
    std::cout << "Enter cell number: " << std::endl;
    std::cin >> cell;
    try
    {
      ticTacGame->makeMove(cell);
    }
    catch (TicTacToe::TicTacToeInvalidMoveException& e)
    {
      std::cout << e.what() << std::endl;
      validMove = false;
    }
  }

  std::cout << *ticTacGame << std::endl;
}

void PlayTicTacToe::computerMove()
{
  std::vector<MiniMaxSearch::Options> options;
  options.push_back(MiniMaxSearch::Options::USE_TRANSPOSITION_TABLE);
  auto start = std::chrono::high_resolution_clock::now();
  std::shared_ptr<TicTacToeAction> computerMove = std::dynamic_pointer_cast<TicTacToeAction>(search.performSearch(options).action);
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  std::cout << "Elapsed time: " << elapsed.count() << " s\n";
  std::cout << "Computer moved to: " << computerMove->cell << std::endl;
  ticTacGame->makeMove(computerMove->cell);
  std::cout << *ticTacGame << std::endl;
}