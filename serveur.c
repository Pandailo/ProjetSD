#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

void error(char* msg)
{
	perror(msg);
	exit(1);
}
int main(int argc, char* argcv[])
{
	int sockfd,newsockfd;
	int n,numport, cli_addrlen;
	char buffer[256];
	struct sockaddr_in serv_addr, client_addr;
	const char *messages[]={"Faut pas respirer la compote ça fait tousser",
				"A ROULEEEEEEEEEEEEEEEEEEETTES",
				"Le caca des canards c'est caca, faut pas manger",
				"Tatan elle fait des flans",
				"Vous rendez la poulette, sinon c'est tout nu dans les orties",
				"AKADOK",
				"Pas du tout les lapins, les lapins c'est gentil",
				"LE POISSON, LE PETIT POISSOOOON !",
				"Sloubi1",
				"Sloubi2",
				"Sloubi3",
				"Sloubi4",
				"Sloubi5"};/*12 trucs de merde*/
	int idmsg=0;
	int maxmsg=13;
	int rescmp=1;
	int rescmp1=1;
	pid_t pid;
	char str1[10],str2[10];
	if (argc<2)
	{
		fprintf(stderr,"ERROR, numero de port manquant \n");
		exit(1);
	}
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		error("Pas possible de créer le socket");
	bzero((char *) &serv_addr,sizeof(serv_addr));
	numport=atoi(argcv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(numport);
	if(bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		error("Error: On bind pas le socket");
	listen(sockfd,5);
	do
	{
		bzero((char *) &client_addr,sizeof(client_addr));
		cli_addrlen=sizeof(client_addr);
		newsockfd=accept(sockfd,(struct sockaddr *) &client_addr,&cli_addrlen);
		if(newsockfd <0)
			error("J'ai refusé la connexion.");
		
		pid=fork();
		if(pid==0)
		{
			bzero(buffer,256);
			n=read(newsockfd,buffer,256);
			if(n<0)
				error("ERROR READING FROM SOCKET");
			strcpy(str1,"AKADOC");
			rescmp=strcmp(buffer,str1);
			strcpy(str2,"akadoc");
			rescmp1=strcmp(buffer,str2);
			if(rescmp==0 || rescmp1==0)
			{
				n=write(newsockfd,messages[idmsg],strlen(messages[idmsg]));
				if(n<0)
					error("ERROR ecrire dans socket");

			}
			else
			{
				close(newsockfd);
				return 0;
				exit(0); 
			}
			exit(0);
		}
		close(newsockfd);
		idmsg++;
		 int tmp=idmsg%maxmsg; 
		 idmsg=tmp;
		 close(newsockfd);
	}
	while(1);
}
		
