#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#endif
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#endif
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include "game.h"
#include "player.h"
#include "monster.h"
#include <algorithm>

extern game* game1;

game::game(){
  quit = false;
  perder = false;
  dosJugadores = false;
  pasarNivel = false;
  fpsTimer.start();
}

void game::gameLoop()
{
  init();
  loadMedia();
  while (!quit)
  {
    menu();
  }

  close();
}

void game::menu()
{
  fondo.w = gMenuWallTexture.getWidth();
  fondo.h = gMenuWallTexture.getHeight();
  fondo.x = 0;
  fondo.y = 0;
  int seleccion = 1;

  gFontMenuTitleTexture.loadFromRenderedText( "{\\The\"Buggiest\"Game\"You\"Will\"Ever\"See\\}", textColor );
  gFontMenuPlayTexture.loadFromRenderedText( "^PLAY\"-\"1\"Jugador*", textColor );
  gFontMenuPlay2Texture.loadFromRenderedText( "^PLAY\"-\"2\"Jugadores*", textColor );
  gFontMenuPlay3Texture.loadFromRenderedText( "^Opciones*", textColor );
  gFontMenuPlay4Texture.loadFromRenderedText( "^Salir*", textColor );
  gFontFirmaTexture.loadFromRenderedText( "^SCHIAVINATO\"MAURO*", textColor );
  gFontMenuPlayBOLDTexture.loadFromRenderedText( "^PLAY\"-\"1\"Jugador*", textColorBold );
  gFontMenuPlay2BOLDTexture.loadFromRenderedText( "^PLAY\"-\"2\"Jugadores*", textColorBold );
  gFontMenuPlay3BOLDTexture.loadFromRenderedText( "^Opciones*", textColorBold );
  gFontMenuPlay4BOLDTexture.loadFromRenderedText( "^Salir*", textColorBold );
  gPromptTextTexture.loadFromRenderedText( "Nombre", textColorBold );

  if( Mix_PlayingMusic() == 0 )
  {
    //Play the music
    Mix_PlayMusic( gMusic, -1 );
  }
  while(!quit)
  {
    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 )
    {
      //User requests quit
      if( e.type == SDL_QUIT )
      {
        quit = true;
      }
      if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
        {
            switch( e.key.keysym.sym )
            {
              case SDLK_UP:
                if (seleccion > 1) {
                  seleccion--;
                }
              break;
              case SDLK_DOWN:
                if (seleccion < 4) {
                  seleccion++;
                }
              break;
              case SDLK_RETURN:
                switch (seleccion) {
                  case 1:
                  game1->iniciarJuego();
                  break;
                  /*case 2:
                  dosJugadores = true;
                  game1->iniciarJuego();
                  break;
                  case 3:
                  game1->opciones();
                  break;*/
                  case 4: quit= true;
                  break;
                }
              break;
            }
        }
    }
    SDL_SetRenderDrawColor( game1->gRenderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( game1->gRenderer );

    gMenuWallTexture.render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, &fondo);
    gFontMenuTitleTexture.render(SCREEN_WIDTH/2 - gFontMenuTitleTexture.getWidth()/2, SCREEN_HEIGHT/10);

    switch (seleccion)
    {
      case 1:
      gFontMenuPlayBOLDTexture.render(SCREEN_WIDTH/3 - gFontMenuPlayTexture.getWidth()/2, SCREEN_HEIGHT/10*2);
      gFontMenuPlay2Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay2Texture.getWidth()/2, SCREEN_HEIGHT/10*3);
      gFontMenuPlay3Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay3Texture.getWidth()/2, SCREEN_HEIGHT/10*4);
      gFontMenuPlay4Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay4Texture.getWidth()/2, SCREEN_HEIGHT/10*5);
      break;
      case 2:
      gFontMenuPlayTexture.render(SCREEN_WIDTH/3 - gFontMenuPlayTexture.getWidth()/2, SCREEN_HEIGHT/10*2);
      gFontMenuPlay2BOLDTexture.render(SCREEN_WIDTH/3 - gFontMenuPlay2Texture.getWidth()/2, SCREEN_HEIGHT/10*3);
      gFontMenuPlay3Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay3Texture.getWidth()/2, SCREEN_HEIGHT/10*4);
      gFontMenuPlay4Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay4Texture.getWidth()/2, SCREEN_HEIGHT/10*5);
      break;
      case 3:
      gFontMenuPlayTexture.render(SCREEN_WIDTH/3 - gFontMenuPlayTexture.getWidth()/2, SCREEN_HEIGHT/10*2);
      gFontMenuPlay2Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay2Texture.getWidth()/2, SCREEN_HEIGHT/10*3);
      gFontMenuPlay3BOLDTexture.render(SCREEN_WIDTH/3 - gFontMenuPlay3Texture.getWidth()/2, SCREEN_HEIGHT/10*4);
      gFontMenuPlay4Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay4Texture.getWidth()/2, SCREEN_HEIGHT/10*5);
      break;
      case 4:
      gFontMenuPlayTexture.render(SCREEN_WIDTH/3 - gFontMenuPlayTexture.getWidth()/2, SCREEN_HEIGHT/10*2);
      gFontMenuPlay2Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay2Texture.getWidth()/2, SCREEN_HEIGHT/10*3);
      gFontMenuPlay3Texture.render(SCREEN_WIDTH/3 - gFontMenuPlay3Texture.getWidth()/2, SCREEN_HEIGHT/10*4);
      gFontMenuPlay4BOLDTexture.render(SCREEN_WIDTH/3 - gFontMenuPlay4Texture.getWidth()/2, SCREEN_HEIGHT/10*5);
    }

    gConstruccionTexture.render(SCREEN_WIDTH/3 - gFontMenuPlay3Texture.getWidth()/4, SCREEN_HEIGHT/10*3, 50, 50);
    gConstruccionTexture.render(SCREEN_WIDTH/3 - gFontMenuPlay3Texture.getWidth()/4, SCREEN_HEIGHT/10*4, 50, 50);
    SDL_RenderPresent( game1->gRenderer );
  }
}

