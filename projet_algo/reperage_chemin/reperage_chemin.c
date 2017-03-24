#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define nbr_station 376
#define t_max 1000

void reperage_gare_2(int gare)
{
	FILE* metro = fopen("metro.txt","r");
	
	char buf[t_max];
	char c;
	int n;
	
	while(fgets(buf,t_max,metro) != NULL)
	{
		sscanf(buf,"%c %d",&c,&n);
		if((c == 'V')&&(gare == n))
		{
			buf[strlen(buf)-1] = '\0';
			printf("%s",&buf[7]);
		}
	}
	
	fclose(metro);
}

void reperage_gare(char* gare)
{
	FILE* metro = fopen("metro.txt","r");
	
	gare[4]='\0';
	
	char buf[t_max];
	char c,num[5];
	
	while(fgets(buf,t_max,metro) != NULL)
	{
		sscanf(buf,"%c %s",&c,num);
		if((c == 'V')&&(strcmp(num,gare) == 0))
		{
			buf[strlen(buf)-1] = '\0';
			printf("%s",&buf[7]);
		}
	}
	
	fclose(metro);
}

void reperage_ligne(char* portion,int ligne)
{
	FILE* fic_ligne = fopen("ligne_man.txt","r");
	
	char gare1[6],gare2[6];
	char buf[t_max];
	int i=0;
	
	strcpy(gare2,&portion[strlen(portion)/5*5-5]);
	portion[5] = '\0';
	strcpy(gare1,portion);
	
	printf("Aller de : ");
	reperage_gare(gare1);
	printf(" a : ");
	reperage_gare(gare2);
	printf(" sur la ");
	
	while(i != ligne)
	{
		fgets(buf,t_max,fic_ligne);
		i++;
	}
	fgets(buf,t_max,fic_ligne);
	fgets(buf,t_max,fic_ligne);
	
	printf("%s\n",buf);
	
	fclose(fic_ligne);
}

char* n_chaine(char* chemin,int longueur,int decalage)
{
	char* n_chemin = malloc((5*longueur+1)*sizeof(char));
	int i;
	
	for(i=decalage*5;i<decalage*5+(longueur*5);i++)n_chemin[i-(decalage*5)] = chemin[i];
	
	return n_chemin;
}

void reperage_chemin(char* chemin,int longueur,char** ligne)
{	
	FILE* fic_ligne = fopen("ligne_man.txt","r");
	
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
				//~ printf("%s\n",n_chaine(chemin,longueur,j));
				//~ printf("%s\n",n_chaine(ligne[i],longueur,k));
				if(strcmp(n_chaine(chemin,longueur,j),n_chaine(ligne[i],longueur,k)) == 0)
				{
					//~ printf("%s\n",n_chaine(chemin,longueur,j));
					//~ printf("%s\n",n_chaine(ligne[i],longueur,k));
					
					strcpy(buf,n_chaine(chemin,longueur,j));
					
					chemin[j*5] = '\0';
					
					if(strlen(chemin)/5 != 0)reperage_chemin(chemin,strlen(chemin)/5,ligne);
					
					reperage_ligne(buf,i);
					
					//if(strlen(&chemin[j*5+longueur*5]) !=5)reperage_chemin(&chemin[j*5+longueur*5],strlen(&chemin[j+longueur])/5,ligne);
					if(strlen(&chemin[j*5+longueur*5])/5 !=0)reperage_chemin(&chemin[j*5+longueur*5],strlen(&chemin[j+longueur])/5,ligne);
					
					k=ligne_len+1-longueur;
					j=chemin_len+1-longueur;
					i=nbr_ligne+5;
				}
			}
		}
	}
	
	if((i < nbr_ligne+5)&&(strlen(chemin)/5 != 0))reperage_chemin(chemin,longueur-1,ligne);
	
	fclose(fic_ligne);
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
/*
int main()
{
	//"0134 0270 0226 0160 "
	
	char* test = malloc(21*sizeof(char));
	//~ test[0] = '0';
	//~ test[1] = '1';
	//~ test[2] = '3';
	//~ test[3] = '4';
	//~ test[4] = ' ';
	//~ test[5] = '0';
	//~ test[6] = '2';
	//~ test[7] = '7';
	//~ test[8] = '0';
	//~ test[9] = ' ';
	//~ test[10] = '0';
	//~ test[11] = '2';
	//~ test[12] = '2';
	//~ test[13] = '6';
	//~ test[14] = ' ';
	//~ test[15] = '0';
	//~ test[16] = '1';
	//~ test[17] = '6';
	//~ test[18] = '0';
	//~ test[19] = ' ';
	//~ test[20] = '\0';
	
	test[0] = '0';
	test[1] = '3';
	test[2] = '2';
	test[3] = '2';
	test[4] = ' ';
	test[5] = '0';
	test[6] = '2';
	test[7] = '3';
	test[8] = '8';
	test[9] = ' ';
	test[10] = '0';
	test[11] = '2';
	test[12] = '3';
	test[13] = '9';
	test[14] = ' ';
	test[15] = '0';
	test[16] = '0';
	test[17] = '2';
	test[18] = '7';
	test[19] = ' ';
	test[20] = '\0';
	
	reperage_chemin(test,strlen(test)/5,recup_ligne());
	
	return 0;
}
*/
