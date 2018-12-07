#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for atoi()
#include <netdb.h>

#include "Libreria.h"
#include "operazioni.h"

#define PORT 48000
#define ECHOMAX 255


int main() {

    int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
    int recvMsgSize;

    char *item;
    int k = 0;
    char *store[3];

    char operazione[10];
    int op1;
    int op2;
    int risultato;
    char str[20];
    char risposta[ECHOMAX];
    int rispLen;
    int tempint;

    //DNS
    struct hostent *host; 
    struct in_addr addr;

    sock=creasocket();
	//CREAZIONE DELLA SOCKET

	/*if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		errorhandler("Creazione socket fallita\n");
	}
	else{
		printf("connessione riuscita\n");
	}
*/
	//COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&echoServAddr, 0, sizeof(echoServAddr)); // ensures that extra bytes contain 0
	//memcpy(&echoClntAddr.sin_addr, host->h_addr_list[0], host->h_length);
    memset(&echoBuffer, 0, sizeof(echoBuffer));
    
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	echoServAddr.sin_port = htons(PORT);
	
	//BIND DELLA SOCKET
	if ((bind(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr))) < 0) 
		errorhandler("bind() failed.\n");     
    
    //DNS
  
    const char* iphost = "127.0.0.1"; 
    addr.s_addr = inet_addr(iphost); 
    host = gethostbyaddr((char *) &addr, 4, AF_INET);
    char* canonical_name = host->h_name;
  
    //RICEZIONE DELLA STRINGA ECHO DAL CLIENT
    while(1)	{
		cliAddrLen = sizeof(echoClntAddr);
		if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
           errorhandler("recvfrom() failed");
		
        item = strtok(echoBuffer, ":");

	    while(item != NULL)	{
	    	store[k] = item;
	    	k++;
	    	item = strtok(NULL, ":");
    	}

        strcpy(operazione, store[0]);
        op1= atoi(store[1]);
        op2= atoi(store[2]);
        
        operazione[0] = toupper(operazione[0]);
		
   
        printf("Richiesta operazione ' %s %d %d ' dal client %s, ip %s\n", operazione, op1, op2, canonical_name, inet_ntoa(echoClntAddr.sin_addr));
		
        switch (operazione[0]){
			case 'A': risultato = add(op1, op2); 

                sprintf(str, "%d", op1);  
                strcat(risposta, str);
                strcat(risposta, " + ");
                sprintf(str, "%d", op2);  
                strcat(risposta, str);
                strcat(risposta, " = ");
                sprintf(str, "%d", risultato);  
                strcat(risposta, str);
                  
                rispLen=strlen(risposta); 
                  
                tempint = sendto(sock, risposta, rispLen, 0, (struct sockaddr *)&echoClntAddr, cliAddrLen);
                if (tempint == -1 ) {
                      printf("Sent response size: %d\n", tempint);
                      errorhandler("sendto() sent a different number of bytes than expected\n");
                  }else{
                      //printf("%s\n", risposta);
                      memset(&risposta, 0, sizeof(risposta));
                  }
                
              break;
                
			case 'S': risultato = sub(op1, op2);

                sprintf(str, "%d", op1);  
                strcat(risposta, str);
                strcat(risposta, " - ");
                sprintf(str, "%d", op2);  
                strcat(risposta, str);
                strcat(risposta, " = ");
                sprintf(str, "%d", risultato);  
                strcat(risposta, str);
                  
                rispLen=strlen(risposta); 
                  
                tempint = sendto(sock, risposta, rispLen, 0, (struct sockaddr *)&echoClntAddr, cliAddrLen);
                if (tempint == -1 ) {
                      printf("Sent response size: %d\n", tempint);
                      errorhandler("sendto() sent a different number of bytes than expected\n");
                  }else{
                      //printf("%s\n", risposta);
                      memset(&risposta, 0, sizeof(risposta));
                  }
              break;
			
            case 'M': risultato = mult(op1, op2); 

                sprintf(str, "%d", op1);  
                strcat(risposta, str);
                strcat(risposta, " * ");
                sprintf(str, "%d", op2);  
                strcat(risposta, str);
                strcat(risposta, " = ");
                sprintf(str, "%d", risultato);  
                strcat(risposta, str);
                  
                rispLen=strlen(risposta); 
                  
                tempint = sendto(sock, risposta, rispLen, 0, (struct sockaddr *)&echoClntAddr, cliAddrLen);
                if (tempint == -1 ) {
                      printf("Sent response size: %d\n", tempint);
                      errorhandler("sendto() sent a different number of bytes than expected\n");
                  }else{
                      //printf("%s\n", risposta);
                      memset(&risposta, 0, sizeof(risposta));
                  }
              break;
			
            case 'D': risultato = divis(op1, op2);

                sprintf(str, "%d", op1);  
                strcat(risposta, str);
                strcat(risposta, " / ");
                sprintf(str, "%d", op2);  
                strcat(risposta, str);
                strcat(risposta, " = ");
                sprintf(str, "%d", risultato);  
                strcat(risposta, str);
                  
                rispLen=strlen(risposta); 
                  
                tempint = sendto(sock, risposta, rispLen, 0, (struct sockaddr *)&echoClntAddr, cliAddrLen);
                if (tempint == -1 ) {
                      printf("Sent response size: %d\n", tempint);
                      errorhandler("sendto() sent a different number of bytes than expected\n");
                  }else{
                      //printf("%s\n", risposta);
                      memset(&risposta, 0, sizeof(risposta));
                  }
              break;
		}
        
        
    }  

    //CHIUSURA DELLA SOCKET
    //    close(sock);
        return 0;
  } 