//PEPIN Thibaut
//PAXENT Laurent

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

/**
 * \struct instruction
 * \brief {contient les information nécessaire au cryptage/décryptage d'un fichier}
 */

typedef struct
{
	char sens;
	int decalage;
	char* path;
}instruction;

/**
 * \struct arg
 * \brief {contient la chaine de caractere a crypter/décrypter ainsi que la partie de cette chaine a modifier par le thread et le décallage a utiliser}
 */

typedef struct
{
	char* chaine;
	int deb, fin, decalage;
}arg;

/**
 * \struct thread_elem
 * \brief {élément d'un liste de structure arg contenant en plus un tid}
 */

struct thread_elem
{
	arg argument;
	pthread_t tid;
	struct thread_elem* addr;
};

typedef struct thread_elem thread_elem;
typedef thread_elem* thread_liste;

/**
 * \fn int nombre_ligne(char* path)
 * \brief {compte le nombre de ligne du fichier spécifié par path}
 * \param path chemin du fichier
 * \return {nombre de ligne du fichier}
 */
 
int nombre_ligne(char* path);

/**
 * \fn instruction* recup_inst(char* nom_fic,int nbr_ligne)
 * \brief {recupére les information concernant le cryptage/décryptage des fichier}
 * \param nom_fic nom du fichier contenant les instruction cryptage/décryptage
 * \param nbr_ligne nombre de ligne du fichier
 * \return {tableau de structure inst rempli a partir du fichier}
 */
 
instruction* recup_inst(char* nom_fic,int nbr_ligne);

/**
 * \fn void* decalage_mot(void* argument)
 * \brief {decale toute les lettre de l'indice debut a l'indice fin dans la chaine spécifié par la structure de type arg}
 * \param argument structure arg contenant la chaine a modifier , les indices de debut et de fin et le décalage a effectuer
 */

void* decalage_mot(void* argument);

/**
 * \fn void insertion_debut(thread_liste* liste)
 * \brief {insère un thread_elem vide au debut d'une liste}
 * \param liste liste ou l'insertion est effectué
 * \return {nouvelle liste contenant l'insertion}
 */

void insertion_debut(thread_liste* liste);

/**
 * \fn void suppression_debut(thread_liste* liste)
 * \brief {supprime un élément thread_elem au debut d'un liste}
 * \param liste liste ou la suppression est effectué
 * \return {nouvelle liste ne contenant plus l'élément supprimé}
 */

void suppression_debut(thread_liste* liste);


