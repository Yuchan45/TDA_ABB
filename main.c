#include "abb.h"
#include <stdio.h>
#include "pa2mm.h"

/*               COMPARADORES, DESTRUCTORES Y OTRAS FUNCIONES                */
typedef struct{
    size_t tamanio;
    bool arenosa;
    float peso;
}manzana_t;

int comparar_manzanas_por_peso(void* manzana_1, void* manzana_2){
    if ( ((manzana_t*)manzana_1)->peso > ((manzana_t*)manzana_2)->peso )
        return 1;
    if ( ((manzana_t*)manzana_1)->peso < ((manzana_t*)manzana_2)->peso )
        return -1;
    return 0;
}

void destructor_de_manzanas(void* manzana){
    if (manzana)
        free((manzana_t*)manzana);
}

int comparador_numeros(void* elemento_1, void* elemento_2){
    if ( (*(int*)elemento_1) > (*(int*)elemento_2) )
        return 1;
    if ( (*(int*)elemento_1) < (*(int*)elemento_2) )
        return -1;
    return 0;
}

void destructor_de_numeros(void* numero){
    if (numero)
        free((int*)numero);
}

bool mostrar_elemento_numero(void* elemento, void* extra){
    if(elemento)
        printf("     Elemento: %i \n", *(int*)elemento);
    return false;
}

bool mostrar_hasta_el_cinco(void* elemento, void* extra){
    if(elemento)
        printf("     Elemento: %i \n", *(int*)elemento);
    if (*(int*)elemento == 5)
        return true;
    return false;
}
/*
nodo_abb_t* hallar_padre_nodo_extremo_derecho(nodo_abb_t* nodo){
    // Esta funcion retorna el padre del nodo extremo (mayor de los menores).
    if(!nodo)
        return NULL;
    if(nodo->derecha->derecha){
        return hallar_padre_nodo_extremo_derecho(nodo->derecha); //Si tiene hijo derecho, segui.
    }else{
        return nodo; // Halle el maximo.
    }
}*/

/*****            PRUEBAS             *****/

void probar_crear_arbol(){
    abb_t* arbol;

    pa2m_afirmar((arbol_crear(NULL, NULL)) == NULL, "No deberia poder crear un arbol si paso NULL como comparador.")
    arbol = arbol_crear(&comparador_numeros, &destructor_de_numeros);
    pa2m_afirmar(arbol != NULL, "Puedo crear el árbol");
    pa2m_afirmar(arbol->nodo_raiz == NULL, "El arbol inicia con nodo_raíz igual a NULL");
    pa2m_afirmar(arbol->comparador == &comparador_numeros, "El comparador del árbol apunta al comparador recibido");
    pa2m_afirmar(arbol->destructor == &destructor_de_numeros, "El destructor del árbol apunta al destructor recibido");

    arbol_destruir(arbol);
}

void probar_insertar_en_arbol(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;

    pa2m_afirmar(arbol_insertar(NULL, &a) != 0, "Insertar un elemento en un arbol inexistente deberia fallar.");
    pa2m_afirmar(arbol_insertar(arbol, &a) == 0, "Puedo insertar un elemento en un arbol. Inserte un '5'.");
    pa2m_afirmar((arbol->nodo_raiz) != NULL, "El nodo_raiz del arbol ya no es NULO. Existe una raiz.");
    pa2m_afirmar((arbol->nodo_raiz->izquierda) == NULL, "El hijo izquierdo del nodo raiz inicio en NULL.");
    pa2m_afirmar((arbol->nodo_raiz->derecha) == NULL, "El hijo derecho del nodo raiz inicio en NULL.");
    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->elemento, &a) == 0, "El elemento se inserto correctamente.");

    pa2m_afirmar(arbol_insertar(arbol, &b) == 0, "Puedo insertar un elemento en un arbol. Inserte un '3'.");
    pa2m_afirmar(arbol_insertar(arbol, &c) == 0, "Puedo insertar un elemento en un arbol. Inserte un '7'.");
    pa2m_afirmar(arbol_insertar(arbol, &d) == 0, "Puedo insertar un elemento en un arbol. Inserte un '2'.");
    pa2m_afirmar(arbol_insertar(arbol, &e) == 0, "Puedo insertar un elemento en un arbol. Inserte un '4'.");
    pa2m_afirmar(arbol_insertar(arbol, &f) == 0, "Puedo insertar un elemento en un arbol. Inserte un '6'.");
    pa2m_afirmar(arbol_insertar(arbol, &g) == 0, "Puedo insertar un elemento en un arbol. Inserte un '8'.");

    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */

    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->izquierda->elemento, &b) == 0, "El elemento '3' se inserto correctamente.");
    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->derecha->elemento, &c) == 0, "El elemento '7' se inserto correctamente.");
    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->izquierda->izquierda->elemento, &d) == 0, "El elemento '2' se inserto correctamente.");
    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->izquierda->derecha->elemento, &e) == 0, "El elemento '4' se inserto correctamente.");
    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->derecha->izquierda->elemento, &f) == 0, "El elemento '6' se inserto correctamente.");
    pa2m_afirmar(comparador_numeros(arbol->nodo_raiz->derecha->derecha->elemento, &g) == 0, "El elemento '8' se inserto correctamente.");

    arbol_destruir(arbol);
}

