#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "bala.h"

class player
{
public:
  player(int jugador, int skinPasado = 1);
  const int velocidad = 100;
  const int playerWidth = 40;
  const int playerHeight = 60;
  void render1();
  void render2();
  void move(double timeStep);
  void handleEvent( SDL_Event& e);
  std::vector<bala*> balas;
  int mana;
  void sacarVida(int damage);
  int saberVida(){return vida;}
  int PosX() {return mPosX;}
  int PosY() {return mPosY;}
private:
  int vida = 10;
  double mPosX;
  double mPosY;
  //SDL_Rect mBox;
  int mVelx;
  int direccionDisparo;
  int cantBalas;
  SDL_Rect animacion[ 10 ];
  SDL_Rect animacionUtilizando;
  int i;
  int frame;
  SDL_Rect punteriaRect;
  int skin;
};
#endif
