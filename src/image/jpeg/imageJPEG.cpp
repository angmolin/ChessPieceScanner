#include "imageJPEG.h"

imageJPEG::imageJPEG(const char *file)
{
    jpeg_decompress_struct _decompress_struct;

    // Abrimos el archivo
    FILE *fileDescriptor = fopen(file, "rb");
    if (fileDescriptor == NULL)
        throw std::runtime_error("¡No se pudo abrir el archivo!");

    this->errorMgr = std::make_shared<jpeg_error_mgr>();

    // Asignamos la estructura de los errores
    _decompress_struct.err = jpeg_std_error(this->errorMgr.get());

    // Abrimos
    jpeg_create_decompress(&_decompress_struct);
    jpeg_stdio_src(&_decompress_struct, fileDescriptor);

    int headerNumber = jpeg_read_header(&_decompress_struct, TRUE);
    if (headerNumber != 1)
        throw std::runtime_error("¡El archivo no es un JPEG normal!");

    jpeg_start_decompress(&_decompress_struct);

    this->width = _decompress_struct.output_width;
    this->height = _decompress_struct.output_height;
    this->pixelSize = _decompress_struct.output_components;
    this->colorSpace = _decompress_struct.out_color_space;

    unsigned int rowStride = this->width * this->pixelSize;

    this->bitmapData.clear();
    this->bitmapData.reserve(this->height);

    while (_decompress_struct.output_scanline < this->height)
    {
        std::vector<unsigned char> _vector(rowStride);
        unsigned char *ptr = _vector.data();

        jpeg_read_scanlines(&_decompress_struct, &ptr, 1);
        this->bitmapData.push_back(_vector);
    }

    jpeg_finish_decompress(&_decompress_struct);

    fclose(fileDescriptor);
}

imageJPEG::imageJPEG(const WebcamV4L2::bufferV4L2_t buffer)
{
    jpeg_decompress_struct _decompress_struct;

    this->errorMgr = std::make_shared<jpeg_error_mgr>();

    // Asignamos la estructura de los errores
    _decompress_struct.err = jpeg_std_error(this->errorMgr.get());

    // Abrimos
    jpeg_create_decompress(&_decompress_struct);
    jpeg_mem_src(&_decompress_struct, (unsigned char *)buffer.data, buffer.size);

    int headerNumber = jpeg_read_header(&_decompress_struct, TRUE);
    if (headerNumber != 1)
        throw std::runtime_error("¡El archivo no es un JPEG normal!");

    jpeg_start_decompress(&_decompress_struct);

    this->width = _decompress_struct.output_width;
    this->height = _decompress_struct.output_height;
    this->pixelSize = _decompress_struct.output_components;
    this->colorSpace = _decompress_struct.out_color_space;

    unsigned int rowStride = this->width * this->pixelSize;

    this->bitmapData.clear();
    this->bitmapData.reserve(this->height);

    while (_decompress_struct.output_scanline < this->height)
    {
        std::vector<unsigned char> _vector(rowStride);
        unsigned char *ptr = _vector.data();

        jpeg_read_scanlines(&_decompress_struct, &ptr, 1);
        this->bitmapData.push_back(_vector);
    }

    jpeg_finish_decompress(&_decompress_struct);
}

void imageJPEG::save(const char *file, const unsigned int quality) const
{
    jpeg_compress_struct _compress_struct;

    unsigned int _quality;

    if (quality > 100)
        _quality = 100;
    else
        _quality = quality;

    // Abrimos el archivo
    FILE *fileDescriptor = fopen(file, "wb");
    if (fileDescriptor == NULL)
        throw std::runtime_error("¡No se pudo abrir el archivo!");

    jpeg_create_compress(&_compress_struct);
    jpeg_stdio_dest(&_compress_struct, fileDescriptor);

    _compress_struct.image_width = this->width;
    _compress_struct.image_height = this->height;
    _compress_struct.input_components = this->pixelSize;
    _compress_struct.in_color_space = (J_COLOR_SPACE)this->colorSpace;

    _compress_struct.err = jpeg_std_error(this->errorMgr.get());

    jpeg_set_defaults(&_compress_struct);
    jpeg_set_quality(&_compress_struct, _quality, TRUE);

    jpeg_start_compress(&_compress_struct, TRUE);

    for (std::vector<unsigned char> _vector : this->bitmapData)
    {
        JSAMPROW row;
        row = _vector.data();

        jpeg_write_scanlines(&_compress_struct, &row, 1);
    }

    jpeg_finish_compress(&_compress_struct);

    fclose(fileDescriptor);
}