void game::iniciarJuego()
{
  int skin = 1;
  bool entrar = false;
  SDL_Rect personaje;
  personaje.x = 0;
  personaje.y = 0;
  personaje.w = 40;
  personaje.h = 60;
  gSkin.loadFromRenderedText( "SKIN", textColor );
  SDL_StartTextInput();

  while(!quit and !entrar)
  {
    bool renderText = true;

    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 )
    {
      //User requests quit
      if( e.type == SDL_QUIT )
      {
        quit = true;
      }
      if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
        {
            switch( e.key.keysym.sym )
            {
              case SDLK_LEFT:
                if (skin > 1) {
                  skin--;
                }
              break;
              case SDLK_RIGHT:
                if (skin < 2) {
                  skin++;
                }
              break;
              case SDLK_RETURN:
              entrar = true;
              break;
              case SDLK_BACKSPACE:
              if (inputText.length() > 0)
              {
                inputText.pop_back();
                renderText = true;
              }
              break;
            }
        }

      //Special text input event
      else if( e.type == SDL_TEXTINPUT and inputText.size() < 9 )
      {
        //Not copy or pasting
        //if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
      //  {
          //Append character
          inputText += e.text.text;
          renderText = true;
      //  }
      }
    }

    SDL_SetRenderDrawColor( game1->gRenderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( game1->gRenderer );

    gMenuWallTexture.render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, &fondo);
    gFontMenuTitleTexture.render(SCREEN_WIDTH/2 - gFontMenuTitleTexture.getWidth()/2, SCREEN_HEIGHT/10);

    if( renderText ) {
      //Text is not empty
      if( inputText != "" )
      {
        //Render new text
        gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
      }
      //Text is empty
      else
      {
        //Render space texture
        gInputTextTexture.loadFromRenderedText( " ", textColor );
      }
    }

    switch (skin)
    {
      case 1:
      gPlayer1Texture.render(SCREEN_WIDTH/3*2, SCREEN_HEIGHT/10*3, 40, 60, &personaje);

      break;
      case 2:
      gPlayer2Texture.render(SCREEN_WIDTH/3*2, SCREEN_HEIGHT/10*3, 40, 60, &personaje);

      break;
    }

    gPromptTextTexture.render( SCREEN_WIDTH/3 - gPromptTextTexture.getWidth()/2, SCREEN_HEIGHT/10*2 );
    gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, SCREEN_HEIGHT/10*2 );
    gFontMenuPlayTexture.render(SCREEN_WIDTH/3 - gFontMenuPlayTexture.getWidth()/2, SCREEN_HEIGHT/10*3);

    SDL_RenderPresent( game1->gRenderer );
  }

  SDL_StopTextInput();

  player1 = new player(1);
  //if (dosJugadores) player player2(2);

  if (!quit and !perder) {
    setNivel1();
    play();
  }
  //nivel1();
  /*if (!quit and !perder) nivel2();
  if (!quit and !perder) nivel3();
  if (!quit and !perder) nivel4();*/

  delete player1;
  delete monster1;

  //if (perder) WALLPAPER PERDER
}

