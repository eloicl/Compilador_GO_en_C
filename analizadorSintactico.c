#include <stdlib.h>
#include <stdio.h>



#include "analizadorLexico.h"
#include "abb.h"
#include "definiciones.h"

//Función para iniciar el análisis sintáctico
void iniciarAnalisis(){
    //declaramos elemento co que traballaremos, de momento vacío
    tipoelem e;
    e.lexema=NULL;
    int flag=0;
    //Comezo bucle productor-consumidor
    do{
        //Pedimos siguiente componente léxico
        e= siguienteComponenteLexico();
        
        //Activamos o flag para salir do bucle en caso de que se chegue ao fin de fichero
        if(e.lexema[0]==EOF){
            flag=1;
        }
        //si no imprimimos o componente consumido
        else{
            //O imprimimos
            printf("<%s,%d>\n",e.lexema,e.ID);
        }
        /*Liberamos a memoria do lexema en caso de que este non esté na tabla de símbolos,
        é decir, excluimos identificadores e palabras reservadas (están entre o rango 300-309)*/
        if(e.ID!=ID_ && (e.ID>309||e.ID<300)){
            free(e.lexema);
            e.lexema=NULL;
        }

    }while(flag==0);
    //Fin análisis
}