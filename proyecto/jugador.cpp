#include "Jugador.h"
#include <iostream>

Jugador::Jugador(const std::string& n, char s, const std::string& c) {
    nombre = n;
    simbolo = s;
    color = c;
    victorias = 0;
    derrotas = 0;
    empates = 0;
}

std::string Jugador::getNombre() const { return nombre; }
char Jugador::getSimbolo() const { return simbolo; }
std::string Jugador::getColor() const { return color; }

void Jugador::agregarVictoria() { victorias++; }
void Jugador::agregarDerrota() { derrotas++; }
void Jugador::agregarEmpate() { empates++; }

int Jugador::getVictorias() const { return victorias; }
int Jugador::getDerrotas() const { return derrotas; }
int Jugador::getEmpates() const { return empates; }

void Jugador::mostrarEstadisticas() const {
    std::cout << "\n--- Estadísticas de " << nombre << " ---\n";
    std::cout << "Victorias: " << victorias << "\n";
    std::cout << "Derrotas: " << derrotas << "\n";
    std::cout << "Empates: " << empates << "\n";
}
