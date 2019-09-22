#include "player.h"
#include "game.h"
#include "ltexture.h"
#include <SDL.h>

extern game* game1;

player::player(int jugador, int skinPasado){
  skin = skinPasado;
  mPosY = game1->SCREEN_HEIGHT - playerHeight - game1->piso;
  if (jugador == 1) mPosX = game1->SCREEN_WIDTH/3;
  if (jugador == 2) mPosX = game1->SCREEN_WIDTH*2/3;
  mana = 5;
  direccionDisparo = 90;
  i = 0;
  mVelx = 0;

  for( int j = 0; j <= 10; j++)
  {
    animacion[j].x = j*playerWidth;
    animacion[j].y = 0;
    animacion[j].w = playerWidth;
    animacion[j].h = playerHeight;
  }

  punteriaRect.x = 0;
  punteriaRect.y = 0;
  punteriaRect.w = game1->gPunteriaTexture.getWidth();
  punteriaRect.h = game1->gPunteriaTexture.getHeight();

  animacionUtilizando = animacion [0];

}

void player::handleEvent( SDL_Event& e)
{
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
          case SDLK_LEFT: //IZQUIERDA
              mVelx -= velocidad;
          break;
          case SDLK_RIGHT: //DERECHA
              mVelx += velocidad;
          break; //derecha

          //Ecuacion y = mx + b --> direccionDisparo = m
          case SDLK_a:
            if (direccionDisparo > 0) direccionDisparo -= 10;
          break;
          case SDLK_d:
            if (direccionDisparo < 180) direccionDisparo += 10;
          break;
          case SDLK_SPACE:
          if (mana > 0)
          {
            balas.push_back ( new bala(direccionDisparo, int(mPosX) - 8, int(mPosY) ) );
            mana--;
          }
          break;
          case SDLK_w: balas.clear();
          break;
          /*default: animacionUtilizando = animacion[5];
          break;*/
        }
    }

    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT: mVelx += velocidad; break;
            case SDLK_RIGHT: mVelx -= velocidad; break;
        }
        animacionUtilizando = animacion [0];
    }
}

void player::move(double timeStep)
{
  mPosX += mVelx*timeStep;

  if( mVelx < 0 )
  {
    if (game1->countedFrames % 300 == 0) i--;
    if (i < 1 or i > 5) i = 5;
    animacionUtilizando = animacion [i];
  }
  else{
    if( mVelx > 0){
      if (game1->countedFrames % 300 == 0) i++;
      if (i < 6 or i > 10) i = 6;
      animacionUtilizando = animacion [i];
    }
  }


  if (mPosX < 0)
  {
    mPosX = 0;
  }
  if ( mPosX > game1->SCREEN_WIDTH - playerWidth )
  {
    mPosX = game1->SCREEN_WIDTH - playerWidth;
  }
}

void player::render1()
{
  switch (skin) {
    case 1:
    game1->gPlayer1Texture.render( int(mPosX), int(mPosY), playerWidth, playerHeight, &animacionUtilizando );
    game1->gScepterTexture.render( int(mPosX) - 15, int(mPosY) + 10, 30, 50, NULL/*, direccionDisparo*2 */);
    break;
    case 2:
    break;
  }

  game1->gPunteriaTexture.render( int(mPosX) - 160, mPosY - 150, 300, 300, &punteriaRect, direccionDisparo-90);
}

void player::sacarVida(int damage)
{
  vida -= damage;
}

void player::render2()
{
	game1->gPlayer2Texture.render(mPosX, mPosY);
}
