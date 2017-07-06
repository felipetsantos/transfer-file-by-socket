#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../file.c"

#define FILE_SIZE 1500

int main(int argc, char *argv[])
{
	struct sockaddr_in	addressServer;
	struct sockaddr_in	addressClient;
	int	socketServer, n;
	socklen_t lenClient,lenServer;
	char *buffer;

	socketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if	(socketServer < 0)  {
		fprintf(stderr,"%s: Prolema ao criar o socket: %s\n",argv[0],strerror(errno));
		exit(1);
 	}

	lenServer = sizeof(addressServer);
	memset((char *)&addressServer, 0, lenServer);

	addressServer.sin_family = AF_INET;
	addressServer.sin_addr.s_addr = INADDR_ANY;
	addressServer.sin_port = htons(32128);

	if	(bind(socketServer,(struct sockaddr *)&addressServer,lenServer) < 0)  {
		fprintf(stderr,"%s: erro em bind(): %s\n",argv[0],strerror(errno));
		exit(1);
	}

	lenClient = sizeof(addressClient);
	while	(1) {
		buffer = malloc(FILE_SIZE);
		n = recvfrom(socketServer,buffer, FILE_SIZE , 0 ,(struct sockaddr *)&addressClient, &lenClient);
		if	(n < 0)  {
			fprintf(stderr,"%s: erro em recvfrom(): %s\n",argv[0],strerror(errno));
			exit(1);
		}
		saveFile("out.txt", buffer, FILE_SIZE);
 	}
	return 0;
}

