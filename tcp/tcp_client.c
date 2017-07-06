#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../file.c"
#define PORT   0x8006

int main(int argc, char *argv[])
{
	struct		hostent *hostinfo;
	struct		sockaddr_in address;
	int		sockfd, result, sizeContent;
	socklen_t	addr_len;
	char * contentFile;
	struct in_addr ipv4addr;


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
	addr_len = sizeof(address);
	bzero((char *)&address,addr_len);
	address.sin_family = AF_INET;
	bcopy((char *)hostinfo->h_addr,(char *)&address.sin_addr.s_addr,hostinfo->h_length);
	address.sin_port = htons(PORT);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	result = connect (sockfd, (struct sockaddr *)&address, addr_len);
	if	(result == -1)  {
		fprintf(stderr,"%s: erro em connect(): %s\n",argv[0],strerror(errno));
		exit(1);
	}
	printf("Vai enviar o arquivo....\n");
	write(sockfd, contentFile, sizeContent);
	printf("Enviou o arquivo.\n");
	close(sockfd);
	return 0;
}

