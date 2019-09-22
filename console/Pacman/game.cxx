//#define LINUX
#define	WINDOWS
#include <iostream>
#include <stdlib.h>
#include <time.h>

#ifdef LINUX
#include <ncurses.h>
#include <thread>
#include <chrono>
#endif

#ifdef WINDOWS
#include <windows.h>
#include "curses.h"
#endif

#include "game.h"
#include <string.h>
#include <fstream>
using namespace std;

enum obstaculos {
	arriba = 1,
	abajo = 2,
	izquierda = 3,
	derecha = 4
};

void dormir(int milliseconds){
	#ifdef WINDOWS
		Sleep(milliseconds);
	#endif
	#ifdef LINUX
		this_thread::sleep_for(chrono::milliseconds(milliseconds));
	#endif
}

juego::juego(){
	#ifdef WINDOWS
	system("mode 101, 45");
	#endif
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_RED, COLOR_BLACK);
	srand(time(NULL));
	int i, j;

  for (i=0; i < alto; i++){
		for (int j=0; j < ancho; j++){
			pos[i][j] = 0;
  }}

	vidas = 3;
	keypad(stdscr, TRUE);
	choque = false;
	win = false;
	obstaculoaux = false;
	obstaculo = 0;
	auxnivel = true;
	CDM = false;
	mapaCustom = false;

	//DIBUJAR MAPA
	for (i=0; i <= 7; i++){
		pos[(alto/2) -1][i] = 1;
		pos[(alto/2) +1][i] = 1;
		pos[(alto/2) -1][ancho-1-i] = 1;
		pos[(alto/2) +1][ancho-1-i] = 1;


		pos[(alto/2) -4 -i][3] =1;
		pos[(alto/2) +4 +i][3] =1;
		pos[(alto/2) -4 -i][ancho -1 -3] =1;
		pos[(alto/2) +4 +i][ancho -1 -3] =1;
	}

	for (i=0; i <= 10; i++){
		pos[(alto/2) +2 +i][0] =1;
		pos[(alto/2) -2 -i][0] =1;
		pos[(alto/2) +2 +i][ancho-1] =1;
		pos[(alto/2) -2 -i][ancho-1] =1;

		//FRUTILLAS
		pos[(alto/2) +2 +i][1] = 2;
		pos[(alto/2) -2 -i][1] = 2;
		pos[(alto/2) +2 +i][ancho-2] = 2;
		pos[(alto/2) -2 -i][ancho-2] = 2;
	}

	for (i=0; i <= 40; i++){
		pos[alto-1][i] = 1;
		pos[0][i] = 1;

		pos[alto-1][ancho-1-i] = 1;
		pos[0][ancho-1-i] = 1;

		pos[alto/2 +2][ancho/2 - 20 + i] = 1;
		pos[alto/2 -2][ancho/2 - 20 + i] = 1;
		pos[alto/2 +6][ancho/2 - 20 + i] = 1;
		pos[alto/2  -6][ancho/2 - 20 + i] = 1;
	}

	//FIN DIBUJO MAPA
	nivel = 1;
	velocidad = 70;
	frutillas = 0;
	for( i = 0; i < alto; i++){
		for( j = 0; j < ancho; j++){
		if (pos[i][j] == 2) frutillas ++;
		}
	}
}

