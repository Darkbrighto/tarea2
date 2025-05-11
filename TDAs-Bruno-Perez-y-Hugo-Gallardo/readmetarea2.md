# Spotifind

## Descripción

**Spotifind** es una aplicación desarrollada en lenguaje C que permite gestionar un catálogo de canciones cargadas desde un archivo CSV. A través de una interfaz por consola, los usuarios pueden buscar canciones por género, artista o tempo, y explorar información detallada de cada una. Actualmente se está desarrollando una funcionalidad adicional para crear listas de reproducción personalizadas.

## Cómo compilar y ejecutar

Este sistema puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con un compilador de C como **GCC**. Para trabajar con la aplicación en tu entorno local, sigue los siguientes pasos:

### Requisitos previos:

* Tener instalado [Visual Studio Code](https://code.visualstudio.com/)
* Instalar la extensión **C/C++** (Microsoft)
* Contar con un compilador de C (como **gcc**). En Windows se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar [WSL](https://learn.microsoft.com/en-us/windows/wsl/)

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** proyecto en una carpeta de tu elección.

2. **Abre el proyecto en Visual Studio Code**

   * Abre VS Code.
   * Selecciona `Archivo > Abrir carpeta...` y elige la carpeta del proyecto.

3. **Compila el código**

   * Abre el archivo `tarea2_lecturacsv.c`.
   * Abre la terminal integrada (`Terminal > Nueva terminal`).
   * Ejecuta el siguiente comando para compilar el programa:

     ```bash
     gcc tarea2_lecturacsv.c tdas/*c
     ```

4. **Ejecuta el programa**

   * Asegúrate de que el archivo `song_dataset_.csv` esté en la misma carpeta.
   * Ejecuta el programa con:

     ``
     ./a.exe
     ```

## Funcionalidades

### Funcionando correctamente:

* 📂 Cargar canciones desde un archivo CSV (`song_dataset_.csv`)
* 🎧 Buscar canciones por género
* 👩‍🎤 Buscar canciones por artista
* 🕒 Buscar canciones por rango de tempo (lento, moderado, rápido)
* 📜 Mostrar información detallada de una canción
* 📦 Uso de estructuras eficientes (`Map`, `List`) para una búsqueda optimizada

### Problemas conocidos:

* No se detectan errores de ejecución críticos hasta ahora, pero puede haber errores en la validación de entradas del usuario.

### A mejorar:

* Mejorar manejo de errores en búsquedas sin resultados.
* Validación más robusta del archivo CSV.

### Funcionalidades planeadas:

* 📑 Crear listas de reproducción personalizadas
* ➕ Agregar canciones a listas
* 📃 Mostrar canciones dentro de una lista de reproducción

## Formato del archivo CSV

El archivo `song_dataset_.csv` debe contener, como mínimo, las siguientes columnas:

* `ID` (índice 0)
* `Artistas` (índice 2, múltiples artistas separados por `;`)
* `Álbum` (índice 3)
* `Nombre de la canción` (índice 4)
* `Tempo` (índice 18)
* `Género` (índice 20)

**Nota:** Asegúrate de que los índices y nombres de columnas coincidan exactamente con el formato del CSV esperado por el programa.

## Ejemplo de uso

**Paso 1: Cargar canciones desde el archivo CSV**

```
Opción seleccionada: 1) Cargar canciones
Canciones cargadas exitosamente desde song_dataset_.csv
```

**Paso 2: Buscar por artista**

```
Opción seleccionada: 2) Buscar canciones por artista
Ingrese el nombre del artista: Queen
- Bohemian Rhapsody
- Don't Stop Me Now
- Another One Bites The Dust
```

**Paso 3: Buscar por tempo**

```
Opción seleccionada: 3) Buscar canciones por tempo
Seleccione el rango de tempo (Lento, Moderado, Rápido): Rápido
- Song A (135 BPM)
- Song B (142 BPM)
```

**Paso 4: Ver detalles de una canción**

```
Opción seleccionada: 4) Mostrar información detallada
Ingrese el nombre de la canción: Bohemian Rhapsody
Artista: Queen
Álbum: A Night at the Opera
Género: Rock
Tempo: 144 BPM
```

## Contribuciones (solo para tareas grupales)

### Nombre del integrante BRUNO PERÉZ:

* Implementación de la carga y búsqueda de canciones por artista, género y tempo
* Implementación de la eliminación de canciónes y creación de listas de reproducción
* Correción de errores y reducir tiempos de carga
* **Auto-evaluación**: 3 (Aporte excelente)

### Nombre del integrante HUGO GALLARDO:

* Comentarios en la aplicación y explicar el uso de cada función.
* Creación del readme y del repositorio de github.
* Implementación en función de leer (elegir cuanto)
* Get tempo range y incluir treemap
* **Auto-evaluación**: 3 (Aporte excelente)
