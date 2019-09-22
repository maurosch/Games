#include "client.h"

enum eDirecciones{
  izquierda,
  derecha,
  arriba,
  abajo
};

Client::Client()
{
  puerto = 1234;
  quit = false;
  gameOver = false;
  tGame.start();
  fpsTimer.start();
  if (!initSDL())
  {
    quit = true;
  }
  gFPSTextTexture.init(gRenderer, " ", textColorDefault, DefaultFont->GetFont(30), 0, 0, texts::eFast);
}
Client::~Client()
{
  exitSDL();
}

std::string getCommandAndErase( std::string& StringToCut );
void setVariables(network::PositionSnakePart* positionsnakepart, int x, int y);
void addPartSnake(network::Snake *SnakeToAddPart);
void readVariables(network::FromServerToClient &from_server_to_client);
void readVariables(network::Snake snake_to_pass);

void Client::renderGame()
{
  SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
  SDL_RenderClear( gRenderer );

  float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
  timeText.str( "" );
  timeText << (int)avgFPS;
  gFPSTextTexture.loadTextFast ( timeText.str().c_str(), textColorDefault, DefaultFont->GetFont(30) );
  gFPSTextTexture.render();
  countedFrames++;

  SDL_Rect ToDraw;

  SDL_SetRenderDrawColor( gRenderer, 60, 255, 60, 255 );
  for (int i = 0; i < other_players_snakes.strawberry_size(); i++)
  {
    if (other_players_snakes.strawberry(i).life())
    {
    SDL_Rect ToDraw = {other_players_snakes.strawberry(i).pos_x()*SpaceBetweenVerticalLines,
    other_players_snakes.strawberry(i).pos_y()*SpaceBetweenHorizontalLines, SpaceBetweenVerticalLines, SpaceBetweenHorizontalLines};
    SDL_RenderFillRect(gRenderer, &ToDraw);
    }
  }
  //RENDER SNAKES
  for (int i = 0; i < other_players_snakes.snakes_game_size(); i++)
  {
    if (other_players_snakes.snakes_game(i).life())
    {
      SDL_Color ColorUsing;
      switch (other_players_snakes.snakes_game(i).color()) {
        case network::Snake::RED:
          ColorUsing = {255, 48, 48};
        break;
        case network::Snake::GREEN:
          ColorUsing = {0, 205, 102};
        break;
        case network::Snake::BLUE:
          ColorUsing = {72, 118, 255};
        break;
        case network::Snake::GREY:
          ColorUsing = {142, 142, 142};
        break;
        case network::Snake::YELLOW:
          ColorUsing = {255, 193, 37};
        break;
        case network::Snake::ORANGE:
          ColorUsing = {255, 97, 3};
        break;
        case network::Snake::VIOLET:
          ColorUsing = {142, 56, 142};
        break;
        case network::Snake::BROWN:
          ColorUsing = {139, 90, 0};
        break;
        default: std::cout << "Error color?" << std::endl;
      }
      for (int j = 0; j < other_players_snakes.snakes_game(i).positionsnakepart_size(); j++)
      {
        SDL_SetRenderDrawColor( gRenderer, ColorUsing.r, ColorUsing.g, ColorUsing.b, 255 );
        ToDraw = {other_players_snakes.snakes_game(i).positionsnakepart(j).pos_x()*SpaceBetweenVerticalLines,
        other_players_snakes.snakes_game(i).positionsnakepart(j).pos_y()*SpaceBetweenHorizontalLines, SpaceBetweenVerticalLines, SpaceBetweenHorizontalLines};
        SDL_RenderFillRect(gRenderer, &ToDraw);
      }
    }
  }

  SDL_SetRenderDrawColor( gRenderer, 86, 86, 245, 255 );
  for ( int i = SpaceBetweenVerticalLines; i < SCREEN_WIDTH; i += SpaceBetweenVerticalLines )
  {
    SDL_RenderDrawLine ( gRenderer, i, 0, i, SCREEN_HEIGHT );
  }
  for ( int i = SpaceBetweenHorizontalLines; i < SCREEN_HEIGHT; i += SpaceBetweenHorizontalLines )
  {
    SDL_RenderDrawLine ( gRenderer, 0, i, SCREEN_WIDTH, i );
  }
  SDL_RenderPresent( gRenderer );
}

