#include "webcamV4L2.h"

static int ioctlV4L2(int fd, unsigned long int request, void *argument)
{
    int result;

    do
    {
        result = ioctl(fd, request, argument);
    } while (result == -1 && EINTR == errno);

    return result;
}

WebcamV4L2::WebcamV4L2(const char *device, const unsigned int width, const unsigned int height)
{
    this->device = (char *)malloc(sizeof(char) * strlen(device) + 1);
    strcpy(this->device, device);

    this->width = width;
    this->height = height;

    this->fileDescriptor = -1;
}

void WebcamV4L2::initialize()
{
#ifndef DEBUG
    // Consultamos si el archivo corresponde a un dispositivo
    struct stat _stat;

    if (stat(this->device, &_stat) == -1)
        throw std::runtime_error("¡No se pudo identificar el dispositivo!");

    if (!S_ISCHR(_stat.st_mode))
        throw std::runtime_error("¡No es un dispositivo de video!");

    // Abrimos el dispositivo
    this->fileDescriptor = open(this->device, O_RDWR, 0); // | O_NONBLOCK, 0);

    if (this->fileDescriptor < 0)
        throw std::runtime_error("¡No se pudo abrir el dispositivo!");

    // Consultamos las capacidades del dispositivo
    struct v4l2_capability _capability;

    if (ioctlV4L2(this->fileDescriptor, VIDIOC_QUERYCAP, &_capability) < 0)
        throw std::runtime_error("¡No se pudieron consultar las capacidades del dispositivo!");

    if (!(_capability.capabilities & V4L2_CAP_VIDEO_CAPTURE))
        throw std::runtime_error("¡El dispositivo no admite la captura de video!");

    if (!(_capability.capabilities & V4L2_CAP_STREAMING))
        throw std::runtime_error("¡El dispositivo no admite el video en streaming!");

    // Establecemos el formato de captura
    struct v4l2_format _format;

    _format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    _format.fmt.pix.pixelformat = this->pixelFormat;
    _format.fmt.pix.width = this->width;
    _format.fmt.pix.height = this->height;

    if (ioctlV4L2(this->fileDescriptor, VIDIOC_S_FMT, &_format) < 0)
        throw std::runtime_error("¡El dispositivo no soporta el formato MJPEG!");

    this->width = _format.fmt.pix.width;
    this->height = _format.fmt.pix.height;

    // Informamos al dispositivo sobre los buffers de captura
    struct v4l2_requestbuffers _requestbuffers;

    _requestbuffers.count = 4;
    _requestbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    _requestbuffers.memory = V4L2_MEMORY_MMAP;

    if (ioctlV4L2(this->fileDescriptor, VIDIOC_REQBUFS, &_requestbuffers) == -1)
        throw std::runtime_error("¡No se pudieron configurar los bufferes!");

    // Comprobamos cuantos bufferes soporta el dispositivo (al menos necesitamos 1)
    if (_requestbuffers.count < 1)
        throw std::runtime_error("¡No hay bufferes disponibles!");

    // Reservamos memoria para los bufferes
    this->captureBuffer = (WebcamV4L2::bufferV4L2_t *)calloc(_requestbuffers.count, sizeof(*captureBuffer));

    if (!this->captureBuffer)
        throw std::runtime_error("¡No queda memoria en el sistema!");

    this->captureBufferCount = _requestbuffers.count;

    // Registramos los bufferes en el dispositivo de captura
    for (unsigned int i = 0; i < this->captureBufferCount; i++)
    {
        struct v4l2_buffer _buffer;

        _buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        _buffer.memory = V4L2_MEMORY_MMAP;
        _buffer.index = i;

        if (ioctlV4L2(this->fileDescriptor, VIDIOC_QUERYBUF, &_buffer) == -1)
        {
            free(captureBuffer);
            throw std::runtime_error("¡No se pudo registrar el buffer en el dispositivo!");
        }

        this->captureBuffer[i].size = _buffer.length;
        this->captureBuffer[i].data = mmap(
            NULL, // Empieza en cualquier lugar
            _buffer.length,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            this->fileDescriptor,
            _buffer.m.offset);

        if (this->captureBuffer[i].data == MAP_FAILED)
        {
            free(this->captureBuffer);
            throw std::runtime_error("¡No se pudo mapear el buffer de captura!");
        }
    }

    this->indexBuffer = 0;

    // Iniciamos el streaming
    int _type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctlV4L2(this->fileDescriptor, VIDIOC_STREAMON, &_type) < 0)
    {
        free(this->captureBuffer);
        throw std::runtime_error("¡No se pudo iniciar el streaming!");
    }
#else
    this->captureBufferCount = 1;
    this->captureBuffer = (WebcamV4L2::bufferV4L2_t *)calloc(1, sizeof(*captureBuffer));
#endif

    sleep(1);
}

