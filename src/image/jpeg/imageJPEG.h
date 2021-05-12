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
     * Constructor publico con un parametro de tipo char
     */
    imageJPEG(const char *file);

    /**
     * Constructor con buffer de parametro de entrada
     */
    imageJPEG(const WebcamV4L2::bufferV4L2_t buffer);

    /**
     * Funcion que guarda un archivo
     */
    void save(const char *file, const unsigned int quality) const;

    /**
     * Funcion que guarda un archivo
     */
    void savePpm(const char *file) const;

    /**
     * Funcion que obtiene la altura de la imagen
     */
    unsigned int getHeight() const;

    /**
     * Funcion que obtiene la anchura de la imagen
     */
    unsigned int getWidth() const;

    /**
     * Funcion que obtiene el tamaño de los pixeles de la imagen
     */
    unsigned int getPixelSize() const;

    /**
     * Funcion que obtiene los pixeles de la imagen
     */
    const std::vector<unsigned char> getPixel(const unsigned int x, const unsigned int y) const;

    /**
     * Funcion que obtiene la altura de la imagen***************************************************************************************************
     */
    unsigned char getLuminance(const unsigned int x, const unsigned int y) const;

    /**
     * Funcion que obtiene el promedio de los valores del tamaño de la caja
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
     * Funcion que añade el valor al final 
     */
    void crop(const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h);

    /**
     * Funcion dibuja las lineas
     */
    void drawLine(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int color);

    /**
     *  Funcion dibuja las lineas del rectangulo de la imagen
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