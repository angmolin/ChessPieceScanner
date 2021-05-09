// sudo apt install libjpeg-dev

#ifndef IMAGE_JPEG_H
#define IMAGE_JPEG_H

#include <iostream>

#include <memory>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <jpeglib.h>

#include "../../webcamV4L2/webcamV4L2.h"

class imageJPEG
{
public:
    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    imageJPEG(const char *file);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    imageJPEG(const WebcamV4L2::bufferV4L2_t buffer);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void save(const char *file, const unsigned int quality) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void savePpm(const char *file) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned int getHeight() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned int getWidth() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned int getPixelSize() const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    const std::vector<unsigned char> getPixel(const unsigned int x, const unsigned int y) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    unsigned char getLuminance(const unsigned int x, const unsigned int y) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    const std::vector<unsigned char> getAverage(const unsigned int x, const unsigned int y, const unsigned int boxSize) const;

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    // void shrink(const unsigned int newWidth);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    // void expand(const unsigned int newWidth);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    // void resize(const unsigned int newWidth);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void crop(const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void drawLine(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int color);

    /**
     * Crea un nuevo objeto WebcamV4L2
     */
    void drawRectangle(const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h, const unsigned int color);

private:
    std::shared_ptr<jpeg_error_mgr> errorMgr;

    std::vector<std::vector<unsigned char>> bitmapData;

    unsigned int width;

    unsigned int height;
    
    unsigned int pixelSize;
    
    int colorSpace;
};

#endif