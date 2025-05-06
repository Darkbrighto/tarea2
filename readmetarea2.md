(readme absolutamente básico, pendiente para mejora.)

🎵 Spotifind
Spotifind es una aplicación en C que permite gestionar un catálogo de canciones cargado desde un archivo CSV. Los usuarios pueden buscar canciones por género, artista o tempo, y también crear listas de reproducción personalizadas (en desarrollo).

✅ Funcionalidades Implementadas
📂 Cargar canciones desde un archivo CSV (song_dataset_.csv).

🎧 Buscar canciones por género.

👩‍🎤 Buscar canciones por artista.

🕒 Buscar canciones por rango de tempo (lento, moderado, rápido).

📜 Mostrar información detallada de cada canción.

📦 Estructuras eficientes con Map y List para búsquedas rápidas.

🔧 Funcionalidades Planeadas (Por implementar)
📑 Crear listas de reproducción personalizadas.

➕ Agregar canciones a listas.

📃 Mostrar canciones dentro de una lista.

📁 Estructura del CSV
El archivo CSV debe tener las siguientes columnas (relevantes):

ID (índice 0)

Artistas (índice 2, separados por ;)

Álbum (índice 3)

Nombre de la canción (índice 4)

Tempo (índice 18)

Género (índice 20)

🚀 Cómo ejecutar
Compila el proyecto:

bash
Copiar
Editar
gcc -o spotifind main.c tdas/*.c
Asegúrate de tener el archivo song_dataset_.csv en la misma carpeta.

Ejecuta el programa:

bash
Copiar
Editar
./spotifind
🗃 Requisitos
Compilador de C (GCC recomendado)

Archivos del proyecto:

main.c

tdas/list.h, tdas/map.h, tdas/extra.h (y sus implementaciones)

Archivo CSV: song_dataset_.csv