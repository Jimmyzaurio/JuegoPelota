/* Para compilar use:
gcc gfxModified.c -c
gcc final_ukranio1.c -c
gcc final_ukranio1.o gfxModified.o -o final_ukranio1 -lX11 -lm

libreria original gfx:
https://www3.nd.edu/~dthain/courses/cse20211/fall2013/gfx/
Tomado de: https://github.com/PatMyron/slime-volleyball
Originally created by Patrick Myron and Anna McMahon
Further Modified by Ukranio Coronilla
*/

#include "gfxModified.h"
#include <iostream>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <vector>
using namespace std;

//Resolución de la pantalla
int const ANCHURA = 800;
int const ALTURA  = 600;

class Pelota {    
public:
	float x;
	float y; //posicion

	float vx; //velocidad
	float vy;    

	int r;  //radio
	vector<int> color; //Color de la pelota

	Pelota(float X, float Y, float VX, float VY, int R, vector<int>& COLOR)
		: x(X), y(Y), vx(VX), vy(VY), r(R), color(COLOR) {}

	void muevePelota(Pelota);
	void checaColisiones(Pelota);
	void mueveAgente(bool tecla[6]);
	void validaMovimientoAgente();
	void calculaDireccionPelota(Pelota);
};

// Actualiza la velocidad y la posición de la pelota
void Pelota::muevePelota(Pelota pAgente){
	//Checa para ver si la pelota colisiona con algo para cambiar su velocidad    
	checaColisiones(pAgente);
	(vy)+=.08; //Para simular la aceleración de la gravedad
	(x) = (x) + (vx); //Cambios para x recordando v=d/t 
	(y) = (y) + (vy);
	gfx_clear();
}

//checa los bordes de la pelota y actualiza su localización, el vector velocidad   cambia si la pelota choca con algo.
void Pelota::checaColisiones(Pelota pAgente){
	float distContacto, agenteBolaDist;
	
	if ((x < r) || (x > ANCHURA - r))  // colision lateral cambia al vector velocidad en x
		vx = -vx;
	if (y < r)   //colision en techo cambia el vector velocidad en y
		vy = -vy;
	if (y >= ALTURA - r)   //colision en piso
		vy = -vy;
	
	//distancia entre el centro del agente y la pelota
	agenteBolaDist = sqrtf(powf(pAgente.y - y, 2) + powf(pAgente.x - x, 2));
	//distancia minima permisible entre los centros del agente y la pelota
	distContacto = pAgente.r + r;
	//checa para ver si existe colision entre la pelota y el agente
	if(agenteBolaDist < distContacto)
		calculaDireccionPelota(pAgente);
}


//Lee el arreglo tecla para realizar un movimiento a la izquierda o a la derecha dependiendo de la tecla que se presionó.
void Pelota::mueveAgente(bool tecla[6]) {
	if (tecla[3] == 1 )  // Si se presionó la tecla de mover a la izquierda
		x -= 10;	
	if (tecla[5] == 1 )  // Si se presionó la tecla de mover a la derecha
		x += 10;
	gfx_flush();
}

//Después de que se ha presionado una tecla, la función se asegura que el agente no se ha salido de la pantalla
void Pelota::validaMovimientoAgente() {	
	//Limita al agente por la orilla derecha
	if (x > ANCHURA - r)
		x = ANCHURA - r;
	
	//Limita al agente por la orilla izquierda
	if (x < r)
		x = r;
}//end func


//Cambia la dirección de la pelota de acuerdo a las leyes de la física
void Pelota::calculaDireccionPelota(Pelota Jugador){ 
	//la velocidad absoluta de la pelota se inicializa a 10 después de que choca con la raqueta debido
	//a que de no hacerlo se movería mas lento cada vez
	float velocidadAbsoluta = 10;
	float pendienteEntreCentros;
	float difCentrosX,difCentrosY;
	float anguloEntreCentros;

	difCentrosX = x - Jugador.x;
	difCentrosY = y - Jugador.y;
	//Aunque puede existir un denominador igual a cero, y pendiente infinita la función arco tangente si resuelve con argumento infinito
	pendienteEntreCentros = difCentrosY / difCentrosX ;
	anguloEntreCentros    = atanf(pendienteEntreCentros);
	
	if(difCentrosX < 0 && difCentrosY < 0)
		anguloEntreCentros += acosf(-1.0); // Ajusta la restricción del angulo sumandole PI = 3.14159...
			
	vx = velocidadAbsoluta * cosf(anguloEntreCentros);
	vy = velocidadAbsoluta * sinf(anguloEntreCentros);
}

bool tecla[6];

typedef Pelota Agente;

int main(int args, char *argv[]) {
	srand(time(NULL));
	int N = atoi(argv[1]);
	//cout << N << endl << endl;
	vector<int> color(3, 255);

	vector<Pelota> balones;
	for (int i = 0; i < N; ++i) {
		int xpos  = rand() % ANCHURA/2;
		int ypos  = rand() % ALTURA/2;
		int radio = 5 + rand() % 15;
		//cout << xpos << "   " << ypos << "  " << radio << endl;
		Pelota p(xpos, ypos, 0, 0, radio, color);
		balones.push_back(p);
	}

	color[0] = 87;
	color[1] = 1;
	color[2] = 2;
	Agente jugador(ANCHURA/4, ALTURA, 0, 0, 25, color);

	fill(tecla, tecla + 6, false);
	gfx_open(ANCHURA, ALTURA, "C++ Juego de pelota");

	while (1) {
		if (gfx_event_waiting2()) {
			gfx_keyPress(tecla); //Obtiene la tecla que se ha presionado como siguiente evento

			jugador.mueveAgente(tecla);
			jugador.validaMovimientoAgente();
		}

		for (int i = 0; i < N; ++i) {
			//Pelota &pelota = balones[i];		
			balones[i].muevePelota(jugador);
		}
		
		for (int i = 0; i < N; ++i) {
			//Dibuja la pelota
			gfx_color(balones[i].color[0], balones[i].color[1], balones[i].color[2]);
			gfx_fill_arc(balones[i].x - balones[i].r, balones[i].y - balones[i].r, 2*balones[i].r, 2*balones[i].r, 0, 360*64);

			//Dibuja al jugador 
			gfx_color(jugador.color[0],jugador.color[1],jugador.color[2]);
			gfx_fill_arc(jugador.x - jugador.r, jugador.y - jugador.r, 2*jugador.r, 2*jugador.r, 0, 180*64);
		}
		gfx_flush();
		usleep(18000);
	}
	return 0;
}
