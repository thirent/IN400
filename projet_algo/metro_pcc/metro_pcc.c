#include "metro_pcc.h"

#define nbr_station 376

int main(int argc, char** argv)
{
	if(argv[1][0] == '1')
	{
		lancement1();
	}
	if(argv[1][0] == '2')
	{
		lancement2();
	}
	
	return 0;
}

void lancement1()
{
	int gare1,gare2,temps,heures,minutes,secondes,valide = 0;
	
	
	
	while(!valide)
	{
		printf("\nrentrer le numero de votre gare de depart : ");
		scanf("%d",&gare1);
		if((gare1 >= 0)&&(gare1 < nbr_station))valide = 1;
		else printf("le numero que vous avez rentre n est pas compirs entre 0 et 375\n");
	}
	valide = 0;
	while(!valide)
	{
		printf("\nrentrer le numero de votre gare d'arriver : ");
		scanf("%d",&gare2);
		if((gare2 >= 0)&&(gare2 < nbr_station)&&(gare1 != gare2))valide = 1;
		else printf("le numero que vous avez rentre n est pas compirs entre 0 et 375 ou votre gare d'arrivee et la meme que celle de depart\n");
	}
		
	char* chemin = dijkstra(creation_graphe(),gare1,gare2,&temps);
	
	//printf("%s\n",chemin);
	
	reperage_chemin(chemin,strlen(chemin)/5,recup_ligne());
		
	heures = temps / 3600;
	minutes = (temps - heures * 60) / 60;
	secondes = temps - heures * 3600 - minutes * 60;
	
	printf("le trajet vous prendra %d h %d m %d s\n",heures,minutes,secondes);
	
}

void lancement2()
{
	
	
}
