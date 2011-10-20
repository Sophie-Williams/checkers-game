#include <algorithm>
#include "ai_ab.h"

using namespace std;

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
  static const Mask64 topLeft = Mask64::rows(3) & Mask64::cols(3);
  static const Mask64 topRight = Mask64::rows(3) & Mask64::cols(-3);
  static const Mask64 bottomLeft = Mask64::rows(-3) & Mask64::cols(3);
  static const Mask64 bottomRight = Mask64::rows(-3) & Mask64::cols(-3);
  static const Mask64 center = (Mask64::rows(4) & Mask64::cols(4)).translated(2, 2);

  int s = 0;
  if (!(m &     topLeft).isNull()) s += 2;
  if (!(m &    topRight).isNull()) s += 2;
  if (!(m &  bottomLeft).isNull()) s += 2;
  if (!(m & bottomRight).isNull()) s += 2;

  if (!(m & topLeft).isNull() && !(m & bottomRight).isNull() && !(m & topRight).isNull() && !(m & bottomLeft).isNull())
    s += 5;

  if (!(m & center).isNull())
    s += 3;

  return s;
}

static int distance(Mask64 checkers, Mask64 blocked)
{
  int s = 0;
  for (auto c: checkers)
  {
    int maxD = 0;
    for (auto b: blocked)
      maxD = max(maxD, sqr(c.first-b.first) + sqr(c.second-b.second));
    s += maxD;
  }
        
  return s;
}

int ABDecider::evaluate(bool player)
{
  int checkers[2] = { field().first().bitcount(), field().second().bitcount() };
  int scores[2] = { field().ones().bitcount(), field().twos().bitcount() };
  int coverages[2] = { coverage(field().first()), coverage(field().second()) };

  int scoreBalance = sqrdiff(scores);
  int coverageBalance = sqrdiff(coverages);
  int checkersBalance = diff(checkers);

  // Fill the whole field
  int domination = 3*checkersBalance + 2*scoreBalance + coverageBalance;

  // Sum of minimal distances to 
  int distances[2] = { 1000/distance(field().first(), field().blocked()), 1000/distance(field().second(), field().blocked()) };
  int distBalance = diff(distances);

  int numBlocked = field().blocked().bitcount();

  // Grab all stars
  int acquisition = distBalance * (10/numBlocked);


  // domination vs. acquisition?
  return domination + acquisition;
}

