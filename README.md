Nombre de los integrantes:
Lorduy Ayus Samir Elias y Joel Santiago Sarmiento Baldayo

## Desarrollos innovadores implementados
1. Menú de pausa en medio de la partida con opción de regresar al juego o salir al menú principal.
2. Interfaz visual con colores ANSI elegibles para cada jugador, hecha para distinguir fichas y turnos.
3. Guardado de estadísticas básicas de jugadores (victorias, derrotas, empates).
4. Validaciones de entrada robustas para evitar errores de usuario.


Innovación 1 — Menú de pausa
¿Para qué se implementó?
Se implemento para permitir a los jugadores detener la partida temporalmente, decidir si quieren continuar o volver al menú principal; posibilitar operaciones relacionadas (guardado, carga o configuración) desde un estado pausado.


Usabilidad: en sesiones largas o interrupciones reales, el jugador debe poder pausar sin cerrar la aplicación.
Robustez: facilita guardar/cargar el estado en un punto seguro del juego.
Experiencia: mejora la sensación de control del usuario y profesionaliza la aplicación.


En el bucle principal de juego, al pedir coordenadas de origen, el jugador puede introducir una señal ('P') en vez de coordenadas numéricas.
Si se detecta 'P', se invoca menuPausa() que muestra un submenú con opciones (ej.: volver al juego, guardar partida, cargar partida, salir al menú principal).
El retorno de menuPausa() informa al bucle principal si debe continuar o abandonar la partida.


Innovación 2 — Colores ANSI
¿Para qué se implementó?
Se implemento para mejorar la legibilidad del tablero en consola y distinguir visualmente las fichas y el turno actual mediante colores.


Claridad visual: colores ayudan a identificar rápidamente quién juega y dónde están las fichas.
Accesibilidad: reduce la carga visual en tableros densos.
Estética: hace la interfaz más profesional y pleasant de usar.


Se añade en Tablero un método que mapee nombres de colores a códigos ANSI (\033[...m)., luego se guarda en Tablero los nombres de color para A y B (colorA, colorB) y se usan en mostrar().
En mostrar(), se imprime cada celda con el color del símbolo si hay ficha; en casillas vacías se imprime un patrón (p. ej. | | o · ) y opciones para colorear casillas oscuras con fondo gris si la terminal lo soporta.



Innovación 3 — Validaciones de entrada
¿Para qué se implementó?
Se implemento para evitar que entradas inválidas rompan el flujo del juego (por ejemplo, caracteres donde se esperan enteros) y ofrecer al usuario retroalimentación clara.


Seguridad: previene excepciones y comportamientos indefinidos, en accesos a índices fuera de rango, etc.
Usabilidad: evita reinicios abruptos; el programa solicita al usuario que repita en caso de error.


Se usa if (!(cin >> var)) para detectar error de extracción y limpiar cin con cin.clear() y cin.ignore(...).
Tambien se valida rangos numéricos (1..10) antes de restar 1 para índices.
Antes de convertir string a entero con stoi, se valida que la cadena contiene solo dígitos o manejar la excepción std::invalid_argument.