void Client::moveSnake()
{
  switch (direccionSnake) {
    case eDirecciones::izquierda:
      for (int i = snake_game.positionsnakepart_size()-1; i > 0; i--)
      {
        snake_game.mutable_positionsnakepart(i)->set_pos_x(snake_game.positionsnakepart(i-1).pos_x());
        snake_game.mutable_positionsnakepart(i)->set_pos_y(snake_game.positionsnakepart(i-1).pos_y());
      }
      if (snake_game.positionsnakepart(0).pos_x() > 0)
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_x(snake_game.positionsnakepart(0).pos_x() - 1);
      }
      else
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_x(VerticalLines);
      }
    break;

    case eDirecciones::derecha:
      for (int i = snake_game.positionsnakepart_size()-1; i > 0; i--)
      {
        snake_game.mutable_positionsnakepart(i)->set_pos_x(snake_game.positionsnakepart(i-1).pos_x());
        snake_game.mutable_positionsnakepart(i)->set_pos_y(snake_game.positionsnakepart(i-1).pos_y());
      }
      if (snake_game.positionsnakepart(0).pos_x() < VerticalLines)
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_x(snake_game.positionsnakepart(0).pos_x() + 1);
      }
      else
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_x(0);
      }
    break;

    case eDirecciones::arriba:
      for (int i = snake_game.positionsnakepart_size()-1; i > 0; i--)
      {
        snake_game.mutable_positionsnakepart(i)->set_pos_x(snake_game.positionsnakepart(i-1).pos_x());
        snake_game.mutable_positionsnakepart(i)->set_pos_y(snake_game.positionsnakepart(i-1).pos_y());
      }
      if (snake_game.positionsnakepart(0).pos_y() > 0)
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_y(snake_game.positionsnakepart(0).pos_y() - 1);
      }
      else
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_y(HorizontalLines);
      }
    break;

    case eDirecciones::abajo:
      for (int i = snake_game.positionsnakepart_size()-1; i > 0; i--)
      {
        snake_game.mutable_positionsnakepart(i)->set_pos_x(snake_game.positionsnakepart(i-1).pos_x());
        snake_game.mutable_positionsnakepart(i)->set_pos_y(snake_game.positionsnakepart(i-1).pos_y());
      }
      if (snake_game.positionsnakepart(0).pos_y() < HorizontalLines)
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_y(snake_game.positionsnakepart(0).pos_y() + 1);
      }
      else
      {
        snake_game.mutable_positionsnakepart(0)->set_pos_y(0);
      }
    break;
  }
}

void Client::CheckCollision()
{
  from_client_to_server.set_hitwith(network::FromClientToServer::NOTHING);
  for (int i = 0; i < other_players_snakes.snakes_game_size(); i++)
  {
    for (int j = 0; j < other_players_snakes.snakes_game(i).positionsnakepart_size(); j++)
    {
      if (other_players_snakes.snakes_game(i).positionsnakepart(j).pos_x() == snake_game.positionsnakepart(0).pos_x()
        && other_players_snakes.snakes_game(i).positionsnakepart(j).pos_y() == snake_game.positionsnakepart(0).pos_y())
      {
        if ( other_players_snakes.snakes_game(i).id() != snake_game.id() )
        {
          from_client_to_server.set_hitwith(network::FromClientToServer::SNAKE);
          std::cout << "CHOQUE SNAKE" << std::endl;
          snake_game.set_life(false);
        }
        else
        {
          if ( j != 0 )
          {
            from_client_to_server.set_hitwith(network::FromClientToServer::SNAKE);
            std::cout << "CLIENTE CHOQUE SNAKE A SI MISMO" << std::endl;
            snake_game.set_life(false);
          }
        }
      }
    }
  }
  for (int i = 0; i < other_players_snakes.strawberry_size(); i++)
  {
    if (other_players_snakes.strawberry(i).pos_x() == snake_game.positionsnakepart(0).pos_x()
      && other_players_snakes.strawberry(i).pos_y() == snake_game.positionsnakepart(0).pos_y())
    {
      from_client_to_server.set_hitwith(network::FromClientToServer::STRAWBERRY);
      std::cout << "CHOQUE STRAWBERRY" << std::endl;
    }
  }
}

