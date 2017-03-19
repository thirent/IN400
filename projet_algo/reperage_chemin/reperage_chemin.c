#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define nbr_station 376
#define t_max 1000

void reperage_gare(int gare)
{
	
}

void reperage_ligne(char* portion)
{
	
}

char* n_chaine(char* chemin,int longueur,int decalage)
{
	char* n_chemin = malloc((5*longueur+1)*sizeof(char));
	int i;
	
	for(i=decalage*5;i<decalage*5+(longueur*5);i++)n_chemin[i-(decalage*5)] = chemin[i];
	
	return n_chemin;
}

void n_chaine_contraire(char* chemin,int longueur,int decalage)
{
	strcpy(&chemin[decalage],&chemin[decalage+longueur]);
}

void reperage_chemin(char* chemin,int longueur,char** ligne)
{
	FILE* fic_ligne = fopen("ligne_man.txt","r");
	FILE* metro = fopen("metro.txt","r");
	
	int i,j,k,chemin_len = strlen(chemin)/5,ligne_len,nbr_ligne = 0;
	char buf[t_max];
	
	while(fgets(buf,t_max,fic_ligne) != NULL) nbr_ligne++;
	fseek(fic_ligne,0,SEEK_SET);
	
	for(i=0;i<nbr_ligne;i+=2)
	{
		ligne_len = strlen(ligne[i])/5;
		for(j=0;j<chemin_len+1-longueur;j++)
		{
			for(k=0;k<ligne_len+1-longueur;k++)
			{
				printf("%s\n",n_chaine(chemin,longueur,j));
				printf("%s\n",n_chaine(ligne[i],longueur,k));
				if(strcmp(n_chaine(chemin,longueur,j),n_chaine(ligne[i],longueur,k)) == 0)
				{
					reperage_ligne(n_chaine(chemin,longueur,j));
					n_chaine_contraire(chemin,longueur,j);
					
					if(strlen(chemin)/5 != 0)reperage_chemin(chemin,strlen(chemin)/5,ligne);
					
					k=ligne_len+1-longueur;
					j=chemin_len+1-longueur;
					i=nbr_ligne+5;
				}
			}
		}
	}
	
	if(i < nbr_ligne+5)reperage_chemin(chemin,longueur-1,ligne);
	
	fclose(fic_ligne);
	fclose(metro);
}

char** recup_ligne()
{
	FILE* fic_ligne = fopen("ligne_man.txt","r");
	
	int nbr_ligne = 0,i;
	char buf[t_max];
	
	while(fgets(buf,t_max,fic_ligne) != NULL) nbr_ligne++;
	
	char** ligne = malloc(nbr_ligne*sizeof(char*));
	for(i=0;i<nbr_ligne;i++)
	{
		ligne[i] = malloc(t_max*sizeof(char));
	}
	
	fseek(fic_ligne,0,SEEK_SET);
	
	for(i=0;i<nbr_ligne;i++)
	{
		fgets(ligne[i],t_max,fic_ligne);
	}
	
	fclose(fic_ligne);
	
	return ligne;
}

int main()
{
	reperage_chemin("0134 0270 0226 0160 0143 0340 0125 ",35,recup_ligne());
	
	exit(0);
}
