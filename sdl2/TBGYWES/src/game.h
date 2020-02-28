#ifndef _GAME_H_
#define _GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "ltexture.h"
#include "player.h"
#include "fps.h"
#include "monster.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

struct Circle
{
  int x, y, r, h, w;
};

class game
{
public:
  game();
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 600;
  const int SCREEN_FPS = 80;
  const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
  SDL_Window* gWindow = NULL;
  SDL_Renderer* gRenderer = NULL;
  Mix_Music *gMusic = NULL;
  TTF_Font *gFont = NULL;
  LTexture gPlayer1Texture;
  LTexture gPlayer2Texture;
  LTexture gBalaTexture;
  LTexture gBala2Texture;
  LTexture gScepterTexture;
  LTexture gPunteriaTexture;
  LTexture gMonstersTexture;
  player* player1;
  monster* monster1;
  std::vector<monster*> monsters;
  void gameLoop();
  void menu();
  SDL_Event e;
  int countedFrames = 0;
  LTimer fpsTimer;
  const int hub = 60;
  const int piso = hub + 30;
  const int pisoAux = hub + 30 + 15;
  LTimer disparos;

private:
  bool pasarNivel;
  LTimer stepTimer;
  LTimer gametime;
  LTimer manatime;
  LTexture gMenuWallTexture;
  LTexture gGameWall1Texture;
  LTexture gBossesTexture;
  LTexture gFontMenuPlayTexture;
  LTexture gFontMenuPlay2Texture;
  LTexture gFontMenuPlay3Texture;
  LTexture gFontMenuPlay4Texture;
  LTexture gFontMenuTitleTexture;
  LTexture gFontMenuPlayBOLDTexture;
  LTexture gFontMenuPlay2BOLDTexture;
  LTexture gFontMenuPlay3BOLDTexture;
  LTexture gFontMenuPlay4BOLDTexture;
  LTexture gFontFirmaTexture;
  LTexture gConstruccionTexture;
  LTexture gLifeTexture;
  LTexture gLifeBarTexture;
  LTexture gInputTextTexture;
  LTexture gPromptTextTexture;
  LTexture gSkin;
  LTexture gAvatar;
  LTexture gPlayButtomTexture;
  std::stringstream timeText;
  std::string inputText = "nombre";
  LTexture gFPSTextTexture;
  SDL_Rect fondo;
  bool init();
  bool loadMedia();
  void close();
  void opciones();
  void iniciarJuego();
  void play();
  void setNivel1();
  void setNivel2();
  void setNivel3();
  void setNivel4();
  bool checkCollision(SDL_Rect& a, Circle& b);
  bool checkCollision2(SDL_Rect a, SDL_Rect b);
  double Distancia2( SDL_Point& p1, SDL_Point& p2 );
  double DistanciaInfinito( SDL_Point& p1, SDL_Point& p2 );
  void renderVidayMana();
  bool quit;
  bool perder;
  bool dosJugadores;
  SDL_Color textColor = { 50, 72, 166 };
  SDL_Color textColorBold = { 22, 37, 112 };
  SDL_Rect mana = { SCREEN_WIDTH/2, SCREEN_HEIGHT - pisoAux/2, 200, 20 };
  SDL_Rect manaContorno = { SCREEN_WIDTH/2, SCREEN_HEIGHT - pisoAux/2, 200, 20 };
  SDL_Rect vida = { 55, SCREEN_HEIGHT - hub/2 - gLifeTexture.getHeight()/2 - 30, 0, 18};
  SDL_Rect PisoYHub = { 0, SCREEN_HEIGHT - pisoAux, SCREEN_WIDTH, pisoAux};
};
#endif
