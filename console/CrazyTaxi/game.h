#define altoVisible 30
#define ancho 8
#define zonaSpawn 4
#define altoTotal altoVisible + zonaSpawn + 1
#define miAuto 1


class juego{
	private:
	int pos[altoVisible + zonaSpawn + 1][ancho];
	void aparecerAutos();
	void moverAutos();
	bool choque;
	int contador;
	int velocidad;
	int z;
	bool aux;
	bool aux2;
	void guardarPartida();
	bool reanudar;

	public:
	juego ();
	void menu();
	void iniciar();
    void moverDer ();
    void moverIzq ();
    void imprimirJuego();
    int puntaje;
    int dificultad;
    void pausa();
    void minimenu();



    };