int juego::menu(){
	nodelay(stdscr,FALSE);
	int z = 0, h = 0;
	int tecla;

	while (tecla != 10)
	{
		clear();
		attron(COLOR_PAIR(3));
		mvprintw(0, 0," _____   ");
		mvprintw(1, 0,"|  __ \\ ");
		mvprintw(2, 0, "| |__) |_ _  ___ _ __ ___   __ _ _ __ ");
		mvprintw(3, 0, "|  ___/ _` |/ __| '_ ` _ \\ / _` | '_ \\ ");
		mvprintw(4, 0,  "| |  | (_| | (__| | | | | | (_| | | | |" );
		mvprintw(5, 0,  "|_|   \\__,_|\\___|_| |_| |_|\\__,_|_| |_|" );
		attroff(COLOR_PAIR(3));
		printw("\n\n              Jugar\n");
		printw("              Records\n");
		printw("              Crea Tu Propio Mapa\n");
		printw("              Juega un mapa creado\n\n\n");
		for(int i=0; i<21; i++) printw("\n");
		for(int i=0; i<60; i++) printw(" ");
		printw("Schiavinato 3ET 10/09/15 - 10/10/15\n");
		mvprintw(7+h, 6, "--->");
		refresh();
		tecla = getch();
		switch (tecla){
			case KEY_UP: if (h > 0) h--;
			break;
			case KEY_DOWN: if (h < 3) h++;
			break;
			case 10:
			if (h != 0 and h != 3) tecla = 0;
				if (h == 1) records();
				if (h == 2) creadorDeMapas();
			break;
		}
	}
	if (h == 3) {
		char nombreMapa[15];
		clear();
		printw("Escriba el nombre del mapa (Sin espacios): ");
		getstr(nombreMapa);
		ifstream in(nombreMapa);
		for(int i = 0; i < alto; i++){
			for (int j = 0; j < ancho; j++){
				in >> pos[i][j];
				}
		}
		in >> yj >> xj >> yc >> xc;

		mapaCustom = true;
	}
	z = iniciar();
	return z;
}

void juego::records(){
	string nombres[30];
	ifstream in("records");
	int i = 0;
	clear();
	mvprintw(0, 0,"      ____                           __ ");
	mvprintw(1, 0,"     / __ \\___  _________  _________/ /____");
	mvprintw(2, 0,"    / /_/ / _ \\/ ___/ __ \\/ ___/ __  / ___/");
	mvprintw(3, 0,"   / _, _/  __/ /__/ /_/ / /  / /_/ (__  ) ");
	mvprintw(4, 0,"  /_/ |_|\\___/\\___/\\____/_/   \\__,_/____/  \n\n");

	while (i < 100 && !in.eof() && in.peek() != EOF) {
		in >> nombres[i];
		i++;
	}
	for (int j = 0; j < i; j++)
	printw("%s \n", nombres[j].c_str());
	refresh();
	dormir(5000);
}

int juego::iniciar(){
	noecho();
	nodelay(stdscr,TRUE);
	if (mapaCustom){
		nivel = 0;
	}
	while (!choque and !win){
		if (auxnivel) niveles();
		int tecla = getch();
		dormir(velocidad);
		clear();
		switch(tecla) {
			case KEY_LEFT: direc = -1; break;
			case KEY_RIGHT: direc = 1; break;
			case KEY_DOWN: direc = -2; break;
			case KEY_UP: direc = 2; break;
			case 112: pausa(); break;
		}
		moverPac();
		imprimirJuego();
	}

	if (choque) return 1;
	else {
		if (win) return 0;
		else return 2;
	}
}

