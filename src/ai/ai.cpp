#include <cstdint>
#include <ctime>
#include <vector>
#include <stack>
#include <iostream>
#include <iomanip>

#include "ai.h"
#include "ai_base.h"
#include "field.h"
#include "debug_io.h"

using namespace std;

const int inf = 100000;
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

  return numOnes - numTwos;
}

// FIXME: Code duplication
Move ABDecider::decideMove(bool player)
{
#ifdef USE_TRACE_OPTIONS
# define trace(x) ((cout << "[0] ") << x << endl)
#else
# define trace(x) 
#endif 

  // Top level tree analysis
  
  resetBestMove();
  int alpha = -inf;

  for (Move m: moves(player))
  {
    makeMove(m);

    int s = -inf;

#ifdef USE_NEGASCOUT
    // Probe with zero-sized window
    if (alpha > -inf)
    {
      int s = -score(!player, maxDepth()-1, -(alpha+1), -alpha);

      trace("PROBE  " << m << " | " 
            << setw(7) << s
            << "\ta = " << alpha);

      // Cannot improve alpha
      if (s <= alpha)
        goto nextMove;

      alpha = s;
    }
#endif // USE_NEGASCOUT

    s = -score(!player, maxDepth()-1, -inf, -alpha);

    trace("OPTION " << m << " | " 
          << setw(7) << s 
          << "\ta = " << alpha);

    alpha = max(alpha, s);
    voteMove(m, s);

nextMove:
    unmakeMove();
  }
  if (bestMoveSet())
  {
    trace("CHOOSE " << bestMove() << " | " 
          << setw(7) << bestMoveScore()
          << "\ta = " << alpha);
    return bestMove();
  }
  else
    throw "No move found. It's not ok.";
#undef trace
}


static inline int q(bool p)
{
  return p? 1 : -1;
}

int ABDecider::score(bool player, int depth, int alpha, int beta)
{
#ifdef USE_TRACE_DEEP
  string indent;
  int printDepth = maxDepth() - depth;
  for (int i=0; i<printDepth; i++)
    indent += "  ";
  list<int> scores;
# define trace(x) ((cout << indent << "[" << printDepth << "] ") << x << endl)
#else
# define trace(x) 
#endif 

  Field::State fst = field().checkState();


  // ------ Trivival cases

  // Game end conditions
  if (fst == Field::Draw)
    return 0;
  if (fst == Field::FirstWins) 
    return q(player) * inf;
  if (fst == Field::SecondWins) 
    return q(player) * (-inf);

  // Compute approximated value
  if (depth <= 0)
  {
    int s = q(player) * evaluate(player);
    return s;
  }


  // ------ Complex cases below

  // Maximum child score
  int thisMax = -inf;

  trace("Score " << (player?'A':'B') 
        << " with bounds" << make_pair(alpha, beta));

  // Recurse deeper
  for (Move m: moves(player))
  {
    makeMove(m);
    trace("Do Move " << m);

    int s;
    
#ifdef USE_NEGASCOUT
    // Probe with zero-sized window
    if (thisMax > -inf && alpha > -inf && beta > alpha+1)
    {
      trace("  Probe?");
      int s = -score(!player, depth-1, -(alpha+1), -alpha);
      trace("  Probe -> " << s << " [a = " << alpha << "]");  

      thisMax = max(thisMax, s);

      // Cannot improve alpha
      if (s <= alpha)
        goto nextMove;

      alpha = s;
    }
#endif // USE_NEGASCOUT

    // Do a full-sized traversal
    trace("  FullScan?");
    s = -score(!player, depth-1, -beta, -alpha);
    trace("  FullScan -> " << s << " [a = " << alpha << "]");  

#ifdef USE_TRACE_DEEP
    scores.push_back(s);
#endif

    alpha = max(alpha, s);
    thisMax = max(thisMax, s);

nextMove:
    trace("Undo Move " << m);
    unmakeMove();
    
    if (alpha>=beta)
      break;
  }

  trace("Chose " << thisMax << " from " << scores);

  return thisMax;
#undef trace
}
