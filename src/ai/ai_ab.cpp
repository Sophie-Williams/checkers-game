#include <cstdint>
#include <ctime>
#include <vector>
#include <stack>
#include <iostream>
#include <iomanip>

#include "ai_ab.h"
#include "field.h"
#include "debug_io.h"

using namespace std;

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

    Score s;

#ifdef USE_NEGASCOUT
    // Probe with zero-sized window
    if (alpha > -inf && alpha < inf)
    {
      s = -score(!player, 1, -(alpha+1), -alpha);

      trace("PROBE  " << m << " | " 
            << setw(7) << s
            << "\ta = " << alpha);

      voteMove(m, s);

      // Cannot improve alpha
      if (s.score <= alpha)
        goto nextMove;

      alpha = s.score;
    }
#endif // USE_NEGASCOUT

    s = -score(!player, 1, -inf, -alpha);

    trace("OPTION " << m << " | " 
          << setw(7) << s 
          << "\ta = " << alpha);

    alpha = max(alpha, s.score);
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

// <score, depth>
DeciderBase::Score ABDecider::score(bool player, int depth, int alpha, int beta)
{
#ifdef USE_TRACE_DEEP
  string indent;
  for (int i=0; i<depth; i++)
    indent += "  ";
  list<Score> scores;
# define trace(x) ((cout << indent << "[" << depth << "] ") << x << endl)
#else
# define trace(x) 
#endif 



  // ------ Trivival cases
  
  Field::State fst = field().checkState();

  // Game end conditions
  if (fst == Field::Draw)
  {
    trace("Stop: Draw");
    return Score(0, depth);
  }
  if (fst == Field::FirstWins) 
  {
    trace("Stop: First wins");
    return Score(q(player) * inf, depth);
  }
  if (fst == Field::SecondWins) 
  {
    trace("Stop: Second wins");
    return Score(q(player) * (-inf), depth);
  }

  // Compute approximated value
  if (depth > maxDepth())
  {
    Score s(q(player) * evaluate(player), depth);
    trace("Stop: Depth " << s);
    return s;
  }


  // ------ Complex cases below

  // Maximum child score
  Score thisMax(-inf-1, inf);

  trace("Score " << (player?'A':'B') 
        << " with bounds" << make_pair(alpha, beta));

  // Recurse deeper
  for (Move m: moves(player))
  {
    makeMove(m);
    trace("Do Move " << m);

    Score s;
    
#ifdef USE_NEGASCOUT
    // Probe with zero-size window
    if (thisMax.score > -inf && alpha > -inf && beta > alpha+1)
    {
      trace("  Probe?");
      s = -score(!player, depth+1, -(alpha+1), -alpha);
      trace("  Probe -> " << s << " [a = " << alpha << "]");  

#ifdef USE_TRACE_DEEP
      scores.push_back(s);
#endif
      
      thisMax = max(thisMax, s);

      // Cannot improve alpha
      if (s.score <= alpha)
        goto nextMove;

      alpha = s.score;
    }
#endif // USE_NEGASCOUT

    // Do a full-sized traversal
    trace("  FullScan?");
    s = -score(!player, depth+1, -beta, -alpha);
    trace("  FullScan -> " << s << " [a = " << alpha << "]");  

#ifdef USE_TRACE_DEEP
    scores.push_back(s);
#endif

    alpha = max(alpha, s.score);
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
