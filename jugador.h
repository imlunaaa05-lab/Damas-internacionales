#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>

class Jugador {
private:
    std::string nombre;
    char simbolo;
    std::string color;
    int victorias;
    int derrotas;
    int empates;

public:
    // Constructor con nombre, símbolo y color
    Jugador(const std::string& n = "", char s = '.', const std::string& c = "");

    std::string getNombre() const;
    char getSimbolo() const;
    std::string getColor() const;

    void agregarVictoria();
    void agregarDerrota();
    void agregarEmpate();

    int getVictorias() const;
    int getDerrotas() const;
    int getEmpates() const;

    void mostrarEstadisticas() const;
};

#endif // JUGADOR_H
