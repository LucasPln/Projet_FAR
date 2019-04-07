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


int main(int argc, char const *argv[]){


	int res;
	int dSocket = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in ad;
	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = INADDR_ANY;
	ad.sin_port = htons(atoi(argv[1]));

	bind(dSocket, (struct sockaddr*)&ad, sizeof(ad));
	listen(dSocket, 7);
	struct sockaddr_in aC;

	socklen_t lg = sizeof(struct sockaddr_in);

	while(1){
		int dSocketClient1 = accept(dSocket, (struct sockaddr*)&aC, &lg);

		if(dSocketClient1 < 0){
			perror("Problème lors de la création du socket client1");
			return 1;
		}

		res = send(dSocketClient1, 1, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie du numéro du client1");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		int dSocketClient2 = accept(dSocket, (struct sockaddr*)&aC, &lg);

		if(dSocketClient2 < 0){
			perror("Problème lors de la création du socket client2");
			return 1;
		}

		res = send(dSocketClient2, 2, sizeof(int),0);

		if(res < 0){
			perror("Problème lors de l'envoie du numéro du client2");
			return 1;
		} else if(res == 0){
			perror("Socket fermé");
			return 1;
		}

		/*
		Envoie confirmation au client1
		*/

		while(1){
			char msgClient1[200];
			char msgClient2[200];

			res = recv(dSocketClient1, msgClient1, sizeof(msgClient1),0);

			if(res < 0){
				perror("Problème lors de la réception du message du client1");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}
			
			res = send(dSocketClient2, &msgClient1, sizeof(msgClient1),0);

			if(res < 0){
				perror("Problème lors de l'envoie du message du client1 au client2");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			res = recv(dSocketClient2, msgClient2, sizeof(msgClient2),0);

			if(res < 0){
				perror("Problème lors de la réception du message du client2");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}

			res = send(dSocketClient1, &msgClient2, sizeof(msgClient2),0);

			if(res < 0){
				perror("Problème lors de l'envoie du message du client2 au client1");
				return 1;
			} else if(res == 0){
				perror("Socket fermé");
				return 1;
			}
		}

		close(dSocketClient1);
		close(dSocketClient2);

	}
	close(dSocket);

	return 0;
}
