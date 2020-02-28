#ifndef _TEXT_H_
#define _TEXT_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

enum texts
{
  eNormal,
  eFast,
  eLong
};

class text
{
public:
  text(){};
  text(SDL_Renderer* WhereToRender, std::string sTextureText, SDL_Color textColor, TTF_Font* font, int x, int y, int wayToLoad, int length = 0 );
  ~text();
  void render();
  void render(int x, int y);
  void setXandY ( int x = -1, int y = -1 );
  int getWidth();
  int getHeight();
  void init(SDL_Renderer* WhereToRender, std::string sTextureText, SDL_Color textColor, TTF_Font* font, int x, int y, int wayToLoad, int length = 0);
  void loadTextFast( std::string sTextureText, SDL_Color textColor, TTF_Font *font );
private:
  void loadText( std::string sTextureText, SDL_Color textColor, TTF_Font *font );
  void loadTextLong( std::string sTextureText, SDL_Color textColor, TTF_Font *font, int length );
  void free();
  SDL_Rect RenderRect;
  SDL_Texture* textTexture;
  SDL_Renderer* WhereIsRendered;
  int width, height;
};

#endif //_TEXT_H_