void Client::SearchGame( std::string whereToConnect )
{
  if ( SDLNet_ResolveHost(&ip, whereToConnect.c_str(), puerto) != 0 )
  {
    std::cout << "Failed to resolve the server IP address: " << SDLNet_GetError() << std::endl;
  }

  client = SDLNet_TCP_Open(&ip);
  if (!client)
  {
    std::cout << "Failed to open socket to server: " << SDLNet_GetError() << std::endl;
    return;
  }

  socketSet = SDLNet_AllocSocketSet(1);
  if ( socketSet == NULL )
  {
    std::cout << "Failed to allocate the socket set: " << SDLNet_GetError() << std::endl;
  }

  if (SDLNet_TCP_AddSocket(socketSet, client) == -1)
  {
    std::cout << "Failed to add socket: " << SDLNet_GetError();
  }

  int activeSockets = SDLNet_CheckSockets( socketSet, 5000 );
  if ( activeSockets != 1 )
  //Devuelve cantidad de sockets que activo, como el cliente solo se conecta con el server tiene que ser 1.
  {
    std::cout << "El cliente tiene + de 1 socket o Funcion devolvio Error " << SDLNet_GetError();
    std::cout << std::endl << activeSockets;
    return;
  }
  std::cout << "***GAME FOUND?*** " << std::endl;
}

