#include "text.h"

text::text( SDL_Renderer* WhereToRender, std::string sTextureText, SDL_Color textColor, TTF_Font* font, int x, int y, int wayToLoad, int length )
{
	textTexture = NULL;
	WhereIsRendered = WhereToRender;
	switch (wayToLoad) {
		case texts::eNormal:
			loadText( sTextureText, textColor, font );
		break;
		case texts::eFast:
			loadTextFast( sTextureText, textColor, font );
		break;
		case texts::eLong:
			loadTextLong( sTextureText, textColor, font, length );
		break;
		default: printf("Error Switch Text.cpp\n");
	}
	setXandY(x, y);
}

void text::init( SDL_Renderer* WhereToRender, std::string sTextureText, SDL_Color textColor, TTF_Font* font, int x, int y, int wayToLoad, int length )
{
	textTexture = NULL;
	WhereIsRendered = WhereToRender;
	switch (wayToLoad) {
		case texts::eNormal:
			loadText( sTextureText, textColor, font );
		break;
		case texts::eFast:
			loadTextFast( sTextureText, textColor, font );
		break;
		case texts::eLong:
			loadTextLong( sTextureText, textColor, font, length );
		break;
		default: printf("Error Switch Text.cpp\n");
	}
	setXandY(x, y);
}

text::~text()
{
	free();
}

void text::setXandY( int x, int y )
{
	if ( x != -1 ) RenderRect.x = x;
	if ( y != -1 ) RenderRect.y = y;
}

void text::loadText( std::string sTextureText, SDL_Color textColor, TTF_Font *font )
{
	//Get rid of preexisting texture
	free();
	//Render text surface
	SDL_Surface* textSurface;

	textSurface = TTF_RenderText_Blended ( font, sTextureText.c_str(), textColor );

	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	//Create texture from surface pixels
	std::cout << textTexture << " ";
	textTexture = SDL_CreateTextureFromSurface( WhereIsRendered, textSurface );
	if( textTexture == NULL )
	{
		printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Get image dimensions
		RenderRect.w = textSurface->w;
		RenderRect.h = textSurface->h;
	}
	//Get rid of old surface
	SDL_FreeSurface( textSurface );
}

void text::loadTextFast( std::string sTextureText, SDL_Color textColor, TTF_Font *font )
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid ( font, sTextureText.c_str(), textColor);
	textTexture = SDL_CreateTextureFromSurface( WhereIsRendered, textSurface );
	RenderRect.w = textSurface->w;
	RenderRect.h = textSurface->h;
	SDL_FreeSurface( textSurface );
}

void text::loadTextLong( std::string sTextureText, SDL_Color textColor, TTF_Font *font, int length )
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped ( font, sTextureText.c_str(), textColor, length );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
		textTexture = SDL_CreateTextureFromSurface( WhereIsRendered, textSurface );
		if( textTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			RenderRect.w = textSurface->w;
			RenderRect.h = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

}

void text::render()
{
	SDL_RenderCopyEx( WhereIsRendered, textTexture, NULL, &RenderRect, 0.0, NULL, SDL_FLIP_NONE );
}

void text::render(int x, int y)
{
	RenderRect.x = x;
	RenderRect.y = y;
	SDL_RenderCopyEx( WhereIsRendered, textTexture, NULL, &RenderRect, 0.0, NULL, SDL_FLIP_NONE );
}

void text::free()
{
	if( textTexture != NULL )
	{
		SDL_DestroyTexture( textTexture );
		textTexture = NULL;
		width = 0;
		height = 0;
	}
}

int text::getWidth()
{
	return RenderRect.w;
}

int text::getHeight()
{
	return RenderRect.h;
}
