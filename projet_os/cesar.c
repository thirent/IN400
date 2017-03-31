#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

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
	
	
	
}arg;

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
	
	instruction* inst = malloc((nbr_ligne + 1)*sizeof(instruction));
	
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
	
	inst[i].path = malloc(sizeof(char));
	inst[i].path[0] = ';';
	
	close(fd);
	
	return inst;
}

int main(int argc, char** argv)
{
	if(argc == 1)exit(0);
	
	int nbr_ligne = nombre_ligne(argv[1]),i,reste;
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
			waitpid(pid[i],NULL,0);
			
			while((reste = read(pipes[i][0],buf,tmax)) != 0) write(STDOUT_FILENO,buf,reste);
		}
	}
	
	else //fils
	{
		close(pipes[i][0]);
		
		int fd = open(inst[i].path,O_RDONLY),taille_chaine = tmax+1, pos = 0, j, k=0;
		
		char* chaine = malloc(taille_chaine*sizeof(char)),;
		
		while((reste = read(pipes[i][0],&buf[pos],tmax)) != 0)
		{
			pos += reste;
			if(pos >= taille_chaine -1)
			{
				taille _chaine *= 2;
				chaine = realloc(chaine,taille_chaine*sizeof(char));
			}
		}
		
		
		for(j=0;j<reste;j++)
			{
				
			}
			pos += (reste-1);
		
		
		close(fd);
	}
	
	return EXIT_SUCCESS;
}

//write(STDOUT_FILENO, msg, strlen(msg));

//http://mtodorovic.developpez.com/linux/programmation-avancee/?page=page_5



