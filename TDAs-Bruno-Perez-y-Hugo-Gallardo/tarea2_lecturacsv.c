#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  int id;
  char* nombre;
  List* artistas;
  char* album;
  char* genero;
  float tempo;
} song_type;


/*
1. **Crear Lista de Reproducción:** La usuaria ingresa un nombre para la nueva lista; la aplicación crea una colección vacía que luego podrás llenar.
2. **Agregar Canción a Lista:** La usuaria ingresa el ID de la canción y el nombre de la lista de reproducción; la aplicación añade la canción seleccionada a esa lista.
3. **Mostrar Canciones de una Lista:** La usuaria ingresa el nombre de la lista y la aplicación despliega todas las canciones que contiene con la información de cada una.
*/

/**
 * Carga canciones desde un archivo CSV
 */

Map* canciones_byid;
Map* canciones_bygenero;
Map* canciones_byartista;
Map* canciones_bytempo;

song_type* crear_cancion(int id, char* nombre, List* artistas, char* album, char* genero, float tempo)
{
  song_type* cancion = (song_type*)malloc(sizeof(song_type));
  cancion->id = id;
  cancion->nombre = strdup(nombre);
  cancion->artistas = artistas;
  cancion->album = strdup(album);
  cancion->genero = strdup(genero);
  cancion->tempo = tempo;
  return cancion;
}

int compare_strings(void* key1, void* key2) {
  return strcmp((char*)key1, (char*)key2) < 0;
}

int compare_ids(void* key1, void* key2) {
  int id1 = *((int*)key1);
  int id2 = *((int*)key2);
  return id1 < id2;
}


char* get_tempo_range(float tempo)
{
  if (tempo < 80) return "Lento";
  else if (tempo < 120) return "Moderado";
  else return "Rápido";
}

void print_song(song_type* cancion)
{
  printf("ID: %d\n", cancion->id);
  printf("Nombre: %s\n", cancion->nombre);
  printf("Artistas: \n");
  for(char *artista = list_first(cancion->artistas); artista != NULL; artista = list_next(cancion->artistas))
    {
      printf("  - %s\n", artista);
    }
  printf("Álbum: %s\n", cancion->album);
  printf("Género: %s\n", cancion->genero);
  printf("Tempo: %.2f\n", cancion->tempo);
  printf("--------------------------------\n");
}

void Impulse_canciones(const char* filename)
{
  canciones_byid = sorted_map_create(compare_ids);
  canciones_bygenero = sorted_map_create(compare_strings);
  canciones_byartista = sorted_map_create(compare_strings);
  canciones_bytempo = sorted_map_create(compare_strings);

  FILE* archivo = fopen(filename, "r");
  if (archivo == NULL){
    perror("Error al abrir el archivo");
    return;
  }
  
  char **campos = leer_linea_csv(archivo, ',');
  int cuenta = 0;
  while ((campos = leer_linea_csv(archivo, ',')) != NULL)
    {
      
      cuenta++;
      if (cuenta == 1000) break;
      int id = atoi(campos[0]);
      char* track_name = campos[4];
      List* artistas = split_string(campos[2], ";");
      char* album = campos[3];
      char* genero = campos[20];
      float tempo = atof(campos[18]);
      song_type* cancion = crear_cancion(id, track_name, artistas, album, genero, tempo);
      printf("%s\n",track_name);

      int* id_ptr = malloc(sizeof(int));
      *id_ptr = id;
      map_insert(canciones_byid, id_ptr, cancion);
      multimap_insert(canciones_bygenero, strdup(genero), cancion);
      for(char *artista = list_first(artistas); artista != NULL; artista = list_next(artistas))
        {
          multimap_insert(canciones_byartista, strdup(artista), cancion);
        }
      multimap_insert(canciones_bytempo, strdup(get_tempo_range(tempo)), cancion);

    }
  fclose(archivo);
  printf("Se cargaron %d canciones\n", cuenta);
}

void buscar_por_genero()
{
  char genero[100];
  printf("Ingrese el género: ");
  scanf(" %99[^\n]", genero);
  printf("\nCanciones del género '%s':\n\n", genero);
  int encontradas = 0;
  MapPair* pair = map_search(canciones_bygenero, genero);
  while (pair != NULL){
    if (strcmp((char*)pair->key, genero) == 0){
      song_type* cancion = (song_type*)pair->value;
      print_song(cancion);
      encontradas++;
      pair = map_next(canciones_bygenero);
    }
    else{
      break;
    }
  }
  if (encontradas == 0){
    printf("No se encontraron canciones del género '%s'\n", genero);
  } else {
    printf("Se encontraron %d canciones del género '%s'\n", encontradas, genero);
  }
  
}

void buscar_por_artista()
{
  char artista[100];
  printf("Ingrese el artista: ");
  scanf(" %99[^\n]", artista);
  printf("\nCanciones del artista '%s':\n\n", artista);
  int encontradas = 0;
  MapPair* pair = map_search(canciones_byartista, artista);
  while (pair != NULL)
    {
      if (strcmp((char*)pair->key, artista) == 0){
        song_type* cancion = (song_type*)pair->value;
        print_song(cancion);
        encontradas++;
        pair = map_next(canciones_byartista);
      }
      else{
        break;
      }
    }
  if (encontradas == 0){
    printf("No se encontraron canciones del artista '%s'\n", artista);
    } else{
      printf("Se encontraron %d canciones del artista '%s'\n", encontradas, artista);
    }
  }


void display_menu()
{
  int opcion = 0;
  while (opcion != 8)
    {
      limpiarPantalla();
      printf("<===><===> SPOTIFIND <===><===>\n");
      printf("1. Cargar canciones\n");
      printf("2. Buscar por Género\n");
      printf("3. Buscar por Artista\n");
      printf("4. Buscar por Tempo\n");
      printf("5. Crear lista de reproducción\n");
      printf("6. Agregar canción a lista\n");
      printf("7. Mostrar canciones de lista\n");
      printf("8. Salir\n");
      printf("Ingrese una opción: ");
      scanf("%d", &opcion);
      switch (opcion){
        case 1:
          Impulse_canciones("data/song_dataset_.csv");
          break;
        
        case 2:
          if (canciones_bygenero == NULL){
            printf("Primero debe cargar las canciones\n");
          }
          else {
            buscar_por_genero();
          }
          break;
        
        case 3:
          if (canciones_byartista == NULL){
            printf("Primero debe cargar las canciones\n");
          }
          else {
            buscar_por_artista();
          }
          break;
        
        case 4:
            //buscar_por_tempo(lista); 
          break;
        
        case 5:
            //crear_lista_reproduccion(lista);
          break;
        
        case 6:
            //agregar_cancion_a_lista(lista);
          break;
        
        case 7:
            //mostrar_canciones_de_lista(lista);
          break;
        
        case 8:
          printf("Saliendo del Spotifind...\n");
          
          break;
        }
      presioneTeclaParaContinuar();
    }
}

int main() { 
  canciones_byid = NULL;
  canciones_bygenero = NULL;
  canciones_byartista = NULL;
  canciones_bytempo = NULL;
  display_menu();

  
  return 0;
}
