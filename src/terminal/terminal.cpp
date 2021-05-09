#include "terminal.h"

Terminal::Terminal(const int in, const int out, const int err)
{
    this->in_fileno = in;
    this->out_fileno = out;
    this->err_fileno = err;

    tcgetattr(this->in_fileno, &this->in_termios);
    tcgetattr(this->out_fileno, &this->out_termios);
    tcgetattr(this->err_fileno, &this->err_termios);
}

Terminal::~Terminal()
{
    restoreDefaults(IN);
    restoreDefaults(OUT);
    restoreDefaults(ERR);
}

int Terminal::disableEcho(const stream_t _stream) const
{
    struct termios modified_termios;

    switch (_stream)
    {
    default:
    case IN:
        modified_termios = this->in_termios;
        break;
    case OUT:
        modified_termios = this->out_termios;
        break;
    case ERR:
        modified_termios = this->err_termios;
        break;
    }

    modified_termios.c_lflag &= ~(ICANON | ECHO);

    switch (_stream)
    {
    default:
    case IN:
        return tcsetattr(this->in_fileno, TCSANOW, &modified_termios);
        break;
    case OUT:
        return tcsetattr(this->out_fileno, TCSANOW, &modified_termios);
        break;
    case ERR:
        return tcsetattr(this->err_fileno, TCSANOW, &modified_termios);
        break;
    }
}

int Terminal::restoreDefaults(stream_t _stream) const
{
    switch (_stream)
    {
    default:
    case IN:
        return tcsetattr(this->in_fileno, TCSANOW, &this->in_termios);
        break;
    case OUT:
        return tcsetattr(this->out_fileno, TCSANOW, &this->out_termios);
        break;
    case ERR:
        return tcsetattr(this->err_fileno, TCSANOW, &this->err_termios);
        break;
    }
}

Terminal::key_t Terminal::getKey() const
{
    wchar_t character1, character2, character3;
    key_t result;

    std::wcin.sync();
    do
    {
        character1 = std::wcin.get();
    } while (character1 == 10);

    switch (character1)
    {
    default:
        result = KEY_INVALID;
        break;
    case 10:
        // Enter
        break;

    case 27:
        character2 = std::wcin.get();
        character3 = std::wcin.get();

        if (character2 == 91)
            switch (character3)
            {
            default:
                result = KEY_INVALID;
                break;
            case 65:
                result = KEY_UP;
                break;
            case 66:
                result = KEY_DOWN;
                break;
            case 67:
                result = KEY_RIGHT;
                break;
            case 68:
                result = KEY_LEFT;
                break;
            }
        else
            result = KEY_INVALID;
        break;

    case 32:
        result = KEY_SPACE;
        break;

    case 43:
        result = KEY_ADD;
        break;
    case 45:
        result = KEY_SUB;
        break;

    case 48:
        result = KEY_0;
        break;
    case 49:
        result = KEY_1;
        break;
    case 50:
        result = KEY_2;
        break;
    case 51:
        result = KEY_3;
        break;
    case 52:
        result = KEY_4;
        break;
    case 53:
        result = KEY_5;
        break;
    case 54:
        result = KEY_6;
        break;
    case 55:
        result = KEY_7;
        break;
    case 56:
        result = KEY_8;
        break;
    case 57:
        result = KEY_9;
        break;

    case 65:
    case 97:
        result = KEY_A;
        break;
    case 66:
    case 98:
        result = KEY_B;
        break;
    case 67:
    case 99:
        result = KEY_C;
        break;
    case 68:
    case 100:
        result = KEY_D;
        break;
    case 69:
    case 101:
        result = KEY_E;
        break;
    case 70:
    case 102:
        result = KEY_F;
        break;
    case 71:
    case 103:
        result = KEY_G;
        break;
    case 72:
    case 104:
        result = KEY_H;
        break;
    case 73:
    case 105:
        result = KEY_I;
        break;
    case 74:
    case 106:
        result = KEY_J;
        break;
    case 75:
    case 107:
        result = KEY_K;
        break;
    case 76:
    case 108:
        result = KEY_L;
        break;
    case 77:
    case 109:
        result = KEY_M;
        break;
    case 78:
    case 110:
        result = KEY_N;
        break;
    case 79:
    case 111:
        result = KEY_O;
        break;
    case 80:
    case 112:
        result = KEY_P;
        break;
    case 81:
    case 113:
        result = KEY_Q;
        break;
    case 82:
    case 114:
        result = KEY_R;
        break;
    case 83:
    case 115:
        result = KEY_S;
        break;
    case 84:
    case 116:
        result = KEY_T;
        break;
    case 85:
    case 117:
        result = KEY_U;
        break;
    case 86:
    case 118:
        result = KEY_V;
        break;
    case 87:
    case 119:
        result = KEY_W;
        break;
    case 88:
    case 120:
        result = KEY_X;
        break;
    case 89:
    case 121:
        result = KEY_Y;
        break;
    case 90:
    case 122:
        result = KEY_Z;
        break;
    }

    return result;
}