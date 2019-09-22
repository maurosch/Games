#include "plantilla.h"
#include <queue>
#include <iostream>

extern SDL_Renderer* MyRenderer;

PLANTILLA::PLANTILLA(int ancho, int alto, SDL_Rect EspacioPlantillaAux) : EspacioPlantilla(EspacioPlantillaAux)
{
    for (int i = 0; i < alto; i++)
    {
        vector<int> aux;
        for (int j = 0; j < ancho; j++)
        {
            aux.push_back(vacio);
        }
        Plantilla.push_back(aux);
    }
    for (int i = 0; i < Plantilla.size(); i++)
    {
        for (int j = 0; j < Plantilla[i].size(); j++)
        {
            nodo aux;
            aux.id = Plantilla[0].size()*i + j;
            aux.visitado = false;
            aux.pared = false;
            if (j > 0)
            {
                aux.Adjacentes.push_back(Plantilla[i].size()*i + j -1);
                /*if (i > 0)
                {
                    ///ARRIBA IZQUIERDA
                    aux.Adjacentes.push_back(Plantilla[0].size()*(i-1) + j -1);
                }*/
            }
            if (j < Plantilla[i].size()-1)
            {
                aux.Adjacentes.push_back(Plantilla[i].size()*i + j +1);
                /*if (i < Plantilla.size())
                {
                    ///ABAJO DERECHA
                    aux.Adjacentes.push_back(Plantilla[0].size()*(i+1) + j +1);
                }*/
            }
            if (i > 0)
            {
                aux.Adjacentes.push_back(Plantilla[i].size()*(i-1) + j);
                /*if (j < Plantilla[0].size()-1)
                {
                    ///ARRIBA DERECHA
                    aux.Adjacentes.push_back(Plantilla[0].size()*(i-1) + j +1);
                }*/
            }
            if (i < Plantilla.size()-1)
            {
                aux.Adjacentes.push_back(Plantilla[i].size()*(i+1) + j);
                /*if (j > 0)
                {
                    ///ABAJO IZQUIERDA
                    aux.Adjacentes.push_back(Plantilla[0].size()*(i+1) + j -1);
                }*/
            }
            aux.padre = -1;
            Nodos.push_back(aux);
        }
    }
    /*for (int i = 0; i < Nodos.size(); i++)
    {
        cout << i << " ";
        for (int j = 0; j < Nodos[i].Adjacentes.size(); j++)
        {
            cout << Nodos[i].Adjacentes[j] << " ";
        }
        cout << endl;
    }*/
    NodoInicial.x = 0;
    NodoInicial.y = 0;
    NodoFinal.x = Plantilla[0].size()-1;
    NodoFinal.y = Plantilla.size()-1;
    separacionHorizontal = EspacioPlantilla.w/Plantilla[0].size();
    separacionVertical = EspacioPlantilla.h/Plantilla.size();
    EspacioPlantilla.h = separacionVertical*Plantilla.size();
    EspacioPlantilla.w = separacionHorizontal*Plantilla[0].size();
}
void PLANTILLA::BFS(int xEnd, int yEnd)
{
    NodoFinal.y = yEnd;
    NodoFinal.x = xEnd;
    BFS();
}
void PLANTILLA::BFS()
{
    ///Plantilla[i].size()*i + j;
    int const idNodoFinal = Plantilla[0].size()*NodoFinal.y + NodoFinal.x;
    int const idPrincipio = Plantilla[0].size()*NodoInicial.y + NodoInicial.x;
    queue<nodo>Cola;
    Cola.push(Nodos[idPrincipio]);
    bool encontrado = false;
    /*cout << "NODO Principio: " << idPrincipio << endl;
    cout << "NODO FINAL: " << idNodoFinal << endl;
    cout << "**************************" << endl;*/
    for (int i = 0; i < Nodos.size(); i++) Nodos[i].visitado = false;
    if (Nodos[idNodoFinal].pared or idNodoFinal == idPrincipio) Cola.pop();
    while(Cola.size() > 0 and !encontrado)
    {
        if (!Nodos[Cola.front().id].visitado)
        {
            ///cout << Cola.front().id << endl;
            if (Cola.front().id == idNodoFinal)
            {
                encontrado = true;
            }
            else
            {
                Nodos[Cola.front().id].visitado = true;
                for (int i = 0; i < Cola.front().Adjacentes.size(); i++)
                {
                    if (!Nodos[Cola.front().Adjacentes[i]].visitado and !Nodos[Cola.front().Adjacentes[i]].pared)
                    {
                        Cola.push(Nodos[Cola.front().Adjacentes[i]]);
                        Nodos[Cola.front().Adjacentes[i]].padre = Cola.front().id;
                    }
                }
            }
        }
        Cola.pop();
    }
    Recorrido.clear();
    if (encontrado)
    {
        par aux;
        aux.y = Nodos[idNodoFinal].padre / Plantilla[0].size();
        aux.x = Nodos[idNodoFinal].padre - Plantilla[0].size()*aux.y;
        int ColaAux = Nodos[idNodoFinal].padre;
        Recorrido.push_back(aux);
        ///cout << "PADRE DEL FINAL: " << ColaAux << endl << endl;
        while (ColaAux != idPrincipio)
        {
            ///cout << "---- NODO: " << Nodos[ColaAux].padre << endl;
            aux.y = Nodos[ColaAux].padre / Plantilla[0].size(); ///cout << "y: " <<aux.y << endl;
            aux.x = Nodos[ColaAux].padre - Plantilla[0].size()*aux.y; ///cout << "x: " << aux.x << endl;
            ///cout << ColaAux << endl; //<< aux.y << " " << aux.x << endl; cout << endl;
            ColaAux = Nodos[ColaAux].padre;
            Recorrido.push_back(aux);
        }
    }
    /*else
    {
        cout << "NO HAY CAMINO" << endl;
    }*/
}
void PLANTILLA::cambiarA(Cuadrados A)
{
    int x, y;
    SDL_GetMouseState( &x, &y );
    if ( x > EspacioPlantilla.x and x < EspacioPlantilla.x + EspacioPlantilla.w and y > EspacioPlantilla.y and y < EspacioPlantilla.y + EspacioPlantilla.h )
    {
        int x2 = x - EspacioPlantilla.x, y2 = y - EspacioPlantilla.y;
        int NodoX = x2/separacionHorizontal;
        int NodoY = y2/separacionVertical;
        int numeroNodo = Plantilla[0].size()*NodoY + NodoX;
        if (A == pared)
        {
            if (Nodos[numeroNodo].pared)
            {
                Nodos[numeroNodo].pared = false;
            }
            else
            {
                Nodos[numeroNodo].pared = true;
            }
        }
        if (A == inicio)
        {
            NodoInicial.x = NodoX;
            NodoInicial.y = NodoY;
        }
    }
}
void PLANTILLA::render()
{
    SDL_SetRenderDrawColor( MyRenderer, 255, 0, 255, 255 );
    for (int i = 0; i < Recorrido.size(); i++)
    {
        SDL_Rect aux;
        aux.x = EspacioPlantilla.x + Recorrido[i].x*separacionHorizontal;
        aux.y = EspacioPlantilla.y + Recorrido[i].y*separacionVertical;
        aux.w = separacionHorizontal;
        aux.h = separacionVertical;
        SDL_RenderFillRect( MyRenderer, &aux );
    }
    SDL_Rect aux;
    aux.x = EspacioPlantilla.x + NodoFinal.x*separacionHorizontal;
    aux.y = EspacioPlantilla.y + NodoFinal.y*separacionVertical;
    aux.w = separacionHorizontal;
    aux.h = separacionVertical;
    SDL_RenderFillRect( MyRenderer, &aux );

    SDL_SetRenderDrawColor( MyRenderer, 0, 0, 0, 255 );
    for (int i = 0; i < Nodos.size(); i++)
    {
        if (Nodos[i].pared)
        {
            int y = i / Plantilla[0].size();
            int x = i - Plantilla[0].size()*y;
            SDL_Rect aux;
            aux.x = EspacioPlantilla.x + x*separacionHorizontal;
            aux.y = EspacioPlantilla.y + y*separacionVertical;
            aux.w = separacionHorizontal;
            aux.h = separacionVertical;
            SDL_RenderFillRect( MyRenderer, &aux );
        }
    }
    SDL_SetRenderDrawColor( MyRenderer, 0, 0, 0, 255);
    ///LINEAS VERTICALES
    for (float i = EspacioPlantilla.x; i <= EspacioPlantilla.x + EspacioPlantilla.w; i += separacionHorizontal)
    {
        SDL_RenderDrawLine(MyRenderer, i, EspacioPlantilla.y, i, EspacioPlantilla.y + EspacioPlantilla.h);
    }
    ///LINEAS HORIZONTALES
    for (float i = EspacioPlantilla.y; i <= EspacioPlantilla.y + EspacioPlantilla.h; i += separacionVertical)
    {
        SDL_RenderDrawLine(MyRenderer, EspacioPlantilla.x, i, EspacioPlantilla.w, i);
    }
};
void PLANTILLA::XYMouse()
{
    int x, y;
    SDL_GetMouseState( &x, &y );
    if ( x > EspacioPlantilla.x and x < EspacioPlantilla.x + EspacioPlantilla.w and y > EspacioPlantilla.y and y < EspacioPlantilla.y + EspacioPlantilla.h )
    {
        int x2 = x - EspacioPlantilla.x, y2 = y - EspacioPlantilla.y;
        if (x2 < NodoFinal.x*separacionHorizontal or x2 > NodoFinal.x*separacionHorizontal+separacionHorizontal or
            y2 < NodoFinal.y*separacionVertical or y2 > NodoFinal.y*separacionVertical+separacionVertical )
        {
            if (x2/separacionHorizontal != NodoInicial.x or y2/separacionVertical != NodoInicial.y)
            {
                NodoFinal.x = x2/separacionHorizontal;
                NodoFinal.y = y2/separacionVertical;
                BFS();
            }
        }
    }
}
