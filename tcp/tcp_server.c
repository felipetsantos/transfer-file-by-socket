#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../file.c"

#define PORT   0x8006

int main(int argc, char *argv[])
{
	struct sockaddr_in	svc_address;
	struct sockaddr_in	clnt_address;
	int			svc_socket,clnt_socket;
	socklen_t		clnt_len,svc_len;
	char *buffer;
	int fileSize;


	if	(argc!=2)  {
		fprintf(stderr,"Uso:\n%s <tamanho em bytes do arquivo que o servidor vai receber> \n\n",argv[0]);
		exit(1);
	}

	fileSize = atoi(argv[1]);

	svc_socket = socket(AF_INET, SOCK_STREAM, 0);
	if	(svc_socket < 0)  {
		fprintf(stderr,"%s: erro em socket(): %s\n",argv[0],strerror(errno));
		exit(1);
	}

	svc_len = sizeof(svc_address);

	bzero((char *)&svc_address,svc_len);

	svc_address.sin_family = AF_INET;
	svc_address.sin_addr.s_addr = INADDR_ANY;
	svc_address.sin_port = htons(PORT);

	if	(bind(svc_socket,(struct sockaddr *)&svc_address,svc_len) < 0)  {
		fprintf(stderr,"%s: erro em bind(): %s\n",argv[0],strerror(errno));
		exit(1);
	}

	listen(svc_socket, 5);

	while	(1)  {
		/* verifica se tem cliente conectado */
		clnt_socket = accept (svc_socket,(struct sockaddr *)&clnt_address, &clnt_len);
		if	(clnt_socket < 0)  {
			fprintf(stderr,"%s: erro em accept(): %s\n",argv[0],strerror(errno));
			exit(1);
		}
		buffer = malloc(fileSize);
		/* le a informação que o cliente mandou */
		read(clnt_socket, buffer, fileSize);
		printf("Recebeu um arquivo.\n");
		saveFile("out.txt", buffer, fileSize);
		close(clnt_socket);
	}
	return 0;
}

