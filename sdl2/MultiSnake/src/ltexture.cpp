#include "ltexture.h"
#include "game.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/*TTF ACLARACIONES
TTF_RenderText_Solid - this one renders basic text(whitout background) and its fast, but its low-quality, and will look pixellated.
TTF_RenderText_Shaded - this one draws nice blended characters, but to a pre-defined background color
TTF_RenderText_Blended - this one draws a nice blended text, but it uses alpha-blending and its slow, but gives very nice results
*/
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path, SDL_Renderer* WhereToRender )
{
		free();

		mTexture = IMG_LoadTexture ( WhereToRender, path.c_str() );
		if ( mTexture != NULL )
		{
			SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
		}
		else
		{
			printf("No se pudo cargar %s\n", path);
			return false;
		}

		return true;
}

/******************VIEJO LOAD FILE*************************
bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
				newTexture = SDL_CreateTextureFromSurface( game1->gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
*********************************************************/

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( SDL_Renderer* WhereToRender, int x, int y, int wDestination, int hDestination, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( wDestination != 0 )
	{
		renderQuad.w = wDestination;
		renderQuad.h = hDestination;
	}

	//Render to screen
	SDL_RenderCopyEx( WhereToRender, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::loadFromRenderedText( SDL_Renderer* WhereToRender, std::string textureText, SDL_Color textColor, TTF_Font *font )
{
	//Get rid of preexisting texture
	free();
	//Render text surface
	SDL_Surface* textSurface;

	textSurface = TTF_RenderText_Blended ( font, textureText.c_str(), textColor );

	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	//Create texture from surface pixels
	mTexture = SDL_CreateTextureFromSurface( WhereToRender, textSurface );
	if( mTexture == NULL )
	{
		printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Get image dimensions
		mWidth = textSurface->w;
		mHeight = textSurface->h;
	}
	//Get rid of old surface
	SDL_FreeSurface( textSurface );

	//Return success
	return mTexture != NULL;
}

void LTexture::loadFromRenderedTextFast ( SDL_Renderer* WhereToRender, std::string textureText, SDL_Color textColor, TTF_Font *font )
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid ( font, textureText.c_str(), textColor);
	mTexture = SDL_CreateTextureFromSurface( WhereToRender, textSurface );
	mWidth = textSurface->w;
	mHeight = textSurface->h;
	SDL_FreeSurface( textSurface );
}

bool LTexture::loadFromRenderedTextBig ( SDL_Renderer* WhereToRender, std::string textureText, SDL_Color textColor, TTF_Font *font, int length )
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped ( font, textureText.c_str(), textColor, length );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface( WhereToRender, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface( textSurface );
	}

	return mTexture != NULL;
}