void game::opciones(){}

void game::setNivel1()
{
  gametime.start();
  manatime.start();
  disparos.start();
  monsters.push_back ( new monster(1, 0, 0) );
  //monster1 = new monster(1, 0, 0);
}

void game::play()
{
  bool seguir = false;
  textColor = { 255, 255, 255 };
  fondo.w = gGameWall1Texture.getWidth();
  fondo.h = gGameWall1Texture.getHeight();
  gFont = TTF_OpenFont( "data/8-BIT.ttf", 10 );
  gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
  int AuxPosYNombre = SCREEN_HEIGHT - hub/2 - gLifeTexture.getHeight()/2 + 5;
  int AuxPosYlife = SCREEN_HEIGHT - hub/2 - gLifeTexture.getHeight()/2;
  SDL_Rect player;
  SDL_Rect bala;
  SDL_Rect monstruos[monsters.size()];
  for ( int j = 0; j != monsters.size(); ++j )
  {
    monstruos[j].w = 34;
    monstruos[j].h = 32;
  }
  player.w = player1->playerWidth;
  player.h = player1->playerHeight;
  bala.w = 10;
  bala.h = 10;
  while (!quit and !perder and !pasarNivel){
    while( SDL_PollEvent( &e ) != 0 )
    {
      //User requests quit
      if( e.type == SDL_QUIT )
      {
        quit = true;
      }
      player1->handleEvent(e);
    }
    float timeStep = stepTimer.getTicks() / 1000.f;
    float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );

    stepTimer.start();

    if( avgFPS > 2000000 )
    {
      avgFPS = 0;
    }
    timeText.str( "" );
    timeText << (int)avgFPS;

    SDL_SetRenderDrawColor( game1->gRenderer, 137, 252, 229, 0x00 );
    SDL_RenderClear( game1->gRenderer );

    gGameWall1Texture.render(0, 0, SCREEN_WIDTH + 100, SCREEN_HEIGHT, &fondo);

    if( !gFPSTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
    {
      printf( "Unable to render FPS texture!\n" );

    }
    gFPSTextTexture.render( SCREEN_WIDTH - gFPSTextTexture.getWidth(), 0 );


    SDL_SetRenderDrawColor( game1->gRenderer, 255, 255, 255, 0xFF );
    SDL_RenderFillRect( game1->gRenderer, &(game1->PisoYHub) );

    gLifeTexture.render(5, AuxPosYlife, 212, 52);
    gAvatar.render(5, AuxPosYlife, 212, 52);
    gInputTextTexture.render( 60, AuxPosYNombre);


    player1->move(timeStep);
    player1->render1();
    player.x = player1->PosX();
    player.y = player1->PosY();
    renderVidayMana();

    for( int i = 0; i != player1->balas.size(); ++i)
    {
      if(player1->balas[i]->estaVivo())
      {
        player1->balas[i]->mover(timeStep);
        player1->balas[i]->render();

        bala.x = player1->balas[i]-> posX();
        bala.y = player1->balas[i]-> posY();
        for( int j = 0; j != monsters.size(); ++j )
        {
          monstruos[j].x = monsters[j]->posX();
          monstruos[j].y = monsters[j]->posY();
          if (checkCollision2 ( monstruos[j], bala ))
          {
            monsters[j]->matar();
            std::cout << "Monstruo Muerto" << std::endl;
            player1->balas[i]->matar();
          }
        }
      }
    }

    seguir = false;
    for ( int i = 0; i != monsters.size(); ++i )
    {
      if (monsters[i]->estaVivo()){
        monsters[i]->mover(timeStep);
        monsters[i]->render();

        seguir = true;

        for( int j = 0; j != monsters[i]->balas.size(); ++j )
        {
          if(monsters[i]->balas[j]->estaVivo())
          {
            monsters[i]->balas[j]->mover(timeStep);
            monsters[i]->balas[j]->render2();
            bala.x = monsters[i]->balas[j]->posX();
            bala.y = monsters[i]->balas[j]->posY();
            player.x = player1->PosX();
            player.y = player1->PosY();
            if ( checkCollision2 ( player, bala ) )
            {
              player1->sacarVida(1);
              monsters[i]->balas[j]->matar();
            }
          }
        }
      }
    }
    if (!seguir) pasarNivel = true;

    //AGREGAR MANA CADA 2 SEG
    if(int(manatime.getTicks()) > 2000) if(player1->mana < 10) {player1->mana += 1; manatime.start();}

    SDL_RenderPresent( game1->gRenderer );
    ++countedFrames;
  }
}

