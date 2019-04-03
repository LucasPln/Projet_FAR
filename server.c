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

		if(dSocketClient < 0){
			perror("Problème lors de la création du socket client1");
			return 1;
		}

		/*
		Envoie message client1
		*/

		int dSocketClient2 = accept(dSocket, (struct sockaddr*)&aC, &lg);

		if(dSocketClient < 0){
			perror("Problème lors de la création du socket client2");
			return 1;
		}

		/*
		Envoie message client2
		Envoie confirmation au client1
		*/

		while(1){
		//Reception message client1
		
		//Envoie message client1 au client2

		//Reception message client2

		//Envoie message client2 au client1
		}
	//fermer sockets client1 & 2

	}
	//fermer socket server
}
