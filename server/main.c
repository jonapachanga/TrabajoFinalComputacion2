#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdbool.h>
#include <arpa/inet.h>


#define DEFAULT_PORT 8000
#define MSG_SIZE 80
#define MAX_CLIENTS 2
#define CLIENT_ADDRESS_LENGTH 100

typedef struct {
    int client_id
    char username;
    char status;
    struct sockaddr_in data;
} Client;

typedef struct {
    int client_id1
    int client_id2
} Relationships;

void usage(char*);

int main(int argc, char** argv) 
{
    int port = 0;
    int c = 0;
    char *error;
    socklen_t longc;
    struct sockaddr_in * server;
    Client client;
    int server_connection, client_connection, count;
    int pid;
    char buffer[MSG_SIZE];
    int connected_clients = 0;
    char client_address[CLIENT_ADDRESS_LENGTH];

    switch (argc) {
        case 1:
            /**
             * Asigno puerto predefinido en caso de que no venga
             */
            port = DEFAULT_PORT;
            break;
        case 3:
            /**
             * En caso de que pase el argumento correcto,
             * asigno el puerto pasado por argumento
             */
            if(strcmp("-p", argv[1])){
                usage(argv[0]);
                exit(1);
            }

            sscanf(argv[2], "%i", &port);
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    /* Creamos el socket */
    if((server_connection = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        error = "Socket Error";
        goto err;
    }
    /* Seteamos la direccion en la que va a escuchare el server */
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    /* asociamos la direccion con el socket */
    if((bind(server_connection, (struct sockaddr *) &server, sizeof(server))) < 0) {
        error = "Binding Error";
        goto err;
    }

    listen(server_connection, MAX_CLIENTS);
    printf("A la escucha en el puerto %d\n", ntohs(server.sin_port));
    longc = sizeof(client.data);
    while(client_connection = accept(server_connection, (struct sockaddr *)&client.data, &longc)){
        if (client_connection < 0){
            error = "Connection Error";
            goto err;
        }

        printf("Conectando con %s:%d\n", inet_ntoa(client.data.sin_addr), htons(client.data.sin_port));

        if (connected_clients < MAX_CLIENTS){
            connected_clients ++;
            printf("Cantidad de clientes conectados: %d\n", connected_clients);
            send(client_connection, "Recibido\n", 13, 0);
            if (!(pid = fork())){		/* Proceso hijo */
                while(count = recv(client_connection, buffer, MSG_SIZE,0)){
                    if (count < 0){
                        error = "recv";
                        goto err;
                    }

                    *(buffer + count) = '\0';
                    printf("Recibiendo datos: %s (proceso %d)\n", buffer, getpid());
                }
                close(server_connection);
                exit(0);
            }
        }
        printf("Conexion delegada al proceso hijo %d\n", pid);
//		waitpid(pid,NULL,WNOWAIT);
    }

    err:
    	fprintf(stderr,"%s %d %s\n", error, errno, strerror(errno));
    return 0;
}



