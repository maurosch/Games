//#include "game.h"

#include "server.h"
#include "client.h"

int main( int argc, char* args[] )
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  //game Juego;
  int Seleccion;
  std:: cout << "Cliente -> 1   Server -> 2" << std::endl;
  std::cin >> Seleccion;

  if (Seleccion == 2)
  {
    Server server_game;
    server_game.SetAll();
    while (!server_game.Get_Quit())
    {
      server_game.Lobby();
      server_game.Play();
    }
  }
  else
  {
    if (Seleccion == 1)
    {
      Client client_game;
      client_game.SearchGame( "192.168.1.5" );
      client_game.Play();
    }
    else std::cout << " Seleccion != de 1 o 2 " << std::endl;
  }

  //if (server) Juego.multiplayerServer();
  //else Juego.multiplayerClient( "127.0.0.1" );

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
