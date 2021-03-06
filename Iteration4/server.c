#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <pthread.h>
#define NMAX 200
#define NB_SALON 5

struct thread_args{
	int numSalon;
	int socketServer;
	int socketClient1;
	int socketClient2;
};

struct salon{
	int numSalon;
	int nbClientConnecte;
	int socketClient1;
	int socketClient2;
};

struct salon salons[NB_SALON];

int *communication(struct thread_args *args){
	int res;
	int tailleMsg;
	char msgClient1[NMAX];
	while(1){
		res = recv(args->socketClient1, &tailleMsg, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de la réception de la taille du message");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		}

		res = recv(args->socketClient1, &msgClient1, NMAX,0);

		if(res < 0){
			perror("Problème lors de la réception du message du client1");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		}

		msgClient1[tailleMsg]='\0';

		printf("Un client dit : %s, de taille : %d\n", msgClient1,tailleMsg);

		res = send(args->socketClient2, &tailleMsg, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie de la taille du message du client1 au client2");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		}


		res = send(args->socketClient2, &msgClient1, strlen(msgClient1),0);
				
		if(strcmp(msgClient1, "fin") == 0){
			salons[args->numSalon].nbClientConnecte = 0;
			close(args->socketClient1);
			close(args->socketClient2);
			break;
		}

		if(strcmp(msgClient1, "file")== 0){
			int tailleNom;
			int tailleContenu;
			char nomFichier[NMAX];
			char contenuFichier[NMAX];
			printf("Prêt à recevoir un ficher...\n");

			res = recv(args->socketClient1, &tailleNom, sizeof(int),0); /* Réception de la taille du nom du fichier */

			if(res < 0){
				perror("Problème lors de la réception de la taille du nom du fichier");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			}

			res = recv(args->socketClient1, &nomFichier, tailleNom,0); /* Réception du nom du fichier */

			if(res < 0){
				perror("Problème lors de la réception du nom du fichier");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			nomFichier[tailleNom] = '\0';

			printf("Nom du fichier : %s\n", nomFichier); /* Affichage du nom du fichier */

			res = send(args->socketClient2, &tailleNom, sizeof(int),0);

			if(res < 0){
				perror("Problème lors de l'envoie de la taille du nom du fichier au client2");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			}

			res = send(args->socketClient2, &nomFichier, tailleNom,0);

			if(res < 0){
				perror("Problème lors de l'envoie du nom du fichier au client2");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			}

			res = recv(args->socketClient1, &tailleContenu, sizeof(int),0); /* Réception de la taille du contenu du fichier */

			if(res < 0){
				perror("Problème lors de la réception de la taille du contenu du fichier");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			}

			res = recv(args->socketClient1, &contenuFichier, tailleContenu,0); /* Réception du contenu du fichier */

			if(res < 0){
				perror("Problème lors de la réception du contenu du fichier");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			}

			contenuFichier[tailleContenu] = '\0';

			printf("Contenu du fichier : %s\n", contenuFichier); /* Affichage du nom du fichier */

			res = send(args->socketClient2, &tailleContenu, sizeof(int),0);

			if(res < 0){
				perror("Problème lors de l'envoie de la taille du contenu du fichier au client2");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			}

			res = send(args->socketClient2, &contenuFichier, tailleContenu,0);

			if(res < 0){
				perror("Problème lors de l'envoie du contenu du fichier au client2");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				salons[args->numSalon].nbClientConnecte = 0;
				return 1;
			}
		}

		if(res < 0){
			perror("Problème lors de l'envoie du message du client1 au client2");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			salons[args->numSalon].nbClientConnecte = 0;
			return 1;
		}
	}
}


int main(int argc, char const *argv[]){


	int res;
	int i;

	int dSocket = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in ad;
	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = INADDR_ANY;
	ad.sin_port = htons(atoi(argv[1]));

	bind(dSocket, (struct sockaddr*)&ad, sizeof(ad));
	listen(dSocket, 7);

	struct sockaddr_in addrClient1;
	struct sockaddr_in addrClient2;

	socklen_t lg1 = sizeof(struct sockaddr_in);
	socklen_t lg2 = sizeof(struct sockaddr_in);

	

	for (i = 0; i < NB_SALON; i++){
		salons[i].numSalon = i;
		salons[i].nbClientConnecte = 0;
	}
	

	while(1){
		char salonChoisitChar[2] = "";
		char salonDispo[NMAX] = "";
		for (i = 0; i < NB_SALON; i++){
			char message1[] = "Salon ";
			char message2[] = "";
			char message3[] = ", place disponible : ";
			char message4[] = "";
			sprintf(message2, "%d", i);
			int placeDispo = 2-salons[i].nbClientConnecte;
			sprintf(message4, "%d", placeDispo);

			strcat(salonDispo,message1);
			strcat(salonDispo,message2);
			strcat(salonDispo,message3);
			strcat(salonDispo,message4);
			strcat(salonDispo,"\n");
		}
		int tailleSalonDispo = strlen(salonDispo);
		
		int dSocketClient = accept(dSocket, (struct sockaddr*)&addrClient1, &lg1);

		if(dSocketClient < 0){
			perror("Problème lors de la création du socket client1");
			return 1;
		}

		res = send(dSocketClient, &tailleSalonDispo, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie de la taille du nom du fichier au client2");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		res = send(dSocketClient, &salonDispo, tailleSalonDispo,0);

		if(res < 0){
			perror("Problème lors de l'envoie du nom du fichier au client2");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		res = recv(dSocketClient, &salonChoisitChar, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie du numéro du client1");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		int salonChoisit = atoi(salonChoisitChar);

		if(salons[salonChoisit].nbClientConnecte == 0){
			int numClient = 1;
			salons[salonChoisit].nbClientConnecte += 1;
			salons[salonChoisit].socketClient1 = dSocketClient;
			res = send(dSocketClient, &numClient, sizeof(int),0);

			if(res < 0){
				perror("Problème lors du numéro au client1");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

		} else if(salons[salonChoisit].nbClientConnecte == 1) {
			int numClient = 2;
			salons[salonChoisit].nbClientConnecte += 1;
			salons[salonChoisit].socketClient2 = dSocketClient;
			res = send(dSocketClient, &numClient, sizeof(int),0);

			if(res < 0){
				perror("Problème lors du numéro au client2");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			char messageConfirmation[NMAX] = "Le client 2 est connecté, vous pouvez commencer la communication\0";
			res = send(salons[salonChoisit].socketClient1, &messageConfirmation, strlen(messageConfirmation),0);

			if(res < 0){
				perror("Problème lors de l'envoie de la confirmation au client1");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			pthread_t threadClient1;
			struct thread_args argsClient1;
			argsClient1.numSalon = salonChoisit;
			argsClient1.socketServer = dSocket;
			argsClient1.socketClient1 = salons[salonChoisit].socketClient1;
			argsClient1.socketClient2 = salons[salonChoisit].socketClient2;
			pthread_create(&threadClient1, 0, communication, &argsClient1);

			pthread_t threadClient2;
			struct thread_args argsClient2;
			argsClient2.numSalon = salonChoisit;
			argsClient2.socketServer = dSocket;
			argsClient2.socketClient1 = salons[salonChoisit].socketClient2;
			argsClient2.socketClient2 = salons[salonChoisit].socketClient1;
			pthread_create(&threadClient2, 0, communication, &argsClient2);

		}
	}
	close(dSocket);

	return 0;
}
