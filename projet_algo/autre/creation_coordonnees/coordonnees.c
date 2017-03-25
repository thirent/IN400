#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL/SDL_ttf.h>

#define t_max 1000

void demande_coordonnees()
{
	FILE* coordonnees;
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
	
	if((coordonnees = fopen("coordonnees.txt","r")) == NULL)
	{
		coordonnees = fopen("coordonnees.txt","w+");
	}
	else
	{
		fclose(coordonnees);
		coordonnees = fopen("coordonnees.txt","a+");
		
		fseek(coordonnees,0,SEEK_SET);
		
		while(fgets(buf1,t_max,coordonnees) != NULL)
		{
			fgets(buf1,t_max,metro);printf("%s\n",buf1);
			sscanf(buf1,"%c",&c);
			while(c != 'V')
			{
				fgets(buf1,t_max,metro);printf("%s\n",buf1);
				sscanf(buf1,"%c",&c);
			}
		}
	}
	
	while(fgets(buf1,t_max,metro) != NULL)
	{
		sscanf(buf1,"%c %s",&c,num);
		if(c == 'V')
		{
			if(!strcmp(&buf1[7],&buf2[7]))
			{
				continuer = 0;
				fprintf(coordonnees,"%s %d %d\n",num,(int)(event.button.x*3.0669800235),(int)(event.button.y*3.0327102));
			}
			else printf("%s\n",&buf1[7]);
			while(continuer)
			{
				SDL_WaitEvent(&event);
				if(event.type == SDL_MOUSEBUTTONUP)
				{
					continuer = 0;
					fprintf(coordonnees,"%s %d %d\n",num,(int)(event.button.x*3.0669800235),(int)(event.button.y*3.0327102));
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

void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;
	
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

Uint32 obtenirPixel(SDL_Surface *surface, int x, int y)
{
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    switch(nbOctetsParPixel)
    {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0; 
    }
}



SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
    if(!Surface || !Width || !Height)
        return 0;
     
    SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
        Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

	double _stretch_factor_x = (double)Width  / (double)Surface->w;
    double _stretch_factor_y = (double)Height / (double)Surface->h;
	
	int y,x,o_y,o_x;
	
    for(y = 0; y < Surface->h; y++)
        for(x = 0; x < Surface->w; x++)
            for(o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for(o_x = 0; o_x < _stretch_factor_x; ++o_x)
					definirPixel(_ret,((Sint32)_stretch_factor_x * x) + o_x, ((Sint32)_stretch_factor_y * y) + o_y,obtenirPixel(Surface,x,y));
                    //DrawPixel(_ret, ((Sint32)_stretch_factor_x * x) + o_x, ((Sint32)_stretch_factor_y * y) + o_y, ReadPixel(Surface, x, y));
 
    return _ret;
}

void pause() //utile pour le test
{
	SDL_Event event;
	int continuer = 1;
	while(continuer)
	{
		SDL_WaitEvent(&event);
		if(event.type == SDL_MOUSEBUTTONUP)
		{
			continuer = 0;
		}
	}
}

void test() //juste un test
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* image = SDL_LoadBMP("plan_metro_paris_2017.bmp");
	SDL_Surface* image_z = ScaleSurface(image,2500,2500);
	SDL_Surface* ecran = SDL_SetVideoMode(image->w,image->h,32,SDL_HWSURFACE);
	SDL_Rect position;position.x=0;position.y=0;
	//SDL_BlitSurface(image,NULL,ecran,&position);
	SDL_BlitSurface(image_z,NULL,ecran,&position);
	SDL_Flip(ecran);
	
	pause();
	
	SDL_FreeSurface(image);
	SDL_FreeSurface(ecran);
	SDL_Quit();
}


int main()
{
	demande_coordonnees();
	
	//test();
	
	return 0;
}

