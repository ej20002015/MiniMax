#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>

enum class Player
{
  Player1,
  Player2
};

std::ostream& operator<< (std::ostream &out, const Player& player);

#endif