#include <stdlib.h>
#include <stdio.h>




#include "sistemaEntrada.h"
#include "analizadorLexico.h"
#include "abb.h"
#include "tablaSimbolos.h"
#include "definiciones.h"
#include "analizadorSintactico.h"


#include <unistd.h>
#include <string.h>


int main(){
    //Declaramos variable del tipo tabla de simbolos, de momento a null
    tablaSimbolos tabla=NULL;
    //Inicializamos sistema de entrada
    inicializarSE("concurrentSum.go");
    //Inicializamos tabla
    inicializarTabla(&tabla);
    //Imprimimos estado inicial da tabla
    printf("\nTABLA DE SIMBOLOS\n");
    ImprimirTabla(tabla);
    printf("\n\n");
    //Inicializamos o analizador léxico
    InicializarLexico(tabla);
    
    //Iniciamos o analizador sintáctico, que comenzará a pedir componentes léxicos ao analizador léxico
    iniciarAnalisis();

    //Imprimimos estado final tabla de símbolos
    printf("\nTABLA DE SIMBOLOS\n");
    ImprimirTabla(tabla);
    printf("\n\n");
    //Liberamos a memoria da tabla
    destruirTabla(&tabla);
    tabla=NULL;
    //cerramos o fichero de entrada
    cerrar();
}