void juego::moverPac(){
	bool turno = true;
	int i, j;

	//MOVER IA
	switch(obstaculo){
		case obstaculos::arriba:
			if (pos[yc+1][xc] == 0) {obstaculo = 0; turno = false;}
			if (obstaculoaux) {
				i = xc;
				j = xc;
				arriba = 0;
				abajo = 0;
				while(pos[yc+1][i]==1){ i++; arriba++;}
				while(pos[yc+1][j]==1){ j--; abajo++;}
				obstaculoaux = false;
			}
			if (turno){
				if (arriba > abajo) {
					if (pos[yc][xc-1] != 1) xc--;
						else {arriba = 0;
							if (arriba == 0 and abajo == 0) obstaculo = 0;}}
				else {
					if (pos[yc][xc+1] != 1) xc++;
					else {
						abajo = 0;
						if (arriba == 0 and abajo == 0) obstaculo = 0;
					}
				}
			}
			turno = false;
		break;

		case obstaculos::abajo:
			if (pos[yc-1][xc] == 0) {obstaculo = 0; turno = false;}
			if (obstaculoaux) {
				i = xc;
				j = xc;
				arriba = 0;
				abajo = 0;
				while(pos[yc-1][i]==1){i++; arriba++;}
				while(pos[yc-1][j]==1){j--; abajo++;}
				obstaculoaux = false;
			}
			if (turno){
				if (arriba > abajo) {
					if (pos[yc][xc-1] != 1) xc--;
						else {arriba = 0;
							if (arriba == 0 and abajo == 0) obstaculo = 0;}}
				else {
					if (pos[yc][xc+1] != 1) xc++;
					else {
						abajo = 0;
						if (arriba == 0 and abajo == 0) obstaculo = 0;
					}
				}
			}
			turno = false;

		break;

		case obstaculos::izquierda:
			if (pos[yc][xc-1] == 0 and turno) {obstaculo = 0; xc--; turno = false;}
			if (obstaculoaux) {
				i = yc;
				j = yc;
				arriba = 0;
				abajo = 0;
				while(pos[i][xc-1]==1){ i++; arriba++;}
				while(pos[j][xc-1]==1){ j--; abajo++;}
				obstaculoaux = false;
			}
			if (turno){
				if (arriba > abajo) {
					if (pos[yc-1][xc] != 1 and yc != 0) yc--;
						else {arriba = 0;
							if (arriba == 0 and abajo == 0) obstaculo = 0;}}
				else {
					if (pos[yc+1][xc] != 1 and yc != alto) yc++;
					else {
						abajo = 0;
						if (arriba == 0 and abajo == 0) obstaculo = 0;
					}
				}
			}
			turno = false;
		break;

		case obstaculos::derecha:
			if (pos[yc][xc+1] == 0 and turno) {obstaculo = 0; xc++; turno = false;}
			if (obstaculoaux) {
				i = yc;
				j = yc;
				arriba = 0;
				abajo = 0;
				while(pos[i][xc+1] == 1){i++; arriba++;}
				while(pos[j][xc+1] == 1){j--; abajo++;}
				obstaculoaux = false;
			}
			if (turno){
				if (arriba > abajo) {
					if (pos[yc-1][xc] != 1 and yc != 0) yc--;
						else {
							arriba = 0;
							if (arriba == 0 and abajo == 0) obstaculo = 0;
						}
				}
				else {
					if (pos[yc+1][xc] != 1 and yc != alto) yc++;
					else {
						abajo = 0;
						if (arriba == 0 and abajo == 0) obstaculo = 0;
					}
				}
			}
			turno = false;
		break;
	}

	if (yc != yj and turno){

		if (yj > yc){
			if (pos[yc+1][xc] != 1) {
				yc++;
				turno = false;
			}
			else {
				if (xc == xj) {
				obstaculo = obstaculos::arriba;
				//obstaculoHorizontalArr = true;
				obstaculoaux = true;
				turno = false;}
			}
		}

		if (yj < yc){
			if (pos[yc-1][xc] != 1){
				yc--;
				turno = false;
			}
			else{
				if (xc == xj) {
				obstaculo = obstaculos::abajo;
				obstaculoaux = true;
				turno = false;}
			}
		}
	}

	if (turno){
		if (xj > xc) {
			if (pos[yc][xc+1] !=1) xc++;
			else {
				obstaculo = obstaculos::derecha;
				obstaculoaux = true;
				turno = false;
			}
		}

		if (xj < xc){
			if (pos[yc][xc-1] !=1) xc--;
			else {
				obstaculo = obstaculos::izquierda;
				obstaculoaux = true;
				turno = false;
			}
		}
	}

	if (yc == yj and xc == xj) {
		vidas--;
		if (vidas == 0) choque = true;
		else auxnivel = true;
	}
	if (pos[yj][xj] == 2){
	pos[yj][xj] = 0;
	frutillas--;
	}

	if (frutillas == 0){
		if (!mapaCustom)
		{
		nivel++;
		auxnivel = true;
		}
		else {
			win = true;
		}
	}


	//MOVER JUGADOR
	switch(direc){
		case -1:
						if (xj==0) { if (pos[yj][ancho-1] != 1) xj = ancho-1;}
						else {
						if (pos[yj][xj-1] != 1) xj--;}
		break;

		case 1:
					   if (xj == (ancho-1)) { if (pos[yj][0] !=1) xj = 0;	}
					   else {
					   if (pos[yj][xj+1] != 1) xj++;}
		break;

		case 2:
					  if (yj == 0) { if (pos[alto-1][xj] != 1) yj = alto-1; }
					  else {
					  if (pos[yj-1][xj] != 1) yj--;}
		break;

		case -2:
					   if (yj == (alto-1)) { if (pos[0][xj] != 1) yj = 0; }
					   else {
					   if (pos[yj+1][xj] != 1) yj++;}
		break;
	}

}

