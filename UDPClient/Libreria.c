#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for atoi()

#include "Libreria.h"

void errorhandler(char *error_message) {
	printf("%s", error_message);
}
int creasocket(){
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		errorhandler("Creazione socket fallita\n");
        close(sock);
    }
	else{
		printf("connessione riuscita\n");
	 return sock;
    }
   
}