void imageJPEG::savePpm(const char *file) const
{
    std::ofstream _fstream(file, std::ios::out | std::ios::binary);
    if (!_fstream)
        throw std::runtime_error("¡No se pudo abrir el archivo!");

    _fstream << "P6 " << this->width << " " << this->height << " 255\n";
    for (std::vector<unsigned char> _vector : this->bitmapData)
        _fstream.write((const char *)_vector.data(), _vector.size());

    _fstream.close();
}

unsigned int imageJPEG::getHeight() const
{
    return this->height;
}

unsigned int imageJPEG::getWidth() const
{
    return this->width;
}

unsigned int imageJPEG::getPixelSize() const
{
    return this->pixelSize;
}

const std::vector<unsigned char> imageJPEG::getPixel(const unsigned int x, const unsigned int y) const
{
    if (y >= this->bitmapData.size())
        throw std::runtime_error("¡Valor de Y fuera de los límites!");

    if (x >= this->bitmapData[0].size() / this->pixelSize)
        throw std::runtime_error("¡Valor de X fuera de los límites!");

    std::vector<unsigned char> _vector;
    for (unsigned char n = 0; n < this->pixelSize; n++)
        _vector.push_back(this->bitmapData[y][x * this->pixelSize + n]);

    return _vector;
}

unsigned char imageJPEG::getLuminance(const unsigned int x, const unsigned int y) const
{
    std::vector<unsigned char> _vector = getPixel(x, y);

    if (_vector.size() == 1)
        return _vector[0];
    else if (_vector.size() == 3)
        return (_vector[0] * 2 + _vector[1] * 3 + _vector[2]) / 6;

    return 0;
}

const std::vector<unsigned char> imageJPEG::getAverage(const unsigned int x, const unsigned int y, const unsigned int boxSize) const
{
    unsigned int _x, _y, i, j;
    std::vector<unsigned int> _channels(3, 0);
    std::vector<unsigned char> _result(3, 0);

    if (boxSize > this->width)
        throw std::runtime_error("¡El tamaño de la caja es mayor que la anchura de la imagen!");
    if (boxSize > this->height)
        throw std::runtime_error("¡El tamaño de la caja es mayor que la altura de la imagen!");

    _x = x + boxSize >= this->width ? this->width - boxSize : x;
    _y = y + boxSize >= this->height ? this->height - boxSize : y;

    for (j = _y; j < _y + boxSize; j++)
        for (i = _x; i < _x + boxSize; i++)
        {
            std::vector<unsigned char> _vector = getPixel(x, y);

            _channels[0] += _vector[0];
            if (_vector.size() == 3)
            {
                _channels[1] += _vector[1];
                _channels[2] += _vector[2];
            }
        }

    _channels[0] /= boxSize * boxSize;
    _result[0] = _channels[0];
    if (this->pixelSize == 3)
    {
        _channels[1] /= boxSize * boxSize;
        _result[1] = _channels[1];
        _channels[2] /= boxSize * boxSize;
        _result[2] = _channels[2];
    }

    return _result;
}

