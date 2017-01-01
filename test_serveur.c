#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
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
	
	
	int taille=param[0];
	int nbbloc=param[1];
	int taillebloc=taille/sqrt(nbbloc);
	int nbbloccote=taille/taillebloc;	
	int points[nbbloc][2];
	int shtr;
	int shenv;
	int* fini;
	int shmid;
	int shmid2;
	int shmid3;
	int* first_lap;
	int shmid4;
	int** grilleT;
	int* envoye;
	int* traite;
	if((shmid = shmget(IPC_PRIVATE, taille*sizeof(int*), IPC_CREAT | IPC_EXCL | 0700))<0)
	{
        	perror("La mémoire paratgée c nul.");
       		exit(1);
    	}
    	if((shmid3 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0700))<0)
	{
        	perror("La mémoire paratgée c nul.");
       		exit(1);
    	}
    	fini=(int*)shmat(shmid3,NULL,0);
    	if((shmid4 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0700))<0)
	{
        	perror("La mémoire paratgée c nul.");
       		exit(1);
    	}
    	first_lap=(int*)shmat(shmid4,NULL,0);
    	first_lap[0]=1;
	grilleT= (int**)shmat(shmid, NULL, 0);
	for(i=0;i<taille;i++)
	{
		shmid2 = shmget(IPC_PRIVATE, taille*sizeof(int), IPC_CREAT | IPC_EXCL | 0700);
		grilleT[i]=(int*)shmat(shmid2,NULL,0);
		if(grilleT[i]==NULL)
		{
			perror("Shmget2");
			exit(1);
		}
	}
	if((shenv = shmget(IPC_PRIVATE, taille*sizeof(int), IPC_CREAT | IPC_EXCL | 0700))<0)
	{
        	perror("La mémoire paratgée c nul.");
       		exit(1);
    	}
    	envoye = (int*)shmat(shenv, NULL, 0);
    	if((shtr = shmget(IPC_PRIVATE, taille*sizeof(int), IPC_CREAT | IPC_EXCL | 0700))<0)
	{
        	perror("La mémoire paratgée c nul.");
       		exit(1);
    	}
    	traite = (int*)shmat(shtr, NULL, 0);
	//printf("taille d'un cote de la grille : %i, bloc/cote : %i, taille du cote d'un bloc : %i \n",n,nbbloccote,taillebloc);
	for(i=0;i<nbbloc;i++)
	{
		points[i][0]=i*taillebloc%(taille);
		points[i][1]= (i*taillebloc/(taille))*taillebloc;
		//CASE EN HAUT A GAUCHE
		//printf("coordonnées du bloc %i x : %i, y : %i \n",i,points[i][0],points[i][1]);	

	}
	/************************* SERVEUR *******************************/
	
	struct sockaddr_in serv_addr, client_addr;
	
	int nbcli=param[2];
	
	int cptcli=0;
	int rescmp=1;
	int retour=0;
	int rescmp1=1;
	int rescmp2=1;
	int rescmp3=1;
	int rescmp4=1;
	int rescmp5=1;
	int arret=0;
	pid_t pid;
	char str1[10],str2[10],str3[10],str4[10],str5[10],str6[10];

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
		if(fini[0]==1)
		{
			first_lap[0]=1;
			for(i=0;i<taille;i++)
			{
				for(j=0;j<taille;j++)
				{
					grille[i][j]=grilleT[i][j];
					printf("%d ",grille[i][j]);
				}
				printf("\n");
			}
			printf("Grille recopiée \n");
		}
		//*******************************Affichage grille initiale*********************
		if(first_lap[0]==1)
		{
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
			first_lap[0]=0;
		}
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
				strcpy(str6,"ARRET");
				rescmp5=strcmp(buffer,str6);
				strcpy(str5,"ENVOI");
				rescmp4=strcmp(buffer,str5);
				
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
				
				
				if(rescmp2==0)
				{/*************ON LIT LE BLOC ************/
					for(i=0;i<nbbloc;i++)
					{
						if(envoye[i]==0||traite[i]==0)
							{retour=i;printf("Retour : %d \n",retour);i=nbbloc;}
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
					envoye[retour]=1;
				
					if(n<0)
						error("ERROR ecrire dans socket");
				
				}
				/*=======================DEMANDE DE VALEURS PAR COORDONNEES=================*/
				if(rescmp4==0)
				{
					//NE PASSE QU'UNE FOIS ICI printf("Yo \n");
					bzero(buffer,256);
					n=read(newsockfd,buffer,256);
					if(n<0)
						error("ERROR lire dans socket");
					int h=0;
					int temp1,temp2;
					temp1=temp2=0;
					int f=0;
					char strt[10];
					int neg=0;
					while(buffer[h]!='\0')
					{
						if(buffer[h]=='I')
						{
							f=0;
							h=h+2;
							//On cherche le nombre de chiffres derriere "I="
							while(buffer[h]!=' ')
							{
								f++;
								h++;
							}
							char strt[f];
							h=h-f;
							f=0;
							//On récupère lapartie utile
							while(buffer[h]!=' ')
							{
								if(buffer[h]=='-')
								{
									h++;
									strt[f]=buffer[h];
									f++;
									h++;
									neg=1;
								}
								else
								{
									strt[f]=buffer[h];
									h++;
									f++;
								}
							}
							if(neg==1)
							{
								temp1=0-atoi(strt);
								//printf("Temp1 %d ",temp1);
							}
							else
							{
								temp1=atoi(strt);
								//printf("Temp1 %d \n",temp1);
							}
						}	
						if(buffer[h]=='J')
						{
							f=0;
							h=h+2;
							while(buffer[h]!=' ')
							{
								f++;
								h++;
							}
							f=f;
							char strt2[f];
							h=h-f;
							neg=0;
							f=0;
							while(buffer[h]!=' ')
							{
								if(buffer[h]=='-')
								{
									h++;
									strt2[f]=buffer[h];
									f++;
									h++;
									neg=1;
								}
								else
								{
									strt2[f]=buffer[h];
									h++;
									f++;
								}
							}
							if(neg==1)
							{
								temp2=0-atoi(strt2);
								//printf("Temp2 %d \n",temp2);
							}
							else
							{
								temp2=atoi(strt2);
								//printf("Temp2 %d \n",temp2);
							}
						}
							char valco[1];
							sprintf(valco,"%d",grille[temp1%taille][temp2%taille]);
							n=write(newsockfd,valco,strlen(valco));
							if(n<0)
								error("ERROR ecrire dans socket");	
						h++;
					
					}

				}
				if(rescmp3==0)
				{
					int k;	
					printf("Nouveau bloc reçu : \n");
					for(j=0;j<taillebloc;j++)
					{
						for(k=0;k<taillebloc;k++)
						{
							bzero(buffer,256);
							n=read(newsockfd,buffer,1);
							if(n<0)
								error("ERROR lire dans socket a btraite");
							if(atoi(buffer)<0)
								k--;
							else
							{
								grilleT[((retour*taillebloc)/taille*taillebloc)+j][((retour*taillebloc)%taille)+k]=atoi(buffer);
								printf("%d ",grilleT[((retour*taillebloc)/taille*taillebloc)+j][((retour*taillebloc)%taille)+k]);
							} 	
						}
						printf("\n");
					}
					char fin[5]="FIN";
					n=write(newsockfd,fin,strlen(fin));
					printf("FIN \n");
					if(n<0)
							error("ERROR ecrire dans socket");
					bzero(buffer,256);
					traite[retour]=1;
					printf("Retour : %d Traite : %d Envoye : %d \n",retour,traite[retour],envoye[retour]);
					if(retour==nbbloc-1)
					{
						printf("FINI \n");
						fini[0]=1;
					}

				}
				if(rescmp5==0)
				{
					
					printf("ARRET \n");
					arret=1;

				}
				/*else
				{
					close(newsockfd);
					return 0;
					exit(0); 
				}
				exit(0);*/
				}
				while(arret==0);
				close(newsockfd);	
					return 0;
					exit(0); 
		}
		
		
	}
	while(1);
	return 0;
}
		
