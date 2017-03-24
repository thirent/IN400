#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define t_max 1000
#define rayon 15

int dans_cercle(int x, int y, int centre_x, int centre_y)
{
	int pos_x = centre_x - x, pos_y = centre_y - y;
	pos_x *= pos_x; pos_y *= pos_y;
	
	if((pos_x + pos_y < rayon*rayon)&&(pos_x + pos_y > 0)) return 1;
	return 0;
}

int reconnaissance_gare(int x,int y)
{
	FILE* coordonnees = fopen("coordonnees.txt","r");
	
	int gare,xi,yi;
	char buf[t_max];
	
	while(fgets(buf,t_max,coordonnees) != NULL)
	{
		sscanf(buf,"%d %d %d",&gare,&xi,&yi);
		if(dans_cercle(x,y,xi,yi))
		{
			fclose(coordonnees);
			return gare;
		}
	}
	
	fclose(coordonnees);
	return -1;
}

