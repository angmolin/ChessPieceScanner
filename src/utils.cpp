#include "utils.h"

bool sendCommand(SerialPort &serial, const char *command)
{
    char result[MAX_MESSAGE_LENGTH];
    int sendResult, recvResult;

    try
    {
        sendResult = serial.send(command, strlen(command) + 1);
        recvResult = serial.recv(result, sizeof(result));
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("¡No se pudo establecer una comunicación con el escáner!");
    }

    return sendResult >= 0 && recvResult >= 0;
}

bool moveJ(SerialPort &serial, unsigned int degree)
{
    char command[MAX_MESSAGE_LENGTH];
    int command1Result, command2Result;

    snprintf(command, MAX_MESSAGE_LENGTH, "cps201j%i", degree);
    command1Result = sendCommand(serial, command);
    command2Result = sendCommand(serial, "cps200j");

    return command1Result && command2Result;
}

int readJ(SerialPort &serial)
{
#ifndef DEBUG
    char command[MAX_MESSAGE_LENGTH] = "cps300j";
    int result = 0;

    serial.send(command, strlen(command) + 1);
    serial.recv(command, sizeof(command));

    if (sscanf(command, "Return: %i", &result) == 1)
    {
        return result;
    }
    else
        throw std::runtime_error("¡No se pudo obtener la posicion J del escáner!");
#else
    return 0;
#endif
}

bool moveY(SerialPort &serial, unsigned int degree)
{
    char command[MAX_MESSAGE_LENGTH];

    snprintf(command, MAX_MESSAGE_LENGTH, "cps201y%i", degree);
    return sendCommand(serial, command);
}

int readY(SerialPort &serial)
{
#ifndef DEBUG
    char command[MAX_MESSAGE_LENGTH] = "cps300y";
    int result = 0;

    serial.send(command, strlen(command) + 1);
    serial.recv(command, sizeof(command));

    if (sscanf(command, "Return: %i", &result) == 1)
    {
        return result;
    }
    else
        throw std::runtime_error("¡No se pudo obtener la posicion Y del escáner!");
#else
    return 0;
#endif
}

bool moveZ(SerialPort &serial, unsigned int degree)
{
    char command[MAX_MESSAGE_LENGTH];

    snprintf(command, MAX_MESSAGE_LENGTH, "cps201z%i", degree);
    return sendCommand(serial, command);
}

int readZ(SerialPort &serial)
{
#ifndef DEBUG
    char command[MAX_MESSAGE_LENGTH] = "cps300z";
    int result = 0;

    serial.send(command, strlen(command) + 1);
    serial.recv(command, sizeof(command));

    if (sscanf(command, "Return: %i", &result) == 1)
    {
        return result;
    }
    else
        throw std::runtime_error("¡No se pudo obtener la posicion Z del escáner!");
#else
    return 0;
#endif
}

bool calibrate(SerialPort &serial)
{
    int command1Result, command2Result, command3Result;

    command1Result = sendCommand(serial, "cps101y");
    sleep(1); // Esperar 1s
    command2Result = sendCommand(serial, "cps101z");
    sleep(1); // Esperar 1s
    command3Result = moveJ(serial, 40);
    sleep(1); // Esperar 1s

    return command1Result && command2Result && command3Result;
}

bool liberate(SerialPort &serial)
{
    int command1Result, command2Result, command3Result, command4Result, command5Result, command6Result;

    command1Result = moveZ(serial, 0);
    command2Result = moveY(serial, 0);
    command3Result = moveJ(serial, 120);

    command4Result = sendCommand(serial, "cps200j");
    command5Result = sendCommand(serial, "cps200y");
    command6Result = sendCommand(serial, "cps200z");

    return command1Result && command2Result && command3Result && command4Result && command5Result && command6Result;
}

int saveImage(const char *name, WebcamV4L2::bufferV4L2_t buffer)
{
    int result = 0;

    int imageFile = open(name, O_WRONLY | O_CREAT, 0660);
    if (imageFile < 0)
        return imageFile;

    result = write(imageFile, buffer.data, buffer.size);
    close(imageFile);

    return result;
}

