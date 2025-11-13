#ifndef FICHA_H
#define FICHA_H

#include <string>

struct Ficha {
    char simbolo;   // símbolo del jugador o '.' si está vacío
    std::string color;   // texto con el color
    bool activa;    // true si pertenece a un jugador
};

#endif // FICHA_H
