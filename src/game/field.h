#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include "mask64.h"

class Field
{
  public:
    enum Contents
    {
      Empty,
      First,
      Second,
      One,
      Two,
      Blocked
    };

    enum State
    {
      FirstWins,
      SecondWins,
      Draw,
      ContinueGame
    };

    Contents at(int x, int y) const;
    void set(int x, int y, Contents c);

    Mask64 first() const { return m_first; }
    Mask64 second() const { return m_second; }
    Mask64 ones() const { return m_ones; }
    Mask64 twos() const { return m_twos; }
    Mask64 blocked() const { return m_blocked; }

    Mask64 movesFrom(int x, int y) const;
    void makeMove(int x, int y, int nx, int ny);

    State checkState() const;

  private:
    Mask64 m_first;
    Mask64 m_second;
    Mask64 m_ones;
    Mask64 m_twos;
    Mask64 m_blocked;
};

#endif // FIELD_H