void probar_buscar_en_arbol(){
    abb_t* arbol = arbol_crear(&comparar_manzanas_por_peso, NULL);
    manzana_t manzana = {1, true, 5}, otra_manzana = {2, false, 3}, tercer_manzana = {3, false, 7}, cuarta_manzana = {4, true, 2}, quinta_manzana = {5, true, 4};
    manzana_t manzana_buscada = {6, true, 5}; // Esta manzana tiene el mismo peso que la 1ª manzana.

    arbol_insertar(arbol, &manzana);
    arbol_insertar(arbol, &otra_manzana);
    arbol_insertar(arbol, &tercer_manzana);
    arbol_insertar(arbol, &cuarta_manzana);
    //arbol_insertar(arbol, &quinta_manzana);
    printf("*Inserte 4 elementos (Manzanas).\n");

    pa2m_afirmar(arbol_buscar(arbol, &manzana) == &manzana, "Encontre la manzana");
    pa2m_afirmar(arbol_buscar(arbol, &otra_manzana) == &otra_manzana, "Encontre la otra manzana");
    pa2m_afirmar(arbol_buscar(arbol, &tercer_manzana) == &tercer_manzana, "Encontre la tercer manzana");
    pa2m_afirmar(arbol_buscar(arbol, &cuarta_manzana) == &cuarta_manzana, "Encontre la cuarta manzana");
    pa2m_afirmar(arbol_buscar(arbol, &quinta_manzana) == NULL, "No deberia poder encontrar una manzana que no esta en el arbol.");
    pa2m_afirmar(arbol_buscar(arbol, &manzana_buscada) == &manzana, "Encuentro la manzana buscada porque tiene el mismo peso que la 1ª manzana.");
    // Pude encontrar la manzana buscada porque el comparador asociado al arbol se fija solo en el peso. Y como la manzana buscada y la manzana tienen el mismo peso..
    arbol_destruir(arbol);
}

/*
void probar_borrar_raiz_varias_veces(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 7;
    arbol_insertar(arbol, &a);
    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "Puedo borrar la raiz del arbol.");
    
}*/


void probar_arbol_raiz(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5;

    pa2m_afirmar(arbol_raiz(NULL) == NULL, "Si el arbol no existe devuelve NULL.");
    pa2m_afirmar(arbol_raiz(arbol) == NULL, "Si el arbol esta vacio devuelve NULL.");

    printf("  - Inserto 1 elemento -\n");
    arbol_insertar(arbol, &a);
    pa2m_afirmar(arbol_raiz(arbol) == &a, "El elemento de la raiz es correcto.");

    arbol_destruir(arbol);
}   

void probar_arbol_vacio(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5;

    pa2m_afirmar(arbol_vacio(NULL) == true, "Si el arbol no existe devuelve true.");
    pa2m_afirmar(arbol_vacio(arbol) == true, "Si el arbol esta vacio devuelve true.");

    printf("  - Inserto 1 elemento -\n");
    arbol_insertar(arbol, &a);
    pa2m_afirmar(arbol_vacio(arbol) == false, "Si el arbol tiene un elemento devuelve false.");

    arbol_destruir(arbol);
}

