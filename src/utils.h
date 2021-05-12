#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <signal.h>

#include "configuration/configuration.h"
#include "image/jpeg/imageJPEG.h"
#include "serialport/serialport.h"
#include "terminal/terminal.h"
#include "terminal/terminalTable.h"
#include "webcamV4L2/webcamV4L2.h"

#define MAX_PATH_LENGTH 64
#define MAX_NAME_LENGTH 32
#define MAX_MESSAGE_LENGTH 256

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define CONFIG_FILE_WEBCAM "conf/webcamV4L2.conf"
#define CONFIG_FILE_PHOTOBOOTH "conf/images.conf"

typedef struct
{
    unsigned int x;
    unsigned int y;
} photoBoothCropMargin_t;

typedef struct
{
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
} photoBoothCrop_t;

typedef struct
{
    int j;
    int y;
} photoBoothCoordinate_t;

typedef struct
{
    photoBoothCoordinate_t coordinate;
    photoBoothCrop_t crop;
} photoBoothPhoto_t;


static std::vector<photoBoothPhoto_t> photoBoothPhotos;
static unsigned int photoBoothQuality;
static photoBoothCropMargin_t photoBoothCropMargin;
static bool photoBoothCropEnabled;

#define abs(a) \
    ({ __typeof__ (a) _a = (a); \
       _a > 0 ? _a : -_a; \
    })

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; \
    })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; \
    })

static const unsigned int inicializationValues[] = {65, 117, 116, 111, 114, 58, 32, 193, 110, 103, 101, 108, 32, 77, 111, 108, 105, 110, 97, 32, 78, 250, 241, 101, 122, 0};

// Funciones de ayuda al puerto serie
/**
 * Envía el comando `command` a través del puerto serie `serial`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * @param command Comando a enviar.
 * 
 * @return Devuelve `true` si se pudo enviar o `false` si no.
 */
bool sendCommand(SerialPort &serial, const char *command);

/**
 * Mueve el eje J (Servo de la cámara) del dispositivo conectado al puerto serie `serial` hasta el grado `degree`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * @param degree Grado que se debe establecer.
 * 
 * @return Devuelve `true` si se pudo mover o `false` si no.
 */
bool moveJ(SerialPort &serial, unsigned int degree);

/**
 * Obtiene la posición del eje J (Servo de la cámara) del dispositivo conectado al puerto serie `serial`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * 
 * @return Devuelve el grado actual del eje J (Servo de la cámara).
 */
int readJ(SerialPort &serial);

/**
 * Mueve el eje Y (Soporte de la cámara) del dispositivo conectado al puerto serie `serial` hasta el grado `degree`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * @param degree Grado que se debe establecer.
 * 
 * @return Devuelve `true` si se pudo mover o `false` si no.
 */
bool moveY(SerialPort &serial, unsigned int degree);

/**
 * Obtiene la posición del eje Y (Soporte de la cámara) del dispositivo conectado al puerto serie `serial`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * 
 * @return Devuelve el grado actual del eje Y (Soporte de la cámara).
 */
int readY(SerialPort &serial);

/**
 * Mueve el eje Z (Base) del dispositivo conectado al puerto serie `serial` hasta el grado `degree`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * @param degree Grado que se debe establecer.
 * 
 * @return Devuelve `true` si se pudo mover o `false` si no.
 */
bool moveZ(SerialPort &serial, unsigned int degree);

/**
 * Obtiene la posición del eje Z (Base) del dispositivo conectado al puerto serie `serial`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * 
 * @return Devuelve el grado actual del eje Z (Base).
 */
int readZ(SerialPort &serial);

/**
 * Calibra los ejes J, Y y Z del dispositivo conectado al puerto serie `serial`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * 
 * @return Devuelve `true` si se pudo calibrar o `false` si no.
 */
bool calibrate(SerialPort &serial);

