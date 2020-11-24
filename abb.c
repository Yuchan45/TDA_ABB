#include "abb.h"
#include <stdio.h>
#include <stdlib.h>


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    if(!comparador)
        return NULL;

    abb_t* raiz_arbol = malloc(sizeof(abb_t));
    if(!raiz_arbol)
        return NULL;
    raiz_arbol->nodo_raiz = NULL;
    raiz_arbol->comparador = comparador;
    raiz_arbol->destructor = destructor;
    return raiz_arbol;
}

/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){

}

nodo_abb_t* nodo_insertar(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){

    if(!nodo){
        nodo_abb_t* nuevo_nodo = malloc(sizeof(nodo_abb_t));
        if (!nuevo_nodo)
            return NULL;
        nuevo_nodo->izquierda = NULL;
        nuevo_nodo->derecha = NULL;    
        nuevo_nodo->elemento = elemento;
        return nuevo_nodo;
    }

    if(comparador(elemento, nodo->elemento) > 0){
        nodo->derecha = nodo_insertar(nodo->derecha, elemento, comparador);
    }else{ // La comparacion devuelve -1 o 0.
        nodo->izquierda = nodo_insertar(nodo->izquierda, elemento, comparador);
    }
    return nodo;

}

int arbol_insertar(abb_t* arbol, void* elemento){
    if(!arbol || !(arbol->comparador)) //Creeeo que no hace falta fijarse si esta el comparador, de eso se encarga arbol_crear().
        return -1;
    
    nodo_abb_t* nodo_aux = nodo_insertar(arbol->nodo_raiz, elemento, arbol->comparador);
    if(!nodo_aux)
        return -1;
    arbol->nodo_raiz = nodo_aux;
    return 0;

}

void* nodo_buscar(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){
    if(!comparador)
        return NULL;

    if(!nodo)  // Si no encuentra nada devuelve nada.
        return NULL;
    int comparacion = comparador(elemento, nodo->elemento);

    if(comparacion == 0) // Si encuentra.
        return nodo->elemento;
    if(comparacion > 0){
        return nodo_buscar(nodo->derecha, elemento, comparador);
    }else{
        return nodo_buscar(nodo->izquierda, elemento, comparador);
    }

}


/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* arbol_buscar(abb_t* arbol, void* elemento){
    if(!arbol || !(arbol->comparador))
        return NULL;
    
    void* elemento_buscado = nodo_buscar(arbol->nodo_raiz, elemento, arbol->comparador);

    return elemento_buscado;
}



/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
/*
int arbol_borrar(abb_t* arbol, void* elemento){

}*/


/*
 * Devuelve el elemento almacenado como raiz o NULL si el árbol está
 * vacío o no existe.
 */
void* arbol_raiz(abb_t* arbol){
    if(!arbol)
        return NULL;
    return arbol->nodo_raiz->elemento;
}

/*
 * Determina si el árbol está vacío.
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene elementos.
 */
bool arbol_vacio(abb_t* arbol){
    if(!arbol || !(arbol->nodo_raiz))
        return true;
    return false;
}