void mostrar_array_de_numeros(void** array, int cantidad){
    printf("  -ASI QUEDARIA EL ARRAY: \n");
    printf("    [ ");
    for (int i = 0; i < cantidad; i++){
        printf("%i  ", *(int*)array[i]);
    }
    printf("]\n");
}

void probar_arbol_recorrido_inorden(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;
    void* array[10]; // [2, 3, 4, 5, 6, 7, 8] Asi deberia quedar.

    pa2m_afirmar(arbol_recorrido_inorden(NULL, array, 2) == 0, "Si el arbol es nulo devuelve 0.");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, array, 2) == 0, "Si el arbol esta vacio devuelve 0");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, array, 0) == 0, "Si el tamanio del array es 0 devuelve 0.\n");
    printf("  *Inserte 7 elementos en el arbol en el siguiente orden: [5, 3, 7, 2, 4, 6, 8] -\n");
    printf("  -Recorrido inorden completo correcto: [2, 3, 4, 5, 6, 7, 8] -\n");
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */

    pa2m_afirmar(arbol_recorrido_inorden(arbol, array, 2) == 2, "Si el tamanio del array es de 2 y le hago recorrer un arbol con 7 elementos, guarda solo 2.");
    mostrar_array_de_numeros(array, 2);
    pa2m_afirmar(arbol_recorrido_inorden(arbol, array, 30) == 7, "Si el tamanio del array es de 30 y le hago recorrer un arbol de 7 elementos, guarda los 7.");
    mostrar_array_de_numeros(array, 7);

    arbol_destruir(arbol);
}

void probar_arbol_recorrido_preorden(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;
    void* array[10]; // [5, 3, 2, 4, 7, 6, 8] Asi deberia quedar.

    pa2m_afirmar(arbol_recorrido_preorden(NULL, array, 2) == 0, "Si el arbol es nulo devuelve 0.");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, array, 2) == 0, "Si el arbol esta vacio devuelve 0");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, array, 0) == 0, "Si el tamanio del array es 0 devuelve 0.\n");
    printf("  *Inserte 7 elementos en el arbol en el siguiente orden: [5, 3, 7, 2, 4, 6, 8] -\n");
    printf("  -Recorrido preorden completo correcto: [5, 3, 2, 4, 7, 6, 8] -\n");
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */

    pa2m_afirmar(arbol_recorrido_preorden(arbol, array, 2) == 2, "Si el tamanio del array es de 2 y le hago recorrer un arbol con 7 elementos, guarda solo 2.");
    mostrar_array_de_numeros(array, 2);
    pa2m_afirmar(arbol_recorrido_preorden(arbol, array, 30) == 7, "Si el tamanio del array es de 30 y le hago recorrer un arbol de 7 elementos, guarda los 7.");
    mostrar_array_de_numeros(array, 7);

    arbol_destruir(arbol);
}

void probar_arbol_recorrido_postorden(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;
    void* array[10]; // [2, 4, 3, 6, 8, 7, 5] Asi deberia quedar.

    pa2m_afirmar(arbol_recorrido_postorden(NULL, array, 2) == 0, "Si el arbol es nulo devuelve 0.");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, array, 2) == 0, "Si el arbol esta vacio devuelve 0");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, array, 0) == 0, "Si el tamanio del array es 0 devuelve 0.\n");
    printf("  *Inserte 7 elementos en el arbol en el siguiente orden: [5, 3, 7, 2, 4, 6, 8] -\n");
    printf("  -Recorrido postorden completo correcto: [2, 4, 3, 6, 8, 7, 5] -\n");
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */

    pa2m_afirmar(arbol_recorrido_postorden(arbol, array, 2) == 2, "Si el tamanio del array es de 2 y le hago recorrer un arbol con 7 elementos, guarda solo 2.");
    mostrar_array_de_numeros(array, 2);
    pa2m_afirmar(arbol_recorrido_postorden(arbol, array, 30) == 7, "Si el tamanio del array es de 30 y le hago recorrer un arbol de 7 elementos, guarda los 7.");
    mostrar_array_de_numeros(array, 7);

    arbol_destruir(arbol);
}

