#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "abb.h"
#include "tablaSimbolos.h"

#include "definiciones.h"


//Función a exportar que permitirá al analizador preguntar acerca de un lexema
void esPalabraReservada (tipoelem * r, tablaSimbolos* tabla){
    //declaramos variable donde se guardará el resultado de la búsqueda
    tipoelem elemento;
    elemento.ID=0;
    //buscamos el nodo
    buscar_nodo(*tabla, r->lexema,&elemento);
    //Comprobamos si ha cambiado el ID(estaba en la tabla) o si sigue a su valor de inicialización
    if(elemento.ID!=0){
        //existe el elemento, liberamos la memoria del lexema recién leído, ya que ya existe una copia en la tabla
        free(r->lexema);
        //establecemos en el tipoelem pasado como parámetro la dirección del lexema almacenado en la tabla
        //y el ID de esta
        r->lexema=elemento.lexema;
        r->ID=elemento.ID;

    }
    else{
        //no existía el elemento, por lo que será un identificador, establecemos el ID
        r->ID=ID_;
        //insertamos el tipoelem en la tabla
        insertar(tabla,*r);

    }
}
//Función para reservar memoria e inicializar la tabla
void inicializarTabla(tablaSimbolos* tabla){
    int i=0,tam=0;
    //Inicializamos la estructura de la tabla, reservando memoria para esta
    crear(tabla); 
    //arrays para la inicialización   
    char palabrasreservadas[10][20]={"chan","for","func","go","if","import","package","range","return","var"};
    int compslexicos[10]={CHAN,FOR,FUNC,GO,IF,IMPORT,PACKAGE,RANGE,RETURN,VAR};
    
    
    //Para cada palabra reservada
    for(i=0;i<10;i++){
        tipoelem e;
        //Reservamos memoria dinámicamente
        tam=strlen(palabrasreservadas[i]);
        e.lexema=malloc((tam+1)*sizeof(char));
        //Copiamos la cadena
        strcpy(e.lexema,palabrasreservadas[i]);
        //Añadiendo el fin de cadena
        e.lexema[tam]='\0';
        //Incluimos comp léxico
        e.ID=compslexicos[i];
        //Insertamos el elemento en la tabla
        insertar(tabla,e);
    }
}
//Función para imprimir la tabla de símbolos
void ImprimirTabla(tablaSimbolos tabla){
    tipoelem elem;
    //Mientras no sea vacío exploramos el árbol imprimiendo la información de cada nodo
    if(!es_vacio(tabla)){
        ImprimirTabla(izq(tabla));//lamada recursiva
        leer(tabla,&elem);//lectura información nodo
        printf("%s:%d\n",elem.lexema,elem.ID);
        ImprimirTabla(der(tabla));//lamada recursiva
    }
    
}
//Función para destruir la tabla, liberando su memoria
void destruirTabla(tablaSimbolos* tabla){
    destruir(tabla);
}
