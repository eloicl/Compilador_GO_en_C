#include <stdlib.h>
#include <stdio.h>
#include "sistemaEntrada.h"
#include "abb.h"
#include "definiciones.h"
#include "tablaSimbolos.h"
#include "errores.h"


#include <unistd.h>

tablaSimbolos TablaSimbolos;

//Función para inicializar el analizador léxico, pasándole la tabla de símbolos creada en main
void InicializarLexico(tablaSimbolos tabla){
    TablaSimbolos=tabla;
}

//Función que representa o autómata de recoñecemento de strings
tipoelem autString(){
    //leeuse unhas " que indican que empeza un string
    //Necesitaremos almacenar el último caracter
    int estado=0;
    char * c;
    do{
        c=leerCaracter();
        if(estado==0){
            if(*c=='\\'){// lee \ pode implicar escapar comillas cambiamos de estado
                estado=1;
            }
            else if(*c=='"'){//cierra comillas acaba string
                estado=2;
            }
            else if((32<=*c)&&(*c<=126)){
                //sigue en este estado de ser un caracter válido
            }
            else{
                break;
            }
        }
        else if(estado==1){//en este estado se ten lido unha barra de escpae
            if((32<=*c)&&(*c<=126)){//leemos caracter válido volvemos a estado inicial
                estado=0;
            }
            else{
                break;
            }
        }
        else if(estado==2){
            break;
        }
    }while(1); 
    //Chegamos a estado de aceptación
    if(estado==2){
        //estado de aceptación
    }
    else{
        ImprimirError(2);
    }
    //Pedimos al sistema de entrada la cadena completa
    devolverCaracter();
    tipoelem resul;
    resul.lexema=leerPalabra(1);
    resul.ID=STRING;
    return resul;

}
//Función que representa o autómata de recoñecemento de comentarios
int autComent(){
    //cheagmos lendo un /
    int estado=0;
    char * c;
    do{
        c=leerCaracter();
        if(estado==0){
            if(*c=='*'){//leeu /*
                estado=1;
            }
            else if(*c=='/'){//leeu //
                estado=4;
            }
            else{
                //en este caso é unha sola / pode ser da división
                //devolvemos caracter retornamos false para que este pase a otro autómata
                devolverCaracter();
                return 0;
            }
        }
        else if(estado==1){
            if(*c=='*'){//leeu /*...*
                estado=2;
            }
            //si no permanece no que está con calquer outro caracter
        }
        else if(estado==2){
            if(*c=='/'){//comentario /*...*/ completado
                estado=3;
            }
            else if(*c=='*'){
                //seguimos en este estado
            }
            else{
                //volvemos al estado de no haber leído el *
                estado=1;
            }
        }
        else if(estado==3){
            break;
        }
        else if(estado==4){
            if(*c=='\n'||*c==EOF){//fin línea o fichero implica fin de comentario de línea
                estado=5;
            }
            else{
                //seguimos en este estado
            }
        }
        else if(estado==5){
            break;
        }
    }while(1); 
    devolverCaracter();
    leerPalabra(0);//pasamos cero xa que non deseamos a copia da palabra xa que os comentarios se eliminan
    return 1;

}
//Función que representa o autómata de recoñecemento de cadenas alphanuméricas
tipoelem autAlpha(){

    char *c;
    //Leemos caracteres mientras sean a-z A-Z 0-9 o _
    do{
        c=leerCaracter();
    }while((*c<=90 && *c>=65)||(*c>=97 && *c<=122)||*c==95||(*c>=48 && *c<=57));
    //devolvemos caracter que implicou salida do bucle
    devolverCaracter();
    //creamos elemento pedindo a palabra ao SE
    tipoelem resul;
    resul.lexema=leerPalabra(1);
    //Consultamos tabla de símbolos
    esPalabraReservada(&resul,&TablaSimbolos);
    return resul;
}
//Función que representa o autómata de recoñecemento de números
tipoelem autNumeros(char *caracter){ //se entra al leer un número o un .
    int estado=0;
    char *c=caracter;
    do{
        if(estado==0){//estado inicial antes de procesar ningún caracter
            //Transicións
            if(*c=='0'){
                estado=1;
            }
            else if(*c>='1'&& *c<='9'){
                estado=2;
            }
            else if(*c=='.'){
                estado=3;
            }
            else{
                break;
            }
        }
        else if(estado==1){ //Estado 1 se leu un 0, polo que pode ser un número hexadecimal aínda
            if(*c>='0'&& *c<='9'){
                estado=2;
            }
            else if(*c=='.'){//transición a punto flotante
                estado=6;
            }
            else if(*c=='e' || *c=='E'){//transición a exponente
                estado=7;
            }
            else if(*c=='x' || *c=='X'){//transición a hexadecimal
                estado=4;
            }
            else if(*c=='i'){
                //pasa a estado de números imaginarios
                estado=10;
            }
            else{
                break;
            }
        }
        else if(estado==2){//estado 2 se leeron 2 dígitos entre 0-9
            if((*c>='0'&& *c<='9')){//sigue en este estado mentres lea dígitos entre 0 e 9
            }
            else if(*c=='i'){
                //pasa a estado de números imaginarios
                estado=10;
            }
            else if(*c=='.'){//pasa a detectar números flotantes
                estado=6;
            }
            else if(*c=='e' || *c=='E'){//pasa a detectar exponente
                estado=7;
            }
            else{
                break;
            }
        }
        else if(estado==3){//se leu un punto
            if(*c>='0'&& *c<='9'){//se lee un dígito pasa a detectar puntos flotante
                estado=6;
            }
            else{
                break;
            }
        }
        else if(estado==4){//leuse unha x de hexadecimal
            //pasamos a estado final si se lee un dígito hexadecimal 0-9 a-f A-F
            if(*c>='0'&& *c<='9'){
                estado=5;
            }
            else if(*c>='a'&& *c<='f'){
                estado=5;
            }
            else if(*c>='A'&& *c<='F'){
                estado=5;
            }
            else{
                break;
            }
        }
        else if(estado==5){//número hexadecimal
            //permanecemos en estado final si se lee un dígito hexadecimal 0-9 a-f A-F
             if((*c>='0'&& *c<='9')){
                 //se mantiene
            }
            else if (*c=='i'){
                //pasa a estado de números imaginarios
                estado=10;
            }
            else if(*c>='a'&& *c<='f'){
                //se mantiene
            }
            else if(*c>='A'&& *c<='F'){
                //se mantiene
            }
            else{
                break;
            }
        }
        else if(estado==6){//estado de lectura parte decimal
             if((*c>='0'&& *c<='9')){
                //sigue en 6 mentras lea dígitos
            }
            else if (*c=='i'){
                //pasa a estado de números imaginarios
                estado=10;
            }
            else if(*c=='e' || *c=='E'){
                //pasa a estado de recoñecemento de exponente
                estado=7;
            }
            else{
                break;
            }
        }
        else if(estado==7){//lectura exponente
            //Pasamos a estado final de exponente si lee un dígito
             if(*c>='0'&& *c<='9'){
                estado=9;
            }
            //Pasamos a estado intermedio de exponente si lee un + o -
            else if(*c=='+' || *c=='-'){
                estado=8;
            }
            else{
                break;
            }
        }
        else if(estado==8){//estado intermedio exponente con +-
             if(*c>='0'&& *c<='9'){//si leemos dígito pasamo a estado final de exponente
                estado=9;
            }
            else{
                break;
            }
        }
        else if(estado==9){//estado final de exponente
             if((*c>='0'&& *c<='9')){
                //sigue leendo exponente si hai máis dígitos
            }
            else if (*c=='i'){
                //pasa a estado de números imaginarios
                estado=10;
            }
            else{
                break;
            }
        }
        else if(estado==10){
            break;
        }

        //Non se ejecutou break polo que seguimos co seguinte caracter
        c=leerCaracter();

    }while(1);
    //Comprobamos si acaba en un estado final
    if(estado==1 || estado==2 || estado==3 || estado==5 || estado==6 || estado==9 || estado==10){
        //se reconoce con éxito
    }
    else{
        ImprimirError(3);
    }
    //devolvemos el caracter que implicó el fin
    devolverCaracter();
    tipoelem resul;
    //recuperamos copia de palabra do buffer
    resul.lexema=leerPalabra(1);
    //Establecemos o tipo de número según o estado final
    if(estado==1 || estado==2 || estado==5){
        resul.ID=INT;
    }
    else if(estado==10){
        resul.ID=IMAGIN;
    }
    else{
        resul.ID=FLOAT;
    }
    return resul;

}
//Función que representa o autómata que reconoce operadores e outros símbolos
tipoelem otro(char *carac){
    char * c=carac;
    tipoelem resul;

    //Lectura de caracteres que pueden ir sueltos o acompañados de =
    if(*c=='*'||*c=='/'||*c=='%'||*c=='='||*c=='^'||*c==':'||*c=='!'){
        char letra= *c;
        c=leerCaracter();
        if(*c!='='){
            devolverCaracter();
            resul.ID=letra;
        }
        else if(letra==':'){
            resul.ID=DOSPTOSIG;
        }
        else{
            resul.ID=GENERICO;
        }
        resul.lexema=leerPalabra(1);
        return resul;
    }
    //Lectura de posibilidades tendo lido un +----> += ++ +
    else if(*c=='+'){
        c=leerCaracter();
        if(*c=='='){
            resul.ID=MASIGUAL;
        }
        else if(*c=='+'){
            resul.ID=MASMAS;
        }
        else{
            devolverCaracter();
            resul.ID='+';
        }
        resul.lexema=leerPalabra(1);
        return resul;
    }
    //Lectura de posibilidades tendo lido un - ----> -= -- -
    else if(*c=='-'){
        c=leerCaracter();
        if(*c=='='){
            resul.ID=MENOSIGUAL;
        }
        else if(*c=='-'){
            resul.ID=MENOSMENOS;
        }
        else{
            devolverCaracter();
            resul.ID='-';
        }
        resul.lexema=leerPalabra(1);
        return resul;
    }
    //Lectura de posibilidades tendo lido un |----> |= || |
    else if(*c=='|'){
        c=leerCaracter();
        if(*c=='='){
            resul.ID=BARRAVIGUAL;
        }
        else if(*c=='|'){
            resul.ID=BARRAVBARRA;
        }
        else{
            devolverCaracter();
            resul.ID='|';
        }
        resul.lexema=leerPalabra(1);
        return resul;
    }
    //Lectura de posibilidades tendo lido un &----> & &^ &= && &^=
    else if(*c=='&'){
        c=leerCaracter();
        if(*c=='^'){
            c=leerCaracter();
            if(*c=='='){
                resul.ID=UPPSASOMBIG;
            }
            else{
                devolverCaracter();
                resul.ID=UPPSASOMBR;
            }
        }
        else if(*c=='&'){
            resul.ID=UPPSAUPPSA;
        }
        else if(*c=='='){
            resul.ID=UPPSAIGUAL;
        }
        else{
            devolverCaracter();
            resul.ID='&';
        }
        resul.lexema=leerPalabra(1);
        return resul;
    }
    //Lectura de posibilidades tendo lido un < ----> << <= <<= <- <
    else if(*c=='<'){
        c=leerCaracter();
        if(*c=='<'){
            c=leerCaracter();
            if(*c=='='){
                resul.ID=PICODPICOIG;
            }
            else{
                devolverCaracter();
                resul.ID=PICODPICO;
            }
        }
        else if(*c=='-'){
            resul.ID=PICODMENOS;
        }
        else if(*c=='='){
            resul.ID=PICODIGUAL;
        }
        else{
            devolverCaracter();
            resul.ID='<';
        }
        resul.lexema=leerPalabra(1);
        return resul;
    }
    //Lectura de posibilidades tendo lido un > ----> >> >= >>= >
    else if(*c=='>'){
        c=leerCaracter();
        if(*c=='>'){
            c=leerCaracter();
            if(*c=='='){
                resul.ID=PICOIZPICIG;
            }
            else{
                devolverCaracter();
                resul.ID=PICOIZPICO;
            }
        }
        else if(*c=='='){
            resul.ID=PICOIZIGUAL;
        }
        else{
            devolverCaracter();
            resul.ID='>';
        }
        resul.lexema=leerPalabra(1);
        return resul;
    }
    else{
        resul.lexema=leerPalabra(1);
        resul.ID=*c;
        return resul;
    }
}
//Función que permite al analizador sintáctico solicitar el siguiente componente léxico
tipoelem siguienteComponenteLexico(){
    char * carac;
    //leemos primer caracter
    carac= leerCaracter();
    //bucle que se repite hasta completar un componente léxico o encontrar el fin de fichero
    while (*carac!=EOF){
        
        //Detecta cadena alphanumérica A-Z a-z _ (non pode empezar por un número)
        if((*carac<='Z' && *carac>='A')||(*carac>='a' && *carac<='z')||*carac=='_'){
            return autAlpha();
        }
        //Detecta número empeza por 0-9 o .
        else if((*carac>='0'&& *carac<='9')||*carac=='.'){
            return autNumeros(carac);
        }
        //Procesa espacios y saltos de línea sin devolverlos
        else if (*carac==' ' || *carac=='\n'){
            leerPalabra(0);//pasamos cero xa que non deseamos a copia da palabra
            carac= leerCaracter();
        }
        //Detecta un inicio de string "
        else if(*carac=='"'){
            return autString();
        }
        //Detecta un posible inicio de comentario
        else if(*carac=='/'){
            //Se invoca al autómata de comentarios
            if(autComent()){
                //era un comentario, el autómata ya lo proceso y no es necesario devolverlo
                //leemos siguiente caracter para la siguiente iteración en busca de un componente léxico
                carac= leerCaracter();
            }
            else{
                //a barra non era de comentario
                //invocamos al autómata restante encargados de operadores y otros símbolos
                return otro(carac);
            }
        }
        //aquí podrían añadirse novas expresión regulares con chamada a novos autómatas según o caracter lido
        else{
            //De no ser ninguno de los anteriores se invoca al autómata de operadores y otros símbolos 
            return otro(carac);
        }
    }
    //En caso de detectar el fin de fichero lo devolvemos construyendo el tipoelem
    tipoelem resul;
    resul.lexema=malloc(2*sizeof(char));
    resul.lexema[0]=EOF;
    resul.lexema[1]='\0';
    resul.ID=FINFICH;
    return resul;
    
}