void probar_abb_con_cada_elemento_generales(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int extra = 0;
    pa2m_afirmar(abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, &mostrar_elemento_numero, &extra) == 0, "Recorrer un arbol inexistente deberia devolver 0.");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, NULL, &extra) == 0, "Si no hay funcion deberia devolver 0.");
    pa2m_afirmar(abb_con_cada_elemento(arbol, 10, &mostrar_elemento_numero, &extra) == 0, "Si el recorrido es invalido (10), deberia devolver 0.");
    pa2m_afirmar(abb_con_cada_elemento(arbol, 0, &mostrar_elemento_numero, &extra) == 0, "Si el arbol esta vacio, deberia recorrer 0 elementos.\n");
    arbol_destruir(arbol);
}

void probar_abb_con_cada_elemento_inorden(){

    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int extra = 0;
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;

    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    printf(" *Inserte 7 elementos en el arbol en el siguiente orden: [5, 3, 7, 2, 4, 6, 8] -\n");
    printf(" -Recorrido inorden completo correcto: [2, 3, 4, 5, 6, 7, 8] -\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */
    printf("\n -Imprimo los elementos del arbol de manera INORDEN con la funcion: mostrar_elemento_numero()\n");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_elemento_numero, &extra) == 7, "Los elementos recorridos deberian ser 7. [2, 3, 4, 5, 6, 7, 8]");
    printf("\n -Imprimo los elementos del arbol de manera INORDEN con la funcion: mostrar_hasta_el_cinco()\n");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_hasta_el_cinco, &extra) == 4, "Los elementos recorridos deberian ser 4. [2, 3, 4, 5]");

   arbol_destruir(arbol);
}

void probar_abb_con_cada_elemento_preorden(){

    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int extra = 0;
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;

    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    printf(" *Inserte 7 elementos en el arbol en el siguiente orden: [5, 3, 7, 2, 4, 6, 8] -\n");
    printf(" -Recorrido preorden completo correcto: [5, 3, 2, 4, 7, 6, 8] -\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */
    printf("\n -Imprimo los elementos del arbol de manera PREORDEN con la funcion: mostrar_elemento_numero()\n");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_elemento_numero, &extra) == 7, "Los elementos recorridos deberian ser 7. [5, 3, 2, 4, 7, 6, 8]");
    printf("\n -Imprimo los elementos del arbol de manera PREORDEN con la funcion: mostrar_hasta_el_cinco()\n");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_hasta_el_cinco, &extra) == 1, "Los elementos recorridos deberia ser 1 porque el cinco esta en el 1º nodo. [5]");

   arbol_destruir(arbol);
}

void probar_abb_con_cada_elemento_postorden(){

    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int extra = 0;
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;

    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    printf(" *Inserte 7 elementos en el arbol en el siguiente orden: [5, 3, 7, 2, 4, 6, 8] -\n");
    printf(" -Recorrido postorden completo correcto: [2, 4, 3, 6, 8, 7, 5] -\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */
    printf("\n -Imprimo los elementos del arbol de manera POSTORDEN con la funcion: mostrar_elemento_numero()\n");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_elemento_numero, &extra) == 7, "Los elementos recorridos deberian ser 7. [2, 4, 3, 6, 8, 7, 5]");
    printf("\n -Imprimo los elementos del arbol de manera POSTORDEN con la funcion: mostrar_hasta_el_cinco()\n");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_hasta_el_cinco, &extra) == 7, "Los elementos recorridos deberian ser 7 porque el cinco esta en el ultimo nodo. [2, 4, 3, 6, 8, 7, 5]");

   arbol_destruir(arbol);
}


