// TODO: See if all of these are even necessary
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

/*for getting file size using stat()*/
#include <sys/stat.h>
 
/*for sendfile()*/
#include <sys/sendfile.h>
 
/*for O_RDONLY*/
#include <fcntl.h>

#define BUFFER 1024
#define PORT 5000
#define MAXPENDING 3
#define SERVER_IP "159.203.82.155"

void ls  (char* command, int client_socket);
void get (char* command, int client_socket);
void put (char* command, int client_socket);
void cd  (char* command, int client_socket);
void mkdr(char* command, int client_socket);
void err (char* command, int client_socket);

int  Socket(int, int, int);
void Bind(int, const struct sockaddr *, socklen_t);
void Listen(int, int);
int  Accept(int, struct sockaddr *restrict, socklen_t *restrict);
void Connect(int, const struct sockaddr *, socklen_t);
int  Send(int, const void *, size_t, int);
int  Recv(int, void *, size_t, int);

void parseSendBuffer(char *, char **, char **, char **);
