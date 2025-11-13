#ifndef TABLERO_H
#define TABLERO_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Celda {
    char simbolo;   // 'A', 'B', 'C', 'D' o '.'
    bool dama;      // true si es dama
};

class Tablero {
private:
    Celda celdas[10][10];
    // Colores visuales (nombres) para cada símbolo
    std::string colorA;
    std::string colorB;

public:
    Tablero();

    void inicializar(int numJugadores);
    void mostrar();

    bool dentro(int f, int c);

    // Validaciones generales
    bool hayMovimientoDisponible(char simbolo);
    bool jugadorTieneMovimientos(char simbolo);
    bool jugadorTieneCapturas(char simbolo);

    // Movimiento y captura
    bool moverSimple(int f1, int c1, int f2, int c2, char simbolo);
    bool moverDama(int f1, int c1, int f2, int c2, char simbolo);

    int moverFicha(int f1, int c1, int f2, int c2, char simbolo);
    // retorno:
    // 0 -> movimiento inválido
    // 1 -> movimiento simple
    // 2 -> captura
    // 3 -> captura múltiple continua

    bool puedeCapturarDesde(int f, int c);
    bool puedeCapturarDama(int f, int c);

    // Capturas internas
    bool capturarPeon(int f1, int c1, int f2, int c2, char simbolo);
    bool capturarDama(int f1, int c1, int f2, int c2, char simbolo);

    void coronar(int f, int c, char simbolo);

    bool verificarVictoria(char simboloOponente);

    // Nuevas utilidades para colores
    void setPlayerColors(const std::string &colA, const std::string &colB);
};

#endif