void juego::imprimirJuego(){
	int i, j;
	move(1, 0);
	for(i=0; i < alto; i++) {
		for(j=0; j < ancho; j++) {

			switch(pos[i][j]) {
				case 0: printw(" "); break;
				case 1: printw("/"); break;
				case 2: attron(COLOR_PAIR(3)); printw("X"); attroff(COLOR_PAIR(3)); break;
				case 3: if (CDM) {attron(COLOR_PAIR(5)); printw("/"); attroff(COLOR_PAIR(5));}
				else printw("/"); break;
				default: printw("h"); break;
			}

		}
		printw("\n");
	}
	move(yc+1, xc);
	attron(COLOR_PAIR(1)); addch(ACS_DIAMOND); attroff(COLOR_PAIR(1));

	move(yj+1, xj);
	attron(COLOR_PAIR(2)); addch(ACS_DIAMOND); attroff(COLOR_PAIR(2));

	if (!CDM){
		mvprintw(alto + 3, 5, "          VIDAS:");
		for(i=0; i < vidas * 15; i+=15){
			mvprintw(alto + 5, 5 + i, ",d88b.d88b,");
			mvprintw(alto + 6, 5 + i, "88888888888");
			mvprintw(alto + 7, 5 + i, "`Y8888888Y'");
		  mvprintw(alto + 8, 5 + i, "  `Y888Y'  ");
		  mvprintw(alto + 9, 5 + i, "    `Y'    ");
		}
	}

	if (nivel == 6){ move(2, ancho - 25);
		printw("I was just joking");
	}
	for (i=0; i < (ancho); i++){
	move(0, i);
	printw("-");
	move((alto+1), i);
	printw("-");
	}

	refresh();
}

