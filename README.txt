Hago entrega del TDA ABB (Arbol binario de busqueda) con todos los metodos necesarios solicitados para su uso. Asi como tambien un archivo con las pruebas necesarias para comprobar el correcto funcionamiento del TDA.

-Compilacion:
    gcc *.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

-Ejecucion:
    ./abb

-Valgrind:
    valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

1- ARBOL BINARIO DE BUSQUEDA

Un arbol binario de busqueda es una estructura para almacenar datos. Esta consta de un conujunto de nodos (en donde se almacenan los elementos) los cuales cada nodo puede tener:
-0 hijos.
-1 hijo izquierdo o derecho .
-2 hijos (izquierdo y derecho). 
El arbol esta formado por un nodo raiz, que seria la "cima" del arbol, a partir de la cual se puede acceder al arbol y realizar las acciones que uno desee.
La caracteristica fundamental del ABB (Arbol binario de busqueda) es que cuenta con una regla para ordenar los elementos en el arbol. Por ejemplo, si dicha regla es la de ordenar los numeros de "menor a mayor", partiendo del nodo raiz, todos los elementos de los nodos que se encuentren a la izquierda seran menores al elemento del nodo raiz, y todos los elementos a la derecha de este seran mayores. Esta misma regla se mantiene con todos los nodos del arbol.
La diferencia entre un ABB y un arbol binario normal es que gracias a la "regla de ordenamiento" de los ABB previamente mencionada, se facilita la busqueda, insercion y borrado de los elementos del arbol ya que mediante el uso de un comparador, es posible descartar secciones del arbol en las que se sabe que el nodo buscado no se encuentra y asi agilizar dicha operacion.

2- FUNCION DE DESTRUCCION

El objetivo de tenes una funcion de destruccion es el de facilitarle la destruccion del arbol al usuario. 
La funcion de destruccion es la que se encarga de liberar los elementos que se encuentran en los nodos del arbol. Si bien la funcion de arbol destruir se encarga de eliminar el arbol y sus nodos, puede surgir el caso en donde el usuario desee almacenar en los nodos elementos que se encuentran en el heap. Por lo cual el hecho de contar con una funcion destructora le permite al arbol, antes de ser liberado, liberar todos los elementos.

En caso de que el usuario desee guardar en el arbol elementos que se encuentran en el heap y NO existiese la funcion de destruccion, el usuario deberia hacerse cargo y antes de destruir el arbol, eliminar manuealmente cada elemento que insertó para asi no tener problemas de perdida de memoria.
En caso de almacenar puramente elementos que se encuentran en el stack, no seria necesario proveer la funcion destructora.

3- COMPLEJIDAD DE OPERACIONES

- arbol_crear() --> Es O(1) 
    Porque siempre, lo unico que se tiene que hacer es reservar el espacio de memoria (en el heap) necesario para crear el arbol.

- arbol_vacio() --> Es O(1)
    Ya que siempre, lo unico que hace es ver el nodo raiz del arbol que recibe. Sin importar que tan grande o chico sea el arbol, siempre le cuesta lo  mismo.

- arbol_raiz() --> Es O(1)
    Ya que al igual que arbol_vacio(), lo unico que tiene que hacer es ver el elemento del nodo raiz. 

- arbol_busqueda() --> Es en promedio O(logN) y en el peor de los casos O(N)
    Ya que al ser un ABB cuenta con un criterio que fue utilizado para su organizacion. Por lo tanto, al realizar la busqueda, dicha busqueda se va a tratar de una busqueda binaria y gracias al uso de un comparador, es posible descartar ciertas ramas del arbol en donde se sabe que no se encunetra el elemento buscado y de esta forma agilizar la operacion.
El mejor de los casos se da cuando el arbol se encuentra balanceado, ya que al realizar la comparacion y definir hacia qeu lado seguir la busqueda, se esta descartando cerca de la mitad de los elementos y asi sucesivamente hasta hallar el elemento buscado.
Sin embargo, en el caso de se inserten elementos de menor a mayor, el arbol quedaria igual que una lista y dejaria de tener complejidad O(LogN) y pasaria a ser O(N) ya que la busqueda dejaria de ser binaria.


- arbol_insertar() y arbol_borrar() --> Son en promedio O(logN) y el el peor de los casos O(N)
    Al igual que en la busqueda, en los metodos de insertar y borrar se tiene que primero BUSCAR el lugar en el que se insertar/borrar dicho elemento, y para eso es necesario recorrer el arbol de la misma forma que se detallo en la busqueda. 

- arbol_recorrido_inorden(), arbol_recorrido_preorden(), arbol_recorrido_postorden() y abb_con_cada_elemento --> Son O(N)
    Estas funciones son todas O(N) ya que siempre terminan recorriendo todos los elementos que hay en el arbol (a menos que en el iterador se pase una funcion que corte al encontrar X cantidad de elementos), sin importar el orden que se le pida.

- arbol_destruir() --> Es O(N)
    Porque para destruir todos los nodos, se tiene que recorrer cado nodo perteneciente al arbol.


