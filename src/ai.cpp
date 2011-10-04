#include <stdint.h>
#include <vector>
#include <stack>
#include "ai.h"
#include "field.h"

using namespace std;

const int inf = 10000;
const int maxdepth = 5;

struct Move
{
  Move(int _x=0, int _y=0, int _nx=0, int _ny=0)
    : x(_x), y(_y), nx(_nx), ny(_ny) {}

  uint8_t x, y, nx, ny;
} __attribute__((packed));

struct ABDecider
{
  Move m;
  Field f;
  stack<Field> saved;

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

  int evaluate();
  int score(bool color, int depth, int alpha, int beta);
};


void Ai::makeTurn(State &st)
{
  ABDecider d;
  d.f = st.field;

  int s = d.score(st.player==0, maxdepth, -inf, inf);
  st.makeMove(d.m.x, d.m.y, d.m.nx, d.m.ny);
}

// ===================

int ABDecider::evaluate()
{
  return f.ones().bitcount() - f.twos().bitcount();
}

vector<Move> ABDecider::moves(bool player)
{
  Mask64 ps = player? f.first() : f.second();
  vector<Move> res;
  for (auto pos: ps)
    for (auto newPos: f.movesFrom(pos.first, pos.second))
      res.push_back(Move(pos.first, pos.second, newPos.first, newPos.second));
  return res;
}

int ABDecider::score(bool color, int depth, int alpha, int beta)
{
  if (depth <= 0)
    return evaluate();

  for (Move m: moves(color))
  {
    makeMove(m);

    int tmp = -score(!color, depth-1, -beta, -alpha);

    unmakeMove();

    if (tmp>alpha)
    {
      alpha = tmp;
      this->m = m;
    }

    if (alpha>beta)
      break;
  }

  return alpha;
}