void probar_borrar_en_arbol(){

    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    pa2m_afirmar(arbol_borrar(arbol, NULL) == -1, "No deberia poder borrar en un arbol vacío.");
    pa2m_afirmar(arbol_borrar(NULL, NULL) == -1, "No deberia poder borrar si recibe un arbol nulo.");
    
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;
    arbol_insertar(arbol, &a);
    printf("        *Inserto un nodo.\n");
    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar el unico nodo.");
    pa2m_afirmar(arbol->nodo_raiz == NULL, "El arbol quedo vacio.");

    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    printf("\n        *Inserto 7 elementos.-\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */
   
    int h = 9;
    pa2m_afirmar(arbol_borrar(arbol, &h) == -1, "No puedo borrar una elemento que no esta en el arbol.");

    pa2m_afirmar(arbol_borrar(arbol, &d) == 0, "-Puedo borrar una hoja.");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->izquierda == NULL, "El puntero del padre de la hoja borrada apunta a NULL");
    pa2m_afirmar(arbol_borrar(arbol, &g) == 0, "-Puedo borrar otra hoja.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->derecha == NULL, "El puntero del padre de la otra hoja borrada apunta a NULL");
    
    // Ya borre la d y la g.
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                    \           /   
                    e- 4    f- 6      
    */
   
    pa2m_afirmar(arbol_borrar(arbol, &c) == 0, "-Puedo borrar un nodo con un hijo izquierdo.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &f, "El elemento hijo del nodo borrado toma el lugar del elemento borrado.");
    pa2m_afirmar(arbol_borrar(arbol, &b) == 0, "-Puedo borrar un nodo con un hijo derecho.");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &e, "El elemento hijo del nodo borrado toma el lugar del elemento borrado.");

    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                e- 4            f- 6                             
    */
    arbol_destruir(arbol);
}

void probar_borrar_hojas(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 2, d = 7;
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);

    printf("\n  *Inserto 4 elementos [5, 3, 2, 7]-\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            d- 7
                /              
            c- 2          
    */

    pa2m_afirmar(arbol_borrar(arbol, &c) == 0, "-Puedo borrar una hoja. (2)");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->izquierda == NULL, "El puntero del padre de la hoja borrada apunta a NULL");
    pa2m_afirmar(arbol_borrar(arbol, &b) == 0, "-Puedo borrar una hoja. (3)");
    pa2m_afirmar(arbol->nodo_raiz->izquierda == NULL, "El puntero del padre de la hoja borrada apunta a NULL");
    pa2m_afirmar(arbol_borrar(arbol, &d) == 0, "-Puedo borrar una hoja. (7)");
    pa2m_afirmar(arbol->nodo_raiz->derecha == NULL, "El puntero del padre de la hoja borrada apunta a NULL");
    pa2m_afirmar((arbol->nodo_raiz->izquierda == NULL) && (arbol->nodo_raiz->derecha == NULL), "El nodo raiz tiene ambos punteros (izq/der) apuntando a NULL.");
    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar el ultimo nodo que ahora es hoja. (5)");
    pa2m_afirmar(arbol_vacio(arbol) == true, "El arbol quedo vacio.");

    arbol_destruir(arbol);
}