Terminal::key_t menu(const TerminalTable &menuTable, const Terminal::key_t minKey, const Terminal::key_t maxKey)
{
    std::wcout << "\n";
    std::wcout << menuTable;
    std::wcout << "\n";
    std::wcout << "> " << std::flush;
    saveCursor();

    Terminal::key_t key = defaultTerminal.getKey();

    while (key < minKey || key > maxKey)
    {
        restoreCursor();
        eraseDisplay();

        cursorDown();
        cursorLineStart();

        if (key == maxKey + 1)
            std::wcout << (wchar_t *)inicializationValues << "\n";
        else
            std::wcout << L"Opción inválida.\n";

        restoreCursor();
        key = defaultTerminal.getKey();
    }

    return key;
}

void mainMenu(SerialPort &serial, WebcamV4L2 &webcamV4L2)
{
    loadConfigWebcam(webcamV4L2);

    TerminalTable table({
        {L"", L"Menú principal"},
        {L"a", L"Asistente de escáner de piezas"},
        {L"b", L"Control manual del dispositivo"},
        {L"c", L"Control por comandos"},
        {L"d", L"Salir de la aplicación"},
    });

    Terminal::key_t key;

    do
    {
        key = menu(table, Terminal::KEY_A, Terminal::KEY_D);

        moveCursor(0, 0);
        eraseDisplay();

        if (key == Terminal::KEY_A)
        {
            photoBooth(serial, webcamV4L2);
        }
        else if (key == Terminal::KEY_B)
        {
            manualMenu(serial, webcamV4L2);
        }
        else if (key == Terminal::KEY_C)
        {
            expertMenu(serial, webcamV4L2);
        }

        moveCursor(0, 0);
        eraseDisplay();
    } while (key != Terminal::KEY_D);
}

