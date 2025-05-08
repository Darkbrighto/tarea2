#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value)
{
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap* createTreeMap(int (*lower_than)(void* key1, void* key2))
{
    // Reservar memoria para un nuevo TreeMap
    TreeMap* nuevoarbol = (TreeMap*)malloc(sizeof(TreeMap));
    if (nuevoarbol == NULL) {
        return NULL;
    }

    // Inicializar las variables del TreeMap
    nuevoarbol->root = NULL;        // El árbol comienza vacío
    nuevoarbol->current = NULL;     // No hay nodo actual al principio
    nuevoarbol->lower_than = lower_than;  // Asignar la función de comparación

    // Retornar el puntero al nuevo TreeMap
    return nuevoarbol;
}


void insertTreeMap(TreeMap* tree, void* key, void* value)
{
    // Primero, buscamos si la clave ya existe en el árbol
    Pair* existingPair = searchTreeMap(tree, key);

    // Si la clave ya existe, no hacemos nada
    if (existingPair != NULL) {
        return;  // La clave ya existe, no insertamos
    }

    // Si la clave no existe, vamos a insertar el nuevo nodo en el árbol
    TreeNode* actual = tree->root;
    TreeNode* padre = NULL;

    // Recorremos el árbol para encontrar la posición de inserción
    while (actual != NULL) {
        padre = actual;
        // Comparar la clave para decidir si vamos a la izquierda o a la derecha
        if (tree->lower_than(key, actual->pair->key)) {
            actual = actual->left;
        } else {
            actual = actual->right;
        }
    }

    // Crear el nuevo nodo
    TreeNode* nuevoNodo = createTreeNode(key, value);

    // Enlazar el nuevo nodo con su padre
    nuevoNodo->parent = padre;

    // Si el árbol está vacío, el nuevo nodo será la raíz
    if (padre == NULL) {
        tree->root = nuevoNodo;
    }
    // Si la clave es menor que la clave del padre, el nuevo nodo va a la izquierda
    else if (tree->lower_than(key, padre->pair->key)) {
        padre->left = nuevoNodo;
    }
    // Si la clave es mayor, el nuevo nodo va a la derecha
    else {
        padre->right = nuevoNodo;
    }

    // Actualizar el puntero 'current' para que apunte al nuevo nodo insertado
    tree->current = nuevoNodo;
}

TreeNode* minimum(TreeNode* x) {
    // Mientras el nodo tenga hijo izquierdo, nos movemos hacia el hijo izquierdo
    while (x->left != NULL) {
        x = x->left;
    }
    return x;  // Retornamos el nodo con la clave mínima
}

void removeNode(TreeMap* tree, TreeNode* node)
{
    // Caso 1: Nodo sin hijos (hoja)
    if (node->left == NULL && node->right == NULL) {
        // Si el nodo tiene padre, anulamos el puntero que apunta al nodo
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else {
            // Si el nodo es la raíz, simplemente ponemos la raíz en NULL
            tree->root = NULL;
        }
        free(node); // Liberamos la memoria del nodo
    }
    // Caso 2: Nodo con un hijo
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child;
        
        // Si el nodo tiene solo un hijo izquierdo o derecho, tomamos ese hijo
        if (node->left != NULL) {
            child = node->left;
        } else {
            child = node->right;
        }

        // Si el nodo tiene un padre, hacemos que el padre apunte al hijo
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
        } else {
            // Si el nodo es la raíz, el hijo será la nueva raíz
            tree->root = child;
        }

        // Enlazamos el hijo con el padre del nodo
        child->parent = node->parent;

        free(node); // Liberamos la memoria del nodo
    }
    // Caso 3: Nodo con dos hijos
    else {
        // Encontramos el nodo mínimo en el subárbol derecho
        TreeNode* successor = minimum(node->right);

        // Copiamos los datos del sucesor al nodo actual
        node->pair = successor->pair;

        // Eliminamos el sucesor, que ahora tiene como padre el nodo original
        removeNode(tree, successor);
    }
}

