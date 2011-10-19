#ifndef AI_AB_H
#define AI_AB_H

#include "ai_base.h"

class ABDecider: public DeciderBase
{
  public:
    static const int inf = 100000;

    ABDecider(int md)
      : DeciderBase(md) {}

    Move decideMove(bool player);
  private:
    int evaluate(bool player);
    int score(bool player, int depth, int alpha, int beta);
};

#endif // AI_AB_H
