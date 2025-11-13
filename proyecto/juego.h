// Juego.h (versión mínima compatible)
#ifndef JUEGO_H
#define JUEGO_H

#include "Tablero.h"
#include "Jugador.h"
#include <vector>

class Juego {
public:
    Juego();
    void menuPrincipal();
    void iniciarPartida();
    void mostrarEstadisticas();
    void mostrarInstrucciones();

    // Pausa (versión mínima: no guarda tablero completo)
    bool menuPausa();


private:
    Tablero tablero;
    std::vector<Jugador> jugadores;
    int turnoActual;

    // bucle principal extraído
    void jugarPartida();
};

#endif
