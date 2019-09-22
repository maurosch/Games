#include "game.h"

game* game1;
int main(int argc, char *argv[])
{
  game1 = new game();
  game1->gameLoop();
  delete game1;
  return 0;
}
