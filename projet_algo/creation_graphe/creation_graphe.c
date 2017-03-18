#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define nbr_station 376
#define t_max 200

int* creation_graphe()
{
	FILE* metro = fopen("metro.txt","r");
	
	int j,k,l;
	char buf[t_max];
	char c;
	
	int** matrice = calloc(nbr_station*sizeof(int*));
	for(j=0;j<nbr_station;j++)
	{
		matrice[j] = calloc(nbr_station*sizeof(int));
	}
	
	while(fgets(buf,t_max,metro) != NULL)
	{
		if(buf[0] == 'E')
		{
			sscanf(buf,"%c %d %d %d",&c,&j,&k,&l);
			matrice[j][k] = l;
			matrice[k][j] = l;
		}
	}
	
	fclose(metro);
	
	return matrice;
}

