#include <stdlib.h>
#include <stdio.h>
#include "errores.h"

#define TB 40 //tamaño búffer sin incluir centinelas

//tamaño de ambos buffers conjuntos que se utilizará para trabajar sobre el array
int N=TB*2;
//Doble búffer
char buffer[TB*2+2];//+2 por los centinelas
//puntero al fichero de entrada
FILE *f;
//apuntadores de navegación sobre el búffer
int inicio=0, delantero=0;
//contador para comprobar si se supera tam máximo lexema
int numcleidos=0;



//Funcion para pruebas imprimir buffer
void Imprimir(){
    int i=0;
    printf("===");
    for(i=0;i<N+2;i++){
        if(buffer[i]==EOF){
            printf("|EOF");
        }
        else{
            printf("|%d%c",i,buffer[i]);
        }
    }
    printf("===\n\n");
}

//Función para cargar un bloque
void cargarBloque(int bloqueACargar){
    //se establece o punto de partido dependendo si se desea cargar un ou outro (0 or 1)
    int comienzo= ((N/2+1))*bloqueACargar;
    
    size_t numdatos;
    //leemos bloque do fichero
    numdatos=fread(&buffer[comienzo],sizeof(char),N/2,f);
    //En caso de que non se lea bloque completo se añade o EOF correspondiente a fin de fichero
    if(numdatos<N/2){
        buffer[comienzo+numdatos]=EOF;
    }
}
//Función que permite al inicializar el sistema de entrada
int inicializarSE(char * nombreFichero){
    //Abrimos fichero
    f= fopen(nombreFichero,"r");
    if(f==NULL){
        ImprimirError(0);
    }
    //establezco centinelas
    buffer[N/2]=EOF;
    buffer[N+1]=EOF;
    //cargo primer bloque
    cargarBloque(0);
    return 0;
}
//Función para cerrar fichero de entrada
int cerrar(){
    if(fclose(f)==0){
        return 0;
    }
    else{
        ImprimirError(4);
        return -1;
    }
}
//función que comprueba si inicio y delantero están en el mismo buffer
int mismobuffer(){
    if((inicio<=N/2 && delantero<=N/2) ||(inicio>N/2 && delantero>N/2) ){
        return 0; //están no mesmo
    }
    return 1;//están en diferentes
}
//Función que permite al analizador léxico pedir el siguiente caracter a procesar
char* leerCaracter(){
    char * caracter;
    //leemos
    caracter=&buffer[delantero];
    //comprobación de si se lee un fin de fichero
    if(*caracter==EOF){
        //Comprobamos si nos encontramos al final del buffer izquierdo
        if(delantero==N/2){
            //cargamos bloque en el buffer derecho
            cargarBloque(1);
            //incrementamos delantero
            delantero++;
            //leemos el siguiente caracter
            caracter=&buffer[delantero];
        }
        //Comprobamos si nos encontramos al final del buffer derecho
        else if(delantero==N+1){
            //cargamos bloque en el buffer izquierdo
            cargarBloque(0);
            //reseteamos delantero
            delantero=0;
            //leemos el siguiente caracter
            caracter=&buffer[delantero];
        }
        else{
            //o EOF pertenece ao propio fichero, o devolvemos
            return caracter;
        }
        
    } 
    //incrementamos contador para detectar tam máximo lexema
    numcleidos++;
    //incrementamos delantero
    delantero++;
    return caracter;
}
//Función que permite al analizador léxico devolver caracteres
void devolverCaracter(){
    //decrementamos delantero
    delantero--;
}
//Función que permite al analizador léxico recuperar la cadena completa una vez llega al estado de aceptación
char * leerPalabra(int devolverCopia){
    char * palabra;
    int tam=0,i=0;
    //Calculamos tamaño de la palabra
    //Para ello tenemos en cuenta 3 casos:
    if(mismobuffer()==0){//que estén en el mismo buffer
        tam=delantero-inicio;
    }
    else{//que estén en distintos buffers
        if(inicio>delantero){ //inicio está en el buffer de la derecha
            tam=N-inicio+1+delantero;
        }
        else{//inicio está en el buffer de la izquierda
            tam=delantero-inicio-1;
        }
    }
    //en el caso de que en llamada devolverCopia esté a uno se debe reservar memoria para la cadena y devolver su puntero
    if(devolverCopia){
        //Comprobación no se supero tam máximo lexema
        if(numcleidos<=N/2){
            //Reservamos memoria
            palabra=malloc((tam+1)*sizeof(char));
            //Copiamos a cadena desde o buffer
            for(i=0;i<tam;i++){
                palabra[i]=buffer[inicio];
                if (inicio==N){//ultima posición buffer dereito saltamos EOF
                    inicio=0;
                }
                else if(inicio==(N/2)-1){//ultima posicion buffer esquerdo saltamos EOF
                    inicio+=2;
                }
                else{
                    inicio++;
                }
            }
            //incluimos fin de cadena
            palabra[tam]='\0';
            //lexema procesado reestablecemos contador
            numcleidos=0;
            return palabra;
        }
        else{
            //se supera tam max lexema, sacamos error
            ImprimirError(1);
            //reservamos memoria para tam max lexema mas caracter vacío
            palabra=malloc(((N/2)+1)*sizeof(char));
            inicio=delantero-1;//partimos do último caracter
            //retrocedemos tantos caracteres como o tamaño máximo de lexema
            for(i=0;i<N/2;i++){
                palabra[(N/2)-1-i]=buffer[inicio];
                inicio--;
                if(inicio==N/2){//estaría sobre un EOF retrocedemos outra
                    inicio--;
                }
                else if(inicio==-1){//comienzo buffer izquierdo al retroceder pasamos a fin buffer derecho
                    inicio=N;
                }
            }
            //devolvemos inicio a nova posición de lectura
            inicio=delantero;
            palabra[N/2]='\0';
            numcleidos=0;
            return palabra;
        }
    }
    else{
        //en este caso se procesa palabra pero no se guarda en memoria ni se devuelve al analizador léxico
        if (inicio==N && delantero==N+1){//ultima posición buffer dereito saltamos EOF
                inicio=0;
            }
            else if(inicio==(N/2)-1 && delantero==N/2){//ultima posicion buffer esquerdo saltamos EOF
                inicio=delantero+1;
            }
            else{
                inicio=delantero;
            }
        numcleidos=0;
        return NULL;
    }

}

