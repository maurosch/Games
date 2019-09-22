#ifndef _PLANTILLA_H_
#define _PLANTILLA_H_
#include <vector>
#include <C:\Users\alumno\Downloads\SDL2\include\SDL2\SDL.h>
using namespace std;
enum Cuadrados
{
        vacio,
        pared,
        inicio,
        final,
        camino
};
struct par {int x, y;};
class PLANTILLA
{
public:
    PLANTILLA(int ancho, int alto, SDL_Rect EspacioPlantillaAux);
    vector<vector<int> >* getPlantilla(){return &Plantilla;};
    void cambiarA(Cuadrados A);
    void cambiarStart(int xStart, int yStart);
    void BFS(int xEnd, int yEnd);
    void BFS();
    vector<par>* getRecorrido(){return &Recorrido;}
    void render();
    void XYMouse();
private:
    struct nodo
    {
       vector<int>Adjacentes;
       int padre;
       int id;
       bool visitado;
       bool pared;
    };
    par NodoInicial;
    par NodoFinal;
    int separacionHorizontal, separacionVertical;
    vector<nodo>Nodos;
    vector<vector<int> >Plantilla;
    vector<par>Recorrido;
    void RecalcularBFS();
    SDL_Rect EspacioPlantilla;
};
#endif
