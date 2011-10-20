#include "ai_chaotic.h"

Move ChaoticDecider::decideMove(bool player)
{
  MoveSet ms = moves(player);
  return ms[rand() % ms.size()];
}