void juego::niveles(){
	switch(nivel){
		case 0:
		clear();
		mvprintw(7, (ancho/2 -20), "     _____   _    _    _____   _______    ____    __  __   ");
		mvprintw(8, (ancho/2 -20), "    / ____| | |  | |  / ____| |__   __|  / __ \\  |  \\/  |  ");
		mvprintw(9, (ancho/2 -20), "   | |      | |  | | | (___      | |    | |  | | | \\  / |  ");
		mvprintw(10, (ancho/2 -20), "   | |      | |  | |  \\___ \\     | |    | |  | | | |\\/| |  ");
		mvprintw(11, (ancho/2 -20), "   | |____  | |__| |  ____) |    | |    | |__| | | |  | |  ");
		mvprintw(12, (ancho/2 -20), "    \\_____|  \\____/  |_____/_    |_|     \\____/  |_|  |_|  ");
		mvprintw(15, (ancho/2 -20), "   |  \\/  |     /\\     |  __ \\      ");
		mvprintw(16, (ancho/2 -20), "   | \\  / |    /  \\    | |__) |   ");
		mvprintw(17, (ancho/2 -20), "   | |\\/| |   / /\\ \\   |  ___/    ");
		mvprintw(18, (ancho/2 -20), "   | |  | |  / ____ \\  | |         ");
		mvprintw(19, (ancho/2 -20), "   |_|  |_| /_/    \\_\\ |_|         ");
		refresh();
		dormir(2000);
		break;

		case 1:
		clear();
		mvprintw(10, (ancho/2 -8), "    __");
		mvprintw(11, (ancho/2 -8), "  /_  | er ");
		mvprintw(12, (ancho/2 -8), "    | |");
		mvprintw(13, (ancho/2 -8), "    | |");
		mvprintw(14, (ancho/2 -8), "    | |");
		mvprintw(15, (ancho/2 -8), "    |_|");
		anio();
		refresh();
		xj = 5;  yj = 5;  xc = ancho/2;  yc = alto/2; direc = 0;
		dormir(2000);
		break;

		case 2:
		clear();
		mvprintw(10, (ancho/2 -8), "    ___");
		mvprintw(11, (ancho/2 -8), "   |__ \\   do");
		mvprintw(12, (ancho/2 -8), "      ) |");
		mvprintw(13, (ancho/2 -8), "     / / ");
		mvprintw(14, (ancho/2 -8), "    / /_ ");
		mvprintw(15, (ancho/2 -8), "   |____|");
		anio();
		refresh();
		dormir(2000);
		xj = 5;  yj = 5;  xc = ancho/2;  yc = alto/2; direc = 0;
		mapa();
		break;

		case 3:
		clear();
		mvprintw(10, (ancho/2 -8), "    ____  ");
		mvprintw(11, (ancho/2 -8), "   |___ \\  r ");
		mvprintw(12, (ancho/2 -8), "     __) |");
		mvprintw(13, (ancho/2 -8), "    |__ < ");
		mvprintw(14, (ancho/2 -8), "    ___) |");
		mvprintw(15, (ancho/2 -8), "   |____/ ");
		anio();
		refresh();
		dormir(2000);
		xj = 5;  yj = 5;  xc = ancho/2;  yc = alto/2; direc = 0;
		mapa();
		break;

		case 4:
		clear();
		mvprintw(10, (ancho/2 -8), "    _  _ ");
		mvprintw(11, (ancho/2 -8), "   | || |  to");
		mvprintw(12, (ancho/2 -8), "   | || |_ ");
		mvprintw(13, (ancho/2 -8), "   |__   _|");
		mvprintw(14, (ancho/2 -8), "      | |  ");
		mvprintw(15, (ancho/2 -8), "      |_|  ");
		anio();
		refresh();
		dormir(2000);
		xj = 5;  yj = 5;  xc = ancho/2;  yc = alto/2; direc = 0;
		mapa();

		break;

		case 5:
		clear();
		mvprintw(10, (ancho/2 -8), "    _____ ");
		mvprintw(11, (ancho/2 -8), "   | ____|  to");
		mvprintw(12, (ancho/2 -8), "   | |__  ");
		mvprintw(13, (ancho/2 -8), "   |___ \\ ");
		mvprintw(14, (ancho/2 -8), "    ___) |");
		mvprintw(15, (ancho/2 -8), "   |____/ ");
		anio();
		refresh();
		dormir(2000);
		xj = ancho -2;  yj = alto -2;  xc = 2;  yc = alto -2; direc = 0;
		mapa();
		break;

		case 6:
		clear();
		mvprintw(10, (ancho/2 -8), "      __ ");
		mvprintw(11, (ancho/2 -8), "     / /   to");
		mvprintw(12, (ancho/2 -8), "    / /_  ");
		mvprintw(13, (ancho/2 -8), "   | '_ \\  ");
		mvprintw(14, (ancho/2 -8), "   | (_) |");
		mvprintw(15, (ancho/2 -8), "    \\___/ ");
		anio();
		xj = 5;  yj = 5;  xc = ancho/2;  yc = alto/2; direc = 0;
		mapa();
		break;

		case 7: win = true;
	}
	auxnivel = false;
}

