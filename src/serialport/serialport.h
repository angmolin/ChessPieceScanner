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
     * Constructor con dos parametros de entrada
     */
    SerialPort(const char *device, const unsigned int speed);

    /**
     * Configura el puerto serie con 8 bits
     */
    void initialize();

    /**
     * Constructor que llama a finalize()
     */
    ~SerialPort();

    /**
     * Finaliza el dispositivo
     */
    void finalize();

    /**
     * Envia y escribe en el dispositivo
     */
    int send(const char *buffer, unsigned int size);

    /**
     * ***************************************************************************************
     */
    int recv(char *buffer, unsigned int size);

private:
    char *device;
    speed_t speed;

    int fileDescriptor;
};

#endif