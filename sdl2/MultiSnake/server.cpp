#include "server.h"

enum eDirecciones{
  izquierda,
  derecha,
  arriba,
  abajo
};

Server::Server()
{
  puerto = 1234;
  quit = false;
  srand (time(NULL));
  tGame.start();
  fpsTimer.getTicks();
  for (int i = 0; i < 8; i++)
  {
    youCanAddAPartAux[i] = false;
  }
  if (!initSDL())
  {
    quit = true;
  }
  gFPSTextTexture.init(gRenderer, " ", textColorDefault, DefaultFont->GetFont(30), 0, 0, texts::eFast);
}
Server::~Server()
{
  exitSDL();
}

std::string getCommandAndErase( std::string& StringToCut )
{
  std::stringstream Command (StringToCut);
  std::string aux;
  Command >> aux;
  StringToCut.erase(0, aux.length()+1);
  return aux;
}

void setVariables(network::PositionSnakePart* positionsnakepart, int x, int y)
{
  positionsnakepart->set_pos_x(x);
  positionsnakepart->set_pos_y(y);
}

void addPartSnake(network::Snake *SnakeToAddPart)
{
  if ( SnakeToAddPart->add_positionsnakepart() != NULL )
  {
    int iPart = SnakeToAddPart->positionsnakepart_size() -1;
    setVariables(SnakeToAddPart->mutable_positionsnakepart(iPart), SnakeToAddPart->positionsnakepart(iPart-1).pos_x(),
    SnakeToAddPart->positionsnakepart(iPart-1).pos_y());
  }
  else
  {
    std::cout << "Failed to add part" << std::endl;
  }
}

void readVariables(network::FromServerToClient &from_server_to_client)
{
  std::cout << "Hay " << from_server_to_client.snakes_game_size() << " Snakes en el paquete recibido" << std::endl;
  for (int i = 0; i < from_server_to_client.snakes_game_size(); i++)
  {
    for (int j = 0; j < from_server_to_client.snakes_game(i).positionsnakepart_size(); j++)
    {
      std::cout << "Snake " << i << " x: " << from_server_to_client.snakes_game(i).positionsnakepart(j).pos_x() << std::endl;
      std::cout << "Snake " << i << " y: " << from_server_to_client.snakes_game(i).positionsnakepart(j).pos_y() << std::endl;
    }
  }
}

void readVariables(network::Snake snake_to_pass)
{
  for (int i = 0; i < snake_to_pass.positionsnakepart_size(); i++)
  {
    std::cout << snake_to_pass.positionsnakepart(i).pos_x() << std::endl;
    std::cout << snake_to_pass.positionsnakepart(i).pos_y() << std::endl;
  }
}

void Server::renderGame()
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

void Server::moveSnake()
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

void Server::addStrawberry()
{
  int ReusarStrawBerryMuerta = -1;
  for (int i = 0; i < other_players_snakes.strawberry_size(); i++)
  {
    if (!other_players_snakes.strawberry(i).life())
    {
      ReusarStrawBerryMuerta = i;
      break;
    }
  }
  if (ReusarStrawBerryMuerta != -1)
  {
    other_players_snakes.mutable_strawberry(ReusarStrawBerryMuerta)->set_pos_x(rand() % VerticalLines);
    other_players_snakes.mutable_strawberry(ReusarStrawBerryMuerta)->set_pos_y(rand() % HorizontalLines);
    other_players_snakes.mutable_strawberry(ReusarStrawBerryMuerta)->set_life(true);
  }
  else
  {
    if (other_players_snakes.strawberry_size() < 10)
    {
    network::StrawBerry* StrawBerryToSet = other_players_snakes.add_strawberry();
    StrawBerryToSet->set_pos_x(rand() % VerticalLines);
    StrawBerryToSet->set_pos_y(rand() % HorizontalLines);
    StrawBerryToSet->set_id(other_players_snakes.strawberry(other_players_snakes.strawberry_size()-1).id());
    StrawBerryToSet->set_life(true);
    }
  }
}

