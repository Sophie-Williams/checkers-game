#include "mask64.h"
#include "field.h"
#include "state.h"
#include "ai.h"
#include "debug_io.h"

using namespace std;

int main()
{
  State st;
  try
  {
    st = State::read("matrix.txt");
  }
  catch (const string &err)
  {
    cout << "Error: " << err << endl;
    return 1;
  }
  catch (...)
  {
    cout << "Unknown reading error" << endl;
  }
 
  Ai::makeTurn(st);
  st.write("matrix.txt");

  return 0;
}

