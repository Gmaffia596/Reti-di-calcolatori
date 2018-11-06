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
#include <stdlib.h>
#define BUFFERSIZE 512
#define PROTOPORT 27015

void errorhandler(char *msg)	{
	printf("%s", msg);
}

void clearwinsock()		{
	#if defined WIN32
	WSACleanup();
	#endif
}


int main(int argc, char *argv[]) 	{

#if defined WIN32 // Initialize Winsock

	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != 0) {
		printf("Error at WSAStartup()\n");
		return 0;
	}

#endif

	//CREAZIONE DELLA SOCKET e controllo
	int c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(c_socket < 0){
		errorhandler("socket creation failed. \n");
		closesocket(c_socket);
		clearwinsock(0);
		return -1;
	}


	// costruzione dell'indirizzo del SERVER
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	//avvalorare l'elemento creato
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(PROTOPORT); // ------------------------------COSA CAZZO server port

	// connessione al server
	if(connect(c_socket, (struct sockaddr*) &sad, sizeof(sad)) <0){
		errorhandler("bind() failed. \n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}

	char *input_string = "a";	//stringa da inviare
	int string_len = strlen(input_string);	//determina la lunghezza

	// INVIARE DATI AL SERVER
	if(send(c_socket, input_string, string_len, 0) != string_len){
		errorhandler("send() sent a different number of bytes than expected\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}

	// RICEVERE DATI DAL SERVER
	int bytes_rcvd;
	int total_bytes_rcvd = 0;
	char buf[BUFFERSIZE];	//buffer for data from the server
	printf("Received: ");	//setup to print the echoed string

	while(total_bytes_rcvd < string_len)	{
		if	((bytes_rcvd = recv(c_socket, buf, BUFFERSIZE -1, 0)) <= 0)
		{
			errorhandler("recv() failed or connection closed prematurely");
			closesocket(c_socket);
			clearwinsock();
			return -1;
		}
		total_bytes_rcvd += bytes_rcvd;
		buf[bytes_rcvd] = '\0';
		printf("%s", buf);
	}

	closesocket(c_socket);
	clearwinsock();
	printf("\n");
	system("pause");
	return (0);
} // main end