void Server::CheckCollision()
{
  for (int i = 0; i < other_players_snakes.snakes_game_size(); i++)
  {
    for (int j = 0; j < other_players_snakes.snakes_game(i).positionsnakepart_size(); j++)
    {
      if (other_players_snakes.snakes_game(i).positionsnakepart(j).pos_x() == snake_game.positionsnakepart(0).pos_x()
        && other_players_snakes.snakes_game(i).positionsnakepart(j).pos_y() == snake_game.positionsnakepart(0).pos_y())
      {
        if ( i != snake_game.id() )
        {
          snake_game.set_life(false);
          other_players_snakes.mutable_snakes_game(0)->CopyFrom(other_players_snakes.snakes_game(other_players_snakes.snakes_game_size()-1));
          other_players_snakes.mutable_snakes_game()->RemoveLast();
          std::cout << "CHOQUE SNAKE" << std::endl;
        }
        else
        {
          if ( j != 0 )
          {
            snake_game.set_life(false);
            other_players_snakes.mutable_snakes_game(0)->CopyFrom(other_players_snakes.snakes_game(other_players_snakes.snakes_game_size()-1));
            other_players_snakes.mutable_snakes_game()->RemoveLast();
            std::cout << "SERVER SE CHOQUE A SI MISMO" << std::endl;
          }
        }
        if (other_players_snakes.snakes_game_size() < 2)
        {
          other_players_snakes.set_gameover(true);
          gameOver = true;
          std::cout << "GAME OVER" << std::endl;
        }
      }

    }
  }
  for (int i = 0; i < other_players_snakes.strawberry_size(); i++)
  {
    if (other_players_snakes.strawberry(i).pos_x() == snake_game.positionsnakepart(0).pos_x()
      && other_players_snakes.strawberry(i).pos_y() == snake_game.positionsnakepart(0).pos_y())
    {
      other_players_snakes.mutable_strawberry(i)->set_life(false);
      addPartSnake(&snake_game);
      std::cout << "Server Choco StrawBerry" << std::endl;
    }
  }
}

void Server::ServerReCheckCollisionFromClient(network::FromClientToServer &Check, int SnakeNum)
{
  switch (Check.hitwith()) {
    case network::FromClientToServer::SNAKE:
      other_players_snakes.mutable_snakes_game(SnakeNum)->set_life(false);
      other_players_snakes.mutable_snakes_game(SnakeNum)->CopyFrom(other_players_snakes.snakes_game(other_players_snakes.snakes_game_size()-1));
      other_players_snakes.mutable_snakes_game()->RemoveLast();

    break;
    case network::FromClientToServer::STRAWBERRY:
      for (int i = 0; i < other_players_snakes.strawberry_size(); i++)
      {
        if (other_players_snakes.strawberry(i).life())
        {
          if (other_players_snakes.strawberry(i).pos_x() == Check.snakeclient().positionsnakepart(0).pos_x()
          && other_players_snakes.strawberry(i).pos_y() == Check.snakeclient().positionsnakepart(0).pos_y())
          {
            other_players_snakes.mutable_strawberry(i)->set_life(false);
            youCanAddAPartAux[SnakeNum] = true;
            std::cout << "CHOCO STRAWBERRY" << std::endl;
          }
        }
      }
    break;
    case network::FromClientToServer::WALL:
      other_players_snakes.mutable_snakes_game(SnakeNum)->set_life(false);
      other_players_snakes.mutable_snakes_game(SnakeNum)->CopyFrom(other_players_snakes.snakes_game(other_players_snakes.snakes_game_size()-1));
      other_players_snakes.mutable_snakes_game()->RemoveLast();
    break;
  }
  if (other_players_snakes.snakes_game_size() < 2)
  {
    other_players_snakes.set_gameover(true);
  }
}

void Server::SetAll()
{
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    esteSocketEstaUsandose[i] = false;
  }

  if ( SDLNet_ResolveHost(&ip, NULL, puerto) != 0 )
  {
    std::cout << "Failed to resolve the server host: " << SDLNet_GetError() << std::endl;
  }

  serverSocket = SDLNet_TCP_Open (&ip);
  if (!serverSocket)
  {
    std::cout << "Failed to open the server socket: " << SDLNet_GetError() << std::endl;
  }

  socketSet = SDLNet_AllocSocketSet(MAX_SOCKETS);
  if ( socketSet == NULL )
  {
    std::cout << "Failed to allocate the socket set: " << SDLNet_GetError() << std::endl;
  }
  else
  {
    SDLNet_TCP_AddSocket(socketSet, serverSocket);
  }
  std::cout << "ALL SET, ENTERING LOBBY..." << std::endl;
}

