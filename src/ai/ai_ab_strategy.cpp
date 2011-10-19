#include "ai_ab.h"

/* NOTE
 * Evaluator's output is the estimation of position for the _first_ player
 */

template<typename T>
static inline T sqr(T x)
{
  return x*x;
}

template<typename T>
static inline T sqrdiff(T xs[2])
{
  return sqr(xs[0]) - sqr(xs[1]);
}

template<typename T>
static inline T diff(T xs[2])
{
  return xs[0] - xs[1];
}


#if 0
// Too dumb to be real
int ABDecider::evaluate(bool player)
{
  int numOnes = field().ones().bitcount();
  int numTwos = field().twos().bitcount();
  int numBlocked = field().blocked().bitcount();

  return numOnes - numTwos;
}
#endif

static int coverage(Mask64 m)
{
  static const Mask64 topLeft = Mask64::rows(4) & Mask64::cols(4);
  static const Mask64 topRight = Mask64::rows(4) & Mask64::cols(-4);
  static const Mask64 bottomLeft = Mask64::rows(-4) & Mask64::cols(4);
  static const Mask64 bottomRight = Mask64::rows(-4) & Mask64::cols(-4);
  static const Mask64 center = topLeft.translated(2, 2);

  int s = 0;
  if (!(m & topLeft).isNull() && !(m & bottomRight).isNull())
    s += 2;
  if (!(m & topRight).isNull() && !(m & bottomLeft).isNull())
    s += 2;
  if (!(m & topLeft).isNull() && !(m & bottomRight).isNull() && !(m & topRight).isNull() && !(m & bottomLeft).isNull())
    s += 5;
  if (!(m & center).isNull())
    s += 3;

  return 3;
}

int ABDecider::evaluate(bool player)
{
  int checkers[2] = { field().first().bitcount(), field().second().bitcount() };
  int scores[2] = { field().ones().bitcount(), field().twos().bitcount() };
  int coverages[2] = { coverage(field().first()), coverage(field().second()) };

  int numBlocked = field().blocked().bitcount();

  int scoreBalance = sqrdiff(scores);
  int coverageBalance = sqrdiff(coverages);
  int checkersBalance = diff(checkers);

  return 30*checkersBalance + scoreBalance + coverageBalance;
}

