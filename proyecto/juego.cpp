#include "Juego.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <cctype>

using std::cout;
using std::cin;
using std::string;

Juego::Juego() {
    turnoActual = 0;
}

void Juego::menuPrincipal() {
    int opcion;
    do {
        cout << "\n===== DAMAS INTERNACIONALES =====\n";
        cout << "1. Iniciar partida\n";
        cout << "2. Ver estadísticas\n";
        cout << "3. Cómo jugar\n";
        cout << "4. Salir\n";
        cout << "Elige una opción: ";

        if (!(cin >> opcion)) {
            cout << "Entrada inválida. Intenta nuevamente.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                iniciarPartida();
                break;
            case 2:
                mostrarEstadisticas();
                break;
            case 3:
                mostrarInstrucciones();
                break;
            case 4:
                cout << "Saliendo del juego...\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 4);
}

void Juego::mostrarInstrucciones() {
    char resp = 'S';
    while (true) {
        cout << "\n===== CÓMO JUGAR =====\n";
        cout << "El objetivo es capturar todas las piezas del oponente o dejarlo sin movimientos.\n";
        cout << "\nReglas básicas:\n";
        cout << "- El tablero es de 10x10 casillas; sólo se usan las casillas oscuras.\n";
        cout << "- Las piezas normales (peones) se mueven 1 casilla diagonal hacia adelante.\n";
        cout << "- Si puedes capturar una pieza rival, la captura es obligatoria.\n";
        cout << "- Si tras una captura puedes seguir saltando, debes continuar la cadena de capturas.\n";
        cout << "- Cuando un peón llega a la última fila rival se convierte en Dama (*).\n";
        cout << "- Para mover, ingresa fila y columna de origen y de destino (ej. 6 1 5 2).\n";
        cout << "\n¿Volver al menú principal? (S/N): ";

        cin >> resp;
        // limpiar restos de la línea
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        resp = static_cast<char>(std::toupper(static_cast<unsigned char>(resp)));
        if (resp == 'S') return;      // volver al menú
        if (resp == 'N') continue;    // mostrar instrucciones otra vez
        // entrada inválida -> repetir pregunta
        cout << "Opción inválida. Responde S o N.\n";
    }
}

void Juego::iniciarPartida() {
    cout << "\nIniciando partida de DAMAS INTERNACIONALES (2 jugadores)...\n";

    jugadores.clear();

    string nombre1, nombre2;
    char simboloJugador1 = 'A';
    char simboloJugador2 = 'B';

    cout << "Nombre del jugador 1: ";
    cin >> nombre1;

    cout << "Nombre del jugador 2: ";
    cin >> nombre2;

    // Simbolo
    cout << "\nJugador " << nombre1 << ", elige símbolo (A/B): ";
    cin >> simboloJugador1;
    simboloJugador1 = static_cast<char>(std::toupper(static_cast<unsigned char>(simboloJugador1)));
    while (simboloJugador1 != 'A' && simboloJugador1 != 'B') {
        cout << "Símbolo inválido. Debe ser A o B: ";
        cin >> simboloJugador1;
        simboloJugador1 = static_cast<char>(std::toupper(static_cast<unsigned char>(simboloJugador1)));
    }
    simboloJugador2 = (simboloJugador1 == 'A') ? 'B' : 'A';

    // Colores (simple, no validaciones extra)
    string listaColores[14] = {
        "rojo","verde","amarillo","azul","magenta","cyan",
        "blanco","gris","rojo_brillante","verde_brillante",
        "amarillo_brillante","azul_brillante","magenta_brillante","cyan_brillante"
    };

    int op1 = 1, op2 = 2;
    cout << "\n" << nombre1 << ", elige color (1-14): ";
    while (!(cin >> op1) || op1 < 1 || op1 > 14) {
        cout << "Opción inválida. Elige 1..14: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    cout << nombre2 << ", elige color (1-14): ";
    while (!(cin >> op2) || op2 < 1 || op2 > 14) {
        cout << "Opción inválida. Elige 1..14: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    string color1 = listaColores[op1 - 1];
    string color2 = listaColores[op2 - 1];

    jugadores.push_back(Jugador(nombre1, simboloJugador1, color1));
    jugadores.push_back(Jugador(nombre2, simboloJugador2, color2));

    string colorA_forTab, colorB_forTab;
    if (simboloJugador1 == 'A') {
        colorA_forTab = color1; colorB_forTab = color2;
    } else {
        colorA_forTab = color2; colorB_forTab = color1;
    }

    tablero.setPlayerColors(colorA_forTab, colorB_forTab);
    tablero.inicializar(2);

    turnoActual = 0;
    jugarPartida();
}

void Juego::jugarPartida() {
    bool partidaActiva = true;
    while (partidaActiva) {

        tablero.mostrar();

        Jugador& jugadorActual = jugadores[turnoActual];
        char simbolo = jugadorActual.getSimbolo();

        cout << "\nTurno de " << jugadorActual.getNombre()
             << " (" << simbolo << ")\n";

        cout << "Ingrese coordenadas de origen (fila y columna) o 'P' para pausar: ";
        string entrada;
        if (!(cin >> entrada)) {
            cout << "Entrada inválida.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (entrada == "P" || entrada == "p") {
            if (menuPausa()) return; // salir a menu principal
            else continue; // volver al juego
        }

        int f1 = std::stoi(entrada);
        int c1, f2, c2;
        cout << "Ingrese coordenadas de destino (fila y columna): ";
        if (!(cin >> c1 >> f2 >> c2)) {
            cout << "Entrada inválida.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (f1 < 1 || f1 > 10 || c1 < 1 || c1 > 10 || f2 < 1 || f2 > 10 || c2 < 1 || c2 > 10) {
            cout << "Coordenadas fuera de rango (1..10). Intenta otra vez.\n";
            continue;
        }

        f1--; c1--; f2--; c2--;

        int resultado = tablero.moverFicha(f1, c1, f2, c2, simbolo);

        if (resultado == 0) {
            cout << "Movimiento inválido. Intenta nuevamente.\n";
            continue;
        }

        // continuar cadena si aplica
        if (resultado == 2 || resultado == 3) {
            while (tablero.puedeCapturarDesde(f2, c2)) {
                tablero.mostrar();
                cout << "\nDebes realizar otra captura desde (" << f2+1 << "," << c2+1 << ")\n";
                int nf, nc;
                cout << "Destino para continuar captura: ";
                if (!(cin >> nf >> nc)) {
                    cout << "Entrada inválida.\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                if (nf < 1 || nf > 10 || nc < 1 || nc > 10) {
                    cout << "Fuera de rango.\n";
                    continue;
                }
                nf--; nc--;
                int r = tablero.moverFicha(f2, c2, nf, nc, simbolo);
                if (r == 0) {
                    cout << "Movimiento inválido. La captura debe continuar.\n";
                    continue;
                }
                f2 = nf; c2 = nc;
            }
        }

        // victoria?
        char simboloOponente = (simbolo == 'A' ? 'B' : 'A');
        if (tablero.verificarVictoria(simboloOponente)) {
            tablero.mostrar();
            cout << "\nFELICITACIONES 🎉\n";
            cout << jugadorActual.getNombre() << " ha ganado la partida.\n";
            jugadorActual.agregarVictoria();
            jugadores[1 - turnoActual].agregarDerrota();
            partidaActiva = false;
            break;
        }

        turnoActual = (turnoActual + 1) % 2;
    }
}

bool Juego::menuPausa() {
    int op;
    cout << "\n=== PAUSA ===\n";
    cout << "1. Volver al juego\n";
    cout << "2. Salir al menu principal\n";
    cout << "Elige una opción: ";
    if (!(cin >> op)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    switch (op) {
        case 1:
            return false; // volver al juego
        case 2:
            cout << "Regresando al menú principal...\n";
            return true;
        default:
            cout << "Opción inválida.\n";
    }
    return false;
}

void Juego::mostrarEstadisticas() {
    if (jugadores.empty()) {
        cout << "\nNo hay estadísticas cargadas.\n";
        return;
    }

    cout << "\n===== ESTADÍSTICAS =====\n";
    for (const auto &j : jugadores) {
        cout << j.getNombre() << " (" << j.getSimbolo() << "):\n";
        cout << "Victorias: " << j.getVictorias() << "\n";
        cout << "Derrotas: " << j.getDerrotas() << "\n";
        cout << "Empates: " << j.getEmpates() << "\n\n";
    }
}