void game::renderVidayMana()
{
  switch (player1->mana) {
    case 0:
      game1->mana.w = 10;
    break;
    case 1:
      game1->mana.w = 20;
    break;
    case 2:
      game1->mana.w = 40;
    break;
    case 3:
      game1->mana.w = 60;
    break;
    case 4:
      game1->mana.w = 80;
    break;
    case 5:
      game1->mana.w = 100;
    break;
    case 6:
      game1->mana.w = 120;
    break;
    case 7:
      game1->mana.w = 140;
    break;
    case 8:
      game1->mana.w = 160;
    break;
    case 9:
      game1->mana.w = 180;
    break;
    case 10:
      game1->mana.w = 200;
    break;
  }
  SDL_SetRenderDrawColor( game1->gRenderer, 25, 85, 250, 0xFF );
  SDL_RenderFillRect( game1->gRenderer, &(game1->mana) );

  switch (player1->saberVida()) {
    case 1:
      game1->vida.w = 5;
      SDL_SetRenderDrawColor( game1->gRenderer, 255, 0, 50, 0xFF );
    break;
    case 2:
      game1->vida.w = 20;
      SDL_SetRenderDrawColor( game1->gRenderer, 255, 102, 0, 0xFF );
    break;
    case 3:
      game1->vida.w = 40;
      SDL_SetRenderDrawColor( game1->gRenderer, 225, 154, 0, 0xFF );
    break;
    case 4:
      game1->vida.w = 60;
      SDL_SetRenderDrawColor( game1->gRenderer, 255, 180, 0, 0xFF );
    break;
    case 5:
      game1->vida.w = 80;
      SDL_SetRenderDrawColor( game1->gRenderer, 250, 255, 0, 0xFF );
    break;
    case 6:
      game1->vida.w = 100;
      SDL_SetRenderDrawColor( game1->gRenderer, 205, 255, 0, 0xFF );
    break;
    case 7:
      game1->vida.w = 120;
      SDL_SetRenderDrawColor( game1->gRenderer, 205, 255, 0, 0xFF );
    break;
    case 8:
      game1->vida.w = 130;
      SDL_SetRenderDrawColor( game1->gRenderer, 205, 255, 0, 0xFF );
    break;
    case 9:
      game1->vida.w = 140;
      SDL_SetRenderDrawColor( game1->gRenderer, 154, 255, 0, 0xFF );
    break;
    case 10:
      game1->vida.w = 150;
      SDL_SetRenderDrawColor( game1->gRenderer, 0, 255, 0, 0xFF );
    break;
    case 0:
      perder = true;
    break;
    defaul: std::cout << "Error vida distinta de los valores elegidos" << std::endl;
    break;
  }
    SDL_RenderFillRect( game1->gRenderer, &(game1->vida) );
    SDL_SetRenderDrawColor( game1->gRenderer, 0, 0, 0, 0xFF );
    SDL_RenderDrawRect( game1->gRenderer, &(game1->manaContorno) );

}

bool game::checkCollision2 (SDL_Rect a, SDL_Rect b)
{
    int Izquierda1 = a.x;
    int Derecha1 = a.x + a.w;
    int Arriba1 = a.y;
    int Abajo1 = a.y + a.h;
    int Izquierda2 = b.x;
    int Derecha2 = b.x + b.w;
    int Arriba2 = b.y;
    int Abajo2 = b.y + b.h;

    if( Abajo1 <  Arriba2 )
    return false;

    if( Arriba1 >  Abajo2 )
    return false;

    if( Derecha1 < Izquierda2 )
    return false;

    if( Izquierda1 > Derecha2 )
    return false;

    return true;
}


