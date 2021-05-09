#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdexcept>
#include <unistd.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <errno.h>

#include <string.h>

#include <termios.h>

class SerialPort
{
public:
    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    SerialPort(const char *device, const unsigned int speed);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void initialize();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    ~SerialPort();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void finalize();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    int send(const char *buffer, unsigned int size);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    int recv(char *buffer, unsigned int size);

private:
    char *device;
    speed_t speed;

    int fileDescriptor;
};

#endif