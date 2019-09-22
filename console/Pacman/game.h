#define alto 30
#define ancho 100

class juego{
	public:
	juego();
	int xj;
	int yj;
	int xc;
	int yc;
	int pos[alto][ancho];
	int direc;
	bool choque;
	int menu();
	int frutillas;
	int nivel;
	bool auxnivel;

	private:
	//Principales
	void imprimirJuego();
	int iniciar();
	void mapa();
	int velocidad;

	//Mecanismos
	void moverPac();
	void pausa();
	void niveles();
	void anio();
	int vidas;
	bool win;

	//Mecanismos IA
	int obstaculo;
	bool obstaculoaux;
	int arriba;
	int abajo;

	//Extras
	void creadorDeMapas();
	bool CDM;
	bool mapaCustom;
	void records();


};
