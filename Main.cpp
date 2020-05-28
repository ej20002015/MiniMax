#include <iostream>

#include "PlayTicTacToe.hpp"

int main()
{
  PlayTicTacToe play(PlayTicTacToe::PlayPolicy::SINGLEPLAYER);
  play.play();

  /* std::array<char, 9> arr = {
    '-', '-', '-',
    '-', '-', '-',
    '-', '-', '-'
  };

  std::array<char, 9> arr2 = {
    '-', '-', '-',
    '-', '-', '-',
    '-', '-', 'O'
  };

  std::shared_ptr<State> ticTacState1 = std::make_shared<TicTacToeState>(arr);
  std::shared_ptr<State> ticTacState2 = std::make_shared<TicTacToeState>(arr2);

  std::cout << ticTacState2->getHash() << std::endl;

  std::cout << (ticTacState1 == ticTacState2) << std::endl; */

  return 0;
}