#include "../include/headers.h"

int main(int argc, char** argv) {

    char *error;
    socklen_t longc;
    int server_connection, client_connection;
    struct sockaddr_in server;
    Client* client = NULL;
    char buffer[MESSAGE_BUFFER];
    pthread_t thread_id;


    //creamos el socket inet-stream
    if((server_connection = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        error = "Socket Error";
        goto err;
    }


    //seteamos la server en la que va a escuchar
    server.sin_family      = AF_INET;
    server.sin_port        = htons(atoi(*(argv + 1)));
    server.sin_addr.s_addr = htonl(INADDR_ANY);


    //asociamos el socket con la server (bind)
    if((bind(server_connection, (struct sockaddr *)&server, sizeof(server))) < 0){
        error = "Binding Error";
        goto err;
    }


    // seteamos la cantidad de conexiones concurrentes en cola
    listen(server_connection, MAX_CLIENTS);
    printf("A la escucha en el puerto %d\n", ntohs(server.sin_port));

    //dejamos escuchando al proceso en el socket ip:puerto
    longc = sizeof(client->data);
    client = (Client *) malloc(sizeof(Client));
    while(client_connection = accept(server_connection, (struct sockaddr *)&client->data, &longc)){
        if (client_connection < 0){
            error = "Accept Error";
            goto err;
        }

        printf("Conectando con %s:%d\n", inet_ntoa(client->data.sin_addr), htons(client->data.sin_port));

        /* Delego la conexion a un hilo */
        if(pthread_create(&thread_id, NULL, sock_handler, (void*)&client_connection)){
            error = "Thread Create Error";
            goto err;
        }

        printf("Conexion delegada al hilo %li\n", thread_id);
    }



    // ya tenemos un client_connection de datos asociado con el cliente conectado
    return 0;

    err:
    fprintf(stderr,"%d %s %s\n", errno, error, strerror(errno));
    exit(1);

}