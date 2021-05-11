#include "serialport.h"

SerialPort::SerialPort(const char *device, const unsigned int speed)
{
    this->device = (char *)malloc(sizeof(char) * strlen(device) + 1);
    strcpy(this->device, device);

    this->speed = speed;

    switch (speed)
    {
    case 4800:
        this->speed = B4800;
        break;
    case 9600:
        this->speed = B9600;
        break;
    case 19200:
        this->speed = B19200;
        break;
    case 38400:
        this->speed = B38400;
        break;
    case 57600:
        this->speed = B57600;
        break;
    case 115200:
        this->speed = B115200;
        break;
    }

    this->fileDescriptor = -1;
}

void SerialPort::initialize()
{
#ifndef DEBUG
    // Consultamos si el archivo corresponde a un dispositivo
    struct stat _stat;

    if (stat(device, &_stat) == -1)
        throw std::runtime_error("¡No se pudo identificar el dispositivo!");

    // Abrimos el dispositivo
    this->fileDescriptor = open(device, O_RDWR | O_NOCTTY | O_SYNC, 0);

    if (this->fileDescriptor < 0)
        throw std::runtime_error("¡No se pudo abrir el dispositivo!");

    // Configuracion del dispositivo
    struct termios _termios;
    if (tcgetattr(this->fileDescriptor, &_termios) != 0)
        throw std::runtime_error("¡No es un dispositivo serie!");

    cfsetispeed(&_termios, this->speed);
    cfsetospeed(&_termios, this->speed);

    // Configuramos el puerto serie con 8 Bits, sin paridad y sin bits de parada
    _termios.c_cflag &= ~PARENB;
    _termios.c_cflag &= ~CSTOPB;
    _termios.c_cflag &= ~CSIZE;
    _termios.c_cflag |= CS8;
    // Sin control de flujo por hardware
    _termios.c_cflag &= ~CRTSCTS;
    // Habilitamos la recepcion e ignoramos las lineas de estado
    _termios.c_cflag |= CREAD | CLOCAL;
    // Deshabilitamos el control de flujo de entrada/salida y los caracteres de reinicio
    _termios.c_iflag &= ~(IXON | IXOFF | IXANY);
    // Deshabilitamos la entrada canónica, el eco, los caracteres de borrado y las señales de teminal
    _termios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // Deshabilitamos la salida procesada
    _termios.c_oflag &= ~OPOST;

    // véase http://unixwiz.net/techtips/termios-vmin-vtime.html
    _termios.c_cc[VMIN] = 0;
    _termios.c_cc[VTIME] = 20;

    if (tcsetattr(this->fileDescriptor, TCSANOW, &_termios) != 0)
        throw std::runtime_error("¡No se pudo configurar el puerto serie!");
#endif

    sleep(1);
}

SerialPort::~SerialPort()
{
    try
    {
        finalize();
    }
    catch (std::runtime_error e)
    {
    }
}

void SerialPort::finalize()
{
    if (this->fileDescriptor != -1)
    {
#ifndef DEBUG
        // Cerramos el dispositivo
        if (close(this->fileDescriptor) < 0)
        {
            throw std::runtime_error("¡Error al cerrar el dispositivo!");
        }
#endif

        this->fileDescriptor = -1;
    }
}

int SerialPort::send(const char *buffer, unsigned int size)
{
#ifndef DEBUG
    int result;

    if (this->fileDescriptor == -1)
        throw std::runtime_error("¡No se puede escribir en un dispositivo sin inicializar!");

    result = write(this->fileDescriptor, buffer, size);
    return result;
#else
    return size;
#endif
}

int SerialPort::recv(char *buffer, unsigned int size)
{
    unsigned int i = 0;
    char byte;
    int n;

#ifndef DEBUG
    if (this->fileDescriptor == -1)
        throw std::runtime_error("¡No se puede leer de un dispositivo sin inicializar!");

    tcflush(this->fileDescriptor, TCIFLUSH);

    do
    {
        n = read(this->fileDescriptor, &byte, 1);

        if (n == -1)
            return -1; // Error al leer
        else if (n == 0)
        {
            usleep(10 * 1000); // Esperar 10ms
            continue;
        }

        buffer[i++] = byte;
    } while (i < size - 1 && byte != '\0');

    buffer[i] = '\0';

    return i;
#else
    return size;
#endif
}