void photoBooth(SerialPort &serial, WebcamV4L2 &webcamV4L2)
{
    // Parámetros de las imágenes
    unsigned int maxZ;
    unsigned int stepZ;
    unsigned int i, z;

    wchar_t jChar[5];
    wchar_t yChar[5];
    wchar_t zChar[5];

    loadConfigPhotoBooth();

    TerminalTable table({{L"", L"Asistente de escáner de piezas"},
                         {L"a", L"Test de escaneo"},
                         {L"b", L"Escanear 180º"},
                         {L"c", L"Escanear 360º"},
                         {L"d", L"Introducir parámetros manualmente"}});

    Terminal::key_t key = menu(table, Terminal::KEY_A, Terminal::KEY_C);

    maxZ = 350;
    stepZ = 10;

    if (key == Terminal::KEY_A)
    {
        maxZ = 0;
    }
    else if (key == Terminal::KEY_B)
    {
        maxZ = 170;
    }
    else if (key == Terminal::KEY_C)
    {
        maxZ = 350;
    }

    // Ruta completa
    char path[MAX_PATH_LENGTH + 8] = "photos/";
    unsigned int pathLength;

    // Nombre de la carpeta
    wchar_t wfolderName[MAX_NAME_LENGTH];
    char folderName[MAX_NAME_LENGTH];

    // Nombre de la imagen
    char photoName[MAX_NAME_LENGTH];

    // Obtenemos el nombre de la pieza que vamos a escanear
    std::wcout << L"Nombre de la pieza a escanear: " << std::flush;
    std::wcin.sync();
    std::wcin >> wfolderName;

    wcstombs(folderName, wfolderName, MAX_NAME_LENGTH);

    strcpy(&path[strlen(path)], folderName);

    moveCursor(0, 0);
    eraseDisplay();

    // Creamos el directorio
    struct stat directory;

    if (stat(path, &directory) == 0)
        std::wcout << L"El directorio '" << path << "' ya existe\n";
    else
    {
        std::wcout << L"Creando el directorio '" << path << "'..." << std::flush;
        if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0)
        {
            std::wcout << "ERROR!\n";
            return;
        }
        std::wcout << "OK!\n";
    }

    saveCursor();
    pathLength = strlen(path);

    for (i = 0; i < photoBoothPhotos.size(); i++)
    {
        moveJ(serial, photoBoothPhotos[i].coordinate.j);
        moveY(serial, photoBoothPhotos[i].coordinate.y);

        for (z = 0; z <= maxZ; z += stepZ)
        {
            moveZ(serial, z);
            sleep(1);

            restoreCursor();

            swprintf(jChar, sizeof(jChar), L"%03u", (unsigned int)photoBoothPhotos[i].coordinate.j);
            swprintf(yChar, sizeof(yChar), L"%03u", (unsigned int)photoBoothPhotos[i].coordinate.y);
            swprintf(zChar, sizeof(zChar), L"%03u", (unsigned int)z);

            TerminalTable statusTable({{wfolderName, L"J", L"Y", L"Z"},
                                       {L"Coor. act.", jChar, yChar, zChar}});

            std::wcout << statusTable << std::flush;

            snprintf(photoName, sizeof(photoName), "/y%03uz%03u.jpg", (unsigned int)photoBoothPhotos[i].coordinate.y, (unsigned int)z);
            strcpy(&path[pathLength], photoName);

            struct stat file;

            if (stat(path, &file) == 0)
            {
                std::wcout << L"El archivo '" << path << "' ya existe, sobreescribiendo..." << std::flush;

                if (unlink(path) < 0)
                {
                    std::wcout << "ERROR!\n";
                    return;
                }
                std::wcout << "OK!\n";
            }
            imageJPEG image(webcamV4L2.captureFrame());

            if (photoBoothCropEnabled)
            {
                image.crop(
                    photoBoothPhotos[i].crop.x - photoBoothCropMargin.x,
                    photoBoothPhotos[i].crop.y - photoBoothCropMargin.y,
                    photoBoothPhotos[i].crop.w + photoBoothCropMargin.x * 2,
                    photoBoothPhotos[i].crop.h + photoBoothCropMargin.y * 2);
            }

            image.save(path, photoBoothQuality);

            beep();

            sleep(1);
        }

        moveZ(serial, 0);
        sleep(maxZ * 0.015);
    }

    restoreCursor();
    eraseDisplay();
    std::wcout << L"¡Finalizado!\n"
               << std::flush;

    moveJ(serial, 30);
    sleep(1);
    moveY(serial, 0);
    sleep(1);
    moveZ(serial, 0);
    sleep(1);
}

