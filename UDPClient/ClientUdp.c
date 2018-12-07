#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> // for DNS
#include <limits.h>
#include <ctype.h>

#include "Libreria.h"

#define PORT 48000
#define ECHOMAX 255


int main() {
	//char resp;
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	unsigned int fromSize;
	char echoString[ECHOMAX];
	char echoBuffer[ECHOMAX];
	int echoStringLen;
	int respStringLen;
	//VARIABILI PER ACQUISIZIONE E STRTOK
	char connServer[ECHOMAX]; 	// stringa contenente host e ip
	char *item; 				// storerà il token
	indirizzo serv; 			// struct necessaria per allocare host e ip separati
	int k = 0; 					// contatore per l'array nel quale verranno storati i token
	char *store[2];				// array contenente i token da utilizzare
	//VAR DNS
	struct hostent *host1;  //echoServAddr

	// CREAZIONE DELLA SOCKET
	sock=creasocket();
	/*
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)	{
		errorhandler("socket creation failed.\n");
		close(sock);
		return -1;
	}
*/
	// CLIENT LEGGE DA NOME E PORT
	puts("Inserisci il nome del server e la porta:");
	scanf("%s", connServer);
	
	item = strtok(connServer, ":");

	while(item != NULL)	{
		store[k] = item;
		k++;
		item = strtok(NULL, ":");
	}

	strcpy(serv.host, store[0]);
	serv.ip = atoi(store[1]);
	
	//printf("%d", serv.ip); //test porta
	if(((host1 = gethostbyname(serv.host)) == NULL) || (serv.ip != PORT))	{
		fprintf(stderr, "host o numero di porta errato\n");
		exit(EXIT_FAILURE);
	} else	{
		// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
		memset(&echoServAddr, 0, sizeof(echoServAddr));
		memcpy(&echoServAddr.sin_addr, host1->h_addr_list[0], host1->h_length); //converte e copia l'host name in ip per il server
		echoServAddr.sin_family = AF_INET;
		echoServAddr.sin_port = htons(serv.ip);
	}

	//printf("%d", fromAddr.sin_addr.s_addr);
char uguale[2];

do
{
	// VISUALIZZA L'HELP
	//help();	
	system("clear");
	puts("---------------------------------------------");
	puts("CALCOLATRICE CLIENT/SERVER UDP");
	printf("Inserire la stringa da calcolare; il primo parametro deve essere\nuna lettera, la quale identifica l'operazione:\nA - Addizione\nM - Moltiplicazione\nS - Sottrazione\nD - Divisione\napplicare uno spazio tra i due operandi come in esempio: A 23 45");
	
	int a, b;
	char op[1];
	char str[20];
	printf("\nInserisci la stringa echo da inviare al server\n");
	
	scanf("%s%d%d", op, &a, &b);
	   
	strcpy(echoString, op);
	   
	strcat(echoString, ":");
	
	sprintf(str, "%d", a);  
	strcat(echoString, str);
	
	strcat(echoString, ":");
	 
	sprintf(str, "%d", b);   
	strcat(echoString, str);
	
	
	//printf("%s\n", echoString);

	if((echoStringLen = strlen(echoString)) > ECHOMAX)	{
		errorhandler("echo word too long\n");
		close(sock);
		return -1;
	}

	if(sendto(sock, echoString, ECHOMAX, 0, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)) != ECHOMAX)	{
		errorhandler("sendto() sent different number of bytes than expected\n");
		close(sock);
		return -1;
	}/*else{
		printf("Stringa inviata!\n");
	}*/
	
	// STRINGA DI RISPOSTA DEL SERVER
	fromSize = sizeof(fromAddr);
	respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*) &fromAddr, &fromSize);
	
	if(echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)	{
		fprintf(stderr, "Error: received a packet from unknown source.\n");
		exit(EXIT_FAILURE);
	}
	
	// DNS PART
	struct in_addr addr;
	char *asd = &echoServAddr.sin_addr;
	addr.s_addr = inet_addr(asd);
	
	//serv1 = gethostbyaddr((char*)&addr, 4, AF_INET);
	//host = gethostbyname(serv.host);
	struct in_addr* ina = (struct in_addr*) host1->h_addr_list[0];
	char* canonical_name = host1->h_name;
	
	printf("Risultato ricevuto dal server %s, ip %s: ", canonical_name, inet_ntoa(*ina));
/*
	char b1[sizeof(echoBuffer)] ="";
	sprintf(b1, "Received: %s\n", echoBuffer);
*/	
	
	printf("%.*s\n", respStringLen, echoBuffer);
	//memset(&echoBuffer, 0, sizeof(echoBuffer));
	//fflush(echoBuffer);
	printf("Vuoi fare un'altra operazione? Se si premere qualsiasi tasto altrimenti digita = per terminare la connessione \n");
	scanf("%s", uguale);
		
}while(uguale[0] != '=');
	
	close(sock);
	return EXIT_SUCCESS;
}