void juego::mapa(){
	int j, i;
	if (nivel > 1 and nivel < 5){
		for (i=0; i <= 10; i++){
			pos[(alto/2) +2 +i][1] = 2;
			pos[(alto/2) -2 -i][1] = 2;
			pos[(alto/2) +2 +i][ancho-2] = 2;
			pos[(alto/2) -2 -i][ancho-2] = 2;
		}
		for (i=0; i <= 40; i+=2){
			pos[(alto/2) ][ancho/2 - 20 + i] = 2;
			pos[(alto/2 +4) ][ancho/2 - 20 + i] = 2;
			pos[(alto/2 -4) ][ancho/2 - 20 + i] = 2;
		}
	}

	if (nivel == 3 or nivel == 4){
		for (i=0; i <= 10; i++){
			pos[alto/2 -5 + i][ancho/4 - 2] = 1;
			pos[alto/2 -5 + i][ancho/4*3 + 2] = 1;
			if(i%2 == 0){
				pos[alto/2 -5 + i][ancho/4] = 2;
				pos[alto/2 -5 + i][ancho/4*3] = 2;
			}
		}


		for (i=0; i <= 40; i+=2){
			pos[(alto/2) ][ancho/2 - 20 + i] = 2;
			pos[(alto/2 +4) ][ancho/2 - 20 + i] = 2;
			pos[(alto/2 -4) ][ancho/2 - 20 + i] = 2;
		}
	}

	if (nivel == 4){
		for (i=0; i < alto; i++){
			pos[i][0] = 1;
			pos[i][ancho-1] = 1;
		}
		for (i=0; i < ancho; i++){
			pos[0][i] = 1;
			pos[alto-1][i] = 1;
		}
		pos[alto/2][0] = 0;
		pos[alto/2][ancho-1] = 0;

	}

	if (nivel == 5){
		for (i = 0; i < alto; i++){
			for (j = 0; j < ancho; j++){
			pos[i][j] = 1;
		}}
		for (i = 0; i < ancho - 1; i++) {
		pos[alto-1][i] = 0;
		pos[alto-2][i] = 0;
		pos[alto-3][i] = 0;
		pos[alto-4][i] = 0;
		pos[alto-5][i] = 0;
		pos[alto-6][i] = 0;
		pos[alto-7][i] = 0;
		pos[alto-8][i] = 0;

		pos[1][i] = 0;
		pos[2][i] = 0;
		pos[3][i] = 0;
		}
		for (i = 0; i < alto - 1; i++) {
		pos[i][2] = 0;
		pos[i][3] = 0;
		pos[i][4] = 0;
		}
		for (i = 1; i < alto - 8; i++) {
		pos[i][ancho-2] = 3;
		pos[i][ancho-3] = 3;
		}

		int z = 7;
		for (i = 0; i < ancho/2; i++) {
		pos[alto-1][i+z] = 1;
		pos[alto-2][i+z*2] = 1;
		pos[alto-3][i+z*3] = 1;
		pos[alto-4][i+z*4] = 1;
		pos[alto-4][i+z*5] = 1;
		pos[alto-4][i+z*6] = 1;
		}
		pos[2][2] = 2;
	}

	if (nivel == 6){
		for( i = 0; i < alto; i++){
			for( j = 0; j < ancho; j++){
				pos[i][j] = 0;
		}
		}
		for( i = 0; i < alto; i++){
			pos[i][0] = 1;
			pos[i][ancho-1] = 1;
		}
		for( j = 0; j < ancho; j++){
			pos[0][j] = 1;
			pos[alto-1][j] = 1;
		}
		for(i=0; i < 6; i++){
			pos[alto/2 - 3 + i][ancho/2 - 3] = 1;
			pos[alto/2 - 3 + i][ancho/2 + 3] = 1;

			pos[alto/2 - 3][ancho/2 - 3 + i] = 1;
			pos[alto/2 + 3][ancho/2 - 3 + i] = 1;
		}
		pos[25][70] = 2;
	}
    frutillas = 0;
	for( i = 0; i < alto; i++){
		for( j = 0; j < ancho; j++){
		if (pos[i][j] == 2) frutillas ++;
		}
	}
}

