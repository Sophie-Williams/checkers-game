#ifndef AI_BASE_H
#define AI_BASE_H

#include <cstdint>
#include <utility>
#include <vector>
#include <stack>

#include "mask64.h"
#include "field.h"

struct Move
{
  Move(int _x=0, int _y=0, int _nx=0, int _ny=0)
    : x(_x), y(_y), nx(_nx), ny(_ny) {}

  std::pair<int, int> from() const 
  { 
    return std::make_pair(int(x), int(y));
  }
  std::pair<int, int> to() const 
  { 
    return std::make_pair(int(nx), int(ny));
  }

  uint8_t x, y, nx, ny;
} __attribute__((packed));


// Implementation inlined for the sake of optimization
class DeciderBase
{
  public:
    DeciderBase(const Field &f, int md = 1) 
      : m_bestMoveSet(false), m_maxDepth(md), m_field(f) {}

    // Make move / rollback
    void makeMove(Move m)
    {
      m_savedFields.push(m_field);
      m_field.makeMove(m.x, m.y, m.nx, m.ny);
    }
    void unmakeMove()
    {
      m_field = m_savedFields.top();
      m_savedFields.pop();
    }

    // Generate all available moves
    std::vector<Move> moves(bool player) const
    {
      Mask64 ps = player? m_field.first() : m_field.second();
      std::vector<Move> res;
      res.reserve(ps.bitcount() * 8);
      for (auto pos: ps)
        for (auto newPos: m_field.movesFrom(pos.first, pos.second))
          res.push_back(Move(pos.first, pos.second, 
                             newPos.first, newPos.second));
      return res;
    }

    // Vote for move selection
    // Intended to be used at top level
    void voteMove(Move m, int score)
    {
      if (!m_bestMoveSet || score > m_bestMoveScore)
      {
        m_bestMove = m;
        m_bestMoveScore = score;
        m_bestMoveSet = true;
      }
    }

    // Access the best move
    void resetBestMove() { m_bestMoveSet = false; }
    bool bestMoveSet() const { return m_bestMoveSet; }
    Move bestMove() const { return m_bestMove; }
    int bestMoveScore() const { return m_bestMoveScore; }

    void setMaxDepth(int d) { m_maxDepth = d; }
    int maxDepth() const { return m_maxDepth; }

    const Field &field() const { return m_field; }
    
  private:
    Move m_bestMove;
    int m_bestMoveScore;
    bool m_bestMoveSet;
    int m_maxDepth;
    Field m_field;
    std::stack<Field> m_savedFields;
};

#endif // AI_BASE_H

