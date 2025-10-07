
//Parchis 26/09/2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parcheesi.h"

const int Parcheesi::safePoint[Parcheesi::kSafePoints] = {5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68};
const int Parcheesi::startPoint[Parcheesi::kMaxPlayers] = {5, 22, 39, 56};
const int Parcheesi::finishPoint[Parcheesi::kMaxPlayers] = {68, 17, 34, 51};

int RollDice(){
  return (rand() % 6) + 1;
}

Parcheesi::Parcheesi(int num_players)
  : num_players_{num_players}
  , current_turn_{0}
  , repeated_sixes_{0}
  , current_player_{kColor_Yellow}
  , player_{kColor_Yellow,kColor_Blue,kColor_Red,kColor_Green} {

    for(int i = 0; i < num_players_; i++){
    for(int j = 0; j < Player::kPieceNum; j++){
      player_[i].piece_[j].position = 0;
    }
  }

}

int Parcheesi::CountPiecesOnBox(int box){
  int occupants = 0;
  for(int i = 0; i < num_players_; i++){
    occupants += player_[i].CountPiecesOnBox(box);
  }
  return occupants;
}

void Parcheesi::NextPlayer(){
  int num_player = static_cast<int>(current_player_);
  num_player = (num_player + 1) % num_players_;
  current_player_ = static_cast<Colors>(num_player);
}

void Parcheesi::PlayTurn(){
  int dice_num;
  dice_num = RollDice();
  for(int i = 0; i < Player::kPieceNum; i++){
    if(dice_num == 5 && player_[current_player_].piece_[i].position == 0 &&
        CountPiecesOnBox(startPoint[current_player_]) < 2){
      player_[current_player_].piece_[i].position = startPoint[current_player_];
      NextPlayer();
      return;
    }
  }
}



MovementOutput Parcheesi::ApplyMovement(int piece_index, int dice_rolled){
  Player player = player_[current_player_];
  Piece piece = player.piece_[piece_index];
  int pos = piece.position;

  // Ficha en casa
  if(pos == 0){
    if(dice_rolled == 5 && CountPiecesOnBox(startPoint[current_player_]) < 2){
      piece.position = startPoint[current_player_];
      NextPlayer();
      return kMovementOutput_BasicMove;
    } else {
      NextPlayer();
      return kMovementOutput_NoMove;
    }
  }

  // Ficha forma muro
  if(player.CountPiecesOnBox(pos) >= 2){
    bool other_can_move = false;
    for(int i = 0; i < Player::kPieceNum; i++){
      if(i == piece_index) continue;
      int other_pos = player.piece_[i].position;
      if(other_pos != 0 || dice_rolled == 6){
        other_can_move = true;
        break;
      }
    }
    if(other_can_move){
      NextPlayer();
      return kMovementOutput_NoMove;
    }
  }

  // Ficha detras de muro
  int behind_box = (pos - 1 + kBoxNum) % kBoxNum;
  if(CountPiecesOnBox(behind_box) >= 2){
    NextPlayer();
    return kMovementOutput_NoMove;
  }

  // Comer ficha
  int target = pos + dice_rolled;
  if(target > kBoxNum) target -= kBoxNum;

  bool is_safe = false;
  for(int i = 0; i < kSafePoints; i++){
    if(safePoint[i] == target){
      is_safe = true;
      break;
    }
  }

  if(!is_safe){
    for(int p = 0; p < num_players_; p++){
      if(p == current_player_) continue;
      for(int i = 0; i < Player::kPieceNum; i++){
        if(player_[p].piece_[i].position == target){
          // Comer ficha rival 
          player_[p].piece_[i].position = 0;
          piece.position = target;

          // Bonus de 20 casillas
          piece.position += 20;
          if(piece.position > kBoxNum) piece.position -= kBoxNum;

          NextPlayer();
          return kMovementOutput_Eat;
        }
      }
    }
  }

  // Dado 6
  if(dice_rolled == 6){
    repeated_sixes_++;
    int advance = player.PiecesAtHome() ? 12 : 6;
    piece.position += advance;
    if(piece.position > kBoxNum) piece.position -= kBoxNum;

    if(repeated_sixes_ >= 3){
      piece.position = 0;
      repeated_sixes_ = 0;
      NextPlayer();
      return kMovementOutput_IllegalMove;
    }
    return kMovementOutput_BasicMove; // Repite turno
  }

  // Normal moviemiento
  int most_advanced = -1;
  int max_pos = -1;
  for(int i = 0; i < Player::kPieceNum; i++){
    int ppos = player.piece_[i].position;
    if(ppos > 0 && ppos > max_pos){ // ignorar las que están en casa
      max_pos = ppos;
      most_advanced = i;
    }
  }

  if(most_advanced != -1){
    player.piece_[most_advanced].position += dice_rolled;
    if(player.piece_[most_advanced].position > kBoxNum)
      player.piece_[most_advanced].position -= kBoxNum;
  }

  repeated_sixes_ = 0;
  NextPlayer();
  return kMovementOutput_BasicMove;
}