void juego::anio(){
	mvprintw(17, (ancho/2 -14), "       __  _               _ ");
	mvprintw(18, (ancho/2 -14), "    /\\ \\ \\(_)__   __  ___ | |");
	mvprintw(19, (ancho/2 -14), "   /  \\/ /| |\\ \\ / / / _ \\| |");
	mvprintw(20, (ancho/2 -14), "  / /\\  / | | \\ V / |  __/| |");
	mvprintw(21, (ancho/2 -14), "  \\_\\ \\/  |_|  \\_/   \\___||_|");
	refresh();
	if (nivel == 6){
		mvprintw(24, (ancho/2 -14), "Preparando el nivel mas dificil de todos...");
		mvprintw(26, (ancho/2 -14), "<");
		mvprintw(26, (ancho/2 +13), ">");
		for (int i = 0; i < 26; i+=2){
		dormir(1000);
		mvprintw(26, (ancho/2 -13 +i), "-");
		mvprintw(26, (ancho/2 -12 +i), "-");
		refresh();
		}
	}
}

void juego::pausa(){
	char p;
	mvprintw(alto/2, ancho/2, "PAUSA");
	refresh();

	while(p!='p') {p = getchar();
		dormir(500);
	}

}

void juego::creadorDeMapas(){
	CDM = true;
	int i, j;
	clear();
	noecho();
	nodelay(stdscr, FALSE);
	bool quit = false;
	while (!quit){
		clear();
		imprimirJuego();
		printw("\n\n(Z) Fantasma  (X) Pared  (C) Frutillas  (V) Pared Falsa  (A) Borrar  (Q) Salir");
		printw("\n(D) Horizontal de pared  (F) Vertical de pared  (L) Borrar Todo");
		printw("\n\n Nota: Si no detecta teclado verificar bloq mayus!");
		refresh();
		int tecla = getch();
		switch(tecla) {
			case KEY_LEFT: if (xj > 0) xj--; break;
			case KEY_RIGHT: if (xj < ancho -1) xj++; break;
			case KEY_DOWN: if (yj < alto -1) yj++; break;
			case KEY_UP: if (yj > 0) yj--; break;
			case 122: xc = xj; yc = yj; //z
			break;
			case 120: pos[yj][xj] = 1; //x
			break;
			case 99: pos[yj][xj] = 2; //c
			break;
			case 118: pos[yj][xj] = 3; //v
			break;
			case 97: pos[yj][xj] = 0; //a
			break;
			case 100:
			for (i = 0; i < ancho; i++){ pos[yj][i] = 1;}
			break; //d
			case 102:  //f
			for (i = 0; i < ancho; i++){ pos[i][xj] = 1;}
			break;
			case 108: printw("\n\nSeguro quiere borrar todo?\n (1) SI   (2) NO");
			tecla = getch();
			if (tecla == 49){
				for(i=0; i < alto; i++){
					for (j=0; j < ancho; j++){
						pos[i][j] = 0;
					}
				}
			}
			break; //l
			case 113: quit = true; //Q
			break;
		}
	}

	clear();
	char nombreMapa[15];
	printw("Desea guardar el mapa?\n (1) Si    (2) No\n\n");
	int tecla = getch();
	if (tecla == 49) {
		printw("Escriba el nombre del mapa (Sin espacios): ");
		getstr(nombreMapa);
		ofstream out(nombreMapa);
		for(i = 0; i < alto; i++){
			for (j = 0; j < ancho; j++){
				out << pos[i][j] << endl;
				}
		}
			out << yj << endl << xj << endl << yc << endl << xc;
	}
	CDM = false;

}