void Server::Lobby()
{
  other_players_snakes.Clear();
  salirServer = false;
  gameOver = false;
  cantidadDeConectados = 0;
  other_players_snakes.set_gameover(false);
  other_players_snakes.set_youcanaddapart(false);
  setVariables(snake_game.add_positionsnakepart(), 6, 6);
  snake_game.set_id(0);
  snake_game.set_color(network::Snake::RED);
  snake_game.set_life(true);
  addSnakeServer(); //AGREGAR SNAKE SERVER
  other_players_snakes.mutable_snakes_game(0)->set_id(snake_game.id());

  bool salirLobby = false;
  std::string inputText = "";
  std::cout << "***YOU ARE IN THE LOBBY***" << std::endl;
  {
    network::FirstMessageFromServerToClient tmp;
    tmp.set_you_are(network::FirstMessageFromServerToClient::WAITING_IN_LOBBY);
    std::string StringToSerialize;
    tmp.SerializeToString(&StringToSerialize);
    for ( int i = 0; i < cantidadDeConectados; i++ )
    {
      int result = SDLNet_TCP_Send( clientSocket[i], StringToSerialize.c_str(), StringToSerialize.size() );
      if(result < StringToSerialize.size())
      {
        printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
      }
    }
  }
  while (!salirLobby and !quit)
  {
    if ( SDLNet_CheckSockets(socketSet, 0) == -1 )
    {
      std::cout << "Perdida de coneccion con el servidor(?): " << SDLNet_GetError() <<std::endl;
      quit = true;
    }

    if ( SDLNet_SocketReady(serverSocket) != 0 )
    {
      std::cout << "Alguien conectandose ... ";
      bool socketLibre = false;
      if ( cantidadDeConectados < MAX_PLAYERS - 1 )
      {
        for (int i = 0; i < MAX_CLIENTS and !socketLibre; i++)
        {
          if ( esteSocketEstaUsandose[i] == false )
          {
            clientSocket[i] = SDLNet_TCP_Accept( serverSocket );
            SDLNet_TCP_AddSocket(socketSet, clientSocket[i]);
            cantidadDeConectados++;
            socketLibre = true;
            std::cout << " Socket libre encontrado (" << i << "). Cliente conectado." << std:: endl << "Hay " << cantidadDeConectados << "personas conectadas" << std::endl;
            esteSocketEstaUsandose[i] = true;
            std::string StringToSerialize;
            addSnakeServer(); //AGREGAR SNAKE CLIENT, ID+1 porque la ID 0 es para Snake del server
            network::FirstMessageFromServerToClient tmp;
            other_players_snakes.mutable_snakes_game(i+1)->set_id(i+1);
            tmp.set_idtogive(other_players_snakes.snakes_game(i+1).id());
            tmp.set_color(other_players_snakes.snakes_game(i+1).color());
            tmp.set_you_are(network::FirstMessageFromServerToClient::WAITING_IN_LOBBY);
            tmp.SerializeToString(&StringToSerialize);
            SDLNet_TCP_Send(clientSocket[i], StringToSerialize.c_str(), StringToSerialize.size() );
          }
        }
        if ( socketLibre == false )
        {
          std::cout << " Server Lleno. Cliente denegado" << std::endl;
          TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);

          // Send a message to the client saying "FULL" to tell the client to go away
          network::FirstMessageFromServerToClient tmp;
          tmp.set_you_are(network::FirstMessageFromServerToClient::SERVER_FULL);
          std::string StringToSerialize;
          tmp.SerializeToString(&StringToSerialize);
          SDLNet_TCP_Send(tempSock, StringToSerialize.c_str(), StringToSerialize.size());
          SDLNet_TCP_Close(tempSock);
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
        switch (user_event.key.keysym.sym ) {
          case SDLK_RETURN:
          if ( inputText != "" )
          {
            for ( int i = 0; i < cantidadDeConectados; i++ )
            {
              int result = SDLNet_TCP_Send( clientSocket[i], inputText.c_str(), inputText.size());
              std::cout << "Mensaje enviado a clientSocket[" << i << "]" << std::endl;
              if( result < inputText.size() )
              {
                printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
              }
            }
            if ( inputText == "/shutdown" )
            {
              quit = true;
            }
            inputText = "";
          }
          break;
          case SDLK_s:
            salirLobby = true;
          break;
        }
      }
    }
  }
  if (!quit)
  {
    network::FirstMessageFromServerToClient tmp;
    tmp.set_you_are(network::FirstMessageFromServerToClient::START_GAME);
    std::string StringToSerialize;
    tmp.SerializeToString(&StringToSerialize);
    for ( int i = 0; i < cantidadDeConectados; i++ )
    {
      int result = SDLNet_TCP_Send( clientSocket[i], StringToSerialize.c_str(), StringToSerialize.size() );
      if(result < StringToSerialize.size())
      {
        printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
      }
    }
  }
}

