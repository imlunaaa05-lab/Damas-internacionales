#ifndef TABLERO_H
#define TABLERO_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Celda {
    char simbolo;   
    bool dama;      
};

class Tablero {
private:
    Celda celdas[10][10];

    std::string colorA;
    std::string colorB;

public:
    Tablero();

    void inicializar(int numJugadores);
    void mostrar();

    bool dentro(int f, int c);

    bool hayMovimientoDisponible(char simbolo);
    bool jugadorTieneMovimientos(char simbolo);
    bool jugadorTieneCapturas(char simbolo);

    bool moverSimple(int f1, int c1, int f2, int c2, char simbolo);
    bool moverDama(int f1, int c1, int f2, int c2, char simbolo);

    int moverFicha(int f1, int c1, int f2, int c2, char simbolo);


    bool puedeCapturarDesde(int f, int c);
    bool puedeCapturarDama(int f, int c);

 
    bool capturarPeon(int f1, int c1, int f2, int c2, char simbolo);
    bool capturarDama(int f1, int c1, int f2, int c2, char simbolo);

    void coronar(int f, int c, char simbolo);

    bool verificarVictoria(char simboloOponente);

    void setPlayerColors(const std::string &colA, const std::string &colB);
};

#endif

