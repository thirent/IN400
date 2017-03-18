#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define nbr_station 376
#define t_max 200

int nbr_terminus = nbr_station;

int meme_gare(int n1,int n2)
{
	FILE* metro = fopen("metro.txt","r");
	
	char buf[t_max];
	char nom[t_max],nom1[t_max],nom2[t_max];
	char tmp;
	int num;
	
	while(fgets(buf,t_max,metro) != NULL)
	{
		if(buf[0] == 'V')
		{
			sscanf(buf,"%c %d %s",&tmp,&num,nom);
			
			if(num == n1)
			{
				//strcpy(nom1,nom);
				strcpy(nom1,&buf[1+1+4+1]);
			}
			if(num == n2)
			{
				//strcpy(nom2,nom);
				strcpy(nom2,&buf[1+1+4+1]);
			}
		}
	}
	
	fclose(metro);
	
	if(strcmp(nom1,nom2) == 0)return 1;
	else return 0;
}

int* terminus()
{
	FILE* metro = fopen("metro.txt","r");
	
	int i,j,k,l;
	int station[nbr_station];
	char buf[t_max];
	char c;
	
	for(i=0;i<nbr_station;i++)station[i] = 0;
	
	for(i=0;i<nbr_station;i++)
	{
		while(fgets(buf,t_max,metro) != NULL)
		{
			if(buf[0] == 'E')
			{
				sscanf(buf,"%c %d %d %d",&c,&j,&k,&l);
				if(((i == j)||(i == k))&&(!meme_gare(j,k)))station[i]++;
			}
		}
		if(station[i] > 1)nbr_terminus--;
		fseek(metro,0,SEEK_SET);
	}
	
	int* terminus = malloc(nbr_terminus*sizeof(int));
	
	for(j=0,i=0;i<nbr_station;i++)
	{
		if(station[i] == 1)
		{
			terminus[j] = station[i];
			j++;
			printf("%d\n",i);
		}
	}
	
	fclose(metro);
	
	return terminus;
}

int prochain_arret(int* traite,int arret)
{
	FILE* metro = fopen("metro.txt","r");
	
	int p_arret;
	
	traite[arret] = 1;
	
	
	
	char buf[t_max];
	char nom[t_max],nom1[t_max],nom2[t_max];
	char c;
	int j,k,l;
	
	while(fgets(buf,t_max,metro) != NULL)
	{
		if(buf[0] == 'E')
		{
			sscanf(buf,"%c %d %d %d",&c,&j,&k,&l);
			
			
		}
	}
	
	fclose(metro);
}

void creation_ligne(int* terminus)
{
	FILE* ligne = fopen("resultat_ligne.txt","w+");
	
	int traite[nbr_station];
	int i;
	
	for(i=0;i<nbr_station;i++)traite[i] = 0;
	
	for(i=0;i<nbr_terminus;i++,p_arret=0)
	{
		if(!traite[terminus[i]])
		{
			fprintf(ligne,"%d ",terminus[i]);
			p_arret = prochain_arret(traite,terminus[i]);
			while(p_arret != -1)
			{
				fprintf(ligne,"%d ",p_arret);
			}
		}
	}
	
	fclose(ligne);
}

int main()
{
	terminus();
	
	
	exit(0);
}