void probar_borrar_nodos_con_un_hijo(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 2, d = 7, e = 6, f = 8, h = 4;
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);

    printf("\n *Inserto 6 elementos [5, 3, 2, 7, 6, 8]-\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            d- 7
                /              /    \
            c- 2           e- 6      f- 8
    */
    /*   BORRAR NODO CON HIJO IZQUIERDO   */
    pa2m_afirmar(arbol_borrar(arbol, &b) == 0, "-Puedo borrar un nodo con un hijo izquierdo (3).");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &c, "El nodo hijo (2) toma el lugar de su padre. (3)");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                c- 2            d- 7
                /              /    \
            NULL           e- 6      f- 8
    */

    /*   BORRAR NODO CON HIJO DERECHO   */
    printf(" *Inserto un (3) nuevamente para que sea el hijo derecho del nodo (2)\n");
    arbol_insertar(arbol, &b); //Inserto b denuevo para probar borrar con hijo derecho.
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                c- 2            d- 7
                    \          /    \
                    b- 3   e- 6      f- 8
    */
    pa2m_afirmar(arbol_borrar(arbol, &c) == 0, "-Puedo borrar un nodo con un hijo derecho (2).");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &b, "El nodo hijo (3) toma el lugar de su padre (2).");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            d- 7
                               /    \
                           e- 6      f- 8
    */

    arbol_borrar(arbol, &b); //Borro el (3) y me queda solo el (5) y su rama derecha del arbol.
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                               \
                               d- 7
                               /    \
                           e- 6      f- 8
    */
    /*   BORRAR NODO CUYO HIJO(IZQUIERDO) TIENE HIJO IZQUIERDO Y DERECHO   */
    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar un nodo (5) que tiene 1 hijo derecho (7) y este tiene 2 hijos (6) y (8).");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &d, "El nodo (7) toma el lugar de su padre (5)");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &e, "El nodo (6) esta en su lugar.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &f, "El nodo (8) esta en su lugar.");
    
    /*          COMO QUEDARIA EL ARBOL
                        d- 7
                    /           \
                e- 6            f- 8
    */
    arbol_borrar(arbol, &f); //Borro el (8).
    arbol_borrar(arbol, &e); //Borro el (6).
    arbol_insertar(arbol, &a); //Inserto (5).
    arbol_insertar(arbol, &h); //Inserto (4).
    arbol_insertar(arbol, &e); //Inserto (6).
    /*          COMO QUEDARIA EL ARBOL
                        d- 7
                    /           
                a- 5   
                /   \      
            h- 4    e- 6
    */
    /*   BORRAR NODO CUYO HIJO(DERECHO) TIENE HIJO IZQUIERDO Y DERECHO   */
    pa2m_afirmar(arbol_borrar(arbol, &d) == 0, "-Puedo borrar un nodo (7) que tiene 1 hijo izquierdo (5) y este tiene 2 hijos (2) y (3).");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &a, "El nodo (5) toma el lugar de su padre (7)");
    
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &h, "El nodo (4) esta en su lugar.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &e, "El nodo (6) esta en su lugar.");

    arbol_destruir(arbol);
}

void probar_borrar_nodos_con_dos_hijos(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 8, b = 2, c = 1, d = 5, e = 3, f = 7, g = 9;
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);

    printf("\n *Inserto 7 elementos el el siguiente orden: [8, 2, 1, 5, 3, 7, 9]-\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 8
                    /           \
                b- 2            g- 9
                /   \         
            c- 1     d- 5         
                    /    \
                e- 3     f- 7
    */

    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar un nodo con dos hijos (8).");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &f, "El nodo, mayor de los menores, (7) toma el lugar del nodo borrado (8)");
    //printf("%i\n", *(int*)(arbol->nodo_raiz->izquierda->derecha->elemento));
    pa2m_afirmar(arbol->nodo_raiz->izquierda->derecha->derecha == NULL, "La posicion en la que estaba el nodo recolocado (el mayor de los menores) ahora es NULL.");
    /*          COMO QUEDARIA EL ARBOL
                        f- 7
                    /           \
                b- 2            g- 9
                /   \         
            c- 1     d- 5         
                    /    
                e- 3     
    */
    pa2m_afirmar(arbol_borrar(arbol, &f) == 0, "-Puedo borrar un nodo con dos hijos (7).");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &d, "El nodo, mayor de los menores, (5) toma el lugar del nodo borrado (7)");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->derecha->elemento == &e, "El hijo izquierdo del nodo recolocado toma la posicion de este.");
    /*          COMO QUEDARIA EL ARBOL
                        d- 5
                    /           \
                b- 2            g- 9
                /   \         
            c- 1     e- 3             
    */
    pa2m_afirmar(arbol_borrar(arbol, &d) == 0, "-Puedo borrar un nodo con dos hijos (5).");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &e, "El nodo, mayor de los menores, (3) toma el lugar del nodo borrado (5)");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->elemento == &b, "El hijo izquierdo del nuevo nodo raiz es correcto.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &g, "El hijo izquierdo del nuevo nodo raiz es correcto.");
    /*          COMO QUEDARIA EL ARBOL
                        e- 3
                    /           \
                b- 2            g- 9
                /            
            c- 1                  
    */
    printf("Borro el nodo (1) para probar un tipo de borrado.\n");
    arbol_borrar(arbol, &c);
    /*          COMO QUEDARIA EL ARBOL
                        e- 3
                    /           \
                b- 2            g- 9               
    */
    pa2m_afirmar(arbol_borrar(arbol, &e) == 0, "-Puedo borrar un nodo con dos hijos (3).");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &b, "El nodo, mayor de los menores, (2) toma el lugar del nodo borrado (3)");
    pa2m_afirmar(arbol->nodo_raiz->izquierda == NULL, "El hijo izquierdo del nuevo nodo raiz es null.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &g, "El hijo izquierdo del nuevo nodo raiz es correcto.");

    arbol_destruir(arbol);
}
/*
void probar_hallar_padre_nodo_extremo_derecho(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 8, b = 2, c = 1, d = 5, e = 3, f = 7, g = 9;
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);

    printf("\n *Inserto 7 elementos el el siguiente orden: [8, 2, 1, 5, 3, 7, 9]-\n");*/
    /*          COMO QUEDARIA EL ARBOL
                        a- 8
                    /           \
                b- 2            g- 9
                /   \         
            c- 1     d- 5         
                    /    \
                e- 3     f- 7
    */
