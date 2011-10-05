#include <stdint.h>
#include <vector>
#include <stack>
#include <iostream>

#include "ai.h"
#include "ai_base.h"
#include "field.h"
#include "debug_io.h"

using namespace std;

const int inf = 10000;
const int maxdepth = 5;



class ABDecider: public DeciderBase
{
  public:
    ABDecider(const Field &f, int md)
      : DeciderBase(f, md) {}

    Move decideMove(bool player);
  private:
    int evaluate(bool player);
    int score(bool player, int depth, int alpha, int beta);
};


void Ai::makeTurn(State &st)
{
  ABDecider d(st.field, maxdepth);

  Move m = d.decideMove(st.player==0);

  st.makeMove(m.x, m.y, m.nx, m.ny);
}

// ===================

int ABDecider::evaluate(bool player)
{
  return field().ones().bitcount() - field().twos().bitcount();
}

Move ABDecider::decideMove(bool player)
{
  // Top level tree analysis
  
  resetBestMove();
  int alpha = -inf;

  for (Move m: moves(player))
  {
    makeMove(m);

    int s = -score(!player, maxDepth()-1, -inf, -alpha);
    alpha = max(alpha, s);

    cout << "OPTION " << m << " | " << s << endl;

    voteMove(m, s);
    unmakeMove();
  }
  if (bestMoveSet())
  {
    cout << "CHOOSE " << bestMove() << " | " << bestMoveScore() << endl;
    return bestMove();
  }
  else
    throw "No move found. It's not ok.";
}

int ABDecider::score(bool player, int depth, int alpha, int beta)
{
  Field::State fst = field().checkState();

  // Draw
  if (fst == Field::Draw)
    return 0;

  // This player wins
  if ((player && fst == Field::FirstWins) 
      || (!player && fst == Field::SecondWins))
    return inf;

  //Other player wins
  if ((player && fst == Field::SecondWins) 
      || (!player && fst == Field::FirstWins))
    return -inf;

  // Compute approximated value
  if (depth <= 0)
    return evaluate(player);

  // Recurse deeper
  for (Move m: moves(player))
  {
    makeMove(m);

    int s = -score(!player, depth-1, -beta, -alpha);
    alpha = max(alpha, s);

    unmakeMove();
    
    if (alpha>beta)
      break;
  }

  return alpha;
}
