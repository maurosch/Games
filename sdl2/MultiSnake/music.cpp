#include "music.h"

Music::Music()
{
  //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  //22050
  canciones.push_back(Mix_LoadMUS("data/runaway.mp3"));
  canciones.push_back(Mix_LoadMUS("data/september_cry_for_you.mp3"));
  //sonidos.push_back(loadChunk ("shoot2"));
  sonidos.push_back(Mix_LoadWAV ("data/sfx/shoot.wav"));
  sonidos.push_back(Mix_LoadWAV ("data/sfx/select"));
  sonidos.push_back(Mix_LoadWAV ("data/sfx/skinBloqueado.wav"));
  //Mix_LoadWAV ("data/sfx/shoot2");
  //prueba = Mix_LoadWAV ("data/sfx/laser.wav");
  if (sonidos[2] == NULL)
  {
    std::cout << "Error";
  }

  cambiarVolumen(50);
  cambiarVolumenEfectos(50);
  cancionReproduciendo = 0;
  //PlayMusica(cancionReproduciendo);

}

Music::~Music()
{
  for ( int i = 0; i < canciones.size(); i++ ) Mix_FreeMusic( canciones[i] );
  for ( int i = 0; i < sonidos.size(); i++ ) Mix_FreeChunk( sonidos[i] );
}

void Music::cambiarVolumen(int volumenAcambiar)
{
  volumen = volumenAcambiar;
  Mix_VolumeMusic(volumenAcambiar);
}

void Music::cambiarVolumenEfectos(int volumenAcambiar)
{
  volumenEfectos = volumenAcambiar;
}

int Music::saberVolumen()
{
  return volumen;
}

int Music::saberVolumenEfectos()
{
  return volumenEfectos;
}

void Music::PasarCancion()
{
  cancionReproduciendo++;
  PlayMusica(cancionReproduciendo);
}

void Music::PlaySonido( int sonidoElegido )
{
	Mix_VolumeChunk(sonidos[sonidoElegido], volumenEfectos);
	Mix_PlayChannel(-1, sonidos[sonidoElegido], 0);
}

void Music::PlayMusica(int NumeroCancion) {

  Mix_PlayMusic(canciones[NumeroCancion], -1);
  NadaCorriendo = false;

}
