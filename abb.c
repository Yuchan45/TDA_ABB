#include "abb.h"
#include <stdio.h>
#include <stdlib.h>


/*      CREAR      */
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


/*      INSERTAR      */
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


/*      BUSCAR      */
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

void* arbol_buscar(abb_t* arbol, void* elemento){
    if(!arbol || !(arbol->comparador))
        return NULL;
    
    void* elemento_buscado = nodo_buscar(arbol->nodo_raiz, elemento, arbol->comparador);

    return elemento_buscado;
}


/*      BORRAR      */

nodo_abb_t* hallar_padre_nodo_extremo_derecho(nodo_abb_t* nodo){
    // Esta funcion retorna el padre del nodo extremo (mayor de los menores).
    if(!nodo || !(nodo->derecha))
        return NULL;
    if(nodo->derecha->derecha){
        return hallar_padre_nodo_extremo_derecho(nodo->derecha); //Si tiene hijo derecho, segui.
    }else{
        return nodo; // Halle el padre del maximo.
    }
}

nodo_abb_t* borrar_con_dos_hijos(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    nodo_abb_t* nodo_aux = nodo->izquierda;

    if (nodo_aux->derecha != NULL){ // Si el nodo izquierdo del nodo a borrar tiene hijos derechos.

        nodo_abb_t* nodo_padre = hallar_padre_nodo_extremo_derecho(nodo->izquierda);
        if (!nodo_padre)
            return NULL;
        nodo_abb_t* nodo_a_recolocar = nodo_padre->derecha;
        if (nodo_a_recolocar->izquierda){ // Si el nodo extremo a recolocar tiene hijo izquierdo.
            nodo_padre->derecha = nodo_a_recolocar->izquierda; // Le asigno el hijo izq del nodo a recolocar.
        }else{
            nodo_padre->derecha = NULL; 
        }

        nodo_a_recolocar->izquierda = nodo->izquierda; // Y al nodo extremo lo pongo en el lugar del nodo borrado.
        nodo_a_recolocar->derecha = nodo->derecha;
        nodo_aux = nodo_a_recolocar; 

    }else{ 
        nodo_aux->derecha = nodo->derecha;
    }

    if (destructor)
        destructor(nodo->elemento);
    free(nodo);
    return nodo_aux;

}

nodo_abb_t* borrar_hoja(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    if (destructor)
        destructor(nodo->elemento);
    free(nodo);
    return NULL;
}

nodo_abb_t* borrar_un_hijo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    nodo_abb_t* hijo;
    if (nodo->izquierda){
        hijo = nodo->izquierda;
    }else{
        hijo = nodo->derecha;
    }
    if (destructor)
        destructor(nodo->elemento);
    free(nodo);
    
    return hijo;
}

nodo_abb_t* nodo_borrar(nodo_abb_t* nodo, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor, bool* resultado){
    if(!nodo || !comparador)
        return NULL;

    int comparacion = comparador(elemento, nodo->elemento);
    if(comparacion == 0){ //Lo encontre
        *resultado = true;
        if(nodo->izquierda && nodo->derecha){
            return borrar_con_dos_hijos(nodo, destructor);

        }else if (nodo->izquierda || nodo->derecha){
            return borrar_un_hijo(nodo, destructor);

        }else{
            return borrar_hoja(nodo, destructor);
        }

    }else if(comparacion > 0){ //Me muevo para la derecha
        nodo->derecha = nodo_borrar(nodo->derecha, elemento, comparador, destructor, resultado);

    }else{ // Comparacion < 0, me muevo a la izquierda.
        nodo->izquierda = nodo_borrar(nodo->izquierda, elemento, comparador, destructor, resultado);

    }
    return nodo;

}

int arbol_borrar(abb_t* arbol, void* elemento){
    if(!arbol || !(arbol->comparador))
        return -1;

    bool resultado = false;
    arbol->nodo_raiz = nodo_borrar(arbol->nodo_raiz, elemento, arbol->comparador, arbol->destructor, &resultado);
    if(resultado)
        return 0;
    return -1;
}


/*      RAIZ      */
void* arbol_raiz(abb_t* arbol){
    if(!arbol || !(arbol->nodo_raiz))
        return NULL;
    return arbol->nodo_raiz->elemento;
}


/*      ARBOL_VACIO      */
bool arbol_vacio(abb_t* arbol){
    if(!arbol || !(arbol->nodo_raiz))
        return true;
    return false;
}


/*      RECORRIDO INORDEN       */
void insertar_elementos_inorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* elementos_insertados){
    // (IND)
    if (!nodo || *elementos_insertados >= tamanio_array) // Condicion de stop.
        return;

    insertar_elementos_inorden(nodo->izquierda, array, tamanio_array, elementos_insertados);
    if (*elementos_insertados < tamanio_array){
        //printf("hola\n");
        array[*elementos_insertados] = nodo->elemento;
        (*elementos_insertados) ++;
    }
    insertar_elementos_inorden(nodo->derecha, array, tamanio_array, elementos_insertados);
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){
    if (arbol_vacio(arbol) || !array || tamanio_array == 0)
        return 0;
    size_t elementos_insertados = 0;

    insertar_elementos_inorden(arbol->nodo_raiz, array, tamanio_array, &elementos_insertados);

    return elementos_insertados;
}


