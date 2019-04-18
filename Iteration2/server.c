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

struct thread_args{
	int socketServer;
	int socketClient1;
	int socketClient2;
};

void *communication(struct thread_args *args){
	printf("%d\n", args->socketServer);

}


int main(int argc, char const *argv[]){


	int res;

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

	while(1){
		int dSocketClient1 = accept(dSocket, (struct sockaddr*)&addrClient1, &lg1);

		if(dSocketClient1 < 0){
			perror("Problème lors de la création du socket client1");
			return 1;
		}

		int numClient1 = 1;
		res = send(dSocketClient1, &numClient1, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie du numéro du client1");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		int dSocketClient2 = accept(dSocket, (struct sockaddr*)&addrClient2, &lg2);

		if(dSocketClient2 < 0){
			perror("Problème lors de la création du socket client2");
			return 1;
		}

		int numClient2 = 2;
		res = send(dSocketClient2, &numClient2, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie du numéro du client2");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		char messageConfirmation[NMAX] = "Le client 2 est connecté, vous pouvez commencer la communication\0";
		res = send(dSocketClient1, &messageConfirmation, strlen(messageConfirmation),0);

		if(res < 0){
			perror("Problème lors de l'envoie de la confirmation au client1");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		/*
		while(1){
			char msgClient1[NMAX];
			char msgClient2[NMAX];

			res = recv(dSocketClient1, &msgClient1, NMAX,0);

			if(res < 0){
				perror("Problème lors de la réception du message du client1");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			printf("Le client1 dit : %s\n", msgClient1);
			
			res = send(dSocketClient2, &msgClient1, strlen(msgClient1),0);
			
			if(strcmp(msgClient1, "fin") == 0){
				close(dSocketClient1);
				close(dSocketClient2);
				break;
			}

			if(res < 0){
				perror("Problème lors de l'envoie du message du client1 au client2");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			res = recv(dSocketClient2, &msgClient2, NMAX,0);

			if(res < 0){
				perror("Problème lors de la réception du message du client2");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			printf("Le client2 dit : %s\n", msgClient2);

			res = send(dSocketClient1, &msgClient2, strlen(msgClient2),0);

			if(strcmp(msgClient2, "fin") == 0){
				close(dSocketClient1);
				close(dSocketClient2);
				break;
			}

			if(res < 0){
				perror("Problème lors de l'envoie du message du client2 au client1");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}
		}
		*/

		pthread_t threadClient1;
		struct thread_args argsClient1;
		argsClient1.socketServer = dSocket;
		argsClient1.socketClient1 = dSocketClient1;
		argsClient1.socketClient2 = dSocketClient2;
		pthread_create(&threadClient1, 0, communication, &argsClient1);

		pthread_t threadClient2;
		struct thread_args argsClient2;
		argsClient2.socketServer = dSocket;
		argsClient2.socketClient1 = dSocketClient2;
		argsClient2.socketClient2 = dSocketClient1;
		pthread_create(&threadClient2, 0, communication, &argsClient2);

		pthread_join(threadClient1,0);
		pthread_join(threadClient2,0);

		close(dSocketClient1);
		close(dSocketClient2);


	}
	close(dSocket);

	return 0;
}
