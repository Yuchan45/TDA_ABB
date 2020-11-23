#include "abb.h"
#include <stdio.h>
#include "pa2mm.h"

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

int comparador_numeros(void* elemento_1, void* elemento_2){
    if ( (*(int*)elemento_1) > (*(int*)elemento_2) )
        return 1;
    if ( (*(int*)elemento_1) < (*(int*)elemento_2) )
        return -1;
    return 0;
}

void destructor(void* elemento){
    free((int*)elemento);
}

/*****            PRUEBAS             *****/

void probar_crear_arbol(){
    abb_t* arbol;

    pa2m_afirmar((arbol_crear(NULL, NULL)) == NULL, "No deberia poder crear un arbol si paso NULL como comparador.")
    arbol = arbol_crear(&comparador_numeros, &destructor);
    pa2m_afirmar(arbol != NULL, "Puedo crear el árbol");
    pa2m_afirmar(arbol->nodo_raiz == NULL, "El arbol inicia con nodo_raíz igual a NULL");
    pa2m_afirmar(arbol->comparador == &comparador_numeros, "El comparador del árbol apunta al comparador recibido");
    pa2m_afirmar(arbol->destructor == &destructor, "El destructor del árbol apunta al destructor recibido");
    
    //Faltan los free.
}

void probar_insertar_en_arbol(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5;
    // b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;

    pa2m_afirmar(arbol_insertar(NULL, &a) != 0, "Insertar un elemento en un arbol inexistente deberia fallar.");
    pa2m_afirmar(arbol_insertar(arbol, &a) == 0, "Puedo insertar un elemento en un arbol. Inserte un '5'.");
    pa2m_afirmar((arbol->nodo_raiz) != NULL, "El nodo_raiz del arbol ya no es NULO. Existe una raiz.");
    pa2m_afirmar((arbol->nodo_raiz->izquierda) == NULL, "El hijo izquierdo del nodo raiz inicio en NULL.");
    pa2m_afirmar((arbol->nodo_raiz->derecha) == NULL, "El hijo derecho del nodo raiz inicio en NULL.");
    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->elemento, &a) == 0, "El elemento se inserto correctamente.");


}

int main(){
    /*PRUEBAS*/

    pa2m_nuevo_grupo("PRUEBAS DE CREACION DE ARBOL");
    probar_crear_arbol();

    pa2m_nuevo_grupo("PRUEBAS DE INSERCION EN ARBOL");
    probar_insertar_en_arbol();

    return pa2m_mostrar_reporte();
} 