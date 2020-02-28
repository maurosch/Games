#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string>
#include "sdl2_text.h"
#include "sdl2_font.h"
#include "plantilla.h"
#include "sdl2_timer.h"
using namespace std;

bool init();
void quit();
SDL_Window* MyWindow = NULL;
SDL_Renderer* MyRenderer = NULL;
SDL2_FONT MyFonts("data/8-BIT.ttf");

int main(int argc, char *argv[])
{
    int alto, ancho;
    cin >> alto >> ancho;
    bool bquit = !init();

    int frames = 0;
    SDL2_TIMER FramesPerSecond;
    FramesPerSecond.start();

    SDL_Event e;
    ///float StepTime = 0;
    SDL_Color textColor = {0, 0, 0};
    SDL2_TEXT title("BFS TEST", MyFonts.getFont(50), good, textColor);
    SDL2_TEXT FPS("0", MyFonts.getFont(20), fast, textColor);
    int wWindow, hWindow;
    SDL_GetRendererOutputSize(MyRenderer, &wWindow, &hWindow);
    ///cout << wWindow << " " << hWindow << endl;
    SDL_Rect EspacioPlantilla = {0, title.getHeight() + 20, wWindow, hWindow - (title.getHeight() + 20) - 10};
    PLANTILLA Plantilla(alto, ancho, EspacioPlantilla);
    while(!bquit)
    {
        float avgFPS = frames / ( FramesPerSecond.getTicks() / 1000.f );
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }
        string b = to_string(avgFPS);
        FPS.cambiarTextoRapido(b);

        ///float auxStepTime = SDL_GetTicks();
        while( SDL_PollEvent( &e ) != 0 )
        {
          //User requests quit
            if( e.type == SDL_QUIT )
            {
                bquit = true;
            }
            if( e.type == SDL_MOUSEMOTION)
            {
                Plantilla.XYMouse();
            }
            if ( e.type == SDL_MOUSEBUTTONDOWN )
            {
                switch (e.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        Plantilla.cambiarA(pared);
                    break;
                    case SDL_BUTTON_RIGHT:
                        Plantilla.cambiarA(inicio);
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor( MyRenderer, 255, 255, 255, 255);
        SDL_RenderClear(MyRenderer);

        title.render(wWindow/2 - title.getWidth()/2, 10);
        FPS.render(wWindow - FPS.getWidth(), 0);
        Plantilla.render();

        SDL_RenderPresent(MyRenderer);

        ///StepTime = (SDL_GetTicks() - auxStepTime)/1000;
        frames++;
        //cout << StepTime << endl;
    }
    quit();
    return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0 )
	{
		cout << "SDL could not initialize! SDL Error:" << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}

		//Create window
		SDL_Rect a;
		SDL_GetDisplayBounds(0, &a);
		MyWindow = SDL_CreateWindow( "TBGYWES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, a.w/2, a.h/2, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED );
		if( MyWindow == NULL )
		{
			cout << "Window could not be created! SDL Error:" << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Create renderer for window
			MyRenderer = SDL_CreateRenderer( MyWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );
			if( MyRenderer == NULL )
			{
                cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( MyRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}

				else
                {
                    if( TTF_Init() == -1 )
                    {
                        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
                        success = false;
                    }
				}
			}
		}
	}
    SDL_SetRenderDrawBlendMode(MyRenderer, SDL_BLENDMODE_BLEND);

	return success;
}

void quit()
{
    //Destroy window
	SDL_DestroyRenderer( MyRenderer );
	SDL_DestroyWindow( MyWindow );
	MyWindow = NULL;
	MyRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
