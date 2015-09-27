#include "header.h"

void handle_response(int, char *, char *);
void ls_r  (char *, int);
void get_r (char *, int);
void put_r (char *, int);
void cd_r  (char *, int);
void mkdr_r(char *, int);
void err_r (char *, int);

int main(void) {
    // Initialize socket_fd and the buffers.
    int socket_fd = 0;
    char recvBuff[BUFFER+1], sendBuff[BUFFER+1];
    char *message;
    char *command;
    char *parameter;

    // Create and initialize serv_addr.
    struct sockaddr_in serv_addr;
    memset(recvBuff, '0', sizeof(recvBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    // Connect to a socket.
    socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

    // Connect to other client.
    Connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // Main loop.
    while (strncmp(sendBuff, ":exit", 5) != 0) {
        fputs("(Client) Enter a message: ", stdout);
        fgets(sendBuff, sizeof(sendBuff), stdin);

        parseSendBuffer(sendBuff, &command, &message, &parameter);

        Send(socket_fd, message, BUFFER+1, 0);

        handle_response(socket_fd, command, parameter);
    }
}

void handle_response(int socket_fd, char *command, char *parameter) {
    char request[6][5] = {"ls   ", "get  ", "put  ", "cd   ", "mkdir", "err  "};
    void (*request_handler[6]) (char* parameter, int client_socket) =
                { ls_r, get_r, put_r, cd_r, mkdr_r, err_r };

    for (int i = 0; i < 6; ++i)
        if(strncmp(command, request[i], 5) == 0 || i == 5)
            {request_handler[i](parameter, socket_fd); break;}
}

void ls_r(char *parameter, int socket_fd) {
    printf("in ls response\n");
    int file_size = 0;
    int count = 0;
    int count_total = 0;

    Recv(socket_fd, &file_size, sizeof(int), 0);
    
    char buffer[file_size+1];
    char temp_buff[1025];

    int filehandle = open("ls.txt", O_WRONLY);

    while (1) {
        if ((count = recv(socket_fd, temp_buff, 1025, 0)) < 0)
            printf("recv error: %d %s\n", errno, strerror(errno));
        if (count == 0) break;
        count_total += count;
        
        strcat(buffer, temp_buff);
    }

    printf("%s", buffer);
}

void get_r(char *parameter, int socket_fd) {
    printf("in get response\n");
}

void put_r(char *parameter, int socket_fd) {
    printf("in put response\n");
}

void cd_r(char *parameter, int socket_fd) {
    printf("in cd response\n");
}

void mkdr_r(char *parameter, int socket_fd) {
    printf("in mkdir response\n");
}

void err_r(char *parameter, int socket_fd) {
    printf("in err response\n");
}

void parseSendBuffer(char *sendBuff, char **cmd, char **message, char **param) {
    int size_of_message = 0;
    char command[12] = "            ";
    char *temp = strtok(sendBuff, " ");

    for (int i = 0; i < 5 && temp[i] != '\n'; ++i)
        command[i] = temp[i];
    strncpy(command+5, "&endm;\n", 7);

printf("command: %s", command);

    char *parameter = strtok(NULL, " ");
    if (parameter == NULL) parameter = "";

    char stringToSend[1025];
    strncpy(stringToSend, command, strlen(command));
    
    if (strlen(parameter) > 0) {
        strcat(stringToSend, parameter);
        strcat(stringToSend, "&endm;\n");
    }

    printf("parse test. stringToSend: %s", stringToSend);

    *message = stringToSend;
    *param = parameter;
    *cmd = command;
}
