#include "sdl2_text.h"
#include <iostream>
extern SDL_Window* MyWindow;
extern SDL_Renderer* MyRenderer;

SDL2_TEXT::SDL2_TEXT(std::string Texto, TTF_Font* Font1, TextQuality quality, SDL_Color textColor1) : Font(Font1), textColor(textColor1)
{
	SDL_Surface* textSurface = NULL;
	switch (quality) {
		case fast:
			textSurface = TTF_RenderText_Solid( Font, Texto.c_str(), textColor );
		break;
		case good:
			textSurface = TTF_RenderText_Blended( Font, Texto.c_str(), textColor );
		break;
		default:
			cout << "switch (quality) error" << endl;
	}
	if( textSurface == NULL )
	{
		cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface( MyRenderer, textSurface );
		if( mTexture == NULL )
		{
            cout << "Unable to create texture from rendered text! SDL Error:" << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
};

SDL2_TEXT::SDL2_TEXT(std::string Texto, TTF_Font* Font, SDL_Color textColor, SDL_Color textColorBG)
{
	SDL_Surface* textSurface = TTF_RenderText_Shaded( Font, Texto.c_str(), textColor, textColorBG );
	if( textSurface == NULL )
	{
		cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface( MyRenderer, textSurface );
		if( mTexture == NULL )
		{
            cout << "Unable to create texture from rendered text! SDL Error:" << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
};
SDL2_TEXT::~SDL2_TEXT()
{
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		width = 0;
		height = 0;
	}
};
void SDL2_TEXT::render(int x, int y)
{
	SDL_Rect renderQuad = {x, y, width, height};
	SDL_RenderCopy( MyRenderer, mTexture, NULL, &renderQuad );
};
void SDL2_TEXT::cambiarTextoRapido(string Texto)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid( Font, Texto.c_str(), textColor );
    mTexture = SDL_CreateTextureFromSurface( MyRenderer, textSurface );
    width = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface( textSurface );
}
