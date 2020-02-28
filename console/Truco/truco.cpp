#include <iostream>
#include <time.h>
#include <thread>
//#include <windows.h>
using namespace std;

void Sleep(int miliSeconds)
{
	this_thread::sleep_for(chrono::milliseconds(miliSeconds));
}

void clean()
{
    //----WINDOWS----
    //system("cls");
    //-----LINUX-----
    system("clear");
}



/// 0 -> ORO  1 -> COPA  2 -> ESPADA  3 -> BASTO
/*1 espada     15
1 de basto     14
7 espada       13
7 oro          12
3 todos        11
2 todos        10
1 oro y copa    8
12 todos        7
11 todos        6
10 todos        5
7 basto y copa  4
6 todos         3
5 todos         2
4 todos         1*/
///------------------DECLARACIONES DE FUNCIONES------------------
void menu();
void mostrar(int carta);
void mostrarCarta(int carta, int renglon);
void empezarJuego(bool elegirCartas);
int puntajeCarta(int carta);
int puntajeCartaEnvido(int cartas[3]);
void randomCartas(int cartasIA[3], int cartasHumano[3]);
void elegirCartas(int cartasIA[3], int cartasHumano[3], bool &anteriorManoEmpezoIA);
bool randomPorcentaje(int porcentaje);
void color(int letra, int fondo);
void mostrarJuego(int cartasHumano[3], int PriSegTercartasIA[3], bool bCartasHumano[3], int puntajeHumano, int puntajeIA);
void cantarTruco(bool cantoIA, bool trucos[3], int cartasIA[3], int &ganaMano, bool &HumanoPuedeCantar, bool &IAPuedeCantar);
void cantarEnvido(int &puntajeHumano, int &puntajeIA, bool cantoIA, int cartasIA[3], int cartasHumano[3], bool &envidoCantado);
void chequearQuienGano(int CartaQueJugoPersona, int CartaQueJugoIA, int ronda[3], int i, int &ganaMano);
void inteligenciaIA(int CartaQueJugoPersona, int &CartaQueJugoIA, int cartasIA[3], int cartasHumano[3], bool bCartasIA[3], int PriSegTercartasIA[3], int ronda[3], bool trucos[3], int &puntajeIA, int &puntajeHumano, int &ganaMano, bool &HumanoPuedeCantar, bool &IAPuedeCantar, bool &envidoCantado);
void humanoDecide(int cartasHumano[3], int cartasIA[3], int PriSegTercartasIA[3], int &ganaMano, bool &HumanoPuedeCantar, bool &IAPuedeCantar, int &CartaQueJugoPersona, int ronda[3], bool trucos[3], bool bCartasHumano[3], bool bCartasIA[3], int &puntajeIA, int &puntajeHumano, bool &envidoCantado);
///---------------------------------------------------------------
void humanoDecide(int cartasHumano[3], int cartasIA[3], int PriSegTercartasIA[3], int &ganaMano, bool &HumanoPuedeCantar, bool &IAPuedeCantar, int &CartaQueJugoPersona,
int ronda[3], bool trucos[3], bool bCartasHumano[3], bool bCartasIA[3], int &puntajeIA, int &puntajeHumano, bool &envidoCantado)
{
    int opcion;
    bool seguir = false;
    while(!seguir and ganaMano == -1)
    {
        mostrarJuego(cartasHumano, PriSegTercartasIA, bCartasHumano, puntajeHumano, puntajeIA);
        if (HumanoPuedeCantar)
        if (trucos[0])
        {
            if (trucos[1])
            {
                if (!trucos[2]) cout << "4) Vale 4" << endl;
            }
            else
            {
                cout << "4) Re Truco" << endl;
            }
        }
        else cout << "4) Truco" << endl;
        if (ronda[0] == -1 and !envidoCantado) cout << "5) Envido" << endl;
        cout << "opcion: ";
        cin >> opcion;
        seguir = true;
        switch(opcion)
        {
            case 1:
                if (!bCartasHumano[opcion-1])
                {
                    CartaQueJugoPersona = cartasHumano[0];
                    bCartasHumano[0] = true;
                }
                else seguir = false;
            break;
            case 2:
                if (!bCartasHumano[opcion-1])
                {
                    CartaQueJugoPersona = cartasHumano[1];
                    bCartasHumano[1] = true;
                }
                else seguir = false;
            break;
            case 3:
                if (!bCartasHumano[opcion-1])
                {
                    CartaQueJugoPersona = cartasHumano[2];
                    bCartasHumano[2] = true;
                }
                else seguir = false;
            break;
            case 4:
                cantarTruco(false, trucos, cartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar);
                seguir = false;
            break;
            case 5:
                if (!envidoCantado) cantarEnvido(puntajeHumano, puntajeIA, false, cartasIA, cartasHumano, envidoCantado);
                seguir = false;
            break;
            default: seguir = false;
        }
    }
}
void chequearQuienGano(int CartaQueJugoPersona, int CartaQueJugoIA, int ronda[3], int i, int &ganaMano)
{
    if (puntajeCarta(CartaQueJugoPersona) > puntajeCarta(CartaQueJugoIA))
    {
        ronda[i] = 1;
    }
    else
    {
        if (puntajeCarta(CartaQueJugoPersona) < puntajeCarta(CartaQueJugoIA))
        {
            ronda[i] = 0;
        }
        else
        {
            ronda[i] = 2;
        }
    }
    if (ronda[1] == 2)
    {
        if (ronda[0] != 2) ganaMano = ronda[0];
        else
        {
            if (ronda[2] != -1)
            {
                if (ronda[2] != 2) ganaMano = ronda[2];
                else ganaMano = 1;
            }
        }
    }
    if (ronda[0] == 2 )
    {
        if (ronda[1] != 2) ganaMano = ronda[1];
    }
    if (ronda[0] == ronda[1] and ronda[0] != -1) ganaMano = ronda[0];
}
void cantarTruco(bool cantoIA, bool trucos[3], int cartasIA[3], int &ganaMano, bool &HumanoPuedeCantar, bool &IAPuedeCantar)
{
    if (trucos[2]) return; ///SALIR SI YA NO HAY MAS QUE CANTAR
    if (cantoIA and IAPuedeCantar)
    {
        clean();
        if (trucos[0])
        {
            if (trucos[1])
            {
                if (!trucos[2]) cout << "IA: Vale 4" << endl;
            }
            else
            {
                cout << "IA: Re Truco" << endl;
            }
        }
        else cout << "IA: Truco" << endl;
        cout << "1) Quiero" << endl << "2) No quiero" << endl << "Opcion: ";
        int opcion = -1;
        while(opcion != 1 and opcion != 2) cin >> opcion;
        if (opcion == 1)
        {
            if (trucos[0])
            {
                if (trucos[1])
                {
                    if (!trucos[2]) trucos[2] = true;
                }
                else
                {
                    trucos[1] = true;
                }
            }
            else trucos[0] = true;
            HumanoPuedeCantar = true;
            IAPuedeCantar = false;
        }
        else
        {
            ganaMano = 0;
        }
    }
    else
    {
        if (!cantoIA and HumanoPuedeCantar)
        {
            clean();
            if (trucos[0])
            {
                if (trucos[1])
                {
                    if (!trucos[2]) cout << "TU: Vale 4" << endl;
                }
                else
                {
                    cout << "TU: Re Truco" << endl;
                }
            }
            else cout << "TU: Truco" << endl;
            if (puntajeCarta(cartasIA[0]) + puntajeCarta(cartasIA[1]) + puntajeCarta(cartasIA[2]) > 20 or randomPorcentaje(25))
            {
                Sleep(1000);
                cout << "IA: Quiero!" << endl;
                Sleep(1000);
                if (trucos[0])
                {
                    if (trucos[1])
                    {
                        if (!trucos[2]) trucos[2] = true;
                    }
                    else
                    {
                        trucos[1] = true;
                    }
                }
                else trucos[0] = true;
                HumanoPuedeCantar = false;
                IAPuedeCantar = true;
            }
            else
            {
                Sleep(1000);
                cout << "IA: No se quiere" << endl;
                Sleep(1000);
                ganaMano = 1;
            }
        }
    }
}
void cantarEnvido(int &puntajeHumano, int &puntajeIA, bool cantoIA, int cartasIA[3], int cartasHumano[3], bool &envidoCantado)
{
    int opcion;
    clean();
    if (cantoIA)
    {
        cout << "IA: Envido!" << endl;
        Sleep(1000);
        cout << "1) Quiero" << endl << "2) No quiero" << endl << "3) Envido" << endl << "4) Real Envido" << endl << "5) Falta Envido" << endl;
        opcion = -1;
        while (opcion !=1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5) cin >> opcion;
        switch (opcion) {
            case 1:
                cout << "IA: " << puntajeCartaEnvido(cartasIA) << endl;
                Sleep(1000);
                if(puntajeCartaEnvido(cartasIA) > puntajeCartaEnvido(cartasHumano))
                {
                    cout << "TU: son buenas" << endl;
                    puntajeIA += 2;
                }
                else
                {
                    cout << "TU: " << puntajeCartaEnvido(cartasHumano) << " son mejores" << endl;
                    puntajeHumano += 2;
                }
                Sleep(1000);
            break;
            case 2: puntajeIA += 1; cout << "TU: No quiero" << endl; Sleep(1000); break;
            case 3:
                if ( puntajeCartaEnvido(cartasIA) > 29 or randomPorcentaje(30) )
                {
                    cout << "IA: Quiero!" << endl;
                    Sleep(1000);
                    cout << "TU: " << puntajeCartaEnvido(cartasHumano) << endl;
                    Sleep(1000);
                    if(puntajeCartaEnvido(cartasIA) > puntajeCartaEnvido(cartasHumano))
                    {
                        cout << "IA: " << puntajeCartaEnvido(cartasIA) << " son mejores";
                        puntajeIA += 4;
                        Sleep(1000);
                    }
                    else
                    {
                        cout << "IA: son buenas" << endl;
                        Sleep(1000);
                        puntajeHumano += 4;
                    }
                }
                else
                {
                    cout << "IA: No quiero" << endl;
                    Sleep(1000);
                    puntajeHumano += 2;
                }
            break;
            case 4:
                if ( puntajeCartaEnvido(cartasIA) > 29 or randomPorcentaje(30) )
                {
                    cout << "IA: Quiero!" << endl;
                    Sleep(1000);
                    cout << "TU: " << puntajeCartaEnvido(cartasHumano) << endl;
                    Sleep(1000);
                    if(puntajeCartaEnvido(cartasIA) > puntajeCartaEnvido(cartasHumano))
                    {
                        cout << "IA: " << puntajeCartaEnvido(cartasIA) << " son mejores";
                        Sleep(1000);
                        puntajeIA += 5;
                    }
                    else
                    {
                        cout << "IA: son buenas" << endl;
                        Sleep(1000);
                        puntajeHumano += 5;
                    }
                }
                else
                {
                    cout << "IA: No quiero" << endl;
                    Sleep(1000);
                    puntajeHumano += 2;
                }
            break;
            case 5:
                if ( puntajeCartaEnvido(cartasIA) > 29 or randomPorcentaje(30) )
                {
                    cout << "IA: Quiero!" << endl;
                    Sleep(1000);
                    cout << "TU: " << puntajeCartaEnvido(cartasHumano) << endl;
                    Sleep(1000);
                    if(puntajeCartaEnvido(cartasIA) > puntajeCartaEnvido(cartasHumano))
                    {
                        cout << "IA: " << puntajeCartaEnvido(cartasIA) << " son mejores";
                        puntajeIA += 15 - puntajeHumano;
                        Sleep(1000);
                    }
                    else
                    {
                        cout << "IA: son buenas" << endl;
                        Sleep(1000);
                        puntajeHumano = 15 - puntajeIA;
                    }
                }
                else
                {
                    cout << "IA: No quiero" << endl;
                    Sleep(1000);
                    puntajeHumano += 2;
                }
            break;
        }
    }
    else
    {
        cout << "1) Envido" << endl <<  "2) Real Envido" << endl <<  "3) Falta Envido" << endl << "Opcion:";
        opcion = -1;
        while (opcion != 1 and opcion != 2 and opcion != 3) cin >> opcion;
        switch (opcion)
        {
            case 1: cout << "TU: Envido!" << endl; break;
            case 2: cout << "TU: Real Envido!" << endl; break;
            case 3: cout << "TU: Falta Envido!" << endl; break;
        }
        if ( ( (puntajeCartaEnvido(cartasIA) > 28 or randomPorcentaje(30)) and (opcion == 1 or opcion == 2) )
        or
        (opcion == 3 and ( puntajeCartaEnvido(cartasIA) > 29 or randomPorcentaje(30) ) ) )
        {
            cout << "IA: Quiero";
            Sleep(1000);
            cout << "TU: " << puntajeCartaEnvido(cartasHumano) << endl;
            Sleep(1000);
            if (puntajeCartaEnvido(cartasIA) > puntajeCartaEnvido(cartasHumano))
            {
                cout << "IA: " << puntajeCartaEnvido(cartasIA) << " son mejores";
                switch (opcion)
                {
                    case 1: puntajeIA += 2; break;
                    case 2: puntajeIA += 3; break;
                    case 3: puntajeIA += 15 - puntajeHumano; break;
                }
            }
            else
            {
                cout << "IA: son buenas" << endl;
                switch (opcion)
                {
                    case 1: puntajeHumano += 2; break;
                    case 2: puntajeHumano += 3; break;
                    case 3: puntajeHumano += 15 - puntajeIA; break;
                }
            }
            Sleep(1000);
        }
        else
        {
            cout << "IA: No quiero";
            Sleep(1000);
            puntajeHumano += 1;
        }
    }
    envidoCantado = true;
}
void inteligenciaIA(int CartaQueJugoPersona, int &CartaQueJugoIA, int cartasIA[3], int cartasHumano[3], bool bCartasIA[3], int PriSegTercartasIA[3], int ronda[3],
bool trucos[3], int &puntajeIA, int &puntajeHumano, int &ganaMano, bool &HumanoPuedeCantar, bool &IAPuedeCantar, bool &envidoCantado)
{
    /// -1 --> NO SE JUGO   0 --> GANA IA   1 --> GANA HUMANO   2 --> EMPATE
    if (ronda[1] != -1)
    {
    ///TERCER RONDA----------------------
        for (int j = 0; j < 3; j++) if (!bCartasIA[j])
        {
            CartaQueJugoIA = cartasIA[j];
            bCartasIA[j] = true;
            PriSegTercartasIA[2] = cartasIA[j];
        }
        if (puntajeCarta(CartaQueJugoIA) > 12 or randomPorcentaje(25))
        cantarTruco(true, trucos, cartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar);
    }
    else
    {
        if (ronda[0] != -1)
        {
        ///SEGUNDA RONDA----------------------
            switch (ronda[0])
            {
                case 0: ///TIRA PRIMERO IA
                    ///TIRAMOS RANDOM LA SEGUNDA CARTA
                    if (bCartasIA[0])
                    {
                        if (randomPorcentaje(50))
                        {
                            bCartasIA[1] = true;
                            CartaQueJugoIA = cartasIA[1];
                            PriSegTercartasIA[1] = cartasIA[1];
                        }
                        else
                        {
                            bCartasIA[2] = true;
                            CartaQueJugoIA = cartasIA[2];
                            PriSegTercartasIA[1] = cartasIA[2];
                        }
                    }
                    else
                    {
                        if (bCartasIA[1])
                        {
                            if (randomPorcentaje(50))
                            {
                                bCartasIA[2] = true;
                                CartaQueJugoIA = cartasIA[2];
                                PriSegTercartasIA[1] = cartasIA[2];
                            }
                            else
                            {
                                bCartasIA[0] = true;
                                CartaQueJugoIA = cartasIA[0];
                                PriSegTercartasIA[1] = cartasIA[0];
                            }

                        }
                        else
                        {
                            if (randomPorcentaje(50))
                            {
                                bCartasIA[1] = true;
                                CartaQueJugoIA = cartasIA[1];
                                PriSegTercartasIA[1] = cartasIA[1];
                            }
                            else
                            {
                                bCartasIA[0] = true;
                                CartaQueJugoIA = cartasIA[0];
                                PriSegTercartasIA[1] = cartasIA[0];
                            }
                        }
                    }
                    if (puntajeCarta(CartaQueJugoIA) > 9 or randomPorcentaje(25))
                    cantarTruco(true, trucos, cartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar);
                break;

                case 1: ///TIRA SEGUNDO IA
                    for (int j = 0; j < 3; j++)
                    {
                        if (puntajeCarta(cartasIA[j]) >= puntajeCarta(CartaQueJugoPersona) and !bCartasIA[j])
                        {
                            bCartasIA[j] = true;
                            CartaQueJugoIA = cartasIA[j];
                            PriSegTercartasIA[1] = cartasIA[j];
                            break;
                        }
                    }
                    if (!bCartasIA[1] and !bCartasIA[2])
                    {
                        bCartasIA[1] = true;
                        CartaQueJugoIA = cartasIA[1];
                        PriSegTercartasIA[1] = cartasIA[1];
                    }
                    if (puntajeCarta(CartaQueJugoIA) > puntajeCarta(CartaQueJugoPersona))
                    cantarTruco(true, trucos, cartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar);
                break;

                case 2: ///PARDA LA MEJOR
                    for (int j = 2; j >= 0; j--) if (bCartasIA[j])
                    {
                        CartaQueJugoIA = cartasIA[j];
                        bCartasIA[j] = true;
                        PriSegTercartasIA[1] = cartasIA[j];
                    }
                    if (puntajeCarta(CartaQueJugoIA) > 10)
                    {
                        cantarTruco(true, trucos, cartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar);
                    }
                break;
                default: cout << "ERROR inteligenciaIA" << endl; Sleep(5000);
            }
        }
        else
        {
        ///PRIMERA RONDA----------------------
            if((puntajeCartaEnvido(cartasIA) > 29 or randomPorcentaje(30)) and !envidoCantado)
            cantarEnvido(puntajeHumano, puntajeIA, true, cartasIA, cartasHumano, envidoCantado);
            if (CartaQueJugoPersona == -1) ///SI ES -1 ENTONCES EMPIEZA JUGANDO IA
            {
                for (int j = 0; j < 3; j++)
                if (puntajeCarta(cartasIA[j]) >= puntajeCarta(CartaQueJugoPersona))
                {
                    bCartasIA[j] = true;
                    CartaQueJugoIA = cartasIA[j];
                    PriSegTercartasIA[0] = cartasIA[j];
                    break;
                }

                if (!bCartasIA[0] and !bCartasIA[1] and !bCartasIA[2])
                {
                    bCartasIA[0] = true;
                    CartaQueJugoIA = cartasIA[0];
                    PriSegTercartasIA[0] = cartasIA[0];
                }
            }
            else
            {
                ///TIRA LA MAS ALTA (DAR MIEDO)
                ///COMO ESTAN ORDENADAS DE MENOR A MAYOR ENTONCES TIRA LA ULTIMA
                bCartasIA[2] = true;
                CartaQueJugoIA = cartasIA[2];
                PriSegTercartasIA[0] = cartasIA[2];
            }
        }
    }
}
void mostrarJuego(int cartasHumano[3], int PriSegTercartasIA[3], bool bCartasHumano[3], int puntajeHumano, int puntajeIA)
{
    clean();
    cout << " YO    |  IA   " << endl;
    cout << "-------|-------" << endl;
    for(int i = 0; i < 3; i++)
    {
        if(puntajeHumano == 1+i*5)
        cout << "  |    |";
        if(puntajeHumano == 2+i*5)
        cout << "  |_   |";
        if(puntajeHumano == 3+i*5)
        cout << "  |_|  |";
        if(puntajeHumano == 4+i*5)
        cout << "  [ ]  |";
        if(puntajeHumano >= 5+i*5)
        cout << "  [/]  |";
        if(puntajeHumano <= i*5)
        cout << "       |";

        if(puntajeIA <= i*5)
        cout << "       "<<endl;
        if(puntajeIA == 1+i*5)
        cout << "  |    "<<endl;
        if(puntajeIA == 2+i*5)
        cout << "  |_   "<<endl;
        if(puntajeIA == 3+i*5)
        cout << "  |_|  "<<endl;
        if(puntajeIA == 4+i*5)
        cout << "  [ ]  "<<endl;
        if(puntajeIA >= 5+i*5)
        cout << "  [/]  "<<endl;
    }
    cout << endl << "TUS CARTAS" << endl;
    cout << "1)                    2)                    3)" << endl;
    for (int i = 0; i < 10; i++)
    {
        if (bCartasHumano[0]) color(0,11); mostrarCarta(cartasHumano[0], i); color(7,0);
        if (bCartasHumano[1]) color(0,11); mostrarCarta(cartasHumano[1], i); color(7,0);
        if (bCartasHumano[2]) color(0,11); mostrarCarta(cartasHumano[2], i); color(7,0);
        cout << endl;
    }
    cout << endl << endl;
    if (PriSegTercartasIA[0] != -1 or PriSegTercartasIA[1] != -1 or PriSegTercartasIA[2] != -1)
    {
        color(0,15); cout << "CARTAS IA"; color(7,0); cout << endl;
        for (int i = 0; i < 10; i++)
        {
            if (PriSegTercartasIA[0] != -1) {color(0,15); mostrarCarta(PriSegTercartasIA[0], i); color(7,0);}
            if (PriSegTercartasIA[1] != -1) {color(0,15); mostrarCarta(PriSegTercartasIA[1], i); color(7,0);}
            if (PriSegTercartasIA[2] != -1) {color(0,15); mostrarCarta(PriSegTercartasIA[2], i); color(7,0);}
            cout << endl;
        }
    }
}
void color(int letra, int fondo)
{
    int x = letra + fondo * 16;
    /* GUIA DE COLORES
    0 = Negro
    1 = Azul
    2 = Verde
    3 = Aguamarina
    4 = Rojo
    5 = Purpura
    6 = Amarillo
    7 = Blanco
    8 = Gris
    9 = Azul claro
    10 = Verde claro
    11 = Aguamarina claro
    12 = Rojo claro
    13 = Purpura claro
    14 = Amarillo claro
    15 = Blanco brillante */
    //SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),x);
}
bool randomPorcentaje(int porcentaje)
{
    ///DADO UN PORCENTAJE DEVUELVE TRUE OR FALSE
    return (rand()%100+1) < porcentaje;
}
int puntajeCarta(int carta)
{
    int puntaje;
    int palo = carta/10;
    int numero = carta%10;
    if (numero <= 6) numero++;
    else numero += 3;
    switch(palo)
    {
        case 0: //ORO
            switch (numero) {
                case 1: puntaje = 8; break;
                case 2: puntaje = 10; break;
                case 3: puntaje = 11; break;
                case 4: puntaje = 1; break;
                case 5: puntaje = 2; break;
                case 6: puntaje = 3; break;
                case 7: puntaje = 12; break;
                case 10: puntaje = 5; break;
                case 11: puntaje = 6; break;
                case 12: puntaje = 7; break;
                default: cout << "ERROR" << endl;
            }
        break;
        case 1: //COPA
            switch (numero) {
                case 1: puntaje = 8; break;
                case 2: puntaje = 10; break;
                case 3: puntaje = 11; break;
                case 4: puntaje = 1; break;
                case 5: puntaje = 2; break;
                case 6: puntaje = 3; break;
                case 7: puntaje = 4; break;
                case 10: puntaje = 5; break;
                case 11: puntaje = 6; break;
                case 12: puntaje = 7; break;
                default: cout << "ERROR" << endl;
            }
        break;
        case 2: //ESPADA
            switch (numero) {
                case 1: puntaje = 15; break;
                case 2: puntaje = 10; break;
                case 3: puntaje = 11; break;
                case 4: puntaje = 1; break;
                case 5: puntaje = 2; break;
                case 6: puntaje = 3; break;
                case 7: puntaje = 13; break;
                case 10: puntaje = 5; break;
                case 11: puntaje = 6; break;
                case 12: puntaje = 7; break;
                default: cout << "ERROR" << endl;
            }
        break;
        case 3: //BASTO
            switch (numero) {
                case 1: puntaje = 14; break;
                case 2: puntaje = 10; break;
                case 3: puntaje = 11; break;
                case 4: puntaje = 1; break;
                case 5: puntaje = 2; break;
                case 6: puntaje = 3; break;
                case 7: puntaje = 4; break;
                case 10: puntaje = 5; break;
                case 11: puntaje = 6; break;
                case 12: puntaje = 7; break;
                default: cout << "ERROR" << endl;
            }
        break;
        default: cout << "ERROR" << endl;
    }
    return puntaje;
}
int puntajeCartaEnvido(int cartas[3])
{
    int a, b, c;
    a = cartas[0]%10;
    if (a <= 6) a++;
    else a = 0;
    b = cartas[1]%10;
    if (b <= 6) b++;
    else b = 0;
    c = cartas[2]%10;
    if (c <= 6) c++;
    else c = 0;

    int envido;
    if (cartas[0]/10 == cartas[1]/10 and cartas[0]/10 == cartas[2]/10 and cartas[1]/10 == cartas[2]/10 )
    {
        if (a >= b and a >= c)
        {
            if (b >= c)
            {
                envido = a + b;
            }
            else envido = a + c;
        }
        else
        {
            if (b >= a and b >= c)
            {
                if (a >= c)
                {
                    envido = b + a;
                }
                else envido = b + c;
            }
            else
            {
                if (a >= b) envido = c + a;
                else envido = c + b;
            }
        }
    }
    else
    {
        if (cartas[0]/10 == cartas[1]/10)
        {
          envido = a + b;
        }
        else
        {
            if (cartas[1]/10 == cartas[2]/10)
            {
                envido = b + c;
            }
            else
            {
                if (cartas[0]/10 == cartas[2]/10)
                {
                    envido = a + c;
                }
            }
        }
    }
    envido += 20;
    if (cartas[0]/10 != cartas[1]/10 and  cartas[0]/10 != cartas[2]/10 and cartas[1]/10 != cartas[2]/10)
    {
        if (a >= b and a >= c) envido = a;
        else if (b >= a and b >= c) envido = b;
                else envido = c;
    }
    return envido;
}
void menu()
{
    cout << "1--> Jugar" << endl << "2--> Elegir Cartas (NO TERMINADO)" << endl << "3--> Salir" << endl;
}
void randomCartas(int cartasIA[3], int cartasHumano[3])
{
    int Mazo[40], cartasAsacar[6];
    for (int i = 0; i < 40; i++)
    {
        Mazo[i] = i;
    }

    ///--------------- REPARTIR ---------------
    for (int i = 0; i < 6; i++)
    {
        bool continuar = false;
        ///WHILE PARA RANDOMEAR CARTA HASTA QUE SEA DIFERENTE DE TODAS LAS ANTERIORES
        while(!continuar)
        {
            continuar = true;
            cartasAsacar[i] = rand()%40;
            for (int j = 0; j < i; j++)
            {
                if (cartasAsacar[i] == cartasAsacar[j]) continuar = false;
            }
        }
    }
    cartasIA[0] = cartasAsacar[0];
    cartasIA[1] = cartasAsacar[2];
    cartasIA[2] = cartasAsacar[4];
    cartasHumano[0] = cartasAsacar[1];
    cartasHumano[1] = cartasAsacar[3];
    cartasHumano[2] = cartasAsacar[5];
    /// ------------------------------------------

    ///--------------ORDENAR CARTAS IA MENOR A MAYOR------------
    int a = puntajeCarta(cartasIA[0]);
    int b = puntajeCarta(cartasIA[1]);
    int c = puntajeCarta(cartasIA[2]);
    bool ordenado = false;
    while(!ordenado)
    {
        ordenado = true;
        for (int i = 0; i < 2; i++)
        {
            if (puntajeCarta(cartasIA[i]) > puntajeCarta(cartasIA[i+1]))
            {
                ordenado = false;
                swap(cartasIA[i], cartasIA[i+1]);
            }
        }
    }
    /// ----------------------------------------------------------
};
void elegirCartas(int cartasIA[3], int cartasHumano[3], bool &anteriorManoEmpezoIA)
{
    clean();
    int opcion;
    cout << "0-9 -> ORO  10-19 -> COPA  20-29 -> ESPADA  30-39 -> BASTO" << endl;
    cout << "NUMERO = CARTA % 10 + 3 (SI ES MAYOR A 6) + 1 (SI ES MENOR A 7)" << endl << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << "Elija carta " << i+1 << " IA (0-40): ";
        opcion = -1;
        while (opcion < 0 or opcion > 39) cin >> opcion;
        cartasIA[i] = opcion;
    }
    cout << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << "Elija carta " << i+1 << " Humano (0-40): ";
        opcion = -1;
        while (opcion < 0 or opcion > 39) cin >> opcion;
        cartasHumano[i] = opcion;
    }
    cout << endl;
    cout << "Quien Empieza? 1) IA  2) TU" << endl;
    opcion = -1;
    while (opcion != 1 and opcion != 2) cin >> opcion;
    if (opcion == 1) anteriorManoEmpezoIA = false;
    else anteriorManoEmpezoIA = true;
}

