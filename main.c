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


int main(){
    /*PRUEBAS*/

    pa2m_nuevo_grupo("PRUEBAS DE CREACION");
    probar_crear_arbol();

    return pa2m_mostrar_reporte();
} 