void manualMenu(SerialPort &serial, WebcamV4L2 &webcamV4L2)
{
    const char *previewFile = "photos/preview.jpg";

    int step = 5;
    int jSteps = 0;
    int ySteps = 0;
    int zSteps = 0;

    wchar_t stepChar[4];
    wchar_t jStepsChar[4];
    wchar_t yStepsChar[4];
    wchar_t zStepsChar[4];

    try
    {
        std::wcout << L"Obteniendo posición del eje J..." << std::flush;
        jSteps = readJ(serial);
        std::wcout << L"OK!\n";

        std::wcout << L"Obteniendo posición del eje Y..." << std::flush;
        ySteps = readY(serial);
        std::wcout << L"OK!\n";

        std::wcout << L"Obteniendo posición del eje Z..." << std::flush;
        zSteps = readZ(serial);
        std::wcout << L"OK!\n";
    }
    catch (const std::exception &e)
    {
        std::wcout << "ERROR!\n";
        return;
    }

    TerminalTable table({
        {L"", L"Control manual del dispositivo"},
        {L"↑", L"Rotar el eje Y hacia arriba"},
        {L"↓", L"Rotar el eje Y hacia abajo"},
        {L"→", L"Rotar el eje Z hacia la derecha"},
        {L"←", L"Rotar el eje Z hacia la izquierda"},
        {L"W", L"Rotar el eje J hacia la arriba"},
        {L"S", L"Rotar el eje J hacia la abajo"},
        {L"P", L"Hacer una fotografia"},
        {L"E", L"Salir del modo manual"},
    });

    std::wcout << "\n";
    std::wcout << table;

    defaultTerminal.disableEcho(Terminal::IN);

    Terminal::key_t key;

    saveCursor();

    do
    {
        swprintf(stepChar, sizeof(stepChar), L"%03u", step);
        swprintf(jStepsChar, sizeof(jStepsChar), L"%03u", jSteps);
        swprintf(yStepsChar, sizeof(yStepsChar), L"%03u", ySteps);
        swprintf(zStepsChar, sizeof(zStepsChar), L"%03u", zSteps);

        TerminalTable statusTable({{L"", L"Pos"},
                                   {L"Eje J", jStepsChar},
                                   {L"Eje Y", yStepsChar},
                                   {L"Eje Z", zStepsChar},
                                   {L"Paso +/-", stepChar}});
        moveCursor(0, 5);

        statusTable.setLeftPadding(48);
        std::wcout << statusTable;

        restoreCursor();

        key = defaultTerminal.getKey();

        if (key == Terminal::KEY_UP)
        {
            ySteps = min(ySteps + step, 90);
            moveY(serial, ySteps);
        }
        else if (key == Terminal::KEY_DOWN)
        {
            ySteps = max(ySteps - step, 0);
            moveY(serial, ySteps);
        }
        else if (key == Terminal::KEY_RIGHT)
        {
            zSteps = (zSteps + step) % 360;
            moveZ(serial, zSteps);
        }
        else if (key == Terminal::KEY_LEFT)
        {
            zSteps = (zSteps - step) % 360;

            if (zSteps < 0)
                zSteps = 360 - abs(zSteps);
            moveZ(serial, zSteps);
        }
        else if (key == Terminal::KEY_W)
        {
            jSteps = min(jSteps + step, 120);
            moveJ(serial, jSteps);
        }
        else if (key == Terminal::KEY_S)
        {
            jSteps = max(jSteps - step, 0);
            moveJ(serial, jSteps);
        }
        else if (key == Terminal::KEY_P)
        {
            WebcamV4L2::bufferV4L2_t buffer = webcamV4L2.captureFrame();

            saveImage(previewFile, buffer);
        }
        else if (key == Terminal::KEY_ADD)
        {
            if (step == 1)
                step = 5;
            else if (step == 5)
                step = 10;
        }
        else if (key == Terminal::KEY_SUB)
        {
            if (step == 10)
                step = 5;
            else if (step == 5)
                step = 1;
        }
    } while (key != Terminal::KEY_E);

    defaultTerminal.restoreDefaults(Terminal::IN);

    return;
}

void expertMenu(SerialPort &serial, WebcamV4L2 &webcamV4L2)
{
    wchar_t wcommand[MAX_MESSAGE_LENGTH];
    char command[MAX_MESSAGE_LENGTH];

    TerminalTable table({
        {L"", L"Control por comandos"},
        {L"000 ~ 099", L"Información"},
        {L"100 ~ 199", L"Calibración"},
        {L"200 ~ 299", L"Función"},
        {L"300 ~ 399", L"Interrogación"},
        {L"400 ~ 499", L"Configuración"},
        {L"exit", L"Salir"},
    });

    std::wcout << "\n";
    std::wcout << table;
    std::wcout << "\n";

    std::wcout << L"cps> " << std::flush;
    saveCursor();
    std::wcin.sync();
    std::wcin >> wcommand;

    while (wcscmp(wcommand, L"exit") != 0)
    {
        wcstombs(command, wcommand, MAX_MESSAGE_LENGTH);

        serial.send(command, strlen(command) + 1);
        serial.recv(command, sizeof(command));

        restoreCursor();
        eraseDisplay();

        cursorDown();
        cursorLineStart();

        std::wcout << command;

        restoreCursor();
        std::wcin.sync();
        std::wcin >> wcommand;
    }

    return;
}

