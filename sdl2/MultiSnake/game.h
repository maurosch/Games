#ifndef _GAME_H_
#define _GAME_H_

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
//#include "font.h"
//#include "text.h"

class game
{
public:
  game();
  ~game();
  void multiplayerServer();
  void multiplayerClient( std::string whereToConnect );
  bool initSDL();
  bool exitSDL();
  bool Get_Quit(){return quit;}
private:
  void Lobby();
  void renderGame();
  void moveSnake();
  void addStrawberry();
  void ClientCheckCollision();
  void ServerCheckCollision();
  void ServerReCheckCollisionFromClient(network::FromClientToServer &Check, int SnakeNum);
  int puerto;
  SDL_Event user_event;
  bool quit;
  SDL_Window* gWindow = NULL;
  SDL_Renderer* gRenderer = NULL;
  const char* CHAT_SAY = "/s";
  const char* CHAT_GOING_DOWN = "/shutdown";
  LTimer tGame;
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
  bool youCanAddAPartAux[7];
  const static int MAX_PLAYERS = 8;
  const static int MAX_SOCKETS = MAX_PLAYERS;
  const static int MAX_CLIENTS = MAX_SOCKETS - 1;
  int cantidadDeConectados;
  SDLNet_SocketSet socketSet;
  TCPsocket serverSocket;
  TCPsocket clientSocket[ MAX_CLIENTS ];
  bool esteSocketEstaUsandose[ MAX_CLIENTS ];
  bool gameOver = false;
};

#endif // _GAME_H_
