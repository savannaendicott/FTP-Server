#include "header.h"
// dem titties doe
// TODO use errno in error messages.
// Example:
// 		if(send(client_socket, sendBuff, sizeof(sendBuff), 0) < 0)
//     		printf("Error: Send\nErrno: %d\n", errno);
// TODO look into TCP_CORK
// TODO should sendfile64() be used for actual file transfers?

int main() {
	int server_socket = 0, client_socket = 0;

	struct sockaddr_in serv_addr;

	char recvBuff[BUFFER+1]; // +1 for the null character.
	char *end = "&endm;\n";
	char *end_pos = NULL;
	int index = 0; // iterator

	char request[6][6] = {"ls   ", "get  ", "put  ", "cd   ", "mkdir", "err  "};
	void (*request_handler[6]) (char* command, int client_socket) = 
												{ ls, get, put, cd, mkdr, err };

	// Connect server_socket to a socket.
	server_socket = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;    
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(PORT);

	Bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	Listen(server_socket, MAXPENDING);

printf("waiting for connection\n");

	client_socket = Accept(server_socket, (struct sockaddr*)NULL ,NULL);


	int bytes_received = 0;
	while (1) {
printf("waiting for message\n");
		while(bytes_received < 1) {
			if((bytes_received = recv(client_socket, recvBuff, BUFFER, 0)) < 0)
				printf("Error: Receive\nErrno: %d\n", errno);
		}
		bytes_received = 0;
		recvBuff[BUFFER] = '\n';

printf("grabbing command out of message\n");
		end_pos = strstr(recvBuff, end);
		if (end_pos == NULL) continue;//printf("Error with end_pos\n");
		index = end_pos - recvBuff;

		char command[index];
		strncpy(command, recvBuff, index);

printf("Command: %s\n", command);
printf("checking for exit command\n");
		if (strncmp(command, ":exit", 5) == 0) break;
printf("handle request\n");
		// Handle request

		pid_t pid = fork();
		if (pid) wait(&pid);
		else {
		for (int i = 0; i < 6; ++i)
			if (strncmp(command, request[i], 5) == 0 || i == 5)
				{ request_handler[i](recvBuff+index+6, client_socket); break; }
		exit(0);
		}
printf("request handled\n");
	}

	close(client_socket);
	close(server_socket); 

	return 0;    
}

// Displays contents of current directory to client
// Stretch goal: allow client to ls directories other than current
// 		e.g. ls test/so_much_bacon/
void ls  (char* command, int client_socket) {
	printf("--in ls\n");
	// call ls and send the result to "ls.txt" and open it
	system("ls > ls.txt");
	int filehandle = open("ls.txt", O_RDONLY);

	// get the size of "ls.txt" (probably a better way to do this)
	struct stat file_info;
	stat("ls.txt", &file_info);
	int size = file_info.st_size;

	// Send the client the size of the file followed by the file itself
	if (send(client_socket, &size, sizeof(int), 0) < 0) {
		printf("send error\n");
		exit(1);
	}
	if (sendfile(client_socket, filehandle, NULL, size) < 0) {
		printf("sendfile error\n");
		exit(1);
	}

	// Clean up ls.txt
	close(filehandle);
	system("rm ls.txt");
}


void get (char* command, int client_socket) {
	printf("--in get\n");
	// Get the path out of command.
	char *end_pos = strstr(command, "&endm;\n");
	int length = end_pos - command;

	// Store the path in it's own string and open the file it points to.
	char **path = NULL;
	*path = (char *) malloc(length);
	//char path[length];
	strncpy((*path), command, length);
	int filehandle = open(*path, O_RDONLY);

	if (filehandle < 0) {
		FILE *fp = fopen("err.txt", "w");
		fprintf(fp, "Could not find file.");
		fclose(fp);

		filehandle = open("err.txt", O_RDONLY);

		free(path);
		*path = (char *) malloc(sizeof("err.txt"));
		*path = "err.txt";
	}

	struct stat file_info;
	stat(*path, &file_info);
	int size = file_info.st_size;

	// Send the size of the file and the file
//	if (send(client_socket, &size, sizeof(int), 0) < 0) {
//		printf("send error\n");
//		exit(1);
//	}
	if (sendfile(client_socket, filehandle, NULL, size) < 0) {
		printf("test\n");
		printf("sendfile error\n");
		printf("Errno: %d, %s\n", errno, strerror(errno));
		exit(1);
	}

	// Close file_handle.
	close(filehandle);
}

void put (char* command, int client_socket) {
	printf("--in put\n");
}


void cd  (char* command, int client_socket) {
    printf("--in cd\n");
}


void mkdr(char* command, int client_socket) {
    printf("--in mkdir\n");
}

// Tell the client that their command was not valid.
void err (char* command, int client_socket) {
// create a file "err.txt" and store the error message in it.
	printf("--in err\n");
	FILE *fp = fopen("err.txt", "w");

	char cmd[BUFFER+1];
	sscanf(command, "%s", cmd);
	cmd[BUFFER] = '\n';

	fprintf(fp, "Command \"%s\" not recognized\n", cmd);

// get the size of "err.txt" (probably a better way to do this)
	struct stat file_info;
	stat("err.txt", &file_info);
	int size = file_info.st_size;

// Send the client the size of the file followed by the file itself
	char test[1025] = "yo holla\n";
	if (send(client_socket, test, 1025, 0) < 0) {
		printf("send error\n");
		exit(1);
	}
	recv(client_socket, test, 1025, 0);
//	if (sendfile(client_socket, fileno(fp), NULL, size) < 0) {
//		printf("sendfile error\n");
//		exit(1);
//	}

// clean up err.txt
	fclose(fp);
	system("rm err.txt");
}
