#ifndef SE_H
#define SE_H
//Función que permite al inicializar el sistema de entrada
int inicializarSE(char * nombreFichero);
//Función que permite al analizador léxico pedir el siguiente caracter a procesar
char * leerCaracter();
//Función que permite al analizador léxico devolver caracteres
void devolverCaracter();
//Función que permite al analizador léxico devolver la cadena completa una vez llega al estado de aceptación
char * leerPalabra();
//Función para cerrar fichero de entrada
int cerrar();




#endif	
