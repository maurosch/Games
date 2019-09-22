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

void dormir(int milliseconds);

int main(int argc, char **argv)
{
	bool quit;
	char nombre[15];
	int a, i = 0;
	char z = '0';
	juego game[10];
	for (i=0; i < 10 or quit == false; i++){
		a = game[i].menu();
		clear();
		if (a==1){
			z = '0';
			attron(COLOR_PAIR(1));
			mvprintw(10, 0 ,"    /\\  \\       /\\  \\       /\\__\\       /\\  \\             /\\  \\      /\\__\\       /\\  \\       /\\  \\   ");
			mvprintw(11, 0 ,"   /::\\  \\     /::\\  \\     /::|  |     /::\\  \\           /::\\  \\    /:/  /      /::\\  \\     /::\\  \\   ");
			mvprintw(12, 0 ,"  /:/\\:\\  \\   /:/\\:\\  \\   /:|:|  |    /:/\\:\\  \\         /:/\\:\\  \\  /:/  /      /:/\\:\\  \\   /:/\\:\\  \\  ");
			mvprintw(13, 0 ," /:/  \\:\\  \\ /::\\~\\:\\  \\ /:/|:|__|__ /::\\~\\:\\  \\       /:/  \\:\\  \\/:/__/  ___ /::\\~\\:\\  \\ /::\\~\\:\\  \\ ");
			mvprintw(14, 0 ,"/:/__/_\\:\\__/:/\\:\\ \\:\\__/:/ |::::\\__/:/\\:\\ \\:\\__\\     /:/__/ \\:\\__|:|  | /\\__/:/\\:\\ \\:\\__/:/\\:\\ \\:\\__\\");
			mvprintw(15, 0 ,"\\:\\  /\\ \\/__\\/__\\:\\/:/  \\/__/~~/:/  \\:\\~\\:\\ \\/__/     \\:\\  \\ /:/  |:|  |/:/  \\:\\~\\:\\ \\/__\\/_|::\\/:/  /");
			mvprintw(16, 0 ," \\:\\ \\:\\__\\      \\::/  /      /:/  / \\:\\ \\:\\__\\        \\:\\  /:/  /|:|__/:/  / \\:\\ \\:\\__\\    |:|::/  / ");
			mvprintw(17, 0 ,"  \\:\\/:/  /      /:/  /      /:/  /   \\:\\ \\/__/         \\:\\/:/  /  \\::::/__/   \\:\\ \\/__/    |:|\\/__/  ");
			mvprintw(18, 0 ,"   \\::/  /      /:/  /      /:/  /     \\:\\__\\            \\::/  /    ~~~~        \\:\\__\\      |:|  |    ");
			mvprintw(19, 0 ,"    \\/__/       \\/__/       \\/__/       \\/__/             \\/__/                  \\/__/       \\|__|		");
			mvprintw(22, ancho/2 - 10 ,"(1) Jugar de Nuevo?");
			mvprintw(23, ancho/2 - 10 ,"(2) Salir");
			attroff(COLOR_PAIR(1));
			refresh();
			while (z != '1' && z != '2'){
				z = getch();
				dormir(500);
			}
			if (z == '2') return 0;
		}
		 else {
			 if (a==0) {
				ofstream out("records", ios_base::app);
				nodelay(stdscr,FALSE);
				mvprintw(alto/2 - 5, ancho/2 - 10, "Has ganado el juego!");
				mvprintw(alto/2 - 3, ancho/2 - 10, "Guardar Nombre?");
				mvprintw(alto/2, ancho/2 - 10, "(1) Si       (2) No\n");
				refresh();
				z = getch();
				if (z == '1'){
					clear();
					mvprintw(alto/2 - 2, ancho/2 - 10, "Ingrese su Nombre: ");
					getstr(nombre);
					out << nombre << endl;
				}
				else quit = true;
			}
			dormir(1000);
			}
		}

	endwin();
	return 0;
}
