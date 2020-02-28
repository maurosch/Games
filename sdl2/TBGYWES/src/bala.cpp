#include "bala.h"
#include "game.h"
#include "player.h"
#include "ltexture.h"
#include "monster.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <math.h>

extern game* game1;
extern player* player1;
extern monster* monster1;

bala::bala(int direccionDisparo_pasada, int x, int y){
  angulo = direccionDisparo_pasada;
  mPosX = x;
  mPosY = y;
  direccionX = velocidad*(cos(angulo*3.14/180));
  direccionY = velocidad*(sin(angulo*3.14/180));
  TiempodeVida1 = game1->fpsTimer.getTicks();
  vida = true;
}

void bala::mover(double timeStep){
  mPosX -= direccionX*timeStep;
  mPosY -= direccionY*timeStep;

  if (mPosX > game1->SCREEN_WIDTH - bala_widht)
  {
    direccionX*=-1;
    mPosX = game1->SCREEN_WIDTH - bala_widht;
  }

  if (mPosX < 0)
  {
    direccionX*=-1;
    mPosX = 0;
  }

  if (mPosY < 0)
  {
    direccionY*=-1;
    mPosY = 0;
  }

  if (mPosY > game1->SCREEN_HEIGHT- bala_height - game1->piso)
  {
    direccionY*=-1;
    mPosY = game1->SCREEN_HEIGHT- bala_height - game1->piso;
  }

  TiempodeVida2 = game1->fpsTimer.getTicks() - TiempodeVida1;
  if(TiempodeVida2 > 5000)
  {
    vida = false;
  }
}

void bala::render()
{
  game1->gBalaTexture.render( int(mPosX), int(mPosY) );
}

void bala::render2()
{
  game1->gBala2Texture.render( int(mPosX), int(mPosY) );
}