void Client::Play()
{
  while (!quit and !disconnectedFromServer)
  {
    snake_game.Clear();
    setVariables(snake_game.add_positionsnakepart(), 3, 4);
    snake_game.set_life(true);
    from_client_to_server.set_hitwith(network::FromClientToServer::NOTHING);
    bool WaitingInLobby = true;
    bool firstMessage = true;
    while ( WaitingInLobby and !quit )
    {
      if ( SDLNet_CheckSockets(socketSet, 0) == -1 )
      {
        std::cout << "Error CheckSockets (socketSet vacio?):  " << SDLNet_GetError() <<std::endl;
        quit = true;
      }
      //CHECK IF WE CAN JOIN
      if ( SDLNet_SocketReady(client) != 0)
      {
        network::FirstMessageFromServerToClient tmp;
        int bytesReceived = SDLNet_TCP_Recv(client, buffer, buffer_zise);
        if(bytesReceived <= 0) {
          // TCP Connection is broken. (because of error or closure)
          std::cout << "Connection Broken " << SDLNet_GetError() << std::endl;
          quit = true;
        }
        else
        {
          std::string StringToParse;
          StringToParse.assign(buffer, bytesReceived);
          if (!tmp.ParseFromString(StringToParse))
          {
            std::cout << "Failed to parse" << std::endl;
          }
          else
          {
            if (firstMessage) {
              snake_game.set_color(tmp.color());
              firstMessage = false;
            }
            snake_game.set_id(tmp.idtogive());
            switch (tmp.you_are()) {
              case network::FirstMessageFromServerToClient::WAITING_IN_LOBBY:
                std::cout << "***YOU ARE IN THE LOBBY***" << std::endl;
              break;
              case network::FirstMessageFromServerToClient::WAITING_TO_FINISH_GAME:
                std::cout << "***GAME IS ON THE RUN, WAITING TO FINISH, YOU ARE SPECTATOR***" << std::endl;
                snake_game.set_life(false);
              break;
              case network::FirstMessageFromServerToClient::SERVER_FULL:
                std::cout << "***GAME IS FULL. QUITING...***" << std::endl;
                quit = true;
              break;
              case network::FirstMessageFromServerToClient::START_GAME:
                std::cout << "***STARTING GAME***" << std::endl;
                WaitingInLobby = false;
              break;
            }
            std::cout << "Color dado: " << tmp.color() << std::endl;
            std::cout << "ROJO " << network::Snake::RED << std::endl;
            std::cout << "VERDE " << network::Snake::GREEN << std::endl;
          }
        }
      }
    }

    SDL_StartTextInput();
    std::string inputText = "";
    from_client_to_server.mutable_snakeclient()->CopyFrom(snake_game);
    while ( !quit and !gameOver )
    {
      SDLNet_CheckSockets(socketSet, 0);

      if ( SDLNet_SocketReady(client) != 0 )
      {
        int bytesReceived = SDLNet_TCP_Recv( client, buffer, buffer_zise );
        if (bytesReceived <= 0)
        {
          std::cout << "Connection Broken" << std::endl;
          quit = true;
        }
        else
        {
          std::string StringToParse;
          StringToParse.assign(buffer, bytesReceived);
          printf("bytes llegados:%d \n", bytesReceived);
          if ( StringToParse[0] == '/' )
          {
            std::string command = getCommandAndErase( StringToParse );
            if ( command == CHAT_SAY )
              std::cout << StringToParse << std::endl;
            else
            {
              if ( command == CHAT_GOING_DOWN )
              {
                std::cout << "Server is going down. Disconnecting..." << std::endl;
                gameOver = true;
              }
            }

          }
          else
          {
            if (!other_players_snakes.ParseFromString(StringToParse))
            {
              std::cout << "Failed to parse" << std::endl;
            }
            else
            {
              readVariables(other_players_snakes);
              if (other_players_snakes.youcanaddapart())
              {
                std::cout << "PODEMOS AGREGAR UNA PARTE" << std::endl;
                addPartSnake(&snake_game);
                other_players_snakes.set_youcanaddapart(false);
              }
              if (other_players_snakes.gameover())
              {
                gameOver = true;
              }
            }
          }
        }
      }

      while( SDL_PollEvent( &user_event ) != 0 )
      {
        if ( user_event.type == SDL_TEXTINPUT )
        {
          inputText += user_event.text.text;
        }
        if (user_event.type == SDL_KEYDOWN && user_event.key.repeat == 0)
        {
          switch (user_event.key.keysym.sym) {
            case SDLK_RETURN:
            if ( inputText != "" )
            {
              SDLNet_TCP_Send( client, inputText.c_str(), inputText.length()+1 );
              if ( inputText == "/shutdown" )
              {
                gameOver = true;
              }
              inputText = "";
            }
            break;
            case SDLK_LEFT:
              if (snake_game.positionsnakepart_size() > 1)
              {
                if (snake_game.positionsnakepart(1).pos_y() != snake_game.positionsnakepart(0).pos_y()
                or snake_game.positionsnakepart(1).pos_x() > snake_game.positionsnakepart(0).pos_x())
                {
                  direccionSnake = eDirecciones::izquierda;
                }
              }
              else
              {
                direccionSnake = eDirecciones::izquierda;
              }
            break;
            case SDLK_RIGHT:
              if (snake_game.positionsnakepart_size() > 1)
              {
                if (snake_game.positionsnakepart(1).pos_y() != snake_game.positionsnakepart(0).pos_y()
                or snake_game.positionsnakepart(1).pos_x() < snake_game.positionsnakepart(0).pos_x())
                {
                  direccionSnake = eDirecciones::derecha;
                }
              }
              else
              {
                direccionSnake = eDirecciones::derecha;
              }
            break;
            case SDLK_UP:
              if (snake_game.positionsnakepart_size() > 1)
              {
                if (snake_game.positionsnakepart(1).pos_x() != snake_game.positionsnakepart(0).pos_x()
                or snake_game.positionsnakepart(1).pos_y() > snake_game.positionsnakepart(0).pos_y())
                {
                  direccionSnake = eDirecciones::arriba;
                }
              }
              else
              {
                direccionSnake = eDirecciones::arriba;
              }
            break;
            case SDLK_DOWN:
              if (snake_game.positionsnakepart_size() > 1)
              {
                if (snake_game.positionsnakepart(1).pos_x() != snake_game.positionsnakepart(0).pos_x()
                or snake_game.positionsnakepart(1).pos_y() < snake_game.positionsnakepart(0).pos_y())
                {
                  direccionSnake = eDirecciones::abajo;
                }
              }
              else
              {
                direccionSnake = eDirecciones::abajo;
              }
            break;
            case SDLK_LSHIFT: timeStepBetweenSnakeMoves /= 2; break;
          }
        }
        if (user_event.type == SDL_KEYUP && user_event.key.keysym.sym == SDLK_LSHIFT)
        {
          timeStepBetweenSnakeMoves*=2;
        }
      }

      /*std::string StringToSerialize;
      if (!from_client_to_server.SerializeToString(&StringToSerialize))
      {
        std::cout << " Failed to serialize " << std::endl;
      }
      SDLNet_TCP_Send( client, StringToSerialize.c_str(), StringToSerialize.size() );*/
      renderGame();
      if ( tGame.getTicks() > timeStepBetweenSnakeMoves && snake_game.life() )
      {
        moveSnake();
        CheckCollision();
        from_client_to_server.mutable_snakeclient()->CopyFrom(snake_game);

        std::string StringToSerialize;
        if (!from_client_to_server.SerializeToString(&StringToSerialize))
        {
          std::cout << "/* Failed to serialize */" << std::endl;
        }
        SDLNet_TCP_Send( client, StringToSerialize.c_str(), StringToSerialize.size() );
        tGame.start();
      }
    }
    SDL_StopTextInput();
    if (!quit)
    {
      int Ganador;
      for (int i = 0; i < other_players_snakes.snakes_game_size(); i++)
      {
        if (other_players_snakes.snakes_game(i).life())
        {
          Ganador = i;
        }
      }
      std::cout << "GANADOR PLAYER: " << Ganador << std::endl;
    }
  }
}

bool Client::initSDL()
{
  //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{

    //Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

    SDL_DisplayMode ventana;
    SDL_GetDesktopDisplayMode(0, &ventana);
    SCREEN_HEIGHT = ventana.h;
    SCREEN_WIDTH = ventana.w;
		  //Create window
		gWindow = SDL_CreateWindow( "MultiSnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ventana.w, ventana.h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED );
    SDL_Surface* IconSurface = IMG_Load( "icon.png" );
		SDL_SetWindowIcon ( gWindow, IconSurface );
		SDL_FreeSurface( IconSurface );
    SpaceBetweenVerticalLines = SCREEN_WIDTH/VerticalLines;
    SpaceBetweenHorizontalLines = SCREEN_HEIGHT/HorizontalLines;
    //std::cout << " " << << " " << << " " << std::endl;

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
					 if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
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
              else
              {
                DefaultFont = new MyFont ("data/LemonMilk.otf", 50);
                //MusicaJuego = new Music();
                SDLNet_Init();
              }
					 }
				}
			}
		}
	}
	return success;
}

bool Client::exitSDL()
{
  SDLNet_TCP_Close(client);

  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );

  Mix_Quit();
  IMG_Quit();
  SDLNet_Quit();
  SDL_Quit();
}
