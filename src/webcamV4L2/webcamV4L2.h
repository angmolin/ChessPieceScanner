// sudo apt install libv4l-dev

#ifndef WEBCAM_V4L2_H
#define WEBCAM_V4L2_H

#include <stdexcept>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include <fcntl.h>

#include <errno.h>

#include <string.h>

#include <linux/videodev2.h>

class WebcamV4L2
{
public:
    typedef struct
    {
        void *data;
        unsigned long size;
    } bufferV4L2_t;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    WebcamV4L2(const char *device, const unsigned int width, const unsigned int height);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void initialize();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    ~WebcamV4L2();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure(__u32 id, __s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_auto_exposure(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_auto_white_balance(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_backlight_compensation(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_brightness(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_contrast(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_exposure_time(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_gain(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_gamma_correction(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_hue(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_power_line_frequency(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_saturation(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_sharpness(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_white_balance_temperature(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_vertical_flip(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    bool configure_horizontal_flip(__s32 value);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void finalize();

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    const WebcamV4L2::bufferV4L2_t &captureFrame();

private:
    const unsigned int pixelFormat = V4L2_PIX_FMT_MJPEG;

    char *device;
    unsigned int width, height;

    int fileDescriptor;

    bufferV4L2_t *captureBuffer;
    unsigned int captureBufferCount;
    unsigned int indexBuffer;
};

#endif