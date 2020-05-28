#include "Player.hpp"

std::ostream& operator<< (std::ostream &out, const Player& player)
{
  if (player == Player::Player1)
    out << "Player1" << std::endl;
  else
    out << "Player2" << std::endl;
  
  return out;
}