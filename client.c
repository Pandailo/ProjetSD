#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

/* USAGE: nom-prog HOST-NAME PORT-NUMBER */

int main(int argc,char *argv[])
{
	int sockfd,newsockfd;
	int n, numport;
	char buffer[256];
	
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n",argv[0]);
		exit(0);
	}
	
	numport=atoi(argv[2]);
	
	/* Créer un descripteur de fichier pour le SOCKET pour communiquer sur le réseau */
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	error("ERROR Impossible de créer socket");
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	/* préparation coté CLIENT : récupérer les infos du SERVEUR */
	/* Il y a mieux et plus facile pour récupérer les infos du serveur ou machine hote */
	/* Voir aussi getaddrinfo() et getnameinfo() */
	server = gethostbyname(argv[1]);
	if(server==NULL){
	error("ERROR:Serveur Inconnu");
	}
	
	/* Remplir une structure pour contenir l'adresse du SERVEUR */
	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port=htons(numport);
	
	/* Demande de connexion au SERVEUR en attente sur ACCEPT */
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	error("ERROR : Connexion Impossible");
	
	/*Init des echanges */
	bzero(buffer,256);
	strcpy(buffer,"AKADOC");
	
	n=write(sockfd,buffer,255);
	if (n<0)
	error("ERROR reading from socket");
	bzero(buffer,256);
	n=read(sockfd,buffer,255);
	if(n<0)
		error("ERROR reading from socket");
	
	printf("*********** BONJOUR ************** \n");
	printf("********************************** \n\n");
	printf("%s\n\n",buffer);
	printf("********************************** \n\n");
	return 0;
	
}
	
	
