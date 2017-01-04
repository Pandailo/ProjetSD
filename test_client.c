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
	char rep='N';	
	do
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
		//printf("*********** TAILLE DE LA GRILLE ************** \n");
		//printf("********************************** \n\n");
		//printf("taille : %d \n\n",taille);
		//printf("********************************** \n\n");
	
	
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
		int bloc[taille_b][taille_b];
		//printf("*********** TAILLE D'UN BLOC ************** \n");
		//printf("********************************** \n\n");
		//printf("taille bloc : %d \n",taille_b);
		//printf("********************************** \n\n\n");
		/*RECEPTION DU BLOC ASSIGNE  */
	
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
					bloc[i][j]=atoi(buffer);
					bloc2[i][j]=bloc[i][j];
					printf("\n Cellule du bloc [%d,%d] : %d \n",i,j,bloc[i][j]);
					if(bloc[i][j]>=0)
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
		int val=-1;
		int cptv=0;
		int tmp=0;
		for(i=0;i<taille_b;i++)
		{//Lignes bloc
			for(j=0;j<taille_b;j++)
			{//colonnes bloc
			cptv=0;
				if(bloc[i][j]==1)
				{//si cellule vivante
							for(u=-1;u<2;u++)
							{
								for(k=-1;k<2;k++)
								{
									if(i+u!=i||j+k!=j)
									{
										//printf("I = %d , J=%d U=%d K=%d I+U=%d J+K=%d \n",i,j,u,k,i+u,j+k);
										if(((i+u)<taille_b)&&((i+u)>=0))
										{
											//printf("I = %d , J=%d U=%d K=%d I+U=%d J+K=%d \n",i,j,u,k,i+u,j+k);	
											if(((j+k)<(taille_b))&&((j+k)>=0))
											{//Si on ne dépasse pas
												if(bloc[i+u][j+k]==1) cptv++;
											}	
											else
											{									
												bzero(buffer,256);
												strcpy(buffer,"ENVOI");
												//printf("ENVOI \n");
												n=write(sockfd,buffer,strlen(buffer));
												if (n<0)
													error("ERROR reading from socket");
												bzero(buffer,256);
												printf("Demande de I=%d J=%d \n",((i+u+taille)%taille),((j+k+taille)%taille));
												sprintf(buffer," I=%d J=%d ",((i+u+taille)%taille),((j+k+taille)%taille));
												//printf("I=%d J=%d \n",i+u,j+k);
												n=write(sockfd,buffer,strlen(buffer));
												sleep(1);
												if (n<0)
													error("ERROR reading from socket");
												bzero(buffer,256);
												n=read(sockfd,buffer,1);
												if(n<0)
													error("ERROR reading from socket");
												val=atoi(buffer);
												//printf("Valeur reçue : %d \n",val);
												if(val==1)
													cptv++;
											}
										}
										else
										{
											
												bzero(buffer,256);
												strcpy(buffer,"ENVOI");
												//printf("ENVOI \n");
												n=write(sockfd,buffer,strlen(buffer));
												if (n<0)
													error("ERROR reading from socket");
												bzero(buffer,256);
												printf("Demande de I=%d J=%d \n",((i+u+taille)%taille),((j+k+taille)%taille));
												sprintf(buffer," I=%d J=%d ",((i+u+taille)%taille),((j+k+taille)%taille));
												//printf("I=%d J=%d \n",i+u,j+k);
												n=write(sockfd,buffer,strlen(buffer));
												sleep(1	);
												if (n<0)
													error("ERROR reading from socket");
												bzero(buffer,256);
												n=read(sockfd,buffer,1);
												if(n<0)
													error("ERROR reading from socket");
												val=atoi(buffer);
												//printf("Valeur reçue : %d \n",val);
												if(val==1)
													cptv++;
											
										}						
									}
								}
						
							}
					printf("Nb voisins %d pour i=%d j=%d \n",cptv,i,j);
					if(cptv!=2 &&cptv!=3)
						bloc2[i][j]=0;
					else printf("Valeur inchangée \n");				
				}//FIN SI CELLULE VIVANTE
				else
				{
					for(u=-1;u<2;u++)
							{
								for(k=-1;k<2;k++)
								{
									if(i+u!=i||j+k!=j)
									{
										if(i+u<taille_b&&i+u>=0)
										{
											
												if(j+k<taille_b&&j+k>=0)
												{//Si on ne dépasse pas
													if(bloc[i+u][j+k]==1) cptv++;
												}
												else
												{

													bzero(buffer,256);
													strcpy(buffer,"ENVOI");
													//printf("ENVOI \n");
													n=write(sockfd,buffer,strlen(buffer));
													if (n<0)
														error("ERROR reading from socket");
													bzero(buffer,256);
													printf("Demande de I=%d J=%d \n",((i+u+taille)%taille),((j+k+taille)%taille));
													sprintf(buffer," I=%d J=%d ",((i+u+taille)%taille),((j+k+taille)%taille));
													//printf("I=%d J=%d \n",i+u,j+k);
													n=write(sockfd,buffer,strlen(buffer));
													sleep(1);
													if (n<0)
														error("ERROR reading from socket");
													bzero(buffer,256);
													bzero(buffer,256);
													n=read(sockfd,buffer,1);
													if(n<0)
														error("ERROR reading from socket");
													val=atoi(buffer);
													//printf("Valeur reçue : %d \n",val);
													if(val==1)
														cptv++;
												}
										}
										else
										{

												bzero(buffer,256);
												strcpy(buffer,"ENVOI");
												//printf("ENVOI \n");
												n=write(sockfd,buffer,strlen(buffer));
												if (n<0)
													error("ERROR reading from socket");
												bzero(buffer,256);
												printf("Demande de I=%d J=%d \n",((i+u+taille)%taille),((j+k+taille)%taille));
												sprintf(buffer," I=%d J=%d ",((i+u+taille)%taille),((j+k+taille)%taille));
												//printf("I=%d J=%d \n",i+u,j+k);
												n=write(sockfd,buffer,strlen(buffer));
												sleep(1);
												if (n<0)
													error("ERROR reading from socket");
												bzero(buffer,256);
												n=read(sockfd,buffer,1);
												if(n<0)
													error("ERROR reading from socket");
												val=atoi(buffer);
												//printf("Valeur reçue : %d \n",val);
												if(val==1)
													cptv++;
										
										}
									}						
								}
						
							}
					printf("Nb voisins %d pour i=%d j=%d \n",cptv,i,j);			
					if(cptv==3)
						bloc2[i][j]=1;
					else printf("Valeur inchangée \n");
				}
			
			
			//FIN FOR
			}
		}//FIN DOUBLE FOR
		printf("Nouveau bloc : \n");
		for(i=0;i<taille_b;i++)
		{
			for(j=0;j<taille_b;j++)
			{
				printf("%d ",bloc2[i][j]);
			}
			printf("\n");
		}
		/*RENVOI*/
		bzero(buffer,256);
		strcpy(buffer,"BTRAITE");
		printf("Bloc traité \n");
		n=write(sockfd,buffer,strlen(buffer));
		if (n<0)
			error("ERROR reading from socket");
		bzero(buffer,256);
		char btraite[1];
		int bt=0;
		for(i=0;i<taille_b;i++)
		{
			for(j=0;j<taille_b;j++)
			{
				sprintf(btraite,"%i",bloc2[i][j]);
				n=write(sockfd,btraite,strlen(btraite));
				if(n<0)
					error("ERROR ecrire dans socket");
			}
		}
		n=write(sockfd,btraite,strlen(btraite));
		if (n<0)
			error("ERROR reading from socket");
		/*ARRET*/
	
		bzero(buffer,256);
		char fin[5]="FIN";
		while(strcmp(buffer,fin)!=0)
		{
			bzero(buffer,256);
			n=read(sockfd,buffer,256);
			if(n<0)
				error("ERROR reading from socket");
		}
		bzero(buffer,256);
		strcpy(buffer,"ARRET");
		n=write(sockfd,buffer,256);
		if (n<0)
			error("ERROR reading from socket");
		bzero(buffer,256);
		printf("Demande d'arret \n");	
		printf("*********** FINI************** \n");
		printf("********************************** \n\n");
		printf("********************************** \n\n");
			/*printf("Recommencer O/N ?\n");
			//while ((rep = getchar()) != '\n' && rep != EOF);
				scanf("%c",&rep);
			if(rep!='O'||rep!='N')
			{
				printf("Merci d'entrer O ou N \n");
				error("Mauvaise réponse\n");
			}*/	
		}
		while(1);
		return 0;
	
}
