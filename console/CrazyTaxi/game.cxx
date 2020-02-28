#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <thread> 
#include <chrono> 
#include "game.h"
#include <string.h> 

using namespace std;

enum partesAuto {
	libre= 0,
	miParte1 = 1,
	miParte2 = 2,
	miParte3 = 3,
	iaparte1 = 7,
	iaparte2 = 4,
	iaparte3 = 5,
	iaparte4 = 6
	
};

void juego::menu() {
	srand(time(NULL));
	char a;
	while (a!='1'){
		clear();	
		printw( " \n                Bienvenido al New CarDriver (Version 2) \n                  Iniciar (1) \n                  Instrucciones (2) \n                  Modo Extreme (4) \n");
		printw( " \n_________________________$$$$$$$$$$$$$$$$" ); 
		printw( " \n______________________$$$$____$$$$$$____$$$$"); 
		printw( " \n____________________$$$$________$$________$$$$$"); 
		printw( " \n___________$$$$$$$$$$$$$________$$________$$$$$$$"); 
		printw( " \n_________$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"); 
		printw( " \n_______$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"); 
		printw( " \n_______$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"); 
		printw( " \n________$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"); 
		printw( " \n___________$$$$$$$____________________$$$$$$$"); 
		printw( " \n____________$$$$$______________________$$$$$");
		
		printw("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n            |Ajuste su consola a esta medida|");
		printw("\n<----------------------------------------------------------------------->\n");
		
		
		a=getch();
		if (a=='2'){
			clear();			
			printw ("\n INSTRUCCIONES:  \n -------------------  \n A --> Moverse Izquierda \n D --> Moverse Derecha \n  \n Iniciar el Juego? (1) \n Opciones (3) \n Volver al menu (Otra Tecla) \n");
			a=getch();
		}
	}
		iniciar();
}
	


juego::juego() {
	initscr();
	for(int i = 0; i < altoTotal; i++) {
		for(int j = 0; j < ancho ; j++ ) {
			pos[i][j]=0;
		}	
	}
	choque = false;
	contador = 1;
	puntaje = 0;
	velocidad = 70;
	aux=true;
	aux2=true;
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, COLOR_RED, COLOR_BLACK);
	
}

/*juego::~juego () {
	for(int i=0; i<y; i++) delete pos[i];
	delete pos;
}*/

void juego::aparecerAutos(){
	z = rand()%ancho;
	if(contador % 4==0) {
		pos[1][z]= partesAuto::iaparte1;
		pos[2][z]= partesAuto::iaparte2;
		pos[3][z]= partesAuto::iaparte3;
		pos[4][z]= partesAuto::iaparte4;
		puntaje++;}
		
	
	/*z = rand()%ancho;
	if (contador%5==0){ 
		pos[0][z]=4;
		pos[1][0]=4;
		pos[2][0]=5;
		pos[3][0]=6;
		puntaje++;
		}*/


/*20      50      70
\         |         /
 \        |        /
  \       |       /
   \      |      /
    \     |     /
     \    |    /*/


	//7 --> |*|
	//4 --> |*|
	//5 --> \*/
	//6 -->  ¯
}  

void juego::moverAutos() {
	int i, j;
	//Mover FIlas		
	for(i = altoTotal - 1;  i > 0 ; i--)
			for ( j = 0 ; j < ancho; j++)
					if(pos[i][j] != miAuto && pos[i][j] != 2 && pos[i][j] != 3) pos[i][j] = pos[i - 1][j] ;
	
	//Chequear Choque				
	for(i = 0;  i < altoTotal ; i++)
		for ( j = 0 ; j < ancho; j++){			
					if (pos[i][j]==1) if (pos[i-1][j]==5) choque = true;
					if (pos[i][j]==2) if (pos[i-1][j]==5) choque = true;}
					
	contador++;
}  




void juego::iniciar() {
	noecho();
	nodelay(stdscr,TRUE);
	
	
	
	pos[altoTotal - 4][ancho / 2] = partesAuto::miParte1;
	pos[altoTotal - 3][ancho / 2] = partesAuto::miParte2;
	pos[altoTotal - 2][ancho / 2] = partesAuto::miParte3;
	pos[altoTotal - 1][ancho / 2] = partesAuto::miParte2;
	
	while(!choque) {
		aparecerAutos();
		char tecla = getch();
		this_thread::sleep_for(chrono::milliseconds(velocidad));
		clear();
		switch(tecla) {
			case 'a': moverIzq(); break;
			case 'A': moverIzq(); break;
			case 'd': moverDer(); break;
			case 'D': moverDer(); break;
			case 'p': pausa(); break;
			case 'P': pausa(); break;
		}
		imprimirJuego();
		moverAutos();
	}
	
}

