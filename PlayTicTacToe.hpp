#ifndef PLAY_TIC_TAC_TOE_H_
#define PLAY_TIC_TAC_TOE_H_

#include "TicTacToe.hpp"

class PlayTicTacToe
{
public:

  enum class PlayPolicy
  {
    SINGLEPLAYER,
    MULTIPLAYER
  };

  PlayTicTacToe(const PlayPolicy playPolicy);
  void play();

private:
  std::shared_ptr<TicTacToe> ticTacGame;
  std::shared_ptr<SearchableGame> searchableGame;
  MiniMaxSearch search;
  const PlayPolicy playPolicy;

  void playerMove();
  void computerMove();

};


#endif