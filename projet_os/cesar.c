//PEPIN Thibaut
//PAXENT Laurent

#include "cesar.h"

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
	
	int taille,signe;
	
	instruction* inst = malloc((nbr_ligne)*sizeof(instruction));
	
	for(i=0;i<nbr_ligne;i++)
	{
		inst[i].decalage = 0;
		taille = 0;
		signe = 1;
		
		while((read(fd,&c,sizeof(char)) != 0) && (c != ';'))taille++;
		
		inst[i].path = malloc((taille+1)*sizeof(char));
		
		lseek(fd,-(taille+1)*sizeof(char),SEEK_CUR);
		
		read(fd,inst[i].path,taille);
		
		lseek(fd,sizeof(char),SEEK_CUR);
		
		
		while((read(fd,&c,sizeof(char)) != 0) && (c != ';'))
		{
			if(c == '-') signe = -1;
			else inst[i].decalage = (inst[i].decalage*10) + (c - '0');
		}
		inst[i].decalage %= ('Z'-'A'+1);
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
		
		a->chaine[i] = ((c>='A')&&(c<='Z'))?
		((c-'A'+d >= 0)?c+d:c-'A'+d+'Z')
		:(((c>='a')&&(c<='z'))?
		((c-'a'+d >= 0)?c+d:c-'a'+d+'z')
		:c);
	}
	
	return NULL;
}

void insertion_debut(thread_liste* liste)
{
	thread_elem* d = malloc(sizeof(thread_elem));
	d->addr = *liste;
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
				
		pid[i] = fork();
		if(pid[i] == 0) goto fin_boucle;
	}
	free(inst);
	fin_boucle:
	
	
	if(i == nbr_ligne) //parent
	{		
		for(i=0;i<nbr_ligne;i++)
		{
			close(pipes[i][1]);
		}
		
		for(j=0;j<nbr_ligne;j++)
		{	
			waitpid(pid[j],NULL,0);
			
			while((reste = read(pipes[j][0],buf,tmax)) != 0)
			{
				write(STDOUT_FILENO,buf,reste);
			}
		}
		for(i=0;i<nbr_ligne;i++)
		{
			close(pipes[0][1]);
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
		
		int fd = open(inst[i].path,O_RDONLY),taille_chaine = tmax+1, pos = 0, j, k = 0;
		
		while((reste = read(fd,buf,tmax)) != 0)taille_chaine += reste;
		
		char* chaine = malloc(taille_chaine*sizeof(char));
		
		lseek(fd,0,SEEK_SET);
		
		while((reste = read(fd,&chaine[pos],tmax)) != 0)pos += reste;
		

		thread_liste liste = NULL;
				
		for(j=0;j<=pos;j++)
		{
			if(!(((chaine[j] >= 65)&&(chaine[j] <=90)) || ((chaine[j] >= 97)&&(chaine[j] <=122))))
			{
				insertion_debut(&liste);
				
				(liste->argument).deb = k;
				(liste->argument).fin = j;
				(liste->argument).chaine = chaine;
				(liste->argument).decalage = inst[i].decalage;
				
				pthread_create(&(liste->tid),NULL,decalage_mot,&(liste->argument));
				
				k=j+1;
			}
			
		}
				
		while(liste != NULL)
		{
			pthread_join(liste->tid,NULL);
			suppression_debut(&liste);
		}
				
		if(inst[i].sens == 'c')
		{
			char n_path[strlen(inst[i].path) + 7];
			strcpy(n_path,inst[i].path);
			strcpy(&n_path[strlen(inst[i].path)],"_cypher");
			int fd2 = open(n_path,O_CREAT|O_WRONLY,0700);
			write(fd2,chaine,strlen(chaine)*sizeof(char));
			close(fd2);
			
			//"le fichier : " = 13
			//" a bien ete crypte.\n" = 20
			// total = 33 caractères
			
			char msg[strlen(inst[i].path)+33];
			strcpy(msg,"le fichier : ");
			strcat(msg,inst[i].path);
			strcat(msg," a bien ete crypte.\n");
			
			write(pipes[i][1],msg,strlen(msg));
		}
		else if(inst[i].sens == 'd')
		{
			write(pipes[i][1],chaine,strlen(chaine)+1);
		}
		
		free(chaine);
		free(inst);
				
		close(fd);
		close(pipes[i][1]);
	}
	
	exit(0);
}



