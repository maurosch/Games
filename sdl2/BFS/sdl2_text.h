#ifndef _TEXT_H_
#define _TEXT_H_
#include <SDL2/SDL.h>
#include <C:\Users\alumno\Downloads\SDL2\include\SDL2\SDL_ttf.h>
#include <string>
using namespace std;
enum TextQuality
{
	fast,
	good
};
class SDL2_TEXT
{
public:
	SDL2_TEXT(string Texto, TTF_Font* Font, TextQuality quality, SDL_Color textColor);
	SDL2_TEXT(string Texto, TTF_Font* Font, SDL_Color textColor, SDL_Color textColorBG);
	~SDL2_TEXT();
	void render(int x, int y);
	void cambiarTextoRapido(string Texto);
	int getWidth(){return width;};
	int getHeight(){return height;};
private:
    TTF_Font* Font;
    SDL_Color textColor;
	SDL_Texture* mTexture;
	int width;
	int height;
};
#endif
