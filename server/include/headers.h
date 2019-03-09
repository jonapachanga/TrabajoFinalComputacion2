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
#include <pthread.h>

#define MESSAGE_BUFFER 1024
#define CLIENT_ADDRESS_LENGTH 100
#define DEFAULT_PORT 8000
#define MAX_CLIENTS 2
#define CLIENT_ADDRESS_LENGTH 100

typedef struct {
    int client_id;
    char username;
    char status;
    struct sockaddr_in data;
    struct Client *next;
} Client;

typedef struct {
    int client_id1;
    int client_id2;
} Relationships;

void usage(char*);

void* sock_handler(void* dato);