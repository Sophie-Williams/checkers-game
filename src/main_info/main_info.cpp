#include "mask64.h"
#include "field.h"
#include "state.h"
#include "ai.h"
#include "debug_io.h"

using namespace std;

static void usage(const char *progname)
{
  cerr << "Usage: " << progname << " <action> <matrix.txt>" << endl;
  cerr << "Where <action> is one of --info, --pretty" << endl;
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    usage(argv[0]);
    return 1;
  }

  enum {Info, Pretty, Invalid} action = Invalid;
  const char *filename = argv[2];

  if (string("--info") == argv[1])
    action = Info;
  else if (string("--pretty") == argv[1])
    action = Pretty;
  else
  {
    cerr << "Bad arguments" << endl;
    usage(argv[0]);
    return 1;
  }

  State st;
  try
  {
    st = State::read(filename);
  }
  catch (const string &err)
  {
    cerr << "Error: " << err << endl;
    return 1;
  }
  catch (...)
  {
    cerr << "Unknown reading error" << endl;
    return 1;
  }

  switch (action)
  {
    case Info:
      {
        cout << "player=" << "AB"[st.player] << endl;
        cout << "turn=" << st.turn << endl;
        char c;
        switch (st.st)
        {
          case State::First:   c = 'A'; break;
          case State::Second:  c = 'B'; break;
          case State::Draw:    c = 'D'; break;
          default:
          case State::Unknown: c = 'U'; break;
        }
        cout << "state=" << c << endl;
        cout << "player1score=" << st.scores[0] << endl;
        cout << "player2score=" << st.scores[1] << endl;
        cout << "player1time=" << st.times[0] << endl;
        cout << "player2time=" << st.times[1] << endl;
      } break;
    case Pretty:
      cout << st.field;
      break;
    default:
      break;
  }

  return 0;
}
