#include "abb.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct{
    size_t tamanio;
    float peso;
}manzana_t;

int comparar_manzanas_por_peso(void* manzana_1, void* manzana_2){
    if ( ((manzana_t*)manzana_1)->peso > ((manzana_t*)manzana_2)->peso )
        return 1;
    if ( ((manzana_t*)manzana_1)->peso < ((manzana_t*)manzana_2)->peso )
        return -1;
    return 0;

}

/*
 * Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    if(!comparador)
        return NULL;

    abb_t* raiz_arbol = malloc(sizeof(abb_t));
    if(!raiz_arbol)
        return NULL;
    raiz_arbol->comparador = comparador;
    raiz_arbol->destructor = destructor;
    return raiz_arbol;
}

/*
 * Inserta un elemento en el arbol.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El arbol admite elementos con valores repetidos.
 */
int arbol_insertar(abb_t* arbol, void* elemento){
    if (!arbol->nodo_raiz){
        nodo_abb_t* raiz = calloc(1, sizeof(nodo_abb_t*));
        if (!raiz)
            return NULL;
        raiz->elemento = elemento;
        return raiz;
    }


}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento){

}


/*
 * Devuelve el elemento almacenado como raiz o NULL si el árbol está
 * vacío o no existe.
 */
void* arbol_raiz(abb_t* arbol){
    if(!arbol)
        return NULL;
    return arbol->nodo_raiz;
}