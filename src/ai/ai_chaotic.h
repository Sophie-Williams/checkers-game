#ifndef AI_CHAOTIC
#define AI_CHAOTIC

#include "ai_base.h"

class ChaoticDecider: public DeciderBase
{
  public:
    Move decideMove(bool player);
};

#endif // AI_CHAOTIC
