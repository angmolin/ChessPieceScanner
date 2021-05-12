#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdarg.h>
#include <string.h>
#include <fstream>

#define CONFIGURATION_LINE_MAX_LENGHT 64

class Configuration
{
public:
    /**
     * Constructor publico de la clase Configuration con un parametro tipo char.
     */
    Configuration(const char *path);

    /**
     * Constructor por defecto
     */
    ~Configuration();

    /**
     * Funcion de tipo boolean siguiente que no recibe ningun parametro
     */
    bool next();

    /**
     * Funcion de tipo char que obtiene la linea
     */
    char *getline();

private:
    std::ifstream *file = NULL;
    char line[CONFIGURATION_LINE_MAX_LENGHT] = { '\0' };
};

#endif