bool WebcamV4L2::configure(__u32 id, __s32 value)
{
#ifndef DEBUG
    if (this->fileDescriptor != -1)
    {
        v4l2_control c = { id, value };

        if (ioctlV4L2(this->fileDescriptor, VIDIOC_S_CTRL, &c) == 0)
            return true;
        else
            return false;
    }

    return false;
#else
    return true;
#endif
}

bool WebcamV4L2::configure_auto_exposure(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_EXPOSURE_AUTO: %i", value);
#endif
    return configure(V4L2_CID_EXPOSURE_AUTO, value);
}

bool WebcamV4L2::configure_auto_white_balance(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_AUTO_WHITE_BALANCE: %i", value);
#endif
    return configure(V4L2_CID_AUTO_WHITE_BALANCE, value);
}

bool WebcamV4L2::configure_backlight_compensation(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_BACKLIGHT_COMPENSATION: %i", value);
#endif
    return configure(V4L2_CID_BACKLIGHT_COMPENSATION, value);
}

bool WebcamV4L2::configure_brightness(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_BRIGHTNESS: %i", value);
#endif
    return configure(V4L2_CID_BRIGHTNESS, value);
}

bool WebcamV4L2::configure_contrast(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_CONTRAST: %i", value);
#endif
    return configure(V4L2_CID_CONTRAST, value);
}

bool WebcamV4L2::configure_exposure_time(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_EXPOSURE: %i", value);
#endif
    return configure(V4L2_CID_EXPOSURE, value);
}

bool WebcamV4L2::configure_gain(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_GAIN: %i", value);
#endif
    return configure(V4L2_CID_GAIN, value);
}

bool WebcamV4L2::configure_gamma_correction(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_GAMMA: %i", value);
#endif
    return configure(V4L2_CID_GAMMA, value);
}

bool WebcamV4L2::configure_hue(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_HUE: %i", value);
#endif
    return configure(V4L2_CID_HUE, value);
}

bool WebcamV4L2::configure_power_line_frequency(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_POWER_LINE_FREQUENCY: %i", value);
#endif
    return configure(V4L2_CID_POWER_LINE_FREQUENCY, value);
}

bool WebcamV4L2::configure_saturation(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_SATURATION: %i", value);
#endif
    return configure(V4L2_CID_SATURATION, value);
}

bool WebcamV4L2::configure_sharpness(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_SHARPNESS: %i", value);
#endif
    return configure(V4L2_CID_SHARPNESS, value);
}

bool WebcamV4L2::configure_white_balance_temperature(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_WHITE_BALANCE_TEMPERATURE: %i", value);
#endif
    return configure(V4L2_CID_WHITE_BALANCE_TEMPERATURE, value);
}

bool WebcamV4L2::configure_vertical_flip(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_VFLIP: %i", value);
#endif
    return configure(V4L2_CID_VFLIP, value);
}

bool WebcamV4L2::configure_horizontal_flip(__s32 value)
{
#ifdef VERBOSE
    printf("V4L2_CID_HFLIP: %i", value);
#endif
    return configure(V4L2_CID_HFLIP, value);
}

WebcamV4L2::~WebcamV4L2()
{
    try
    {
        finalize();
    }
    catch (std::runtime_error e)
    {
    }
}

void WebcamV4L2::finalize()
{
#ifndef DEBUG
    if (this->fileDescriptor != -1)
    {
        free(this->device);

        // Detenemos el streaming
        int _type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (ioctlV4L2(this->fileDescriptor, VIDIOC_STREAMOFF, &_type) < 0)
        {
            throw std::runtime_error("¡No se pudo detener el streaming!");
        }

        // Liberamos la memoria usada por los bufferes
        for (unsigned int i = 0; i < this->captureBufferCount; i++)
            if (munmap(this->captureBuffer[i].data, this->captureBuffer[i].size) == -1)
            {
                throw std::runtime_error("¡No se pudo liberar el buffer de captura!");
            }

        free(this->captureBuffer);

        // Cerramos el dispositivo
        if (close(this->fileDescriptor) < 0)
        {
            throw std::runtime_error("¡No se pudo cerrar el dispositivo!");
        }

        this->fileDescriptor = -1;
    }
#else
    free(this->captureBuffer);
#endif
}

const WebcamV4L2::bufferV4L2_t &WebcamV4L2::captureFrame()
{
#ifndef DEBUG
    struct v4l2_buffer _buffer;

    _buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    _buffer.memory = V4L2_MEMORY_MMAP;
    _buffer.index = this->indexBuffer % this->captureBufferCount;

    // Encolamos la imagen
    if (ioctlV4L2(this->fileDescriptor, VIDIOC_QBUF, &_buffer) < 0)
        throw std::runtime_error("¡No se pudo encolar la imagen!");

    // Desencolamos la imagen
    if (ioctlV4L2(this->fileDescriptor, VIDIOC_DQBUF, &_buffer) < 0)
        throw std::runtime_error("¡No se pudo desencolar la imagen!");

    this->indexBuffer++;

    return this->captureBuffer[_buffer.index];
#else
    return this->testBuffer;
#endif
}