#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <math.h>

void error(char* msg)
{
	perror(msg);
	exit(1);
}
int** la_vie_mais_pas_en_chine(int** grille, int taille)
{
	
	return grille;
	
}

int main(int argc, char* argcv[])
{
	int sockfd,newsockfd;
	int n,numport, cli_addrlen;
	char buffer[256];
	FILE *init;
	if((init = fopen("init.txt","r"))==NULL)
		return EXIT_FAILURE;
	int param[3];
	int i=0;
	int a=0;
	int ret;
	int j=0;
	
	//**********************Récupération des premiers parametres******************
	while (i<3 && (ret = fscanf (init,"%i ",&a)) != EOF && ret != 0)
	{	
			param[i]=a;
			printf(" init : %i \n",param[i]);
			i++;
	}
	//************************Taille> bloc*****************************************
	/*if(param[0]<param[1])
		error("Espece de singe");*/
	//************************Taille pas divisible par nb blocs**********************
	/*if(param[0]%param[1]!=0)
		error("Apprend a compter");*/
	
	//****************************Création de la grille de 0 de la bonne taille*****
	int grille[param[0]][param[0]];
	for(i=0;i<param[0];i++)
	{
		for(j=0;j<param[0];j++)
		{
			grille[i][j]=0;	
		}
	}
	
	//***************************Récupération des cellules vivantes de l'init**************
	int cv[param[0]*param[0]];
	int cpt=0;	
	while ((ret = fscanf (init,"%i ",&a)) != EOF && ret != 0)
	{
			if(a>param[0])
				error("Elle est pas dans la grille laul");
			cv[cpt]=a;
			printf(" cv : %i \n",cv[cpt]);
			cpt++;

	}
	
	//***************************Remplissage des cellules vivantes de l'init**************
	j=0;
	for(j=0;j<cpt;j=j+2)
	{
		grille[cv[j]][cv[j+1]]=1;
		printf("%i,%i \n",cv[j],cv[j+1]);
	}	
	fclose(init);
	
	//*******************************Affichage grille initiale*********************
	i=0;
	for (i;i<param[0];i++)
	{
		j=0;
		for (j;j<param[0];j++)
		{
			printf("%i ",grille[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	//************************La vie mais pas en chine******************************
	srand(time(NULL));
	int LAUL= rand()%100;
	if(LAUL<30)
	{
		srand(time(NULL));
		int i_random= rand()%param[0];
		int j_random= rand()%param[0];
		if(grille[i_random][j_random]==1)
			grille[i_random][j_random]=0;
			else
				grille[i_random][j_random]=1;
	}
	//*******************************Affichage grille pas chinoise********************
	i=0;
	for (i;i<param[0];i++)
	{
		j=0;
		for (j;j<param[0];j++)
		{
			printf("%i ",grille[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	/*********************************LA MORT DIAGONALE***********************/
	
	srand(time(NULL));
	int unponey= rand()%100;
	if(unponey<30)
	{
		srand(time(NULL));
		int i_random= rand()%param[0];
		int j_random= rand()%param[0];
		while(i_random<param[0]&&j_random<param[0])
		{
			grille[i_random][j_random]=0;
			i_random++;
			j_random++;
		}
			
	}
	//*******************************Affichage grille après la mort********************
	i=0;
	for (i;i<param[0];i++)
	{
		j=0;
		for (j;j<param[0];j++)
		{
			printf("%i ",grille[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	int taille=param[0];
	int nbbloc=param[1];
	int taillebloc=taille/sqrt(nbbloc);
	int nbbloccote=taille/taillebloc;	
	int points[nbbloc][2];
	int envoye[nbbloc];
	memset( envoye, 0, nbbloc*sizeof(int) );
	//printf("taille d'un cote de la grille : %i, bloc/cote : %i, taille du cote d'un bloc : %i \n",n,nbbloccote,taillebloc);
	for(i=0;i<nbbloc;i++)
	{
		points[i][0]=i*taillebloc%(taille);
		points[i][1]= (i*taillebloc/(taille))*taillebloc;
		//CASE EN HAUT A GAUCHE
		//printf("coordonnées du bloc %i x : %i, y : %i \n",i,points[i][0],points[i][1]);	

	}
	/**************************AFFICHAGE DES BLOCS *******************************/
	for(i=0;i<nbbloc;i++)
	{
		printf("Bloc n°%i :\n",i);
		for (j=0;j<taillebloc;j++)
		{
			int k=0;
			for (k=0;k<taillebloc;k++)
			{
				printf("%i ",grille[((i*taillebloc)/taille*taillebloc)+j][((i*taillebloc)%taille)+k]);		
			}printf("\n");
		}
	}
	/************************* SERVEUR *******************************/
	
	struct sockaddr_in serv_addr, client_addr;
	
	int nbcli=param[2];
	
	int cptcli=0;
	int rescmp=1;
	int rescmp1=1;
	int rescmp2=1;
	int rescmp3=1;
	int rescmp4=1;
	pid_t pid;
	char str1[10],str2[10],str3[10],str4[10];

	/********************************NUMERO DE PORT**************************/
	if (argc<2)
	{
		fprintf(stderr,"ERROR, numero de port manquant \n");
		exit(1);
	}
	/********************************CREATION SOCKET***************************/
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
		error("Pas possible de créer le socket");
		
	bzero((char *) &serv_addr,sizeof(serv_addr));
	numport=atoi(argcv[1]);
	char tempo[3];
	char tempo2[3];
	char tempo3[3];
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(numport);
	/**********************************BIND*********************************/
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
			do
			{
				bzero(buffer,256);
				n=read(newsockfd,buffer,256);
				if(n<0)
					error("ERROR READING FROM SOCKET");
				strcpy(str1,"TAILLE_G");
				rescmp=strcmp(buffer,str1);
				strcpy(str2,"TAILLE_B");
				rescmp1=strcmp(buffer,str2);
				strcpy(str3,"BLOC");
				rescmp2=strcmp(buffer,str3);
				strcpy(str4,"BTRAITE");
				rescmp3=strcmp(buffer,str4);
				sprintf(tempo,"%d",(taille));
				sprintf(tempo2,"%d",(taillebloc));
				if(rescmp==0)
				{
					n=write(newsockfd,tempo,strlen(tempo));
					if(n<0)
						error("ERROR ecrire dans socket");

				}
			
			
				if(rescmp1==0)
				{
					n=write(newsockfd,tempo2,strlen(tempo2));
					if(n<0)
						error("ERROR ecrire dans socket");

				}
				int retour=0;
				
				if(rescmp2==0)
				{/*************ON LIT LE BLOC ************/
					for(i=0;i<nbbloc;i++)
					{
						if(envoye[i]==0)
							{retour=i;i=nbbloc;}
					}
					i=retour;
					bzero(buffer,256);
					j=0;
					int k=0;
					while(j<taillebloc)
					{
						k=0;
						while(k<taillebloc)
						{
								sprintf(tempo3,"%i",(grille[((i*taillebloc)/taille*taillebloc)+j][((i*taillebloc)%taille)+k]));
								n=write(newsockfd,tempo3,strlen(tempo3));
								if(n<0)
									error("ERROR ecrire dans socket");							
							k++;
						}
						j++;
					}
					envoye[i]=1;
				
					if(n<0)
						error("ERROR ecrire dans socket");
				
				}
				
				
				/*else
				{
					close(newsockfd);
					return 0;
					exit(0); 
				}
				exit(0);*/
				}
				while(strcmp(buffer,"STOP")!=0);
				close(newsockfd);
					return 0;
					exit(0); 
		}
		close(newsockfd);
	}
	while(1);
	return 0;
}
		