bool game::checkCollision(SDL_Rect& b, Circle& a)
{
 //Closest point on collision box
 int cX, cY;
 //Find closest x offset
 if( a.x < b.x )
 {
   cX = b.x;
 }
 else
  if( a.x > b.x + b.w )
  {
    cX = b.x + b.w;
  }
  else
  {
    cX = a.x;
  }

 //Find closest y offset
  if( a.y < b.y )
  {
   cY = b.y;
  }
  else
  if( a.y > b.y + b.h )
  {
   cY = b.y + b.h;
  }
  else
  {
   cY = a.y;
  }
  //If the closest point is inside the circle
  SDL_Point p1 = {a.x, a.y};
  SDL_Point p2 = {cX, cY};
  if( game1->Distancia2( p1, p2 ) < a.r * a.r )
  {
    //This box and the circle have collided
    return true;
  }
  //If the shapes have not collided
  return false;
}

double game::Distancia2( SDL_Point& p1, SDL_Point& p2 )
{
  int deltaX = p2.x - p1.x;
  int deltaY = p2.y - p1.y;
  return sqrt(deltaX * deltaX + deltaY * deltaY);
}

double game::DistanciaInfinito( SDL_Point& p1, SDL_Point& p2 )
{
  int deltaX = abs(p2.x - p1.x);
  int deltaY = abs(p2.y - p1.y);
  return std::max(deltaX, deltaY);
}

bool game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "TBGYWES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				else {
					 //Initialize SDL_mixer
					 if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
					 {
						 printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
						 success = false;
					 }
					 else
					 {
						  //Initialize SDL_ttf
							if( TTF_Init() == -1 )
							{
								printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
								success = false;
							}
					 }
				}
			}
		}
	}
	return success;
}

bool game::loadMedia()
{
	bool success = true;

	gMusic = Mix_LoadMUS( "data/runaway.mp3" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

  if( !gConstruccionTexture.loadFromFile( "data/construccion.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

	if( !gPlayer1Texture.loadFromFile( "data/player1.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}

  if( !gPlayer2Texture.loadFromFile( "data/player2.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

  if( !gLifeTexture.loadFromFile( "data/life.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

  if( !gLifeBarTexture.loadFromFile( "data/life_bar.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

  if( !gAvatar.loadFromFile( "data/avatar1.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

  if( !gMonstersTexture.loadFromFile( "data/monsters.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

	if( !gBossesTexture.loadFromFile( "data/bosses.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}

  if( !gBalaTexture.loadFromFile( "data/bala.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

  if( !gBala2Texture.loadFromFile( "data/bala.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }
  gBala2Texture.setColor( 50, 105, 190 );

  if( !gPunteriaTexture.loadFromFile( "data/punteria.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }
  gPunteriaTexture.setBlendMode( SDL_BLENDMODE_BLEND );
  gPunteriaTexture.setAlpha( 127 );

  if( !gScepterTexture.loadFromFile( "data/scepter.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

  if( !gMenuWallTexture.loadFromFile( "data/wallpaper.png" ) )
	{
		printf( "Failed to load player texture!\n" );
		success = false;
	}

  if( !gGameWall1Texture.loadFromFile( "data/gameWallpaper1.jpg" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

  if( !gPlayButtomTexture.loadFromFile( "data/playButtom.png" ) )
  {
    printf( "Failed to load player texture!\n" );
    success = false;
  }

	gFont = TTF_OpenFont( "data/yorkwhiteletter.ttf", 50 );
	if( gFont == NULL )
	{
		printf( "No se pudo cargar la fuente de texto: ! SDL_ttf Error: %s\n", TTF_GetError() );
    success = false;
	}

	return success;
}

void game::close()
{
	//Free loaded images
	gPlayer1Texture.free();
  gPlayer2Texture.free();
	gBossesTexture.free();
  gFontMenuPlayTexture.free();
  gFontMenuPlay2Texture.free();
  gFontMenuPlay3Texture.free();
  gFontMenuPlay4Texture.free();
  gFontMenuPlayBOLDTexture.free();
  gFontMenuPlay2BOLDTexture.free();
  gFontMenuPlay3BOLDTexture.free();
  gFontMenuPlay4BOLDTexture.free();
  gMenuWallTexture.free();
  gGameWall1Texture.free();
  gFontMenuTitleTexture.free();
  gFontFirmaTexture.free();
  gConstruccionTexture.free();
  gLifeTexture.free();
  gLifeBarTexture.free();
  gInputTextTexture.free();
  gPromptTextTexture.free();
  gSkin.free();
  gAvatar.free();
  gPlayButtomTexture.free();
	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
  Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