/*
void imageJPEG::shrink(const unsigned int newWidth)
{
    unsigned int newHeight, i, j;
    float scaleFactor;

    if (newWidth == this->width)
        return;
    else if (newWidth == 0)
        throw std::runtime_error("¡El nuevo tamaño no puede ser 0!");
    else if (newWidth > this->width)
        throw std::runtime_error("¡El nuevo tamaño debe ser menor que el de la imagen!");

    scaleFactor = newWidth / this->width;
    newHeight = scaleFactor * this->height;

    std::vector<std::vector<unsigned char>> newBitmapData;
    newBitmapData.reserve(newHeight);

    std::vector<unsigned int> runningTotals(newWidth * this->pixelSize);
    std::vector<unsigned int> runningCounts(newWidth * this->pixelSize);

    unsigned int oldJ = 0;
    for (j = 0; j < this->height; j++)
    {
        for (i = 0; i < this->width * this->pixelSize; i++)
        {
            unsigned int index = i * scaleFactor;

            runningTotals[index] += this->bitmapData[j][i];
            runningCounts[index]++;
        }

        if ((unsigned int)(j * scaleFactor) > oldJ)
        {
            oldJ = j * scaleFactor;

            std::vector<unsigned char> newLine;
            newLine.reserve(newWidth * this->pixelSize);

            for (i = 0; i < newWidth * this->pixelSize; i++)
            {
                newLine.push_back(runningTotals[i] / runningCounts[i]);
                runningTotals[i] = 0;
                runningCounts[i] = 0;
            }

            newBitmapData.push_back(newLine);
        }
    }

    this->bitmapData = newBitmapData;
    this->height = this->bitmapData.size();
    this->width = this->bitmapData[0].size() / this->pixelSize;

    std::cout << "w: " << this->width << ", h: " << newBitmapData.size();
}
*/
/*
void imageJPEG::expand(const unsigned int newWidth)
{
    unsigned int newHeight, i, j, k;
    float scaleFactor;

    if (newWidth == this->width)
        return;
    else if (newWidth == 0)
        throw std::runtime_error("¡El nuevo tamaño no puede ser 0!");
    else if (newWidth < this->width)
        throw std::runtime_error("¡El nuevo tamaño debe ser mayor que el de la imagen!");

    scaleFactor = newWidth / this->width;
    newHeight = scaleFactor * this->height;

    std::vector<std::vector<unsigned char>> newBitmapData;
    newBitmapData.reserve(newHeight);

    for (j = 0; j < this->height; j++)
    {
        unsigned int oldJ = j / scaleFactor;

        std::vector<unsigned char> newLine;
        newLine.reserve(newWidth * this->pixelSize);

        for (i = 0; i < this->width * this->pixelSize; i++)
        {
            unsigned int oldI = i / scaleFactor;

            for (k = 0; k < this->pixelSize; k++)
                newLine[i * this->pixelSize + k] = this->bitmapData[oldJ][oldI * this->pixelSize + k];

            newBitmapData.push_back(newLine);
        }

        this->bitmapData = newBitmapData;
        this->height = this->bitmapData.size();
        this->width = this->bitmapData[0].size() / this->pixelSize;
    }
}
*/
/*
void imageJPEG::resize(const unsigned int newWidth)
{
    if (this->width > newWidth)
        shrink(newWidth);
    else if (this->width < newWidth)
        expand(newWidth);
}
*/
void imageJPEG::crop(const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h) {
    unsigned int i, j;

    if (w == 0)
        throw std::runtime_error("¡La anchura no puede ser 0!");
    else if (h == 0)
        throw std::runtime_error("¡La altura no puede ser 0!");

    std::vector<std::vector<unsigned char>> newBitmapData;
    newBitmapData.reserve(h);

    for (j = y; j < y + h && j < this->height; j++)
    {
        std::vector<unsigned char> newLine;
        newLine.reserve(w * this->pixelSize);

        for (i = x * this->pixelSize; i <= (x + w) * this->pixelSize && i < this->width * this->pixelSize; i++)
            newLine.push_back(this->bitmapData[j][i]);
        
        newBitmapData.push_back(newLine);
    }

    this->bitmapData = newBitmapData;
    this->height = this->bitmapData.size();
    this->width = this->bitmapData[0].size() / this->pixelSize;
}

void imageJPEG::drawLine(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int color)
{
    float i, j, hFactor, vFactor;
    unsigned int xMin, yMin, xMax, yMax, k;
    std::vector<unsigned int> _channels(3, 0);

    if (x1 < x2)
    {
        xMin = x1;
        xMax = x2;
    }
    else
    {
        xMin = x2;
        xMax = x1;
    }

    if (y1 < y2)
    {
        yMin = y1;
        yMax = y2;
    }
    else
    {
        yMin = y2;
        yMax = y1;
    }

    _channels[0] = (color >> 16) & 0xFF;
    _channels[1] = (color >> 8) & 0xFF;
    _channels[2] = (color)&0xFF;

    if (xMin == xMax || yMin == yMax)
    {
        hFactor = xMin == xMax ? 0 : 1;
        vFactor = yMin == yMax ? 0 : 1;
    }
    else if (xMax - xMin == yMax - yMin)
    {
        hFactor = 1;
        vFactor = 1;
    }
    else if (xMax - xMin > yMax - yMin)
    {
        hFactor = 1;
        vFactor = (float)(yMax - yMin) / (float)(xMax - xMin);
    }
    else
    {
        hFactor = (float)(xMax - xMin) / (float)(yMax - yMin);
        vFactor = 1;
    }

    for (i = xMin, j = yMin; i <= xMax && j <= yMax && i < this->width && j < this->height; i += hFactor, j += vFactor)
        for (k = 0; k < this->pixelSize; k++)
            this->bitmapData[j][i * this->pixelSize + k] = _channels[k];
}

void imageJPEG::drawRectangle(const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h, const unsigned int color)
{
    drawLine(x, y, x + w, y, color);
    drawLine(x, y, x, y + h, color);
    drawLine(x + w, y, x + w, y + h, color);
    drawLine(x, y + h, x + w, y + h, color);
}