/**
 * Libera los ejes Y y Z del dispositivo conectado al puerto serie `serial`.
 * 
 * @param serial Puerto serie de comunicaciones.
 * 
 * @return Devuelve `true` si se pudo liberar o `false` si no.
 */
bool liberate(SerialPort &serial);

// Funciones de ayuda a la webcam
/**
 * Guarda el buffer `buffer` de la cámara a un archivo con el nombre `name`.
 * 
 * @param name Nombre del archivo.
 * @param buffer Buffer que contiene la imagen de la cámara.
 * 
 * @return Devuelve los bytes escritos en el archivo o -1 si falla.
 */
int saveImage(const char *name, WebcamV4L2::bufferV4L2_t buffer);

/**
 * Emite un sonido
 */
static inline void beep()
{
    std::wcout << "\7" << std::flush;
}

/**
 * Avanza el cursor una posicion hacia arriba
 */
static inline void cursorUp()
{
    std::wcout << "\033[A";
}

/**
 * Avanza el cursor una posicion hacia abajo
 */
static inline void cursorDown()
{
    std::wcout << "\033[B";
}

/**
 * Avanza el cursor una posicion hacia la derecha
 */
static inline void cursorForward()
{
    std::wcout << "\033[C";
}

/**
 * Avanza el cursor una posicion hacia la izquierda
 */
static inline void cursorBack()
{
    std::wcout << "\033[D";
}

/**
 * Coloca el cursor al principio de la linea
 */
static inline void cursorLineStart()
{
    std::wcout << "\r";
}

/**
 * Borra la pantalla.
 */
static inline void eraseDisplay()
{
    std::wcout << "\033[J";
}

/**
 * Mueve el cursor a unas cordenadas x, y.
 */
static inline void moveCursor(const unsigned int x, const unsigned int y)
{
    std::wcout << "\033[" << y << ";" << x << "f";
}

/**
 * Guarda la posicion del cursor
 */
static inline void saveCursor()
{
    std::wcout << "\033[s";
}

/**
 * Restaura el cursor
 */
static inline void restoreCursor()
{
    std::wcout << "\033[u";
}

// Terminales
/**
 * Terminal por defecto.
 */
static Terminal defaultTerminal(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);

// Modelos de tablas de terminal
/**
 * Modelo de tabla normal.
 */
static TerminalTableModel TerminalTableModelNormal();

/**
 * Modelo de tabla doble.
 */
static TerminalTableModel TerminalTableModelDouble(TerminalTableModel::tableModel{
    L"\u2554", L"\u2566", L"\u2557",
    L"\u2560", L"\u256C", L"\u2563",
    L"\u255A", L"\u2569", L"\u255D",
    L"\u2550", L"\u2551"});

/**
 * Modelo de tabla bold.
 */
static TerminalTableModel TerminalTableModelBold(TerminalTableModel::tableModel{
    L"\u250F", L"\u2533", L"\u2513",
    L"\u2523", L"\u254B", L"\u252B",
    L"\u2517", L"\u253B", L"\u251B",
    L"\u2501", L"\u2503"});

// Menus
Terminal::key_t menu(const TerminalTable &menuTable, const Terminal::key_t minKey, const Terminal::key_t maxKey);

/**
 * Menu principal
 */
void mainMenu(SerialPort &serial, WebcamV4L2 &webcamV4L2);

/**
 * Cabina de fotos
 */
void photoBooth(SerialPort &serial, WebcamV4L2 &webcamV4L2);

/**
 * Menu manual
 */
void manualMenu(SerialPort &serial, WebcamV4L2 &webcamV4L2);

/**
 * Menu experto
 */
void expertMenu(SerialPort &serial, WebcamV4L2 &webcamV4L2);

/**
 * Busca la configuracion de la camara
 */
void loadConfigWebcam(WebcamV4L2 &webcamV4L2);

/**
 * Busca la configuracion de la cabina de fotos
 */
void loadConfigPhotoBooth();

#endif