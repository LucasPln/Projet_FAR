#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#define NMAX 20

int main(int argc, char ** argv){

	char* addr_ip = argv[1];
	short port = argv[2];

	int dSock = socket(PF_INET, SOCK_STREAM, 0);
	if (dSock < 0){
		perror("Probleme de socket");
		return -1;
	}

	struct sockaddr_in adServ;
	adServ.sin_family = AF_INET
	adServ.sin_port = htons((short)atoi(port));
	int res = inet_pton(AF_INET, addr_ip, &(adServ.sin_addr)); 
	socklen_t lgA = sizeof(struct sockaddr_in); 
	res = connect(dSock, (struct sockaddr *) &adServ, lgA); 
	if (res<0){
		perror("Probleme de connection au serveur");
		return -2
	}
	/*
	Reception message server 
	*/

	if(numCLient == 1){
	//Attendre 2ème client
	//Envoie puis reception




	} else if (numClient == 2){

	//Reception puis envoie



	}

	char mot [NMAX]; 
	printf("Que voulez vous envoyer ?\n");
	fgets(mot,NMAX,stdin,0); 
	res = send(dSock,mot,NMAX,0);
	if (res<w0){
		perror("Message non envoyé")
		return -3
	}
	printf("%d\n",res);
	return 0; 
}