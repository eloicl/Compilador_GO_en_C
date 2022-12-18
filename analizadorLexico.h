#ifndef AL_H
#define AL_H


#include "tablaSimbolos.h"
    //Función para inicializar el analizador léxico, pasándole la tabla de símbolos creada en main
    void InicializarLexico(tablaSimbolos tabla);
    //Función que permite al analizador sintáctico solicitar el siguiente componente léxico
    tipoelem siguienteComponenteLexico();


#endif	