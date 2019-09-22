#ifndef _MONSTER_H_
#define _MONSTER_H_
#include <SDL.h>
#include <vector>
#include "bala.h"
class monster
{
public:
  monster(int TipodeMonstruo, int x, int y);
  const int monsterWidth = 34;
  const int monsterHeight = 32;
  void mover(double timeStep);
  void render();
  void matar(){vida = false;}
  bool estaVivo(){return vida;}
  std::vector<bala*> balas;
  int posX(){return mPosX;}
  int posY(){return mPosY;}
private:
  bool vida;
  double mPosX;
  double mPosY;
  int velocidad = 150;
  SDL_Rect mBox;
  SDL_Rect animacion[ 9 ];
  SDL_Rect animacionUtilizando;
  int frames = 0;
};
#endif
