#include "monster.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <iostream>
extern game* game1;

monster::monster(int TipodeMonstruo, int x, int y)
{
  int j, i;
  vida = true;
  mPosX = x;
  mPosY = y;
  switch (TipodeMonstruo) {
    case 1:
      for( j = 0, i = 0; j < 3; j++, i++ )
      {
        animacion[j].x = i*monsterWidth;
        animacion[j].y = 0;
        animacion[j].w = monsterWidth;
        animacion[j].h = monsterHeight;
      }
      for( j = 3, i = 0; j < 6; j++, i++ )
      {
        animacion[j].x = i*monsterWidth;
        animacion[j].y = monsterWidth;
        animacion[j].w = monsterWidth;
        animacion[j].h = monsterHeight;
      }
      for( j = 6, i = 0; j < 9; j++, i++ )
      {
        animacion[j].x = i*monsterWidth;
        animacion[j].y = monsterWidth*2;
        animacion[j].w = monsterWidth;
        animacion[j].h = monsterHeight;
      }
    break;
  }
  animacionUtilizando = animacion[0];
}

void monster::mover(double timeStep)
{
  /*#ifdef DEBUG
    std::cout << "caca";
  #endif*/
    mPosX += velocidad * timeStep;
    /*++frame;
    //Cycle animation
    if( frame / 4 >= 3 )
    {
      frame = 0;
    }
    animacionUtilizando = animacion[frames/4];
    else animacionUtilizando*/
    if ( mPosX > game1->SCREEN_WIDTH - monsterWidth)
    {
      mPosX = game1->SCREEN_WIDTH - monsterWidth;
      velocidad *=-1;
    }
    if ( mPosX < 0 )
    {
      mPosX = 0;
      velocidad *=-1;
    }

    //CALCULAR DISPAROS MONSTRUO
    if ( int(game1->disparos.getTicks()) > 1500 )
    {
      int x, y, direccionDisparo;
      if ( int(mPosX) > game1->player1->PosX() ) x = int(mPosX) - game1->player1->PosX();
      else {
        if(int(mPosX) < game1->player1->PosX()) x = game1->player1->PosX() - int(mPosX);
        else
        {
          direccionDisparo = 90;
          x = 0;
        }
      }
      y = game1->player1->PosY();
      direccionDisparo = atan2 (x,y) * 180 / 3.14;
      if (game1->player1->PosX() > int(mPosX)) direccionDisparo+=90;

      balas.push_back ( new bala((direccionDisparo)*-1, int(mPosX), int(mPosY)+monsterWidth )  );
      //balas.push_back ( new bala((direccionDisparo+180), int(mPosX), int(mPosY)+monsterWidth )  );

      std::cout << direccionDisparo << std::endl;
      game1->disparos.start();
    }

}

void monster::render()
{
    game1->gMonstersTexture.render( int(mPosX), int(mPosY), monsterWidth*3/2, monsterHeight*3/2, &animacionUtilizando );
}
