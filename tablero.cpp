// Tablero.cpp
#include "Tablero.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

Tablero::Tablero() {
    colorA = "blanco";
    colorB = "gris";
    for (int f = 0; f < 10; f++) {
        for (int c = 0; c < 10; c++) {
            celdas[f][c].simbolo = '.';
            celdas[f][c].dama = false;
        }
    }
}

void Tablero::setPlayerColors(const std::string &colA, const std::string &colB) {
    colorA = colA;
    colorB = colB;
}

bool Tablero::dentro(int f, int c) {
    return (f >= 0 && f < 10 && c >= 0 && c < 10);
}

void Tablero::inicializar(int) {
    // limpiar tablero
    for (int f = 0; f < 10; f++) {
        for (int c = 0; c < 10; c++) {
            celdas[f][c].simbolo = '.';
            celdas[f][c].dama = false;
        }
    }

    // Fichas de B (arriba): filas 0..3 en casillas "negras" (paridad 1)
    for (int f = 0; f < 4; f++) {
        for (int c = 0; c < 10; c++) {
            if ((f + c) % 2 == 1) {
                celdas[f][c].simbolo = 'B';
                celdas[f][c].dama = false;
            }
        }
    }

    // Fichas de A (abajo): filas 6..9 en casillas "negras"
    for (int f = 6; f < 10; f++) {
        for (int c = 0; c < 10; c++) {
            if ((f + c) % 2 == 1) {
                celdas[f][c].simbolo = 'A';
                celdas[f][c].dama = false;
            }
        }
    }
}

// ANSI helper (incluye algunas variantes "brillantes")
static string ansiCodeForName(const string &name) {
    if (name == "rojo") return "\033[31m";
    if (name == "verde") return "\033[32m";
    if (name == "amarillo") return "\033[33m";
    if (name == "azul") return "\033[34m";
    if (name == "magenta") return "\033[35m";
    if (name == "cyan") return "\033[36m";
    if (name == "blanco") return "\033[37m";
    if (name == "gris") return "\033[90m";
    if (name == "rojo_brillante") return "\033[91m";
    if (name == "verde_brillante") return "\033[92m";
    if (name == "amarillo_brillante") return "\033[93m";
    if (name == "azul_brillante") return "\033[94m";
    if (name == "magenta_brillante") return "\033[95m";
    if (name == "cyan_brillante") return "\033[96m";
    return "\033[0m";
}

void Tablero::mostrar() {
    cout << "    ";
    for (int c = 1; c <= 10; c++) cout << c << " ";
    cout << "\n";

    string codeA = ansiCodeForName(colorA);
    string codeB = ansiCodeForName(colorB);

    for (int f = 1; f <= 10; f++) {
        cout << (f < 10 ? " " : "") << f << " | ";
        for (int c = 1; c <= 10; c++) {
            int idxF = f - 1;
            int idxC = c - 1;
            char sym = celdas[idxF][idxC].simbolo;
            bool isDama = celdas[idxF][idxC].dama;

            // imprimimos siempre 2 caracteres por celda para mantener alineación
            if (sym == 'A') {
                cout << codeA << sym << (isDama ? "*" : " ") << "\033[0m";
            } else if (sym == 'B') {
                cout << codeB << sym << (isDama ? "*" : " ") << "\033[0m";
            } else {
                cout << ". ";
            }
        }
        cout << "|\n";
    }
}

/* ---------- DETECCIÓN DE CAPTURAS ---------- */

bool Tablero::puedeCapturarDesde(int f, int c) {
    if (!dentro(f,c)) return false;
    char s = celdas[f][c].simbolo;
    if (s == '.') return false;
    if (celdas[f][c].dama) return puedeCapturarDama(f,c);

    // Peón en damas internacionales puede capturar en las 4 diagonales (adelante y atrás)
    int df[4] = { 1, 1, -1, -1 };
    int dc[4] = { 1, -1, 1, -1 };
    char enemigo = (s == 'A') ? 'B' : 'A';

    for (int k = 0; k < 4; k++) {
        int mf = f + df[k];
        int mc = c + dc[k];
        int lf = f + 2*df[k];
        int lc = c + 2*dc[k];

        if (!dentro(mf,mc) || !dentro(lf,lc)) continue;

        if (celdas[mf][mc].simbolo == enemigo &&
            celdas[lf][lc].simbolo == '.' &&
            ((lf + lc) % 2 == 1)) { // landing debe ser casilla jugable
            return true;
        }
    }
    return false;
}

