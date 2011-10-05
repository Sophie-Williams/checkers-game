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



struct ABDecider
{
  Move m;
  bool haveMove;
  Field f;
  stack<Field> saved;

  ABDecider() : haveMove(false) {}

  void makeMove(Move m)
  {
    saved.push(f);
    f.makeMove(m.x, m.y, m.nx, m.ny);
  }
  void unmakeMove()
  {
    f = saved.top();
    saved.pop();
  }
  
  vector<Move> moves(bool player);

  int evaluate(bool player);
  int score(bool color, int depth, int alpha, int beta);
};


void Ai::makeTurn(State &st)
{
  ABDecider d;
  d.f = st.field;

  int s = d.score(st.player==0, maxdepth, -inf, inf);

  cout << (st.player==0? 'A': 'B') << ": " 
    << d.m << " | " << s << endl; 

  st.makeMove(d.m.x, d.m.y, d.m.nx, d.m.ny);
}

// ===================

int ABDecider::evaluate(bool player)
{
  return f.ones().bitcount() - f.twos().bitcount();
}

vector<Move> ABDecider::moves(bool player)
{
  Mask64 ps = player? f.first() : f.second();
  vector<Move> res;
  for (auto pos: ps)
    for (auto newPos: f.movesFrom(pos.first, pos.second))
      res.push_back(Move(pos.first, pos.second, 
                         newPos.first, newPos.second));
  return res;
}

int ABDecider::score(bool player, int depth, int alpha, int beta)
{
  Field::State fst = f.checkState();

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

    int tmp = -score(!player, depth-1, -beta, -alpha);

    if (depth == maxdepth)
      cout << "EVAL " << m << " | " << tmp << endl;

    unmakeMove();

    if (depth == maxdepth && (!haveMove || tmp>alpha))
    {
      haveMove = true;
      this->m = m;
    }

    if (tmp>alpha)
      alpha = tmp;

    if (alpha>beta)
      break;
  }

  return alpha;
}
