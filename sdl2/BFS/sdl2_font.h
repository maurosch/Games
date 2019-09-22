#ifndef _SDL2_FONT_H_
#define _SDL2_FONT_H_
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
class SDL2_FONT
{
public:
	SDL2_FONT(std::string FileLocation) : Ubicacion(FileLocation){};
	~SDL2_FONT();
	TTF_Font* getFont(int size);
private:
	std::string Ubicacion;
	std::map<int, TTF_Font*> Fonts;
};
#endif
