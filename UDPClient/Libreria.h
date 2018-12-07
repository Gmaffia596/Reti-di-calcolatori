#ifndef LIBRERIA_H
#define LIBRERIA_H

typedef struct server	{
	char host[50];
	int ip;
}indirizzo;

void errorhandler(char *error_message);

int creasocket();

#endif