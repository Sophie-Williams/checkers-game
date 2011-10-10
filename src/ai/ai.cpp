#include <cstdint>
#include <ctime>
#include <vector>
#include <stack>
#include <iostream>

#include "ai.h"
#include "ai_base.h"
#include "field.h"
#include "debug_io.h"

using namespace std;

const int inf = 10000;
const int maxdepth = 6;



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
  int clockStart = clock();

  ABDecider d(st.field, maxdepth);
  Move m = d.decideMove(st.player==0);

  double dt = double(clock() - clockStart) / CLOCKS_PER_SEC;

  st.makeMove(dt, m.x, m.y, m.nx, m.ny);
}

// ===================

int ABDecider::evaluate(bool player)
{
  int numOnes = field().ones().bitcount();
  int numTwos = field().twos().bitcount();
  int numBlocked = field().blocked().bitcount();

  int q = player? 1 : -1;

  return (10 * q * (numOnes - numTwos)) / numBlocked;
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

#ifdef PRINT_OPTIONS
    cout << "OPTION " << m << " | " << s << endl;
#endif 

    voteMove(m, s);
    unmakeMove();
  }
  if (bestMoveSet())
  {
#ifdef PRINT_OPTIONS
    cout << "CHOOSE " << bestMove() << " | " << bestMoveScore() << endl;
#endif
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
    return -inf;

  //Other player wins
  if ((player && fst == Field::SecondWins) 
      || (!player && fst == Field::FirstWins))
    return inf;

  // Compute approximated value
  if (depth <= 0)
    return evaluate(player);

  int thisMax = -inf;

  // Recurse deeper
  for (Move m: moves(player))
  {
    makeMove(m);

    int s = -score(!player, depth-1, -beta, -alpha);
    alpha = max(alpha, s);
    thisMax = max(thisMax, s);

    unmakeMove();
    
    if (alpha>beta)
      break;
  }

  return thisMax;
}
