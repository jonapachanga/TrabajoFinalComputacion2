#include "../include/headers.h"

void usage(char *appName)
{
    printf("\n Usage: \n \t %s -p <port> - Example: ./server -p 8000 \n\n", appName);
}

void* sock_handler(void* dato)
{
    char buffer[MESSAGE_BUFFER];
    int conn_sock_th, count;
    pthread_detach(pthread_self());

    conn_sock_th = *((int*)dato);

    printf("Iniciando hilo %li\n", pthread_self());

    while(count=recv(conn_sock_th, buffer, MESSAGE_BUFFER,0)){
        *(buffer + count) = '\0';
        printf("Recibiendo datos (%li): %s\n", pthread_self(), buffer);
    }
    close(conn_sock_th);
    pthread_exit(NULL);
}

void add_client(Client * client) {

    if (client == NULL) printf( "No hay memoria disponible!\n");

    printf("Ingrese nombre de usuario: "); fflush(stdout);
    scanf("%s",client->username);
    strcpy("idle", client->status);

    /* el campo siguiente va a ser NULL por ser el último elemento
       de la lista */
    client->siguiente = NULL;

    /* ahora metemos el client elemento en la lista. lo situamos
       al final de la lista */
    /* comprobamos si la lista está vacía. si primero==NULL es que no
       hay ningún elemento en la lista. también vale ultimo==NULL */
    if (primero==NULL) {
        printf( "Primer elemento\n");
        primero = client;
        ultimo = client;
    }
    else {
        /* el que hasta ahora era el último tiene que apuntar al nuevo */
        ultimo->siguiente = nuevo;
        /* hacemos que el nuevo sea ahora el último */
        ultimo = nuevo;
    }
}