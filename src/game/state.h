#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <string>
#include "field.h"

struct State
{
  enum GameState 
  {
    First,
    Second,
    Draw,
    Unknown
  };

  State() {}
  State(int player, int turn, GameState st, 
      int score1, double time1,
      int score2, double time2,
      const Field &field);

  static State read(std::istream &src);
  static State read(const std::string &fn);

  void write(std::ostream &dest) const;
  void write(const std::string &fn) const;

  void makeMove(double dt, int x, int y, int nx, int ny);

  // Props
  int player;
  int turn;
  GameState st;
  int scores[2];
  double times[2];
  Field field;
};

#endif // STATE_H
