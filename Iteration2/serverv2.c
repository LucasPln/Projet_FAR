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
#define NB_CLIENT_MAX 10


int tabSocketClient[NB_CLIENT_MAX];
pthread_t tabThread[NB_CLIENT_MAX];
int nbClient = 0;

struct thread_args{
	int socketServer;
	int socketClient;
	int numClient;
};

void *communication(struct thread_args *args){
	while(1){
		int i;
		int res;
		char msgClient[NMAX];

		res = recv(args->socketClient, &msgClient, NMAX,0);

		if(res < 0){
			perror("Problème lors de la réception du message du client1");
			exit(1);
		} else if(res == 0){
			perror("Socket fermé");
			exit(1);
		}

		printf("Un client dit : %s\n", msgClient);

		for (i = 0; i < NB_CLIENT_MAX; ++i){
			if(tabSocketClient[i] != 0 && i != (args->numClient)-1){
				res = send(tabSocketClient[i], &msgClient, strlen(msgClient),0);
			}
			
		}
		
				
		if(strcmp(msgClient, "fin") == 0){
			close(args->socketClient);
			for (i = 0; i < NB_CLIENT_MAX; ++i){
				if(tabSocketClient[i] != 0 && i != (args->numClient)-1){
					close(tabSocketClient[i]);
				}
				
			}
			break;
		}

		if(res < 0){
			perror("Problème lors de l'envoie du message du client1 au client2");
			exit(1);
		} else if(res == 0){
			perror("Socket fermé");
			exit(1);
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

	while(nbClient < NB_CLIENT_MAX){

		struct sockaddr_in addrClient;
		socklen_t lg = sizeof(struct sockaddr_in);

		int dSocketClient = accept(dSocket, (struct sockaddr*)&addrClient, &lg);
		tabSocketClient[nbClient] = dSocketClient;

		if(dSocketClient < 0){
			perror("Problème lors de la création du socket client");
			return 1;
		}

		nbClient = nbClient + 1;

		res = send(dSocketClient, &nbClient, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie du numéro du client2");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		if(nbClient == 2){
			char messageConfirmation[NMAX] = "Le client 2 est connecté, vous pouvez commencer la communication\0";
			res = send(tabSocketClient[0], &messageConfirmation, strlen(messageConfirmation),0);

			if(res < 0){
				perror("Problème lors de l'envoie de la confirmation au client1");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}
		}

		


		pthread_t threadClient;
		struct thread_args argsClient;
		argsClient.socketServer = dSocket;
		argsClient.socketClient = dSocketClient;
		argsClient.numClient = nbClient;
		tabThread[nbClient-1] = threadClient;
		pthread_create(&tabThread[nbClient-1], 0, communication, &argsClient);
	}

	for (i = 0; i < NB_CLIENT_MAX; ++i){
		pthread_join(tabThread[i],0);
	}
	close(dSocket);

	return 0;
}
