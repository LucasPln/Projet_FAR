#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h> 
#define NMAX 200


void *EnvoiMessage(int dSock){
	char mot[NMAX];
	char nomFichier [NMAX];
	FILE* fichier; 
	int res;
	while(1){
		printf("Que voulez vous envoyer ?\n");
		fgets(mot,NMAX,stdin);
		*strchr(mot, '\n') = '\0';
		res = send(dSock,&mot,strlen(mot),0);
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
			DIR * rep = opendir (".");
			if (rep != NULL){
				struct dirent * ent;
				while ((ent = readdir (rep)) != NULL){
					printf ("%s\n", ent->d_name);
				}
				closedir (rep);
			}
			printf("Quel fichier voulez vous envoyer ? \n");
			fgets(nomFichier, NMAX, stdin);
			fichier = fopen(nomFichier, "r");

		}
	}
}

void *RecoitMessage(int dSock){
	char msg[NMAX];
	int res;
	while(1){ 
		res = recv(dSock, &msg, NMAX, 0);
		if (res<0){
			perror ("le message pour dire que le client n'a pas recu le message de l'autre n'a pas ete recu");
			exit(0);
		}
		printf("Le client dit : %s\n", msg);
		if(strcmp(msg, "fin") == 0){
			printf("Communication fermée\n");
			close(dSock);
			exit(0);
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
