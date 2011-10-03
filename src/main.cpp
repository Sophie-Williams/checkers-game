#include "mask64.h"
#include "field.h"

using namespace std;

static Field readField()
{
  FILE *f = fopen("field.txt", "r");
  char str[16];
  Field res;
  for (int i=0; i<8; i++)
  {
    fgets(str, 16, f);
    for (int j=0; j<8; j++)
    {
      Field::Contents c;
      switch(str[j])
      {
        default:
        case '-': c = Field::Empty; break;
        case 'A': c = Field::First; break;
        case 'B': c = Field::Second; break;
        case '1': c = Field::One; break;
        case '2': c = Field::Two; break;
        case '*': c = Field::Blocked; break;
      }
      res.set(j, i, c);
    }
  }
  fclose(f);
  return res;
}

struct Move
{
  uint8_t x, y, nx, ny;
};

int main()
{
  Field f = readField();
  
  cout << f << endl;

  int x=4, y=5;
  cout << "Moves from (" << x << ", " << y << ")" << endl;
  cout << f.movesFrom(x, y) << endl;

  for (auto pos: f.movesFrom(x, y))
  {
    Field f1 = f;
    f1.move(x, y, pos.first, pos.second);
    cout << f1 << endl;
  }

  return 0;
}