void juego::imprimirJuego() {
	int z, x;
	z = rand()%6+1;
	for (int i = zonaSpawn + 1; i < altoTotal; i++){
		for (int j=0; j< ancho; j++){
			switch(pos[i][j]){
			case 0: 
					x = rand()%5+1;
					attron(COLOR_PAIR(x)); printw("|       |"); attroff(COLOR_PAIR(x)); break;	
				
			case 7: printw("|  |"); attron(COLOR_PAIR(z)); addch(ACS_DIAMOND); attroff(COLOR_PAIR(z)); printw("|  |");	break;		
			case 4: printw("|  |"); attron(COLOR_PAIR(z)); addch(ACS_DIAMOND); attroff(COLOR_PAIR(z)); printw("|  |");	break;	
			case 5: printw("|  ");addch(ACS_PLUS); printw(" "); addch(ACS_PLUS); printw("  |"); break;	
			case 6: printw("|  |"); addch(ACS_S9); printw("|  |"); break;
			
			case 1: printw("|  "); attron(COLOR_PAIR(1)); printw("/"); addch(ACS_UARROW); printw("\\");  attroff(COLOR_PAIR(1)); printw("  |"); break;
			case 2: printw("| "); attron(COLOR_PAIR(1)); addch(ACS_BLOCK); printw("|");  addch(ACS_DIAMOND); printw("|"); addch(ACS_BLOCK); attroff(COLOR_PAIR(1)); printw(" |");  break;
			case 3: printw("|  "); attron(COLOR_PAIR(1)); printw("|"); addch(ACS_DIAMOND); printw("|  "); attroff(COLOR_PAIR(1)); printw("|"); break;
			
			default: printw("  CHOQUE ");
			choque = true;
			}			
		}
	printw("\n");		
	}
	printw("-------------------------------------------------------------------------");
	
	if (puntaje==50 && aux==true) {velocidad -= 20; aux=false;}
	if (puntaje==100 && aux2==true) {velocidad -= 20; aux2=false;}
	printw("\n                                         Puntaje: %d", puntaje);
	switch(velocidad){
		case 70: printw("\n 20       50         70\n  \\    \\    |    /    /\n                      \n    \\                \n     \\              \n      \\            \n       \\          \n       ----------"); break;
		case 50: printw("\n 20       50         70\n  \\    \\    |    /    /\n                      \n            |        \n            |       \n            |      \n            |     \n       ----------"); break;
		case 30: printw("\n 20       50         70\n  \\    \\    |    /    /\n                      \n                    /\n                   /\n                  /\n                 /\n       ----------"); break;
		}
	if(puntaje > 250 && puntaje < 500) printw("\n\nTOO EZ FOR ME! MAKE IT HARDER!!");
	refresh();	
}	

void juego::moverDer () {
		int j;
		for (j=0;  pos[altoTotal - 4][j]  != 1;  j++) ;
		if (j < ancho - 1) {
			pos[altoTotal - 4][j] = partesAuto::libre;
			pos[altoTotal - 3][j] = partesAuto::libre;
			pos[altoTotal - 2][j] = partesAuto::libre;
			pos[altoTotal - 1][j] = partesAuto::libre;
			pos[altoTotal - 4][j+1] += partesAuto::miParte1;
			pos[altoTotal - 3][j+1] += partesAuto::miParte2;
			pos[altoTotal - 2][j+1] += partesAuto::miParte3;
			pos[altoTotal - 1][j+1] += partesAuto::miParte2;
		}
			
}

void juego::moverIzq () {
	int j;
		for (j=0;  pos[altoTotal - 4][j]  != 1;  j++) ;
		if (j > 0) {
			pos[altoTotal - 4][j] = partesAuto::libre;
			pos[altoTotal - 3][j] = partesAuto::libre;
			pos[altoTotal - 2][j] = partesAuto::libre;
			pos[altoTotal - 1][j] = partesAuto::libre;
			pos[altoTotal - 4][j-1] += partesAuto::miParte1;
			pos[altoTotal - 3][j-1] += partesAuto::miParte2;
			pos[altoTotal - 2][j-1] += partesAuto::miParte3;
			pos[altoTotal - 1][j-1] += partesAuto::miParte2;
		}
}  

void juego::pausa(){
	char p;
	p = getchar();
	while(p!='p') this_thread::sleep_for(chrono::milliseconds(velocidad));
}





