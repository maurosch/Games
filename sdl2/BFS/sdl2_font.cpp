#include "sdl2_font.h"
#include <iterator>
#include <iostream>
using namespace std;
SDL2_FONT::~SDL2_FONT()
{
	for (map<int, TTF_Font*>::iterator it = Fonts.begin(); it != Fonts.end(); it++)
	{
		TTF_CloseFont(it->second);
	}
	Fonts.clear();
};
TTF_Font* SDL2_FONT::getFont(int size)
{
	map<int, TTF_Font*>::iterator it = Fonts.find(size);
	if (it != Fonts.end())
		return it->second;
	else
	{
		TTF_Font* auxFont = TTF_OpenFont(Ubicacion.c_str(), size);
		if (!auxFont)
        {
            cout << "TTF_OpenFont: " << TTF_GetError() << endl;
            return NULL;
        }
		else
        {
            Fonts.insert(pair<int, TTF_Font*>(size, auxFont));
            return auxFont;
		}
	}
};
