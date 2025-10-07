#pragma once
#ifndef __PARCHEESI_H__
#define __PARCHEESI_H__

#include "player.h"

enum MovementOutput{
  kMovementOutput_BasicMove = 0,
  kMovementOutput_NoMove,
  kMovementOutput_Eat,
  kMovementOutput_IllegalMove,
  kMovementOutput_End
};

struct Parcheesi{
  static const int kMaxPlayers = 4;
  static const int kSafePoints = 12; 
  static const int kBoxNum = 68;

  static const int safePoint[kSafePoints];
  static const int startPoint[kMaxPlayers];
  static const int finishPoint[kMaxPlayers];
  
  int num_players_;
  int current_turn_;
  int repeated_sixes_;

  Colors current_player_;   
  Player player_[kMaxPlayers];

  Parcheesi(int num_players);

  int CountPiecesOnBox(int box);
  void NextPlayer();
  void PlayTurn();
  MovementOutput ApplyMovement(int pieceIndex, int roll_dice);
  int RollDice();
};

#endif // __PARCHEESI_H_