void Server::addSnakeServer()
{
  other_players_snakes.add_snakes_game();
  int id = other_players_snakes.snakes_game_size() - 1;
  other_players_snakes.mutable_snakes_game(id)->set_id(id);
  other_players_snakes.mutable_snakes_game(id)->set_life(true);
  setVariables (other_players_snakes.mutable_snakes_game(id)->add_positionsnakepart(), 6, 6);

  switch (id) {
    case network::Snake::RED:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::RED);
      std::cout << "***RED***" << std::endl;
    break;
    case network::Snake::GREEN:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::GREEN);
      std::cout << "***GREEN***" << std::endl;
    break;
    case network::Snake::BLUE:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::BLUE);
    break;
    case network::Snake::GREY:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::GREY);
    break;
    case network::Snake::YELLOW:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::YELLOW);
    break;
    case network::Snake::ORANGE:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::ORANGE);
    break;
    case network::Snake::VIOLET:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::VIOLET);
    break;
    case network::Snake::BROWN:
      other_players_snakes.mutable_snakes_game(id)->set_color(network::Snake::BROWN);
    break;
    default: std::cout << "Error color?" << std::endl;
  }
}

void Server::Play()
{
  SDL_StartTextInput();
  std::string inputText = "";
  tStrawberries.start();
  LTimer TimeToRefresh[7];
  for (int i = 0; i < 7; i++)
  {
    TimeToRefresh[i].start();
  }
  int timeBetweenSendingPackages = 50;
  while ( !salirServer and !quit and !gameOver )
  {
    if ( SDLNet_CheckSockets(socketSet, 0) == -1 )
    {
      std::cout << "Perdida de coneccion con el servidor(?): " << SDLNet_GetError() <<std::endl;
      quit = true;
    }

    if ( SDLNet_SocketReady(serverSocket) != 0 )
    {
      std::cout << "Alguien conectandose ... ";
      bool socketLibre = false;
      if ( cantidadDeConectados < MAX_PLAYERS - 1 )
      {
        for (int i = 0; i < MAX_CLIENTS and !socketLibre; i++)
        {
          if ( esteSocketEstaUsandose[i] == false )
          {
            clientSocket[i] = SDLNet_TCP_Accept( serverSocket );
            SDLNet_TCP_AddSocket(socketSet, clientSocket[i]);
            cantidadDeConectados++;
            socketLibre = true;
            std::cout << " Socket libre encontrado (" << i << "). Cliente conectado." << std:: endl << "Hay " << cantidadDeConectados << "personas conectadas" << std::endl;
            esteSocketEstaUsandose[i] = true;
            std::string StringToSerialize;
            addSnakeServer(); //AGREGAR SNAKE CLIENT, ID+1 porque la ID 0 es para Snake del server
            network::FirstMessageFromServerToClient tmp;
            other_players_snakes.mutable_snakes_game(i+1)->set_id(i+1);
            tmp.set_idtogive(other_players_snakes.snakes_game(i+1).id());
            tmp.set_color(other_players_snakes.snakes_game(i+1).color());
            tmp.set_you_are(network::FirstMessageFromServerToClient::WAITING_TO_FINISH_GAME);
            tmp.SerializeToString(&StringToSerialize);
            SDLNet_TCP_Send(clientSocket[i], StringToSerialize.c_str(), StringToSerialize.size() );
          }
        }
        if ( socketLibre == false )
        {
          std::cout << " Server Lleno. Cliente denegado" << std::endl;
          TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);
          // Send a message to the client saying "FULL" to tell the client to go away
          network::FirstMessageFromServerToClient tmp;
          tmp.set_you_are(network::FirstMessageFromServerToClient::SERVER_FULL);
          std::string StringToSerialize;
          tmp.SerializeToString(&StringToSerialize);
          SDLNet_TCP_Send(tempSock, StringToSerialize.c_str(), StringToSerialize.size());
          SDLNet_TCP_Close(tempSock);
        }
      }
    }
    for ( int i = 0; i < cantidadDeConectados; i++ )
    {
      int SnakeNum;
      for (int j = 0; j < other_players_snakes.snakes_game_size(); j++)
      {
        if (other_players_snakes.snakes_game(j).id() == (i+1)) //I+1 porque la id 0 es del server
        {
          SnakeNum = j;
          std::cout << "SnakeNum es: " << SnakeNum << std::endl;
          printf("%d\n", SnakeNum );
          break;
        }
      }
      if ( SDLNet_SocketReady(clientSocket[i]) )
      {
        int bytesReceived = SDLNet_TCP_Recv(clientSocket[i], buffer, buffer_zise);
        if (bytesReceived <= 0)
        {
          std::cout << "Connection Broken" << std::endl;
          salirServer = true;
        }
        else
        {
          std::string StringToParse(buffer, bytesReceived);
          //printf("bytes llegados:%d  --- String en bytes:%d\n", a, StringToParse.size());
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
                salirServer = true;
              }

            }
          }
          else
          {
            network::FromClientToServer tmp;
            tmp.ParseFromString(StringToParse);
            other_players_snakes.mutable_snakes_game(SnakeNum)->CopyFrom(tmp.snakeclient()); //i+1 porque el 0 esta reservado para el server
            if (tmp.hitwith() != network::FromClientToServer::NOTHING)
            {
              ServerReCheckCollisionFromClient(tmp, SnakeNum);
            }
          }
        }
      }
      if (TimeToRefresh[i].getTicks() > timeBetweenSendingPackages)
      {
      if (youCanAddAPartAux[SnakeNum] == true)
      {
        other_players_snakes.set_youcanaddapart(true);
        youCanAddAPartAux[SnakeNum] = false;
      }
      std::string StringToSerialize;
      if (!other_players_snakes.SerializeToString(&StringToSerialize))
      {
        std::cout << "/* Failed to serialize */" << std::endl;
      }
      /*if (other_players_snakes.IsInitialized())
      {
        std::cout << "Todo bien para mandar!" << std::endl;
      }*/
      //std::cout << "Hay " << other_players_snakes.snakes_game_size() << " Snakes en el paquete enviado"<< std::endl;
      //std::cout << StringToSerialize.size() << std::endl;
      SDLNet_TCP_Send(clientSocket[i], StringToSerialize.c_str(), StringToSerialize.size());
      if (other_players_snakes.youcanaddapart())
      {
        other_players_snakes.set_youcanaddapart(false);
      }
      TimeToRefresh[i].start();
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
            for ( int i = 0; i < cantidadDeConectados; i++ )
            {
              int result = SDLNet_TCP_Send( clientSocket[i], inputText.c_str(), inputText.length()+1 );
              std::cout << "Mensaje enviado a clientSocket[" << i << "]" << std::endl;
              if(result < inputText.length()+1)
              {
                printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                // It may be good to disconnect sock because it is likely invalid now.
              }
            }
            if ( inputText == "/shutdown" )
            {
              salirServer = true;
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
    renderGame();
    if ( tGame.getTicks() > timeStepBetweenSnakeMoves && snake_game.life() )
    {
      moveSnake();
      CheckCollision();
      if (snake_game.life()) other_players_snakes.mutable_snakes_game(0)->CopyFrom(snake_game);
      tGame.start();
    }
    if ( tStrawberries.getTicks() > 4000 )
    {
      addStrawberry();
      tStrawberries.start();
    }
  }
  SDL_StopTextInput();
}

bool Server::initSDL()
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
                //MusicaJuego = new Music();
                DefaultFont = new MyFont ("data/LemonMilk.otf", 50);
                SDLNet_Init();
              }
					 }
				}
			}
		}
	}
	return success;
}

bool Server::exitSDL()
{
  SDLNet_FreeSocketSet(socketSet);
  SDLNet_TCP_Close(serverSocket);

  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );

  Mix_Quit();
  IMG_Quit();
  SDLNet_Quit();
  SDL_Quit();
}
