#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/treemap.h"
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

typedef struct {
  char* nombre;
  List* canciones;
} playlist_type;

typedef struct {
  TreeMap* canciones_byid;
  TreeMap* canciones_bygenero;
  TreeMap* canciones_byartista;
  TreeMap* canciones_bytempo;
  List* playlists;
} SpotifindApp;

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

playlist_type* crear_playlist(char* nombre)
{
  playlist_type* playlist = (playlist_type*)malloc(sizeof(playlist_type));
  playlist->nombre = strdup(nombre);
  playlist->canciones = list_create();
  return playlist;
}

int compare_strings(void* key1, void* key2) {
  if(strcmp((char*)key1, (char*)key2) < 0) return 1;
  return 0;
}

int compare_ids(void* key1, void* key2) {
  int id1 = *((int*)key1);
  int id2 = *((int*)key2);
  if(id1 < id2) return 1;
  return 0;
}

int compare_strings_equal(void* key1, void* key2) {
  return strcmp((char*)key1, (char*)key2) == 0;
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

SpotifindApp* initializeApp() {
  SpotifindApp* app = (SpotifindApp*)malloc(sizeof(SpotifindApp));
  app->canciones_byid = createTreeMap(compare_ids);
  app->canciones_bygenero = createTreeMap(compare_strings);
  app->canciones_byartista = createTreeMap(compare_strings);
  app->canciones_bytempo = createTreeMap(compare_strings);
  app->playlists = list_create();
  return app;
}

void Impulse_canciones(SpotifindApp* app, const char* filename)
{
  FILE* archivo = fopen(filename, "r");
  if (archivo == NULL){
    perror("Error al abrir el archivo");
    return;
  }

  int maximo_a_leer = 0;
  printf("Ingrese el número máximo de canciones a leer (0 para leer todas): ");
  scanf("%d", &maximo_a_leer);
  if (maximo_a_leer < 0){
    printf("Número inválido. Leyendo todas las canciones.\n");
    maximo_a_leer = 0;
  }
  

  char **campos = leer_linea_csv(archivo, ',');
  int cuenta = 0;

  while ((campos = leer_linea_csv(archivo, ',')) != NULL && (maximo_a_leer == 0 || cuenta < maximo_a_leer))
  {
    cuenta++;
    int id = atoi(campos[0]);
    char* track_name = campos[4];
    List* artistas = split_string(campos[2], ";");
    char* album = campos[3];
    char* genero = campos[20];
    float tempo = atof(campos[18]);
    song_type* cancion = crear_cancion(id, track_name, artistas, album, genero, tempo);
    printf("%s\n", track_name);

    int* id_ptr = malloc(sizeof(int));
    *id_ptr = id;
    insertTreeMap(app->canciones_byid, id_ptr, cancion);
    Pair* genero_pair = searchTreeMap(app->canciones_bygenero, genero);
    if (genero_pair == NULL) {
      List* canciones_genero = list_create();
      list_pushBack(canciones_genero, cancion);
      insertTreeMap(app->canciones_bygenero, strdup(genero), canciones_genero);
    } else {
      List* canciones_genero = (List*)genero_pair->value;
      list_pushBack(canciones_genero, cancion);
    }
    
    for(char *artista = list_first(artistas); artista != NULL; artista = list_next(artistas)) {
      Pair* artista_pair = searchTreeMap(app->canciones_byartista, artista);
      if (artista_pair == NULL) {
        List* canciones_artista = list_create();
        list_pushBack(canciones_artista, cancion);
        insertTreeMap(app->canciones_byartista, strdup(artista), canciones_artista);
      } else {
        List* canciones_artista = (List*)artista_pair->value;
        list_pushBack(canciones_artista, cancion);
      }
    }
    
  
    char* tempo_range = get_tempo_range(tempo);
    Pair* tempo_pair = searchTreeMap(app->canciones_bytempo, tempo_range);
    if (tempo_pair == NULL) {
      List* canciones_tempo = list_create();
      list_pushBack(canciones_tempo, cancion);
      insertTreeMap(app->canciones_bytempo, strdup(tempo_range), canciones_tempo);
    } else {
      List* canciones_tempo = (List*)tempo_pair->value;
      list_pushBack(canciones_tempo, cancion);
    }
  }

  fclose(archivo);
  printf("Se cargaron %d canciones\n", cuenta);
}

void buscar_por_genero(SpotifindApp* app)
{
  char genero[100];
  printf("Ingrese el género: ");
  scanf(" %99[^\n]", genero);
  printf("\nCanciones del género '%s':\n\n", genero);
  
  Pair* pair = searchTreeMap(app->canciones_bygenero, genero);
  if (pair == NULL) {
    printf("No se encontraron canciones del género '%s'\n", genero);
    return;
  }
  
  List* canciones_genero = (List*)pair->value;
  int encontradas = 0;
  
  for (song_type* cancion = list_first(canciones_genero); cancion != NULL; cancion = list_next(canciones_genero)) {
    print_song(cancion);
    encontradas++;
  }
  
  printf("Se encontraron %d canciones del género '%s'\n", encontradas, genero);
}

void buscar_por_artista(SpotifindApp* app)
{
  char artista[100];
  printf("Ingrese el artista: ");
  scanf(" %99[^\n]", artista);
  printf("\nCanciones del artista '%s':\n\n", artista);
  
  Pair* pair = searchTreeMap(app->canciones_byartista, artista);
  if (pair == NULL) {
    printf("No se encontraron canciones del artista '%s'\n", artista);
    return;
  }
  
  List* canciones_artista = (List*)pair->value;
  int encontradas = 0;
  
  for (song_type* cancion = list_first(canciones_artista); cancion != NULL; cancion = list_next(canciones_artista)) {
    print_song(cancion);
    encontradas++;
  }
  
  printf("Se encontraron %d canciones del artista '%s'\n", encontradas, artista);
}

void buscar_por_tempo(SpotifindApp* app)
{
  printf("Seleccione la categoría de tempo:\n");
  printf("1. Lentas (menos de 80 BPM)\n");
  printf("2. Moderadas (entre 80 y 120 BPM)\n");
  printf("3. Rápidas (más de 120 BPM)\n");
  int option;
  printf("Ingrese su opción: ");
  scanf("%d", &option);
  char* tempo_category;
  switch (option) {
    case 1:
      tempo_category = "Lento";
      break;
    case 2:
      tempo_category = "Moderado";
      break;
    case 3:
      tempo_category = "Rápido";
      break;
    default:
      printf("Opción inválida.\n");
      return;
  }
  
  printf("\nCanciones con tempo %s:\n\n", tempo_category);
  
  Pair* pair = searchTreeMap(app->canciones_bytempo, tempo_category);
  if (pair == NULL) {
    printf("No se encontraron canciones con tempo %s.\n", tempo_category);
    return;
  }
  
  List* canciones_tempo = (List*)pair->value;
  int found = 0;
  
  for (song_type* cancion = list_first(canciones_tempo); cancion != NULL; cancion = list_next(canciones_tempo)) {
    print_song(cancion);
    found++;
  }
  
  printf("Se encontraron %d canción(es) con tempo %s.\n", found, tempo_category);
}

void crear_lista_reproduccion(SpotifindApp* app)
{
  char nombre[100];
  printf("Ingrese el nombre de la lista de reproducción: ");
  scanf(" %99[^\n]", nombre);
  
  for (playlist_type* playlist = list_first(app->playlists); playlist != NULL; playlist = list_next(app->playlists)) {
    if (strcmp(playlist->nombre, nombre) == 0) {
      printf("Ya existe una lista de reproducción con el nombre '%s'.\n", nombre);
      return;
    }
  }
  
  playlist_type* nueva_playlist = crear_playlist(nombre);
  list_pushBack(app->playlists, nueva_playlist);
  printf("Lista de reproducción '%s' creada.\n", nombre);
}

void agregar_cancion_a_lista(SpotifindApp* app)
{
  if (list_size(app->playlists) == 0) {
    printf("No hay listas de reproducción creadas.\n");
    return;
  }
  
  printf("Listas de reproducción disponibles:\n");
  int idx = 1;
  for (playlist_type* playlist = list_first(app->playlists); playlist != NULL; playlist = list_next(app->playlists)) {
    printf("%d. %s\n", idx++, playlist->nombre);
  }
  
  int lista_idx;
  printf("Seleccione una lista (1-%d): ", idx-1);
  scanf("%d", &lista_idx);
  
  if (lista_idx < 1 || lista_idx > idx-1) {
    printf("Selección inválida.\n");
    return;
  }
  
  playlist_type* playlist_seleccionada = list_first(app->playlists);
  for (int i = 1; i < lista_idx; i++) {
    playlist_seleccionada = list_next(app->playlists);
  }
  
  int cancion_id;
  printf("Ingrese el ID de la canción: ");
  scanf("%d", &cancion_id);
  

  Pair* pair = searchTreeMap(app->canciones_byid, &cancion_id);
  if (pair == NULL) {
    printf("No se encontró una canción con el ID %d.\n", cancion_id);
    return;
  }
  
  song_type* cancion = (song_type*)pair->value;
  list_pushBack(playlist_seleccionada->canciones, cancion);
  printf("Canción '%s' agregada a la lista '%s'.\n", cancion->nombre, playlist_seleccionada->nombre);
}

void mostrar_canciones_de_lista(SpotifindApp* app)
{
  if (list_size(app->playlists) == 0) {
    printf("No hay listas de reproducción creadas.\n");
    return;
  }
  

  printf("Listas de reproducción disponibles:\n");
  int idx = 1;
  for (playlist_type* playlist = list_first(app->playlists); playlist != NULL; playlist = list_next(app->playlists)) {
    printf("%d. %s\n", idx++, playlist->nombre);
  }
  
  int lista_idx;
  printf("Seleccione una lista (1-%d): ", idx-1);
  scanf("%d", &lista_idx);
  
  if (lista_idx < 1 || lista_idx > idx-1) {
    printf("Selección inválida.\n");
    return;
  }
  

  playlist_type* playlist_seleccionada = list_first(app->playlists);
  for (int i = 1; i < lista_idx; i++) {
    playlist_seleccionada = list_next(app->playlists);
  }
  
  printf("\nCanciones en la lista '%s':\n\n", playlist_seleccionada->nombre);
  
  if (list_size(playlist_seleccionada->canciones) == 0) {
    printf("La lista está vacía.\n");
    return;
  }
  
  int count = 0;
  for (song_type* cancion = list_first(playlist_seleccionada->canciones); cancion != NULL; cancion = list_next(playlist_seleccionada->canciones)) {
    print_song(cancion);
    count++;
  }
  
  printf("Total: %d canciones.\n", count);
}

void display_menu(SpotifindApp* app)
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
          Impulse_canciones(app, "data/song_dataset_.csv");
          break;
        
        case 2:
          if (app->canciones_bygenero->root == NULL){
            printf("Primero debe cargar las canciones\n");
          }
          else {
            buscar_por_genero(app);
          }
          break;
        
        case 3:
          if (app->canciones_byartista->root == NULL){
            printf("Primero debe cargar las canciones\n");
          }
          else {
            buscar_por_artista(app);
          }
          break;
        
        case 4:
          if (app->canciones_bytempo->root == NULL){
            printf("Primero debe cargar las canciones\n");
          }
          else {
            buscar_por_tempo(app);
          }
          break;
        
        case 5:
          crear_lista_reproduccion(app);
          break;
        
        case 6:
          if (app->canciones_byid->root == NULL){
            printf("Primero debe cargar las canciones\n");
          }
          else {
            agregar_cancion_a_lista(app);
          }
          break;
        
        case 7:
          mostrar_canciones_de_lista(app);
          break;
        
        case 8:
          printf("Saliendo del Spotifind...\n");
          break;
      }
      presioneTeclaParaContinuar();
    }
}

void cleanupApp(SpotifindApp* app) {
  free(app);
}

int main() { 
  SpotifindApp* app = initializeApp();
  display_menu(app);
  cleanupApp(app);
  return 0;
}