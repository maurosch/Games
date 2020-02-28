#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include "network.protoc.pb.h"
#include <iostream>
//#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <thread>
//#include "ltexture.h"
#include "fps.h"
//#include "player.h"
#include "music.h"
#include <stdlib.h>
#include <time.h>
#include "font.h"
#include "text.h"

class Client
{
public:
  Client();
  ~Client();
  void Start( std::string whereToConnect );
  void SearchGame( std::string whereToConnect );
  void Play();
  bool initSDL();
  bool exitSDL();
  bool Get_Quit(){return quit;}
private:
  void renderGame();
  void moveSnake();
  void CheckCollision();
  int puerto;
  SDL_Event user_event;
  bool quit;
  bool disconnectedFromServer;
  bool gameOver;
  SDL_Window* gWindow = NULL;
  SDL_Renderer* gRenderer = NULL;
  MyFont* DefaultFont;
  std::stringstream timeText;
  const char* CHAT_SAY = "/s";
  const char* CHAT_GOING_DOWN = "/shutdown";
  LTimer tGame;
  LTimer fpsTimer;
  text gFPSTextTexture;
  int countedFrames = 0;
  network::Snake snake_game;
  network::FromServerToClient other_players_snakes;
  network::FromClientToServer from_client_to_server;
  int direccionSnake = 2;
  const int VerticalLines = 80;
  const int HorizontalLines = 45;
  int SpaceBetweenVerticalLines, SpaceBetweenHorizontalLines;
  int SCREEN_WIDTH = 1600;
  int SCREEN_HEIGHT = 900;
  int timeStepBetweenSnakeMoves = 80;
  const static int buffer_zise = 700;
  char buffer[buffer_zise];
  TCPsocket client;
  IPaddress ip;
  SDLNet_SocketSet socketSet;
  SDL_Color textColorDefault {0, 0, 0, 255};
};

#endif //_CLIENT_H_
