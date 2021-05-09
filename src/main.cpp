/**
 * Autor: Angel Molina Núñez
 * 
 * Fecha: 12/03/2021
 * 
 * Descripción: Programa de control para el escáner de piezas de ajedrez.
 * La comunicación con el escáner se realiza mediante protocolo serie con
 * la clase SerialPort y el manejo de la cámara se realiza con la 
 * biblioteca libv4l-dev a través de la clase WebcamV4L2.
 */

#include <iostream>
#include <locale>

#include "utils.h"

int main(int argc, char **argv)
{
    setlocale(LC_ALL, std::locale("").name().c_str());
    
    if (argc != 3)
    {
        std::wcout << L"Uso: " << argv[0] << L" <dispositivo captura> <puerto serie>\n";
        return 1;
    }
    
    WebcamV4L2 webcam(argv[1], 1280, 720);
    SerialPort serial(argv[2], 115200);

    moveCursor(0, 0);
    eraseDisplay();

    try
    {
        std::wcout << L"Inicializando cámara web..." << std::flush;
#ifndef DEBUG
        webcam.initialize();
#endif
        std::wcout << L"OK!\n";

        std::wcout << L"Inicializando puerto serie..." << std::flush;
#ifndef DEBUG
        serial.initialize();
#endif
        std::wcout << L"OK!\n";

        std::wcout << L"Calibrando el dispositivo..." << std::flush;
#ifndef DEBUG
        calibrate(serial);
#endif
        std::wcout << L"OK!\n";
    }
    catch (const std::exception &e)
    {
        std::wcout << L"ERROR!\n";
        return -1;
    }
    
    mainMenu(serial, webcam);

    try
    {
        std::wcout << L"Cerrando la aplicación..." << std::flush;

        defaultTerminal.restoreDefaults(Terminal::IN);
#ifndef DEBUG
        liberate(serial);
#endif

        std::wcout << L"OK!\n";
    }
    catch (const std::exception &e)
    {
        std::wcout << L"ERROR!\n";
        return -1;
    }

    return 0;
}

/*
Y: 5, Z: 0, J: 30  //  0 grados

Y: 75, Z: 0, J: 45 // 90 grados
*/