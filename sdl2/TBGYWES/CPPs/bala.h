#ifndef _BALA_H_
#define _BALA_H_
#include <SDL.h>
#include <map>
class bala
{
public:
    bala(int direccionDisparo_pasada, int x, int y);
    void mover(double timeStep);
    const int bala_widht = 10;
    const int bala_height = 10;
    const int velocidad = 200;
    void render();
    void render2();
    void render3();
    int posX(){return mPosX;}
    int posY(){return mPosY;}
    bool estaVivo(){return vida;}
    void matar(){vida = false;}
  private:
    SDL_Rect mBox;
    double mPosX;
    double mPosY;
    int direccionX;
    int direccionY;
    int TiempodeVida1;
    int TiempodeVida2;
    bool vida;
    int angulo;

};

//Derivated Class
//class hechizo: public
#endif
