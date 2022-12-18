#include <stdlib.h>
#include <stdio.h>
//Función que imprime la información del error producido
void ImprimirError(int codigo){
    switch(codigo){
        case 0:
            printf("\n==Error abriendo el archivo==\n\n");
            exit(1);
            break;
        case 1:
            printf("\n==Error tamaño máximo de lexema superado==\n\n");
            break;
        case 2:
            printf("\n==Error de formato en el número==\n\n");
            break;
        case 3:
            printf("\n==Error de formato en el string==\n\n");
            break;
        case 4:
            printf("\n==Error cerrando el archivo==\n\n");
            exit(1);
            break;
    }
}