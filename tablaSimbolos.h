#ifndef TS_H
#define TS_H
#include "abb.h"

//Redefinimos el tipo con un nombre adecuado al contexto
typedef abb tablaSimbolos; 

//Función para reservar memoria e inicializar la tabla
void inicializarTabla(tablaSimbolos* tabla);
//Función para destruir la tabla, liberando su memoria
void destruirTabla(tablaSimbolos* tabla);
//Función a exportar que permitirá al analizador preguntar acerca de un lexema
void esPalabraReservada (tipoelem * r, tablaSimbolos* tabla);
//Función para imprimir la tabla de símbolos
void ImprimirTabla(tablaSimbolos tabla);
#endif	
