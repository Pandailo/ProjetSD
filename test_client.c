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
	
	
	/**********************************************************CLIENT SERVEUR****************************************************************/
	
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
	strcpy(buffer,"TAILLE_G");
	
	n=write(sockfd,buffer,255);
	if (n<0)
	error("ERROR reading from socket");
	bzero(buffer,256);
	n=read(sockfd,buffer,255);
	if(n<0)
		error("ERROR reading from socket");
	int taille=0;
	taille=atoi(buffer);
	printf("*********** TAILLE DE LA GRILLE ************** \n");
	printf("********************************** \n\n");
	printf("taille : %d \n\n",taille);
	printf("********************************** \n\n");
	
	
	bzero(buffer,256);
	strcpy(buffer,"TAILLE_B");
	
	n=write(sockfd,buffer,255);
	if (n<0)
	error("ERROR reading from socket");
	bzero(buffer,256);
	n=read(sockfd,buffer,255);
	if(n<0)
		error("ERROR reading from socket");
	int taille_b=0;
	taille_b=atoi(buffer);
	int bloc2[taille_b][taille_b];
	printf("*********** TAILLE D'UN BLOC ************** \n");
	printf("********************************** \n\n");
	printf("taille bloc : %d \n",taille_b);
	printf("********************************** \n\n\n");
	/*RECEPTION DU BLOC ASSIGNE BLBLBLBL */
	
	bzero(buffer,256);
	strcpy(buffer,"BLOC");
	
	n=write(sockfd,buffer,strlen(buffer));
	if (n<0)
		error("ERROR reading from socket");
	bzero(buffer,256);
	int i=0;
	int j=0;
	for(i;i<taille_b;i++)
	{
		for(j=0;j<taille_b;j++)
		{
			bzero(buffer,256);
			n=read(sockfd,buffer,1);
			if(n<0)
				error("ERROR reading from socket");
			if(atoi(buffer)<0)
			{j--;}
			else
			{	
				bloc2[i][j]=atoi(buffer);
				printf("\n Cellule du bloc [%d,%d] : %d \n",i,j,bloc2[i][j]);
				if(bloc2[i][j]>=0)
				{
					bzero(buffer,256);
				}
			}
		}
	}
	bzero(buffer,256);
	
	/*TRAITEMENT*/
	int u=0;
	int k=0;
	int cptv=0;
	for(i;i<taille_b;i++)
	{//Lignes bloc
		for(j=0;j<taille_b;j++)
		{//colonnes bloc
		cptv=0;
			if(bloc2[i][j]==0)
			{//si cellule vivante
				if(i+1==taille_b)
				{//si bord
					if(j+1==taille_b)
					{//si coin
					}
					else
					{
					}
				}
				else
				{
					if(j+1==taille_b)
					{//si bord
					}
					else
					{
						for(u=-1;u<2;u++)
						{
							for(k=-1;k<2;k++)
							{
								if(bloc2[i+u][j+k]==1) cptv++;						
							}
						
						}
					}
					
				}
				if(cptv!=2 &&cptv!=3)
					bloc2[i][j]=0;				
			}//FIN SI CELLULE VIVANTE
			else
			{
				if(i+1==taille_b)
				{//si bord
					if(j+1==taille_b)
					{//si coin
					}
					else
					{
					}
				}
				else
				{
					if(j+1==taille_b)
					{//si bord
					}
					else
					{
						for(u=-1;u<2;u++)
						{
							for(k=-1;k<2;k++)
							{
								if(u!=0&&k!=0)
									{if(bloc2[i+u][j+k]==1) cptv++;}						
							}
						
						}
					}
					
				}
				if(cptv==3)
					bloc2[i][j]==1;
			}
			printf("Nouveau bloc [%d][%d] : %d \n",i,j,bloc2[i][j]);
			
		//FIN FOR
		}
	}//FIN DOUBLE FOR
		
	/*RENVOI*/
	
	/*ARRET*/
	
	bzero(buffer,256);
	strcpy(buffer,"STOP");
	
	n=write(sockfd,buffer,255);
	if (n<0)
	error("ERROR reading from socket");

	printf("*********** FINI************** \n");
	printf("********************************** \n\n");
	printf("********************************** \n\n");
	bzero(buffer,256);
	return 0;
	
}