bool Tablero::puedeCapturarDama(int f, int c) {
    if (!dentro(f,c)) return false;
    char s = celdas[f][c].simbolo;
    if (s == '.') return false;
    char enemigo = (s == 'A') ? 'B' : 'A';
    int dirs[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

    for (int d = 0; d < 4; d++) {
        int df = dirs[d][0], dc = dirs[d][1];
        int nf = f + df, nc = c + dc;
        bool vistoEnemigo = false;

        while (dentro(nf, nc)) {
            char cur = celdas[nf][nc].simbolo;
            if (!vistoEnemigo) {
                if (cur == enemigo) {
                    vistoEnemigo = true;
                } else if (cur != '.') {
                    break; // pieza propia u obstáculo
                }
            } else {
                // ya vimos un enemigo; cualquier casilla vacía después y jugable => landing válido
                if (cur == '.') {
                    if ((nf + nc) % 2 == 1) return true;
                    // si la primera vacía no es jugable, quizá haya otra jugable más adelante
                    int nf2 = nf + df, nc2 = nc + dc;
                    while (dentro(nf2, nc2)) {
                        if (celdas[nf2][nc2].simbolo == '.') {
                            if ((nf2 + nc2) % 2 == 1) return true;
                        } else break;
                        nf2 += df; nc2 += dc;
                    }
                    break;
                } else {
                    break; // bloque por otra pieza
                }
            }
            nf += df; nc += dc;
        }
    }
    return false;
}

/* ---------- CAPTURAS PEÓN ---------- */

bool Tablero::capturarPeon(int f1, int c1, int f2, int c2, char s) {
    if (!dentro(f1,c1) || !dentro(f2,c2)) return false;
    if (celdas[f1][c1].simbolo != s) return false;
    if (celdas[f2][c2].simbolo != '.') return false;
    if (abs(f2 - f1) != 2 || abs(c2 - c1) != 2) return false;

    int mf = (f1 + f2) / 2;
    int mc = (c1 + c2) / 2;
    char enemy = (s == 'A' ? 'B' : 'A');

    if (!dentro(mf,mc)) return false;
    if (celdas[mf][mc].simbolo != enemy) return false;
    if ((f2 + c2) % 2 == 0) return false; // landing debe ser casilla jugable

    // ejecutar captura (preservando si era dama)
    Celda moved = celdas[f1][c1];
    celdas[mf][mc].simbolo = '.';
    celdas[mf][mc].dama = false;
    celdas[f2][c2] = moved;
    celdas[f1][c1].simbolo = '.';
    celdas[f1][c1].dama = false;
    return true;
}

/* ---------- CAPTURA DAMA ---------- */

bool Tablero::capturarDama(int f1, int c1, int f2, int c2, char s) {
    if (!dentro(f1,c1) || !dentro(f2,c2)) return false;
    if (celdas[f1][c1].simbolo != s) return false;
    if (celdas[f2][c2].simbolo != '.') return false;
    if ((f2 + c2) % 2 == 0) return false; // landing debe ser jugable

    int df = f2 - f1;
    int dc = c2 - c1;
    if (abs(df) != abs(dc) || df == 0) return false;

    int stepF = (df > 0 ? 1 : -1);
    int stepC = (dc > 0 ? 1 : -1);
    bool encontrado = false;
    int ef = -1, ec = -1;
    char enemy = (s == 'A') ? 'B' : 'A';

    int nf = f1 + stepF;
    int nc = c1 + stepC;
    // recorrer hasta la celda destino (ambas coordenadas llegan al mismo final porque abs(df)==abs(dc))
    while (nf != f2 && nc != c2) {
        if (!dentro(nf,nc)) return false; // seguridad
        char cur = celdas[nf][nc].simbolo;
        if (cur == enemy) {
            if (encontrado) return false; // más de un enemigo en el camino -> inválido para una sola captura
            encontrado = true;
            ef = nf; ec = nc;
        } else if (cur != '.') {
            return false; // obstrucción propia
        }
        nf += stepF;
        nc += stepC;
    }

    if (!encontrado) return false;

    // ejecutar captura: eliminar enemigo y mover dama (preservando dama)
    celdas[ef][ec].simbolo = '.';
    celdas[ef][ec].dama = false;
    Celda moved = celdas[f1][c1];
    celdas[f2][c2] = moved;
    celdas[f1][c1].simbolo = '.';
    celdas[f1][c1].dama = false;
    return true;
}

/* ---------- MOVIMIENTO SIMPLE ---------- */

bool Tablero::moverSimple(int f1, int c1, int f2, int c2, char s) {
    if (!dentro(f1,c1) || !dentro(f2,c2)) return false;
    if (celdas[f2][c2].simbolo != '.') return false;
    if ((f2 + c2) % 2 == 0) return false; // solo casillas jugables

    // Peón (solo 1 paso hacia adelante en damas internacionales)
    if (!celdas[f1][c1].dama) {
        int dir = (s == 'A' ? -1 : 1); // A sube (fila--), B baja (fila++)
        if (f2 == f1 + dir && abs(c2 - c1) == 1) {
            Celda moved = celdas[f1][c1];
            celdas[f2][c2] = moved;
            celdas[f1][c1].simbolo = '.';
            celdas[f1][c1].dama = false;
            return true;
        }
        return false;
    }

    // Dama: movimiento largo simple (camino libre)
    int df = f2 - f1;
    int dc = c2 - c1;
    if (abs(df) != abs(dc) || df == 0) return false;
    int stepF = (df > 0 ? 1 : -1);
    int stepC = (dc > 0 ? 1 : -1);

    int nf = f1 + stepF;
    int nc = c1 + stepC;
    while (nf != f2 && nc != c2) {
        if (!dentro(nf,nc)) return false;
        if (celdas[nf][nc].simbolo != '.') return false;
        nf += stepF;
        nc += stepC;
    }

    if ((f2 + c2) % 2 == 0) return false; // landing jugable
    Celda moved = celdas[f1][c1];
    celdas[f2][c2] = moved;
    celdas[f1][c1].simbolo = '.';
    celdas[f1][c1].dama = false;
    return true;
}

/* ---------- MOVIMIENTO PRINCIPAL -------- */

int Tablero::moverFicha(int f1, int c1, int f2, int c2, char s) {
    // Validaciones básicas
    if (!dentro(f1,c1) || !dentro(f2,c2)) return 0;
    if (celdas[f1][c1].simbolo != s) return 0;
    if (celdas[f2][c2].simbolo != '.') return 0;
    if ((f2 + c2) % 2 == 0) return 0; // casilla clara inválida

    bool capturaObligatoria = jugadorTieneCapturas(s);
    bool esDama = celdas[f1][c1].dama;

    // Dama
    if (esDama) {
        if (capturarDama(f1,c1,f2,c2,s)) {
            return puedeCapturarDama(f2,c2) ? 3 : 2;
        }
        if (!capturaObligatoria && moverSimple(f1,c1,f2,c2,s)) return 1;
        return 0;
    }

    // Peón: intentar captura (puede ser en cualquiera de las 4 diagonales)
    if (capturarPeon(f1,c1,f2,c2,s)) {
        if (puedeCapturarDesde(f2,c2)) return 3;
        coronar(f2,c2,s);
        return 2;
    }

    // Peón: movimiento simple (solo si no hay captura obligatoria)
    if (!capturaObligatoria && moverSimple(f1,c1,f2,c2,s)) {
        coronar(f2,c2,s);
        return 1;
    }

    // inválido
    return 0;
}

void Tablero::coronar(int f, int c, char s) {
    if (!dentro(f,c)) return;
    if (s == 'A' && f == 0) celdas[f][c].dama = true;
    if (s == 'B' && f == 9) celdas[f][c].dama = true;
}

bool Tablero::verificarVictoria(char oponente) {
    return !jugadorTieneMovimientos(oponente);
}

/* ----------- Helpers ----------- */

bool Tablero::jugadorTieneCapturas(char s) {
    for (int f = 0; f < 10; f++)
        for (int c = 0; c < 10; c++)
            if (celdas[f][c].simbolo == s)
                if (puedeCapturarDesde(f,c)) return true;
    return false;
}

bool Tablero::jugadorTieneMovimientos(char s) {
    for (int f = 0; f < 10; f++) {
        for (int c = 0; c < 10; c++) {
            if (celdas[f][c].simbolo != s) continue;

            if (puedeCapturarDesde(f,c)) return true;

            if (!celdas[f][c].dama) {
                int dir = (s == 'A' ? -1 : 1);
                if (dentro(f+dir,c-1) && celdas[f+dir][c-1].simbolo == '.') return true;
                if (dentro(f+dir,c+1) && celdas[f+dir][c+1].simbolo == '.') return true;
            } else {
                int dirs[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
                for (auto &d: dirs) {
                    int nf = f + d[0];
                    int nc = c + d[1];
                    while (dentro(nf,nc)) {
                        if (celdas[nf][nc].simbolo == '.') return true;
                        if (celdas[nf][nc].simbolo != '.') break;
                        nf += d[0];
                        nc += d[1];
                    }
                }
            }
        }
    }
    return false;
}

// Wrappers
bool Tablero::moverDama(int f1, int c1, int f2, int c2, char s) {
    return moverSimple(f1,c1,f2,c2,s);
}

bool Tablero::hayMovimientoDisponible(char s) {
    return jugadorTieneMovimientos(s);
}