#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define nbr_station 376
#define t_max 200

/*FILE* copie_fichier_metro()
{
	char buf[1024];
	int reste;
	
	FILE* metro = fopen("metro.txt","r");
	FILE* metro_c = fopen("metro_copie.txt","w+");
	
	while((reste = fread(buf,1,1024,metro)) != 0)
		fwrite(buf,1,reste,metro_c);
	
	fclose(metro);
	
	return metro_c;
}*/

int main()
{
	
	FILE* ligne = fopen("resultat_ligne.txt","w+");
	FILE* metro = fopen("metro.txt","r");
	//FILE* metro_c = copie_fichier_metro();
	
	int i,j,k,l;
	int station[nbr_station];
	int nbr_terminus = nbr_station;
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
				//printf("%d %d\n",j,k);
				if(((i == j)||(i == k))&&((l<180)||(l==287)))station[i]++;
			}
		}
		if(station[i] > 1)nbr_terminus--;
		fseek(metro,0,SEEK_SET);
	}
	
	int terminus[nbr_terminus];
	
	j = 0;
	
	for(i=0;i<nbr_station;i++)
	{
		if(station[i] == 1)
		{
			terminus[j] = station[i];
			j++;
			printf("%d\n",i);
		}
	}
	
	
	
	
	
	fclose(metro);
	fclose(ligne);
	
	//system("rm metro_copie.txt");
	
	exit(0);
}
