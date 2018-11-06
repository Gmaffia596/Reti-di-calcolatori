#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
//#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include <stdio.h>
#define BUFFERSIZE 512

void errorhandler(char *msg)	{
	printf("%s", msg);
}

void clearwinsock()		{
	#if defined WIN32
	WSACleanup();
	#endif
}

struct {
	int a;
} msgStruct;

int main(int argc, char *argv[]) 	{

	int port;
	if(argc >1)	{
		port = atoi(argv[1]);
	}else
		port = 27015;
	if(port < 0)	{
		printf("bad port number %s\n", argv[1]);
		return 0;
	}

#if defined WIN32 // Initialize Winsock

	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != 0) {
		printf("Error at WSAStartup()\n");
		return 0;
	}

#endif

	//CREAZIONE DELLA SOCKET e controllo
	int my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(my_socket < 0){
		errorhandler("socket creation failed. \n");
		return -1;
	}

	//ASSEGNAZIONE DI UN LOCAL ADDRESS ALLA SOCKET

	//creare un elemento di tipo sockaddr_in
	struct sockaddr_in sad;

	//avvalorare l'elemento creato
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port);

	//assegnazione porta e ip alla socket e controllo errori
	if(bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) <0){
		errorhandler("bind() failed. \n");
		closesocket(my_socket);
		return -1;
	}

	//SETTAGGIO LA SOCKET ALL'ASCOLTO
	int qlen = 6;
	if(listen(my_socket, qlen) < 0){
		errorhandler("listen() failed. \n");
		closesocket(my_socket);
		return -1;
	}

	/*
	 * ITERATIVAMENTE:
	 * a) accettare una nuova connessione;
	 * b) inviare e ricevere dati;
	 * c) chiudere la connessione.
	 */

	// a)
	struct sockaddr_in cad;		//struttura per l'indirizzo del client
	int client_socket;			//socket descrittore del client
	int client_len;				//dimensione dell'indirizzo del client
	printf("Waiting for a client to connect...");

	while(1){
		client_len = sizeof(cad);
		if((client_socket = accept(my_socket, (struct sockaddr*) &cad, &client_len)) <0) {
			errorhandler("accept() failed.\n");
			closesocket(my_socket);
			clearwinsock();
			return -1;
		}

		//client_socket è connesso al client
		printf("Connection estabilished with %s: %d\n", inet_ntoa(cad.sin_addr), ntohs(cad.sin_port));
		//handleclientconnection(client_socket);
	}



	// RICEVERE DATI DAL CLIENT
	int bytes_rcvd;
	int total_bytes_rcvd = 0;
	char buf[BUFFERSIZE];	//buffer for data from the server
	printf("Received: ");	//setup to print the echoed string

	while(total_bytes_rcvd < 100)	{
		if	((bytes_rcvd = recv(my_socket, buf, BUFFERSIZE -1, 0)) <= 0)
		{
			errorhandler("recv() failed or connection closed prematurely");
			closesocket(my_socket);
			clearwinsock();
			return -1;
		}
		total_bytes_rcvd += bytes_rcvd;
		buf[bytes_rcvd] = '\0';
		printf("%s", buf);
	}

	char *input_string = "b";	//stringa da inviare
	int string_len = strlen(input_string);	//determina la lunghezza

	// INVIARE DATI AL CLIENT
		if(send(my_socket, input_string, string_len, 0) != string_len){
			errorhandler("send() sent a different number of bytes than expected\n");
			closesocket(my_socket);
			clearwinsock();
			return -1;
		}
	//send(my_socket, &msg, sizeof(msgStruct), 0);
	//printf("questa è la risposta del server");


	closesocket(my_socket);

#if defined WIN32
	WSACleanup();
#endif
	return 0;
} // main end
