#include <fstream>
#include <ctime>
#include "state.h"

using namespace std;

State State::read(const std::string &fn)
{
  ifstream f;
  f.open(fn);
  if (!f.is_open()) 
    throw "Could not open file";
  
  State st = read(f);
  
  f.close();
  return st;
}

State State::read(istream &src)
{
  static const string badformat = "Bad format";

  try {
    // Detect input exceptions
    ios_base::iostate exn = src.exceptions();
    src.exceptions(ios_base::failbit | ios_base::badbit);

    int pl;
    GameState st;
    int score1, score2;
    double time1, time2;
    Field field;

    // <Player Turn State>
    string pl_c, st_c;
    int turn;
    src >> pl_c >> turn >> st_c;

    // Parse player
    if (pl_c == "A") 
      pl = 0;
    else if (pl_c == "B") 
      pl = 1;
    else
      throw badformat;

    // Parse state
    if (st_c == "A") 
      st = First;
    else if (st_c == "B")
      st = Second;
    else if (st_c == "D")
      st = Draw;
    else if (st_c == "U")
      st = Unknown;
    else
      throw badformat;

    // <Score Time>
    src >> score1 >> time1 >> score2 >> time2;
    src.ignore(256, '\n');
    
    // <Field>
    for (int i=0; i<8; i++)
    {
      string str;
      getline(src, str);
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
          default: throw badformat;
        }
        field.set(j, i, c);
      }
    }

    src.exceptions(exn); // reset iostream exceptions

    return State(pl, turn, st,
                 score1, time1, 
                 score2, time2,
                 field);
  }
  catch (ios_base::failure)
  {
    throw badformat;
  }
}


void State::write(const std::string &fn) const
{
  ofstream f;
  f.open(fn);
  if (!f.is_open()) 
    throw "Could not open file";
  write(f);
  f.close();
}

void State::write(ostream &dest) const
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
  dest << (player==0?'A':'B') << " " << turn << " " << st_c << endl;
  dest << scores[0] << " " << times[0] << endl;
  dest << scores[1] << " " << times[1] << endl;

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
      dest << c;
    }
    dest << endl;
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

  // Update state
  switch (field.checkState())
  {
    case Field::FirstWins:    st = First; break;
    case Field::SecondWins:   st = Second; break;
    case Field::Draw:         st = Draw; break;
    case Field::ContinueGame: st = Unknown; break;
  }

  // Update score
  scores[0] = field.first().bitcount();
  scores[1] = field.second().bitcount();
}

void State::makeTurn(vector<DeciderBase *> &ais)
{

  Move m;
  int clockStart = clock();
  for (auto ai: ais)
  {
    ai->setField(field);
    m = ai->decideMove(player==0);
    if (ai->bestMoveScore().score > DeciderBase::inf)
      break;
  }
  double dt = double(clock() - clockStart) / CLOCKS_PER_SEC;

  makeMove(dt, m.x, m.y, m.nx, m.ny);
}
