#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h> 
#include <errno.h>
#define NMAX 200


void *EnvoiMessage(int dSock){
	char mot[NMAX];
	char nomFichier [NMAX];
	FILE* fichier; 
	int res;
	int tailleMsg;
	while(1){
		printf("Que voulez vous envoyer ?\n");
		fgets(mot,NMAX,stdin);
		*strchr(mot, '\n') = '\0';
		tailleMsg = strlen(mot);
		res = send(dSock,&tailleMsg,sizeof(int),0);
				
		if (res<0){
			perror("Taille du message non envoyé");
			exit(0);
		}

		res = send(dSock,&mot,tailleMsg,0);
		if (res<0){
			perror("Message non envoyé");
			exit(0);
		}

		if(strcmp(mot, "fin") == 0){
			printf("Communication fermée\n");
			close(dSock);
			exit(0);
		}

		if(strcmp(mot, "file") == 0){
			char chaine [NMAX]="";
			char copie [NMAX] = {0};
			char nomFichier [15];
			int tailleNom;
			int tailleContenu;

			DIR * rep = opendir (".");
			if (rep != NULL){
				struct dirent * ent;
				while ((ent = readdir (rep)) != NULL){
					printf ("%s\n", ent->d_name);
				}
				closedir (rep);
			} else {
				perror ("le dossier est vide ou n'existe pas\n");
			}

			printf("Quel fichier voulez vous envoyer ? \n");
			fgets(nomFichier, NMAX, stdin);
			*strchr(nomFichier, '\n') = '\0';

			tailleNom = strlen(nomFichier);
			res = send(dSock,&tailleNom,sizeof(int),0); /* Envoie de la taille du nom du fichier */

			if (res<0){
				perror("Taille du nom du fichier non envoyé");
				exit(1);
			} else if(res == 0){
				perror("Socket fermée");
				exit(0);
			}

			res = send(dSock,&nomFichier,tailleNom,0); /* Envoie du nom du fichier */

			if (res<0){
				perror("Nom du fichier non envoyé");
				exit(1);
			} else if(res == 0){
				perror("Socket fermée");
				exit(0);
			}

			FILE* file = NULL;
			file = fopen(nomFichier, "r");

			while (fgets(chaine, NMAX, file) != NULL){
				printf("Données copiées : %s\n",chaine);
				strcat(copie, chaine);  /* pour pouvoir concaténer toutes les lignes du fichier */
			}

			fclose(file);
			
			tailleContenu = strlen(chaine);
			
			res = send(dSock,&tailleContenu,sizeof(int),0); /* Envoie de la taille du contenu du fichier */

			if (res<0){
				perror("Taille du contenu du fichier non envoyé");
				exit(1);
			} else if(res == 0){
				perror("Socket fermée");
				exit(0);
			}

			res = send(dSock,&chaine,strlen(chaine),0); /* Envoie du contenu du fichier */

			if(res<0){
				perror("Contenu du fichier non envoyé");
				exit(1);
			} else if(res == 0){
				perror("Socket fermée");
				exit(0);
			}
			
		}
	}
}

void *RecoitMessage(int dSock){
	int tailleMsg;
	char msg[NMAX];
	int res;
	while(1){
		do {
			res = recv(dSock, &tailleMsg, sizeof(int), 0);
		} while(tailleMsg == 0 || tailleMsg > NMAX);

		res = recv(dSock, &msg, NMAX, 0);
		if (res<0){
			perror ("le message pour dire que le client n'a pas recu le message de l'autre n'a pas ete recu");
			exit(1);
		} else if(res == 0){
			perror("Socket fermé");
			exit(0);
		}

		msg[tailleMsg]='\0';
		printf("Le client dit : %s\n", msg);
		if(strcmp(msg, "fin") == 0){
			printf("Communication fermée\n");
			close(dSock);
			exit(0);
		}

		if(strcmp(msg, "file") == 0){
			int tailleNom;
			int tailleContenu;
			char nomFichier[NMAX];
			char contenuFichier[NMAX];
			char* res2;
			printf("Prêt à recevoir un ficher...\n");

			res = recv(dSock, &tailleNom, sizeof(int),0); /* Réception de la taille du nom du fichier */

			if(res < 0){
				perror("Problème lors de la réception de la taille du nom du fichier");
				exit(1);
			} else if(res == 0){
				perror("Socket fermé");
				exit(0);
			}

			res = recv(dSock, &nomFichier, tailleNom,0); /* Réception du nom du fichier */

			if(res < 0){
				perror("Problème lors de la réception du nom du fichier");
				exit(1);
			} else if(res == 0){
				perror("Socket fermé");
				exit(0);
			}

			printf("Nom du fichier : %s\n", nomFichier); /* Affichage du nom du fichier */

			res = recv(dSock, &tailleContenu, sizeof(int),0); /* Réception de la taille du contenu du fichier */

			if(res < 0){
				perror("Problème lors de la réception de la taille du contenu du fichier");
				exit(1);
			} else if(res == 0){
				perror("Socket fermé");
				exit(1);
			}

			res = recv(dSock, &contenuFichier, tailleContenu,0); /* Réception du contenu du fichier */

			if(res < 0){
				perror("Problème lors de la réception du contenu du fichier");
				exit(1);
			} else if(res == 0){
				perror("Socket fermé");
				exit(1);
			}

			contenuFichier[tailleContenu] = '\0';

			printf("Contenu du fichier : %s\n", contenuFichier); /* Affichage du nom du fichier */

			FILE* newFile = NULL;

    		newFile = fopen(nomFichier, "a+");

			if (newFile != NULL){
				res2 = fputs(contenuFichier, newFile);
				if(res2 == EOF){
					perror("Erreur lors de l'écriture dans le fichier");
				}
				fclose(newFile);
			} else {
				perror("Erreur lors de la création/ouverture du fichier");
			}

		}
	}
}

int main(int argc, char ** argv){

	char* addr_ip = argv[1];
	

	int dSock = socket(PF_INET, SOCK_STREAM, 0);
	if (dSock < 0){
		perror("Probleme de socket");
		return 1;
	}

	struct sockaddr_in adServ;
	adServ.sin_family = AF_INET;
	adServ.sin_port = htons(atoi(argv[2]));
	int res = inet_pton(AF_INET, addr_ip, &(adServ.sin_addr)); 
	socklen_t lgA = sizeof(struct sockaddr_in); 
	res = connect(dSock, (struct sockaddr *) &adServ, lgA); 
	if (res<0){
		perror("Probleme de connection au serveur");
		return 1;
	}

	int numClient;
	res = recv(dSock, &numClient, sizeof(numClient), 0);
	if (res<0){
		perror ("Erreur lors de la réception du numéro de client");
		return 1;
	}

	if(numClient == 1){
		printf("%s\n", "Bonjour client1. En attente du deuxième client...");
		//Attendre 2ème client
		char msgAttente[NMAX];
		res = recv(dSock, &msgAttente, NMAX, 0);
		if (res<0){
			perror("le message pour dire que le deuxieme client s'est connecte n'a pas ete recu");
			return 1;
		} else {
			printf("%s\n", msgAttente);
		}
	}

	
	pthread_t Envoie;
	pthread_create(&Envoie, 0, EnvoiMessage, dSock);

	pthread_t Recoit;
	pthread_create(&Recoit, 0, RecoitMessage, dSock);

	
	pthread_join(Envoie,0);
	pthread_join(Recoit,0);

	close(dSock);
	
	return 0;
}
