#include "player.h"



Player::Player(Colors color)
  : color_{color}
  , piece_{0,0,0,0} {
}

bool Player::PiecesAtHome(){
  if(CountPiecesOnBox(0) != 0){
    return true;
  }
  return false;
}

int Player::CountPiecesOnBox(int box){
  int occupants = 0;
  for(int i = 0; i < kPieceNum; i++){
    if(piece_[i].position == box){
      occupants++;
    }
  }
  return occupants;
}