void empezarJuego(bool bElegirCartas)
{
    int puntajeIA = 0, puntajeHumano = 0;
    bool anteriorManoEmpezoIA = true;

    while(puntajeHumano < 15 and puntajeIA < 15)
    {
        int cartasIA[3], cartasHumano[3], PriSegTercartasIA[3];
        bool trucos[3]; ///TRUCO, RETRUCO, VALE4
        bool HumanoPuedeCantar = true, IAPuedeCantar = true, bCartasHumano[3], bCartasIA[3], envidoCantado = false;
        int CartaQueJugoIA, CartaQueJugoPersona;
        int ronda[3], ganaMano = -1;
         /// -1 --> Todavia nadie gano    0 --> GANA IA    1 --> GANA HUMANO    2 --> EMPATE
        for(int i = 0; i < 3; i++)
        {
            PriSegTercartasIA[i] = -1;
            bCartasIA[i] = false;
            bCartasHumano[i] = false;
            trucos[i] = false;
            ronda[i] = -1;
        }

        if (!bElegirCartas) randomCartas(cartasIA, cartasHumano);
        else elegirCartas(cartasIA, cartasHumano, anteriorManoEmpezoIA);

        mostrarJuego(cartasHumano,  PriSegTercartasIA, bCartasHumano, puntajeHumano, puntajeIA);
        if (anteriorManoEmpezoIA)
        {
            humanoDecide(cartasHumano, cartasIA, PriSegTercartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar, CartaQueJugoPersona, ronda, trucos, bCartasHumano, bCartasIA, puntajeIA, puntajeHumano, envidoCantado);
            if (ganaMano == -1) inteligenciaIA(CartaQueJugoPersona, CartaQueJugoIA, cartasIA, cartasHumano, bCartasIA, PriSegTercartasIA, ronda, trucos, puntajeIA, puntajeHumano, ganaMano, HumanoPuedeCantar, IAPuedeCantar, envidoCantado);
            anteriorManoEmpezoIA = false;
        }
        else
        {
            cout << endl << "Escribe algo y Presiona Enter Para Continuar ..." << endl; string aux; cin >> aux;
            inteligenciaIA(CartaQueJugoPersona, CartaQueJugoIA, cartasIA, cartasHumano, bCartasIA, PriSegTercartasIA, ronda, trucos, puntajeIA, puntajeHumano, ganaMano, HumanoPuedeCantar, IAPuedeCantar, envidoCantado);
            if (ganaMano == -1) humanoDecide(cartasHumano, cartasIA, PriSegTercartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar, CartaQueJugoPersona, ronda, trucos, bCartasHumano, bCartasIA, puntajeIA, puntajeHumano, envidoCantado);
            anteriorManoEmpezoIA = true;
        }
        if (ganaMano == -1) chequearQuienGano(CartaQueJugoPersona, CartaQueJugoIA, ronda, 0, ganaMano);

        for (int i = 1; i < 3 and ganaMano == -1 ; i++)
        {
            mostrarJuego(cartasHumano, PriSegTercartasIA, bCartasHumano, puntajeHumano, puntajeIA);
            if (ronda[i-1] == 1)
            {
                humanoDecide(cartasHumano, cartasIA, PriSegTercartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar, CartaQueJugoPersona, ronda, trucos, bCartasHumano, bCartasIA, puntajeIA, puntajeHumano, envidoCantado);
                if (ganaMano == -1) inteligenciaIA(CartaQueJugoPersona, CartaQueJugoIA, cartasIA, cartasHumano, bCartasIA, PriSegTercartasIA, ronda, trucos, puntajeIA, puntajeHumano, ganaMano, HumanoPuedeCantar, IAPuedeCantar, envidoCantado);
            }
            else
            {
                if (ronda[i-1] == 0)
                {
                    inteligenciaIA(CartaQueJugoPersona, CartaQueJugoIA, cartasIA, cartasHumano, bCartasIA, PriSegTercartasIA, ronda, trucos, puntajeIA, puntajeHumano, ganaMano, HumanoPuedeCantar, IAPuedeCantar, envidoCantado);
                    if (ganaMano == -1) humanoDecide(cartasHumano, cartasIA, PriSegTercartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar, CartaQueJugoPersona, ronda, trucos, bCartasHumano, bCartasIA, puntajeIA, puntajeHumano, envidoCantado);
                }
                else
                {
                    if (anteriorManoEmpezoIA) ///QUIEN JUGO PRIMERA EN LA PRIMERA RONDA?
                    {
                        inteligenciaIA(CartaQueJugoPersona, CartaQueJugoIA, cartasIA, cartasHumano, bCartasIA, PriSegTercartasIA, ronda, trucos, puntajeIA, puntajeHumano, ganaMano, HumanoPuedeCantar, IAPuedeCantar, envidoCantado);
                        if (ganaMano == -1) humanoDecide(cartasHumano, cartasIA, PriSegTercartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar, CartaQueJugoPersona, ronda, trucos, bCartasHumano, bCartasIA, puntajeIA, puntajeHumano, envidoCantado);
                    }
                    else
                    {
                        humanoDecide(cartasHumano, cartasIA, PriSegTercartasIA, ganaMano, HumanoPuedeCantar, IAPuedeCantar, CartaQueJugoPersona, ronda, trucos, bCartasHumano, bCartasIA, puntajeIA, puntajeHumano, envidoCantado);
                        if (ganaMano == -1) inteligenciaIA(CartaQueJugoPersona, CartaQueJugoIA, cartasIA, cartasHumano, bCartasIA, PriSegTercartasIA, ronda, trucos, puntajeIA, puntajeHumano, ganaMano, HumanoPuedeCantar, IAPuedeCantar, envidoCantado);
                    }
                }
            }
            if (ganaMano == -1)
            {
                chequearQuienGano(CartaQueJugoPersona, CartaQueJugoIA, ronda, i, ganaMano);
            }
        }
        if (ronda[2] != -1 and ganaMano == -1) ganaMano =  ronda[2];
        mostrarJuego(cartasHumano, PriSegTercartasIA, bCartasHumano, puntajeHumano, puntajeIA);
        ///DAR PUNTAJE CORRESPONDIENTE
        int CalcularPuntaje = 0;
        if (trucos[2]) CalcularPuntaje += 4;
        else
        {
            if(trucos[1]) CalcularPuntaje += 3;
            else
            {
                if (trucos[0]) CalcularPuntaje += 2;
                else CalcularPuntaje += 1;
            }
        }
        if (ganaMano == 0) puntajeIA += CalcularPuntaje;
        else puntajeHumano += CalcularPuntaje;
        Sleep(3000);
    }
    if (puntajeHumano >= 15) cout << "Ganaste!";
    if (puntajeIA >= 15) cout << "Perdiste!";
}

