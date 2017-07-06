#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "../file.c"

int main(int argc, char *argv[])
{
	struct hostent		*hostinfo;
	struct sockaddr_in	addressServer;
	int	socketClient, sizeContent, n;
	struct in_addr ipv4addr;
	socklen_t	lenServer;
	char * contentFile;

	if	(argc!=3)  {
		fprintf(stderr,"Uso:\n%s <ip> <caminho do arquivo>\n\n",argv[0]);
		exit(1);
	}


	inet_pton(AF_INET, argv[1], &ipv4addr);
	hostinfo = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
	if	(!hostinfo)  {
		fprintf(stderr,"%s: Erro ao pegar as info do host: %s.\n",argv[0], argv[1]);
		exit(1);
	}
	if	(hostinfo->h_addrtype!=AF_INET)  {
		fprintf(stderr,"%s: \"%s\" nao e um ip valido.\n",argv[0],argv[1]);
		exit(1);
	}

	contentFile = readFile(argv[2], &sizeContent);

	lenServer = sizeof(addressServer);

	/*Limpa memória*/
	memset((char *)&addressServer,0,lenServer);

	addressServer.sin_family = AF_INET;

	memcpy(&hostinfo->h_addr, &addressServer.sin_addr.s_addr, hostinfo->h_length);
	addressServer.sin_port = htons(32128);
	
	/* inicializa client socket */
	socketClient = socket(AF_INET, SOCK_DGRAM, 0);

	n = sendto (socketClient, contentFile , sizeContent, 0, (struct sockaddr *)&addressServer, lenServer);
	if	(n < 0)  {
		fprintf(stderr,"%s: erro no sendto: %s\n",argv[0],strerror(errno));
		exit(1);
	}

	return 0;
}

