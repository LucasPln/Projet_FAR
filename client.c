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
	adServ.sin_family = AF_INET;
	adServ.sin_port = htons((short)atoi(port));
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
	res = recv(dSock, numClient, sizeof(numCLient), 0);
	if (res<0){
		perror ("le message pour dire que le client s'est connecte n'a pas ete recu");
		return 1;
	}

	if(numCLient == 1){
	//Attendre 2ème client
	char msgAttente;
	res = recv(dSock, msgAttente, sizeof(msgAttente), 0);
	if (res<0){
		perror("le message pour dire que le deuxieme client s'est connecte n'a pas ete recu");
		return 1;
	}
	
	//Envoi du message à 2 
	char mot [NMAX]; 
	printf("Que voulez vous envoyer ?\n");
	fgets(mot,NMAX,stdin,0); 
	res = send(dSock,mot,NMAX,0);
	if (res<0){
		perror("Message non envoyé");
		return 1;
	}
	printf("%d\n",res);
	return 0; 
	//reception du message de 2
	char msg2;
	res = recv(dSock, msg2, sizeof(msg2), 0);
	if (res<0){
		perror ("le message pour dire que le client s'est connecte n'a pas ete recu");
		return 1;

	} else if (numClient == 2){

	//Reception du message de 1 
	char msg1; 
	res = recv(dSock, msg1, sizeof(msg1), 0);
	if (res<0){
		perror ("le message pour dire que le client 2 a recu un message du client 1 n'a pas ete recu");
		return 1;

	//Envoie du message à 1
	char mot [NMAX]; 
	printf("Que voulez vous envoyer ?\n");
	fgets(mot,NMAX,stdin,0); 
	res = send(dSock,mot,NMAX,0);
	if (res<0){
		perror("Message non envoyé");
		return 1;
	}
	printf("%d\n",res);
	return 0; 
	}
}