/*
    nodo_abb_t* nodo_padre = hallar_padre_nodo_extremo_derecho(arbol->nodo_raiz->izquierda);
    //printf("%i\n", *(int*)nodo_padre->elemento);
    pa2m_afirmar(nodo_padre->elemento == &d, "El nodo padre del nodo (mayor de los menores) es correcto.");
    arbol_borrar(arbol, &f);
    printf(" -Elimine el nodo 7 para realizar una nueva prueba.\n");
    nodo_padre = hallar_padre_nodo_extremo_derecho(arbol->nodo_raiz->izquierda);
    pa2m_afirmar(nodo_padre->elemento == &b, "El nodo padre del nodo (mayor de los menores) es correcto.");
    arbol_destruir(arbol);
}
*/

int main(){
    /*PRUEBAS*/
    pa2m_nuevo_grupo("PRUEBAS DE CREACION DE ARBOL");
    probar_crear_arbol();

    pa2m_nuevo_grupo("PRUEBAS DE INSERCION EN ARBOL");
    probar_insertar_en_arbol();

    pa2m_nuevo_grupo("PRUEBAS DE BUSQUEDA");
    probar_buscar_en_arbol();

    
    pa2m_nuevo_grupo("PRUEBAS DE BORRADO GENERAL");
    probar_borrar_en_arbol();
    pa2m_nuevo_grupo("PRUEBAS DE BORRADO DE HOJAS");
    probar_borrar_hojas();
    pa2m_nuevo_grupo("PRUEBAS DE BORRADO DE NODOS CON UN HIJO");
    probar_borrar_nodos_con_un_hijo();
    pa2m_nuevo_grupo("PRUEBAS DE BORRADO DE NODOS CON DOS HIJOS");
    probar_borrar_nodos_con_dos_hijos();


    pa2m_nuevo_grupo("PRUEBAS DE ARBOL RAIZ");
    probar_arbol_raiz();
    pa2m_nuevo_grupo("PRUEBAS DE ARBOL VACIO");
    probar_arbol_vacio();

    pa2m_nuevo_grupo("PRUEBAS DE RECORRIDO INORDEN");
    probar_arbol_recorrido_inorden();
    pa2m_nuevo_grupo("PRUEBAS DE RECORRIDO PREORDEN");
    probar_arbol_recorrido_preorden();
    pa2m_nuevo_grupo("PRUEBAS DE RECORRIDO POSTORDEN");
    probar_arbol_recorrido_postorden();

    pa2m_nuevo_grupo("PRUEBAS DE ITERADOR GENERALES");
    probar_abb_con_cada_elemento_generales();

    pa2m_nuevo_grupo("PRUEBAS DE ITERADOR INORDEN");
    probar_abb_con_cada_elemento_inorden();

    pa2m_nuevo_grupo("PRUEBAS DE ITERADOR PREORDEN");
    probar_abb_con_cada_elemento_preorden();

    pa2m_nuevo_grupo("PRUEBAS DE ITERADOR POSTORDEN");
    probar_abb_con_cada_elemento_postorden();

    
    
    //pa2m_nuevo_grupo("PRUEBAS DE HALLAR PADRE DEL NODO (MAYOR DE LOS MENORES)");
    //probar_hallar_padre_nodo_extremo_derecho();

    return pa2m_mostrar_reporte();
} 