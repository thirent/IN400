#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL/SDL_ttf.h>

#define t_max 1000

void demande_coordonnees()
{
	FILE* coordonnees = fopen("coordonnees.txt","w+");
	FILE* metro = fopen("metro.txt","r");
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* image = SDL_LoadBMP("plan_metro_paris_2017.bmp");
	SDL_Surface* ecran = SDL_SetVideoMode(image->w,image->h,32,SDL_HWSURFACE);
	SDL_Rect position;position.x=0;position.y=0;
	SDL_BlitSurface(image,NULL,ecran,&position);
	SDL_Flip(ecran);
	SDL_Event event;
	
	char buf1[t_max],buf2[t_max];
	char c,num[5];
	int continuer = 1;
	
	buf2[8] = '\0';
	
	while(fgets(buf1,t_max,metro) != NULL)
	{
		sscanf(buf1,"%c %s",&c,num);
		if(c == 'V')
		{
			if(!strcmp(&buf1[7],&buf2[7]))
			{
				continuer = 0;
				fprintf(coordonnees,"%s %d %d\n",num,event.button.x,event.button.y);
			}
			else printf("%s\n",&buf1[7]);
			while(continuer)
			{
				SDL_WaitEvent(&event);
				if(event.type == SDL_MOUSEBUTTONUP)
				{
					continuer = 0;
					fprintf(coordonnees,"%s %d %d\n",num,event.button.x,event.button.y);
				}
				if(event.type == SDL_QUIT)goto fin;
			}
			continuer = 1;
			strcpy(buf2,buf1);
		}
	}
	
	fin:
	
	SDL_FreeSurface(image);
	SDL_FreeSurface(ecran);
	SDL_Quit();
	
	fclose(coordonnees);
	fclose(metro);
}





int main()
{
	demande_coordonnees();
	
	return 0;
}
