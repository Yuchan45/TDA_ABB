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

void* arbol_buscar(abb_t* arbol, void* elemento){
    if(!arbol || !(arbol->comparador))
        return NULL;
    
    void* elemento_buscado = nodo_buscar(arbol->nodo_raiz, elemento, arbol->comparador);

    return elemento_buscado;
}

nodo_abb_t* hallar_nodo_extremo_derecho(nodo_abb_t* nodo){
    // A esta funcion, le pasas el nodo hijo izquierdo y lo que hace es recorrer todo el lado derecho del hijo y retorna el ultimo(maximo) nodo del lado derecho.
    if(!nodo)
        return NULL;
    if(nodo->derecha){
        return hallar_nodo_extremo_derecho(nodo->derecha); //Si tiene hijo derecho, segui.
    }else{
        return nodo; // Halle el maximo.
    }
}

nodo_abb_t* borrar_con_dos_hijos(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    nodo_abb_t* nodo_aux = nodo->izquierda;
    //printf("%i\n", *(int*)nodo_aux->elemento);

    if (nodo_aux->derecha != NULL){
        //printf("asdasdas\n\n\n");
        nodo_abb_t* nodo_extremo = hallar_nodo_extremo_derecho(nodo->izquierda);
        if (nodo_extremo->izquierda){ // Si el nodo extremo a recolocar tiene hijo izquierdo.
            nodo_aux->derecha = nodo_extremo->izquierda; // Le asigno el hijo izq del nodo a recolocar.
        }else{
            nodo_aux->derecha = NULL; 
        }

        nodo_extremo->izquierda = nodo->izquierda; // Y al nodo extremo lo pongo en el lugar del nodo borrado.
        nodo_extremo->derecha = nodo->derecha;
        nodo_aux = nodo_extremo;

    }else{ 
        //printf("%i\n", *(int*)nodo_aux->elemento);
        nodo_aux->derecha = nodo->derecha;

        //nodo->izquierda->derecha = nodo->derecha;
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

    }else if(comparacion == 1){ //Me muevo para la derecha
        nodo->derecha = nodo_borrar(nodo->derecha, elemento, comparador, destructor, resultado);

    }else{ // Comparacion = -1, me muevo a la izquierda.
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


void* arbol_raiz(abb_t* arbol){
    if(!arbol || !(arbol->nodo_raiz))
        return NULL;
    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
    if(!arbol || !(arbol->nodo_raiz))
        return true;
    return false;
}

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

void insertar_elementos_preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* elementos_insertados){
    // (NID)
    if (!nodo || *elementos_insertados >= tamanio_array) // Condicion de stop.
        return;

    if (*elementos_insertados < tamanio_array){
        //printf("hola\n");
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


void insertar_elementos_postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* elementos_insertados){
    // (IDN)
    if (!nodo || *elementos_insertados >= tamanio_array) // Condicion de stop.
        return;

    insertar_elementos_postorden(nodo->izquierda, array, tamanio_array, elementos_insertados);
    insertar_elementos_postorden(nodo->derecha, array, tamanio_array, elementos_insertados);
    if (*elementos_insertados < tamanio_array){
        //printf("hola\n");
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

/*
 * Iterador interno. Recorre el arbol e invoca la funcion con cada
 * elemento del mismo. El puntero 'extra' se pasa como segundo
 * parámetro a la función. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos.
 * El recorrido se realiza de acuerdo al recorrido solicitado.  Los
 * recorridos válidos son: ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN.
 * Devuelve la cantidad de elementos que fueron recorridos.
*/
/*
size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
    
}*/

void nodo_liberar(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    // Recorro los nodos y los voy cerrando de atras para adelante. Si no tengo hijos borro, si puedo avanar para izq avanzo, y si puedo avanzar para drcha avanzo.
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