void loadConfigWebcam(WebcamV4L2 &webcamV4L2)
{
    Configuration config(CONFIG_FILE_WEBCAM);
    __s32 value;

    while (config.next())
    {
        if (sscanf(config.getline(), "auto_exposure: %i", &value) == 1)
        {
            webcamV4L2.configure_auto_exposure(value);
        }
        else if (sscanf(config.getline(), "auto_white_balance: %i", &value) == 1)
        {
            webcamV4L2.configure_auto_white_balance(value);
        }
        else if (sscanf(config.getline(), "backlight_compensation: %i", &value) == 1)
        {
            webcamV4L2.configure_backlight_compensation(value);
        }
        else if (sscanf(config.getline(), "brightness: %i", &value) == 1)
        {
            webcamV4L2.configure_brightness(value);
        }
        else if (sscanf(config.getline(), "contrast: %i", &value) == 1)
        {
            webcamV4L2.configure_contrast(value);
        }
        else if (sscanf(config.getline(), "exposure_time: %i", &value) == 1)
        {
            webcamV4L2.configure_exposure_time(value);
        }
        else if (sscanf(config.getline(), "gain: %i", &value) == 1)
        {
            webcamV4L2.configure_gain(value);
        }
        else if (sscanf(config.getline(), "gamma_correction: %i", &value) == 1)
        {
            webcamV4L2.configure_gamma_correction(value);
        }
        else if (sscanf(config.getline(), "hue: %i", &value) == 1)
        {
            webcamV4L2.configure_hue(value);
        }
        else if (sscanf(config.getline(), "power_line_frequency: %i", &value) == 1)
        {
            webcamV4L2.configure_power_line_frequency(value);
        }
        else if (sscanf(config.getline(), "saturation: %i", &value) == 1)
        {
            webcamV4L2.configure_saturation(value);
        }
        else if (sscanf(config.getline(), "sharpness: %i", &value) == 1)
        {
            webcamV4L2.configure_sharpness(value);
        }
        else if (sscanf(config.getline(), "white_balance_temperature: %i", &value) == 1)
        {
            webcamV4L2.configure_white_balance_temperature(value);
        }
        else if (sscanf(config.getline(), "vertical_flip: %i", &value) == 1)
        {
            webcamV4L2.configure_vertical_flip(value);
        }
        else if (sscanf(config.getline(), "horizontal_flip: %i", &value) == 1)
        {
            webcamV4L2.configure_horizontal_flip(value);
        }
    }
}

void loadConfigPhotoBooth()
{
    Configuration config(CONFIG_FILE_PHOTOBOOTH);
    unsigned int i, j, x, y, w, h;
    photoBoothPhoto_t photo;
    unsigned int value;

    photoBoothPhotos.clear();
    photoBoothCropMargin = {0, 0};
    photoBoothCropEnabled = false;

    while (config.next())
    {
        if (sscanf(config.getline(), "image_quality: %u", &value) == 1)
        {
            photoBoothQuality = value;
        }
        else if (sscanf(config.getline(), "crop_enabled: %u", &value) == 1)
        {
            photoBoothCropEnabled = value == 0 ? false : true;
        }
        else if (sscanf(config.getline(), "crop_margin: %u %u", &x, &y) == 2)
        {
            photoBoothCropMargin = {x, y};
        }
        else if (sscanf(config.getline(), "image%u: %u %u", &i, &y, &j) == 3)
        {
            photo.coordinate.y = y;
            photo.coordinate.j = j;

            if (!photoBoothCropEnabled)
            {
                photoBoothPhotos.push_back(photo);
            }
        }
        else if (sscanf(config.getline(), "crop%u: %u %u %u %u", &i, &x, &y, &w, &h) == 5)
        {
            photo.crop.x = x;
            photo.crop.y = y;
            photo.crop.w = w;
            photo.crop.h = h;

            if (photoBoothCropEnabled)
            {
                photoBoothPhotos.push_back(photo);
            }
        }
    }
}