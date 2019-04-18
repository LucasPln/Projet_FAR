#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define NMAX 200

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
	/*
	Reception message server 
	*/
	int numClient;
	res = recv(dSock, &numClient, sizeof(numClient), 0);
	if (res<0){
		perror ("le message pour dire que le client s'est connecte n'a pas ete recu");
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
		while(1){
			
			//Envoi du message à 2 
			char mot[NMAX]; 
			printf("Que voulez vous envoyer ?\n");
			fgets(mot,NMAX,stdin);
			*strchr(mot, '\n') = '\0';
			res = send(dSock,&mot,strlen(mot),0);
			if (res<0){
				perror("Message non envoyé");
				return 1;
			}

			if(strcmp(mot, "fin") == 0){
				printf("Communication fermée\n");
				close(dSock);
				return 0;
			}

			//reception du message de 2
			char msg2[NMAX];
			res = recv(dSock, &msg2, NMAX, 0);
			if (res<0){
				perror ("le message pour dire que le client s'est connecte n'a pas ete recu");
				return 1;
			}

			printf("Le client2 dit : %s\n", msg2);

			if(strcmp(msg2, "fin") == 0){
				printf("Communication fermée\n");
				close(dSock);
				return 0;
			}
		}
		close(dSock);
		

	} else if (numClient == 2){
		printf("%s\n", "Bonjour client2.");
		while(1){
			//Reception du message de 1 
			char msg1[NMAX]; 
			res = recv(dSock, &msg1, NMAX, 0);

			if (res<0){
				perror ("le message pour dire que le client 2 a recu un message du client 1 n'a pas ete recu");
				return 1;
			}

			printf("Le client1 dit : %s\n", msg1);

			if(strcmp(msg1, "fin") == 0){
				printf("Communication fermée\n");
				close(dSock);
				return 0;
			}

			//Envoie du message à 1
			char mot[NMAX]; 
			printf("Que voulez vous envoyer ?\n");
			fgets(mot,NMAX,stdin);
			*strchr(mot, '\n') = '\0';
			res = send(dSock,&mot,strlen(mot),0);
			if (res<0){
				perror("Message non envoyé");
				return 1;
			}

			if(strcmp(mot, "fin") == 0){
				printf("Communication fermée\n");
				close(dSock);
				return 0;
			}
		}
		close(dSock);
		
	}
	
	return 0;
}