void eraseTreeMap(TreeMap* tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return; // Si el árbol está vacío, no hacer nada

    // Buscar el nodo con la clave
    if (searchTreeMap(tree, key) == NULL) return; // Si no se encuentra el nodo, no hacer nada

    TreeNode* node = tree->current; // El nodo encontrado por searchTreeMap
    removeNode(tree, node); // Eliminar el nodo
}

Pair* searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* actual = tree->root;

    // Recorremos el árbol binario
    while (actual != NULL) {
        // Si la clave es igual a la del nodo actual, encontramos el nodo
        if (tree->lower_than(key, actual->pair->key) == 0 && tree->lower_than(actual->pair->key, key) == 0) {
            tree->current = actual;  // Apuntar al nodo encontrado
            return actual->pair;     // Retornar el par clave-valor
        }

        // Si la clave es menor que la del nodo actual, ir al subárbol izquierdo
        if (tree->lower_than(key, actual->pair->key)) {
            actual = actual->left;
        }
        // Si la clave es mayor, ir al subárbol derecho
        else {
            actual = actual->right;
        }
    }

    // Si no encontramos la clave, retornar NULL
    return NULL;
}


Pair* upperBound(TreeMap* tree, void* key)
{
    TreeNode* currentNode = tree->root;
    TreeNode* ub_node = NULL;  // Nodo con la clave mayor o igual a key
    
    // Realizamos la búsqueda en el árbol
    while (currentNode != NULL) {
        if (is_equal(tree, currentNode->pair->key, key)) {
            // Si encontramos un nodo con la clave igual, lo retornamos
            return currentNode->pair;
        } else if (tree->lower_than(key, currentNode->pair->key)) {
            // Si la clave proporcionada es menor que la clave del nodo actual,
            // actualizamos el nodo auxiliar y vamos al subárbol izquierdo
            ub_node = currentNode;
            currentNode = currentNode->left;
        } else {
            // Si la clave proporcionada es mayor que la clave del nodo actual,
            // vamos al subárbol derecho
            currentNode = currentNode->right;
        }
    }

    // Si encontramos un nodo válido, retornamos su par (clave-valor)
    return (ub_node != NULL) ? ub_node->pair : NULL;
}


Pair* firstTreeMap(TreeMap* tree)
{
    if (tree == NULL || tree->root == NULL) {
        return NULL;  // Si el árbol está vacío, no hay primer elemento
    }
    
    // Empezamos desde la raíz
    TreeNode* currentNode = tree->root;
    
    // Vamos al nodo más a la izquierda
    while (currentNode->left != NULL) {
        currentNode = currentNode->left;
    }
    
    // Actualizamos el puntero current
    tree->current = currentNode;
    
    // Retornamos el par del nodo más a la izquierda
    return currentNode->pair;
}


Pair* nextTreeMap(TreeMap* tree)
{
    if (tree == NULL || tree->current == NULL) {
        return NULL;  // Si el árbol o el nodo actual son NULL, no hay siguiente
    }

    TreeNode* currentNode = tree->current;

    // Caso 1: Si el nodo tiene hijo derecho, el siguiente es el nodo más a la izquierda en el subárbol derecho
    if (currentNode->right != NULL) {
        currentNode = currentNode->right;
        while (currentNode->left != NULL) {
            currentNode = currentNode->left;
        }
        tree->current = currentNode;  // Actualizamos el puntero current
        return currentNode->pair;
    }

    // Caso 2: Si el nodo no tiene hijo derecho, subimos por el árbol hasta encontrar un nodo
    // que sea hijo izquierdo de su padre
    TreeNode* parentNode = currentNode->parent;
    while (parentNode != NULL && parentNode->right == currentNode) {
        currentNode = parentNode;
        parentNode = parentNode->parent;
    }

    // Actualizamos el puntero current
    tree->current = parentNode;
    
    // Si encontramos un nodo al que se llegó desde su hijo izquierdo, retornamos el par
    return (parentNode != NULL) ? parentNode->pair : NULL;
}