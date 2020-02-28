#ifndef _MUSIC_H_
#define _MUSIC_H_
#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <iostream>

enum eSounds
{
  shoot,
  selector,
  skinBloqueado
};

class Music
{
public:
  Mix_Music* loadMusic(std::string fileName);
	Mix_Chunk* loadChunk(std::string fileName);
  Music();
  ~Music();
  void cambiarVolumen(int volumenAcambiar);
  void cambiarVolumenEfectos(int volumenAcambiar);
  int saberVolumen();
  int saberVolumenEfectos();
  void PlaySonido( int sonidoElegido );
  void PlayMusica( int NumeroCancion );
  void PasarCancion();

private:
  std::vector<Mix_Music*> canciones;
  std::vector<Mix_Chunk*> sonidos;
  int volumen;
  int volumenEfectos;
  int cancionReproduciendo;
  bool NadaCorriendo = true;

};
#endif