void mostrarCarta(int carta, int renglon)
{
    int resto = carta/10;
    int numero = carta%10;
    if (numero <= 6) numero++;
    else numero += 3;
    string palo;
    switch(resto)
    {
    case 0:
        switch (renglon)
        {
            case 0: cout << " ____________________ " ; break;
            case 1:
                if (numero < 8)
                    cout << "|"<<numero<<"                   |" ;
                else
                    cout << "|"<<numero<<"                  |" ;
            break;
            case 2: cout << "|         ==         |" ; break;
            case 3: cout << "|      ==    ==      |" ; break;
            case 4: cout << "|    ==        ==    |" ; break;
            case 5: cout << "|   =            =   |" ; break;
            case 6: cout << "|    ==        ==    |" ; break;
            case 7: cout << "|      ==    ==      |" ; break;
            case 8: cout << "|         ==         |" ; break;
            case 9:
                if (numero < 8)
                    cout << "|___________________" << numero << "|";
                else
                    cout << "|__________________" << numero << "|";
            break;
        }
    break;
    case 1:
        switch (renglon)
        {
            case 0: cout << " ____________________ " ; break;
            case 1:
                if (numero < 8)
                    cout << "|"<<numero<<"                   |" ;
                else
                    cout << "|"<<numero<<"                  |" ;
            break;
            case 2: cout << "|    |@@@@@@@@@@|    |" ; break;
            case 3: cout << "|    (((((())))))    |" ; break;
            case 4: cout << "|     \\        /     |" ; break;
            case 5: cout << "|      \\      /      |" ; break;
            case 6: cout << "|       \\    /       |" ; break;
            case 7: cout << "|        (  )        |" ; break;
            case 8: cout << "|       __||__       |" ; break;
            case 9:
                if (numero < 8)
                    cout << "|___________________" << numero << "|";
                else
                    cout << "|__________________" << numero << "|";
            break;
        }
    break;
    case 2:
        switch (renglon)
        {
            case 0: cout << " ____________________ " ; break;
            case 1:
                if (numero < 8)
                    cout << "|"<<numero<<"                   |" ;
                else
                    cout << "|"<<numero<<"                  |" ;
            break;
            case 2: cout << "|         ^          |" ; break;
            case 3: cout << "|        | |         |" ; break;
            case 4: cout << "|        | |         |" ; break;
            case 5: cout << "|        | |         |" ; break;
            case 6: cout << "|        | |         |" ; break;
            case 7: cout << "|     ((( * )))      |" ; break;
            case 8: cout << "|        \\ /         |" ; break;
            case 9:
                if (numero < 8)
                    cout << "|___________________" << numero << "|";
                else
                    cout << "|__________________" << numero << "|";
            break;
        }
    break;
    case 3:
        switch (renglon)
        {
            case 0: cout << " ____________________ " ; break;
            case 1:
                if (numero < 8)
                    cout << "|"<<numero<<"                   |" ;
                else
                    cout << "|"<<numero<<"                  |" ;
            break;
            case 2: cout << "|          ____      |" ; break;
            case 3: cout << "|         /    \\     |" ; break;
            case 4: cout << "|        /      /    |" ; break;
            case 5: cout << "|      \\/     \\      |" ; break;
            case 6: cout << "|       /    /       |" ; break;
            case 7: cout << "|      /   /\\        |" ; break;
            case 8: cout << "|     /__/           |" ; break;
            case 9:
                if (numero < 8)
                    cout << "|___________________" << numero << "|";
                else
                    cout << "|__________________" << numero << "|";
            break;
        }
    break;
    }
}
int main()
{
    srand(time(NULL));
    bool salir = false;
    while(!salir)
    {
        int opcion;
        menu();
        cin >> opcion;
        switch (opcion)
        {
            case 1:
                empezarJuego(false);
            break;
            case 2:
                empezarJuego(true);
            break;
            case 3:
                salir = true;
            break;
        }
    }
    return 0;
}
