#include "abb.h"
#include <stdio.h>
#include "pa2mm.h"

/*               COMPARADORES Y DESTRUCTORES                */
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
    free((int*)numero);
}


/*****            PRUEBAS             *****/

void probar_crear_arbol(){
    abb_t* arbol;

    pa2m_afirmar((arbol_crear(NULL, NULL)) == NULL, "No deberia poder crear un arbol si paso NULL como comparador.")
    arbol = arbol_crear(&comparador_numeros, &destructor_de_numeros);
    pa2m_afirmar(arbol != NULL, "Puedo crear el árbol");
    pa2m_afirmar(arbol->nodo_raiz == NULL, "El arbol inicia con nodo_raíz igual a NULL");
    pa2m_afirmar(arbol->comparador == &comparador_numeros, "El comparador del árbol apunta al comparador recibido");
    pa2m_afirmar(arbol->destructor == &destructor_de_numeros, "El destructor del árbol apunta al destructor recibido");
    
    //Faltan los free.
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
    printf("Inserte 4 elementos (Manzanas).\n");

    pa2m_afirmar(arbol_buscar(arbol, &manzana) == &manzana, "Encontre la manzana");
    pa2m_afirmar(arbol_buscar(arbol, &otra_manzana) == &otra_manzana, "Encontre la otra manzana");
    pa2m_afirmar(arbol_buscar(arbol, &tercer_manzana) == &tercer_manzana, "Encontre la tercer manzana");
    pa2m_afirmar(arbol_buscar(arbol, &cuarta_manzana) == &cuarta_manzana, "Encontre la cuarta manzana");
    pa2m_afirmar(arbol_buscar(arbol, &quinta_manzana) == NULL, "No deberia poder encontrar una manzana que no esta en el arbol.");
    pa2m_afirmar(arbol_buscar(arbol, &manzana_buscada) == &manzana, "Encuentro la manzana buscada porque tiene el mismo peso que la 1ª manzana.");
    // Pude encontrar la manzana buscada porque el comparador asociado al arbol se fija solo en el peso. Y como la manzana buscada y la manzana tienen el mismo peso..
    arbol_destruir(arbol);
}
void probar_borrar_nodo_con_dos_hijos_arboles(){
    //printf("\n-Pruebo borrar un nodo con 2 hijos (y cuyo hijo izquierdo tiene uno o mas hijos derechos.)");
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    printf("-Inserto 7 elementos.-\n");
    /*          COMO QUEDARIA EL ARBOL
                        a- 5
                    /           \
                b- 3            c- 7
                /   \           /   \
            d- 2    e- 4    f- 6    g- 8    
    */
    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar un nodo con dos hijos. (Y su hijo izquierdo tiene un hijo derecho.)");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &e, "El nodo extremo derecho se convierte en raiz.");
    pa2m_afirmar(arbol->nodo_raiz->izquierda->derecha == NULL, "Se borra el nodo extremo derecho.");
    /*          COMO QUEDARIA EL ARBOL
                        e- 4
                    /           \
                b- 3            c- 7
                /               /   \
            d- 2             f- 6    g- 8    
    */

   arbol_destruir(arbol);
}   
void probar_borrar_nodo_con_dos_hijos_hojas(){
    //printf("\n-Pruebo borrar un nodo con 2 hijos (hijos hojas)\n");
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 6;
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);

    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar un nodo con dos hijos.");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &b, "El hijo izquierdo se convierte en raiz.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &c, "La raiz (5) apunta al nodo que contiene al elemento (6).");

    arbol_destruir(arbol);
}
void probar_borrar_en_arbol(){

    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    pa2m_afirmar(arbol_borrar(arbol, NULL) == -1, "No deberia poder borrar en un arbol vacío.");
    pa2m_afirmar(arbol_borrar(NULL, NULL) == -1, "No deberia poder borrar si recibe un arbol nulo.");
    
    int a = 5, b = 3, c = 7, d = 2, e = 4, f = 6, g = 8;
    arbol_insertar(arbol, &a);
    printf("        -Inserto un nodo.\n");
    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar el unico nodo.");
    pa2m_afirmar(arbol->nodo_raiz == NULL, "El arbol quedo vacio.");

    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);
    arbol_insertar(arbol, &d);
    arbol_insertar(arbol, &e);
    arbol_insertar(arbol, &f);
    arbol_insertar(arbol, &g);
    printf("\n        -Inserto 7 elementos.-\n");
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
    *//*
    printf("\n-Ahora lo que queda es un nodo con dos hijos nada mas. 5, 4, 6 (Recorrido IND)\n");
    pa2m_afirmar(arbol_borrar(arbol, &a) == 0, "-Puedo borrar un nodo con dos hijos.");
    pa2m_afirmar(arbol->nodo_raiz->elemento == &e, "El hijo izquierdo se convierte en raiz.");
    pa2m_afirmar(arbol->nodo_raiz->derecha->elemento == &f, "La raiz (5) apunta al nodo que contiene al elemento (6).");*/

    arbol_destruir(arbol);
}

void mostrar_array_de_numeros(void** array, int cantidad){
    for (int i = 0; i < cantidad; i++){
        printf("%i, ", *(int*)array[i]);
    }
}

void probar_arbol_recorrido_inorden(){
    abb_t* arbol = arbol_crear(&comparador_numeros, NULL);
    int a = 5, b = 3, c = 6;
    void* array[10];
    pa2m_afirmar(arbol_recorrido_inorden(NULL, array, 2) == 0, "Si el arbol es nulo devuelve 0.");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, array, 2) == 0, "Si el arbol esta vacio devuelve 0");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, array, 0) == 0, "Si el tamanio del array es 0 devuelve 0.");
    arbol_insertar(arbol, &a);
    arbol_insertar(arbol, &b);
    arbol_insertar(arbol, &c);

    pa2m_afirmar(arbol_recorrido_inorden(arbol, array, 3) == 3, "Recorre todos los elementos. (Hay 3 en el arbol)");
    mostrar_array_de_numeros(array, 3);

    arbol_destruir(arbol);
}


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
    pa2m_nuevo_grupo("PRUEBAS DE BORRADO CON NODO DE DOS HIJOS HOJAS");
    probar_borrar_nodo_con_dos_hijos_hojas();
    pa2m_nuevo_grupo("PRUEBAS DE BORRADO CON NODO DE DOS HIJOS ARBOLES");
    probar_borrar_nodo_con_dos_hijos_arboles();
    pa2m_nuevo_grupo("PRUEBAS DE RECORRIDO INORDEN");
    probar_arbol_recorrido_inorden();

    return pa2m_mostrar_reporte();
} 