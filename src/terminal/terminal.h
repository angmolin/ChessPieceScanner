#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>

#include <unistd.h>

#include <termios.h>

class Terminal
{
public:
    typedef enum
    {
        IN,
        OUT,
        ERR
    } stream_t;

    typedef enum
    {
        KEY_INVALID,

        KEY_LEFT,
        KEY_UP,
        KEY_RIGHT,
        KEY_DOWN,

        KEY_SPACE,

        KEY_ADD,
        KEY_SUB,

        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,

        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z
    } key_t;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    Terminal(const int in, const int out, const int err);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    ~Terminal();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    int disableEcho(const stream_t _stream) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    int restoreDefaults(const stream_t _stream) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    key_t getKey() const;

private:
    int in_fileno, out_fileno, err_fileno;
    struct termios in_termios, out_termios, err_termios;
};

#endif