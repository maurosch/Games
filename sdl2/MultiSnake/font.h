#ifndef _FONT_H_
#define _FONT_H_

#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <stdio.h>

class MyFont
{
public:

  MyFont(){};
	MyFont( std::string FileLocation, int initialSize )
	{
    FontSizes.insert( std::pair<int, TTF_Font*>( initialSize, TTF_OpenFont( FileLocation.c_str(), initialSize ) ) );
    FontLocation = FileLocation;
	}

	TTF_Font* GetFont(int TamanioFuente)
	{
		std::map<int, TTF_Font*>::iterator it;
		it = FontSizes.find(TamanioFuente);

		if (it != FontSizes.end())
			return it->second;
		else
		{
			TTF_Font* mFont = TTF_OpenFont(FontLocation.c_str(), TamanioFuente);
			FontSizes.insert(std::pair<int, TTF_Font*>(TamanioFuente, mFont));
			return mFont;
		}
	}

  TTF_Font* GetFont()
  {
    std::map<int, TTF_Font*>::iterator it = FontSizes.begin();
    return it->second;
  }

	~MyFont()
	{
    for ( std::map<int, TTF_Font*>::iterator it = FontSizes.begin(); it != FontSizes.end(); ++it )
    {
      TTF_CloseFont (it->second);
      printf("Cerrando Font\n");
    }
	}
private:
  std::string FontLocation;
  //TIENE QUE SER MAPA PORQUE SON MUCHOS TTF_FONT
	std::map<int, TTF_Font*> FontSizes;
};

#endif _FONT_H_