/*      RECORRIDO PREORDEN       */
void insertar_elementos_preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* elementos_insertados){
    // (NID)
    if (!nodo || *elementos_insertados >= tamanio_array) // Condicion de stop.
        return;

    if (*elementos_insertados < tamanio_array){
        array[*elementos_insertados] = nodo->elemento;
        (*elementos_insertados) ++;
    }
    insertar_elementos_preorden(nodo->izquierda, array, tamanio_array, elementos_insertados);
    insertar_elementos_preorden(nodo->derecha, array, tamanio_array, elementos_insertados);
}


size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){
    if (arbol_vacio(arbol) || !array || tamanio_array == 0)
        return 0;
    size_t elementos_insertados = 0;

    insertar_elementos_preorden(arbol->nodo_raiz, array, tamanio_array, &elementos_insertados);

    return elementos_insertados;
}


/*      RECORRIDO POSTORDEN       */
void insertar_elementos_postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* elementos_insertados){
    // (IDN)
    if (!nodo || *elementos_insertados >= tamanio_array) // Condicion de stop.
        return;

    insertar_elementos_postorden(nodo->izquierda, array, tamanio_array, elementos_insertados);
    insertar_elementos_postorden(nodo->derecha, array, tamanio_array, elementos_insertados);
    if (*elementos_insertados < tamanio_array){
        array[*elementos_insertados] = nodo->elemento;
        (*elementos_insertados) ++;
    }
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
    if (arbol_vacio(arbol) || !array || tamanio_array == 0)
        return 0;
    size_t elementos_insertados = 0;

    insertar_elementos_postorden(arbol->nodo_raiz, array, tamanio_array, &elementos_insertados);

    return elementos_insertados;
}


/*      ITERADORES      */
bool recorrer_nodos_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* elementos_recorridos){
    if (!nodo || !funcion || !elementos_recorridos)
        return false;

    bool fin = recorrer_nodos_inorden(nodo->izquierda, funcion, extra, elementos_recorridos);

    if (!fin){
        fin = (*funcion)(nodo->elemento, extra);
        (*elementos_recorridos)++;
    }
    if (!fin)
        fin = recorrer_nodos_inorden(nodo->derecha, funcion, extra, elementos_recorridos); 

    return fin;
}

bool recorrer_nodos_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* elementos_recorridos){
    if (!nodo || !funcion || !elementos_recorridos)
        return false;
    bool fin;

    fin = (*funcion)(nodo->elemento, extra);
    (*elementos_recorridos)++;

    if (!fin)
        fin = recorrer_nodos_preorden(nodo->izquierda, funcion, extra, elementos_recorridos);

    if (!fin)
        fin = recorrer_nodos_preorden(nodo->derecha, funcion, extra, elementos_recorridos); 

    return fin;
}

bool recorrer_nodos_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* elementos_recorridos){
    if (!nodo || !funcion || !elementos_recorridos)
        return false;
    bool fin;

    fin = recorrer_nodos_postorden(nodo->izquierda, funcion, extra, elementos_recorridos);

    if (!fin)
        fin = recorrer_nodos_postorden(nodo->derecha, funcion, extra, elementos_recorridos); 

    if (!fin){
        fin = (*funcion)(nodo->elemento, extra);
        (*elementos_recorridos)++;  
    }    

    return fin;
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
    if (arbol_vacio(arbol) || !funcion)
        return 0;

    size_t elementos_recorridos = 0;
    if (recorrido == ABB_RECORRER_INORDEN){
        recorrer_nodos_inorden(arbol->nodo_raiz, funcion, extra, &elementos_recorridos);
    }else if (recorrido == ABB_RECORRER_PREORDEN){
        recorrer_nodos_preorden(arbol->nodo_raiz, funcion, extra, &elementos_recorridos);
    }else if (recorrido == ABB_RECORRER_POSTORDEN){
        recorrer_nodos_postorden(arbol->nodo_raiz, funcion, extra, &elementos_recorridos);
    }

    return elementos_recorridos;
}


/*      ARBOL DESTRUIR      */
void nodo_liberar(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    if (!nodo)
        return;
    nodo_liberar(nodo->izquierda, destructor);
    nodo_liberar(nodo->derecha, destructor);
    if (destructor)
        destructor(nodo->elemento);
    free(nodo);
    
}

void arbol_destruir(abb_t* arbol){
    if (!arbol)
        return;

    nodo_liberar(arbol->nodo_raiz, arbol->destructor);
    free(arbol);

}