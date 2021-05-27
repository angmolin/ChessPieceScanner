# ChessPieceScanner
## Introducción
Escáner de piezas de ajedrez semiatomático para el entrenamiento de redes neuronales.

## Compilación
El código fuente hace uso de la librería V4L2 para el manejo de la webcam y la librería JPEG para el postprocesado de las imágenes, que pueden ser instaladas en un sistema Linux ejecutando `sudo apt-get install l4vl libjpeg`. Después clonamos el repositorio con el comando `git clone https://github.com/angmolin/ChessPieceScanner.git` Para compilar el proyecto nos posicionamos en la carpeta raíz del proyecto con `cd ./chesspiecescanner` y ejecutamos `make`.

## Ejecución
Para iniciar la aplicación ejecutamos `bin/chesspiecescanner <fichero de la webcam> <fichero del puerto serie>`.
