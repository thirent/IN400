#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define tmax 1024
#define tmot 30

typedef struct
{
	char sens;
	int decalage;
	char* path;
}instruction;

typedef struct
{
	char* chaine;
	int deb, fin, decalage;
}arg;

struct thread_elem
{
	arg argument;
	pthread_t tid;
	struct thread_elem* addr;
};

typedef struct thread_elem thread_elem;
typedef thread_elem* thread_liste;


int nombre_ligne(char* path);
instruction* recup_inst(char* nom_fic,int nbr_ligne);
void* decalage_mot(void* argument);
void insertion_debut(thread_liste* liste);
void suppression_debut(thread_liste* liste);


