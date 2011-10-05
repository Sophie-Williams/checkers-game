#include <cstdarg>
#include "state.h"

State State::read(const std::string &fn)
{
  FILE *f = fopen(fn.c_str(), "r");
  if (!f) throw "Could not open file";
  
  State st = read(f);
  
  fclose(f);
  return st;
}

static void xfscanf(FILE *f, int n, const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  int res = vfscanf(f, format, ap);
  if (res == EOF)
    throw "Error reading file";
  if (res != n)
    throw "Bad format";
  va_end(ap);
}

State State::read(FILE *f)
{
  char pl_c, st_c;
  int turn;
  int pl;
  GameState st;
  int score1, score2;
  double time1, time2;
  Field field;

  // <Player Turn State>
  xfscanf(f, 3, "%c %d %c\n", &pl_c, &turn, &st_c);

  // Parse player
  switch (pl_c)
  {
    case 'A': pl = 0; break;
    case 'B': pl = 1; break;
    default: goto badformat;
  }

  // Parse state
  switch (st_c)
  {
    case 'A': st = First; break;
    case 'B': st = Second; break;
    case 'D': st = Draw; break;
    case 'U': st = Unknown; break;
    default: goto badformat;
  }

  // <Score Time>
  xfscanf(f, 2, "%d %lf\n", &score1, &time1);
  xfscanf(f, 2, "%d %lf\n", &score2, &time2);
  
  // <Field>
  for (int i=0; i<8; i++)
  {
    char str[9];
    xfscanf(f, 1, "%8s\n", str);
    for (int j=0; j<8; j++)
    {
      Field::Contents c;
      switch(str[j])
      { 
        case '-': c = Field::Empty; break;
        case 'A': c = Field::First; break;
        case 'B': c = Field::Second; break;
        case '1': c = Field::One; break;
        case '2': c = Field::Two; break;
        case '*': c = Field::Blocked; break;
        default: goto badformat;
      }
      field.set(j, i, c);
    }
  }

  return State(pl, turn, st,
               score1, time1, 
               score2, time2,
               field);

badformat:
  throw "Bad format";
}


void State::write(const std::string &fn) const
{
  FILE *f = fopen(fn.c_str(), "w");
  if (!f) throw "Could not open file";
  write(f);
  fclose(f);
}

void State::write(FILE *f) const
{
  char st_c;
  switch (st)
  {
    case First:   st_c = 'A'; break;
    case Second:  st_c = 'B'; break;
    case Draw:    st_c = 'D'; break;
    default:
    case Unknown: st_c = 'U'; break;
  }
  fprintf(f, "%c %d %c\n", player==0?'A':'B', turn, st_c);
  fprintf(f, "%d %f\n", scores[0], float(times[0]));
  fprintf(f, "%d %f\n", scores[1], float(times[1]));

  for (int y=0; y<8; y++)
  {
    for (int x=0; x<8; x++)
    {
      char c;
      switch (field.at(x, y))
      {
        default:
        case Field::Empty:   c = '-'; break;
        case Field::First:   c = 'A'; break;
        case Field::Second:  c = 'B'; break;
        case Field::One:     c = '1'; break;
        case Field::Two:     c = '2'; break;
        case Field::Blocked: c = '*'; break;
      }
      fputc(c, f);
    }
    fputc('\n', f);
  }
}

State::State(int _player, int _turn, GameState _st, 
             int _score1, double _time1,
             int _score2, double _time2,
             const Field &_field)
  : player(_player), turn(_turn), st(_st),
    scores{_score1, _score2}, 
    times{_time1, _time2},
    field(_field)
{
}

void State::makeMove(double dt, int x, int y, int nx, int ny)
{
  times[player] -= dt;
  player = 1-player;
  turn++;
  field.makeMove(x, y, nx, ny);
  // TODO: Update state
  // TODO: Update score (?!)
}
