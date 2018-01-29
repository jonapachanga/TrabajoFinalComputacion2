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

#define MYPORT 8000
#define MSG_SIZE 80
#define MAX_CLIENTS 2

void usage(char*);

int main(int argc, char** argv) 
{
    int port = 0;
    int c = 0;
    char *error;
    struct sockaddr_in dir;
    int sockid, conn_sock, count;
    int pid;
    char buffer[MSG_SIZE];
    int num_clients = 0;

    switch (argc)
    {
        case 1:
            /*
                Si no pasa los argumentos asigno el puerto definido
                en las macros
            */
            port = MYPORT; 
            break;
        case 3:
            /*
                En caso de que pase el argumento correcto,
                asigno el puerto pasado por argumento
            */
            if(!strcmp("-p", argv[1])){
                sscanf(argv[2], "%i", &port);
            }else{
                usage(argv[0]);
                exit(1);
            }
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //creamos el socket:
    if((sockid=socket(PF_INET, SOCK_STREAM, 0)) < 0){
        error = "Socket Error";
        goto err;
    }
    //Seteamos la direccion en la que va a escuchare el server
    dir.sin_family      = AF_INET;
    dir.sin_port        = htons(port);
    dir.sin_addr.s_addr = htonl(INADDR_ANY);

    //asociamos la direccion con el socket
    if((bind(sockid, (struct sockaddr *) &dir, sizeof(dir))) < 0) {
        error = "Binding Error";
        goto err;
    }

    listen(sockid, MAX_CLIENTS);

    while(conn_sock = accept(sockid, NULL, NULL)){
        if (conn_sock < 0){
            error = "Connection Accepted";
            goto err;
        }
        if (num_clients < MAX_CLIENTS){
            if (!(pid=fork())){		//proceso hijo
                while(count=recv(conn_sock,buffer,MSG_SIZE,0)){
                    if (count < 0){
                        error="recv";
                        goto err;
                    }

                    *(buffer+count)='\0';
                    printf("Recibiendo datos: %s (proceso %d)\n",buffer, getpid());
                }
                close(sockid);
                exit(0);
            }
        }
        printf("Conexion delegada al proceso hijo %d\n", pid);
//		waitpid(pid,NULL,WNOWAIT);
    }

    err:
    	fprintf(stderr,"%s %d %s\n",error,errno,strerror(errno));
    return 0;
}

void usage(char *progname){
	printf("\n Usage: \n \t %s -p <port> - Example: ./server -p 8000 \n\n",progname);
}

