#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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

int nombre_ligne(char* path)
{
	int fd = open(path,O_RDONLY);
	
	int res=0;char c;
	
	while(read(fd,&c,sizeof(char)) != 0)
	{
		if(c == '\n') res++;
	}
	close(fd);
	return res;
}

instruction* recup_inst(char* nom_fic,int nbr_ligne)
{
	int fd = open(nom_fic,O_RDONLY);
	int i=0;
	char c;
	
	int taille_max,taille,pos,signe;
	
	instruction* inst = malloc((nbr_ligne)*sizeof(instruction));
	
	for(i=0;i<nbr_ligne;i++)
	{
		inst[i].path = malloc(20*sizeof(char));
		inst[i].decalage = 0;
		taille_max=20;
		taille = 0;
		pos = 0;
		signe = 1;
		
		while((read(fd,&c,sizeof(char)) != 0) && (c != ';'))
		{
			inst[i].path[pos] = c;
			pos++;
			taille++;
			if(taille == taille_max-1)
			{
				taille_max *= 2;
				inst[i].path = realloc(inst[i].path,taille_max*sizeof(char));
			}
		}
		inst[i].path[pos] = '\0';
				
		while((read(fd,&c,sizeof(char)) != 0) && (c != ';'))
		{
			if(c == '-') signe = -1;
			else inst[i].decalage = (inst[i].decalage*10) + (c - '0');
		}
		inst[i].decalage *= signe;
		
		read(fd,&(inst[i].sens),sizeof(char));
		
		while((read(fd,&c,sizeof(char)) != 0) && (c != '\n'));
	}
	
	close(fd);
	
	return inst;
}

void* decalage_mot(void* argument)
{
	arg* a = (arg*)argument;
	int i,c,d;
	
	for(i=a->deb;i <= a->fin;i++)
	{
		c = a->chaine[i];
		d = a->decalage;
		
		a->chaine[i] = ((c>='A')&&(c<='Z'))?('Z'+(c-'A'-d))%('Z'+1):
		(((c>='a')&&(c<='z'))?('z'+(c-'a'-d))%('z'+1):a->chaine[i]);
	}
	
	return NULL;
}

void insertion_debut(thread_liste* liste)
{
	thread_elem* d = malloc(sizeof(thread_elem));
	d->addr = (*liste)->addr;
	*liste = d;
}

void suppression_debut(thread_liste* liste)
{
	thread_elem* d = *liste;
	*liste = (*liste)->addr;
	free(d);
}

int main(int argc, char** argv)
{
	if(argc == 1)exit(0);
	
	int nbr_ligne = nombre_ligne(argv[1]),i,j,reste;
	pid_t pid[nbr_ligne];
	instruction* inst = recup_inst(argv[1],nbr_ligne);
	int pipes[nbr_ligne][2];
	
	char buf[tmax];
		
	for(i=0;i<nbr_ligne;i++)
	{
		pipe(pipes[i]);
		
		//printf("%s %d %c\n",inst[i].path,inst[i].decalage,inst[i].sens);
		
		pid[i] = fork();
		if(pid[i] == 0) goto fin_boucle;
	}	
	fin_boucle:
	
	
	
	if(i == nbr_ligne) //parent
	{
		for(i=0;i<nbr_ligne;i++)
		{
			close(pipes[i][1]);
		}
		for(i=0;i<nbr_ligne;i++)
		{
			wait(&i);
			
			while((reste = read(pipes[i][0],buf,tmax)) != 0) write(STDOUT_FILENO,buf,reste);
		}
	}
	
	else //fils
	{
		for(j=0;j<i;j++)
		{
			close(pipes[j][1]);
			close(pipes[j][0]);
		}
		close(pipes[i][0]);
		
		int fd = open(inst[i].path,O_RDONLY),taille_chaine = tmax+1, pos = 0, j, k = 0/*, nbr_mot_max = tmax, nbr_mot = 0*/;
		
		char* chaine = malloc(taille_chaine*sizeof(char));
		
		//while((reste = read(pipes[i][0],&buf[pos],tmax)) != 0)
		while((reste = read(fd,&buf[pos],tmax)) != 0)
		{
			pos += reste;
			if(pos >= taille_chaine -1)
			{
				taille_chaine *= 2;
				chaine = realloc(chaine,taille_chaine*sizeof(char));
			}
		}
		
		thread_liste liste = NULL;
		
		for(j=0;j<pos;j++)
		{
			if(!(((chaine[i] >= 65)&&(chaine[i] <=90)) || ((chaine[i] >= 97)&&(chaine[i] <=122))))
			{
				insertion_debut(&liste);
				
				(liste->argument).deb = k;
				(liste->argument).fin = j;
				(liste->argument).chaine = chaine;
				(liste->argument).decalage = inst[i].decalage;
				
				//appel des thread avec l'argument : &args[i]
				
				pthread_create(&(liste->tid),NULL,decalage_mot,&(liste->argument));
				
				//appel des thread avec l'argument : &args[i]
				
				k=j+1;
			}
			
		}
		
		while(liste != NULL)
		{
			pthread_join(liste->tid,NULL);
			suppression_debut(&liste);
		}
		
		
		
		
		
		
		close(fd);
		
		exit(i);
	}
	
	return EXIT_SUCCESS;
}
//http://mtodorovic.developpez.com/linux/programmation-avancee/?page=page_5



