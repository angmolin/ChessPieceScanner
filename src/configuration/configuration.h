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
     * Crea un nuevo objeto WebcamV4L2
     */
    Configuration(const char *path);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    ~Configuration();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool next();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    char *getline();

private:
    std::ifstream *file = NULL;
    char line[CONFIGURATION_LINE_MAX_LENGHT] = { '\0' };
};

#endif