#ifndef AI_AB_H
#define AI_AB_H

#include "ai_base.h"

class ABDecider: public DeciderBase
{
  public:
    ABDecider(int md)
      : DeciderBase(md) {}

    Move decideMove(bool player);
  private:
    int evaluate(bool player);
    Score score(bool player, int depth, int alpha, int beta);
};

#endif // AI_AB_H
