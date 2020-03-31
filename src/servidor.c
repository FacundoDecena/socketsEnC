#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFFER_SIZE 256

void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    // sockdf y newsockfd son desciptores de archivo
    // portno guarda el numero del puerto en el que va a aceptar conecciones
    // clilen almacena el tamaño de la direccion del cliente
    // n es el valor de retorno para las llamadas de read() y write()
    int sockfd, newsockfd, portno, clilen, n;

    // el servidor lee caracteres de la coneccion de socket en este buffer
    char buffer[BUFFER_SIZE];

    // sockaddr_in es una estructura que contiene una direccion de internet
    struct sockaddr_in serv_addr, cli_addr;

    // Si no se especifica el puerto debe informarse al usuario
    if (argc < 2) {
        fprintf(stderr, "ERROR, no se ha proporcionado un puerto");
        exit(1);
    }

    // Direccion de dominio de socket, tipo de socket, protocolo
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error abriendo el socket");

    // Setea todos los valores en un buffer a su valor cero.
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // El numero de puerto usado es convertido a entero
    portno = atoi(argv[1]);

    // Setea la familia de direcciones.
    serv_addr.sin_family = AF_INET;

    // Setea el numero de puerto
    serv_addr.sin_port = htons(portno);

    // Setea la direccion IP del host
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    //La llamada al sistema bind () vincula un socket a una dirección, en este caso
    // la dirección del host actual y el número de puerto en el que se ejecutará el servidor.
    if (bind (sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Error en bindeo") ;
    
    //Permite a los procesos escuchar en un socket por conexiones
    listen(sockfd, 5);

    // accept bloquea el proceso hasta que un cliente se conecta al servidor.
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("Error en accept");

    // Solo se llega a este punto si se logro una conexion con un cliente satisfactoriamente
    bzero(buffer, BUFFER_SIZE);
    n = recv(newsockfd, buffer, BUFFER_SIZE - 1, 0);
    if (n < 0)
        error("Error leyendo del socket");

    char message[BUFFER_SIZE + 18];

    strcpy(message,"Hola ");
    strcat(message, buffer);
    strcat(message,", bienvenido!\n");
    printf("Hola %s, bienvenido!\n", buffer);

    n = send(newsockfd, message, BUFFER_SIZE + 18, 0);
    if (n < 0)
        error("Error escribiendo al socket");

    return 0;

}