#ifndef _LTEXTURE_H_
#define _LTEXTURE_H_
#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path, SDL_Renderer* WhereToRender );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		 //Creates image from font string
		 bool loadFromRenderedText( SDL_Renderer* WhereToRender, std::string textureText, SDL_Color textColor, TTF_Font *font );
		 void loadFromRenderedTextFast ( SDL_Renderer* WhereToRender, std::string textureText, SDL_Color textColor, TTF_Font *font );
		 bool loadFromRenderedTextBig ( SDL_Renderer* WhereToRender, std::string textureText, SDL_Color textColor, TTF_Font *font, int length );
		//Renders texture at given point
		void render( SDL_Renderer* WhereToRender, int x, int y, int wDestination = 0, int hDestination = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};
#endif
