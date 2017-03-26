//#include <stdio.h>
//#include <stdlib.h>
//#include <stdarg.h>
//#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL/SDL_ttf.h>
#include <time.h>

#include "../recuperation_clic/recuperation_clic.h"
#include "../reperage_chemin/reperage_chemin.h"
#include "../dijkstra/dijkstra.h"

#define COULEUR 255,0,0,255
#define cote_rect 16
#define ep_ligne 10

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

Uint32 moy_zone(SDL_Surface *Surface,int x,int y,int largeur,int hauteur)
{
	Uint32 pixel,pixel_tmp;
	Uint8 r,g,b,a;
	int m_r=0,m_g=0,m_b=0,m_a=0;
	int i,j;
	
	if(Surface->w - x < largeur)largeur = Surface->w - x;
	if(Surface->h - y < hauteur)hauteur = Surface->h - y;
	
	for(i=0;i<largeur;i++)
	{
		for(j=0;j<largeur;j++)
		{
			pixel_tmp = obtenirPixel(Surface,x+i,y+j);
			SDL_GetRGBA(pixel_tmp,Surface->format,&r,&g,&b,&a);
			m_r += (int)r;
			m_g += (int)g;
			m_b += (int)b;
			m_a += (int)a;
		}
	}
	
	m_r /= (largeur*hauteur);
	m_g /= (largeur*hauteur);
	m_b /= (largeur*hauteur);
	m_a /= (largeur*hauteur);
	
	pixel = SDL_MapRGBA(Surface->format,(Uint8)m_r,(Uint8)m_g,(Uint8)m_b,(Uint8)m_a);
	
	return pixel;
}

SDL_Surface* dezoom_vf(SDL_Surface *Surface,int z_lvl,int recouvrement)
{
	int Width = (Surface->w/(z_lvl-recouvrement))+(Surface->w%(z_lvl-recouvrement) != 0);
	int Height = (Surface->h/(z_lvl-recouvrement))+(Surface->h%(z_lvl-recouvrement) != 0);
	
	SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);
	
	int i,j,largeur,hauteur;
	
	for(i=0;i<Width;i++)
	{
		for(j=0;j<Height;j++)
		{
			definirPixel(_ret,i,j,moy_zone(Surface,i*(z_lvl-recouvrement),j*(z_lvl-recouvrement),z_lvl,z_lvl));
		}
	}
	
	return _ret;
}

void draw_rectangle(SDL_Surface** Surface,int x, int y,int cote,Uint32 pixel)
{
	//Uint32 pixel;
	//pixel = SDL_MapRGBA((*Surface)->format,COULEUR);
	
	int i,j;
	for(i=x-(cote/2);i<=x+(cote/2);i++)
	{
		for(j=y-(cote/2);j<=y+(cote/2);j++)
		{
			definirPixel(*Surface,i,j,pixel);
		}
	}
}

void draw_line(SDL_Surface** Surface,int x1, int y1, int x2, int y2, int epaisseur,Uint32 pixel)
{
	int xmin, xmax;
	int ymin, ymax;
	int i,j;
	float a,b,ii,jj;
	
	if (x1 < x2) {xmin=x1; xmax=x2;} else{xmin=x2; xmax=x1;}
	if (y1 < y2) {ymin=y1; ymax=y2;} else{ymin=y2; ymax=y1;}
	
	if (xmin==xmax) for (j=ymin;j<=ymax;j++) draw_rectangle(Surface,xmin,j,epaisseur,pixel);
	if (ymin==ymax) for (i=xmin;i<=xmax;i++) draw_rectangle(Surface,i,ymin,epaisseur,pixel);
	
	if ((xmax-xmin >= ymax-ymin) && (ymax-ymin>0))
	{
	a = (float)(y1-y2) / ((float)(x1-x2));
	b = y1 - a*x1;
	for (i=xmin;i<=xmax;i++)
		{
		jj = a*i+b;
		j = jj;
		if ((jj-j) > 0.5) j++;
		draw_rectangle(Surface,i,j,epaisseur,pixel);
		}
	}
	
	if ((ymax-ymin > xmax-xmin) && (xmax-xmin>0))
	{
	a = (float)(y1-y2) / ((float)(x1-x2));
	b = y1 - a*x1;
	for (j=ymin;j<=ymax;j++)
		{
		ii = (j-b)/a;
		i = ii;
		if ((ii-i) > 0.5) i++;
		draw_rectangle(Surface,i,j,epaisseur,pixel);
		}
	}
}
void dessine_chemin(SDL_Surface** Surface,char* chemin,Uint32 pixel)
{	
	int i=0,j,x1,y1,x2,y2;
	char gare[5];
	gare[4] = '\0';
	
	for(j=0;j<4;j++)gare[j] = chemin[i+j];
	recup_coord(gare,&x1,&y1);
	draw_rectangle(Surface,x1,y1,cote_rect,pixel);
	i += 5;
	
	while(strlen(&chemin[i])/5 != 0)
	{
		for(j=0;j<4;j++)gare[j] = chemin[i+j];
		
		recup_coord(gare,&x2,&y2);
		
		draw_rectangle(Surface,x2,y2,cote_rect,pixel);		
		draw_line(Surface,x1,y1,x2,y2,ep_ligne,pixel);
		
		x1 = x2; y1 = y2;
		
		i += 5;
	}
}

void dessine_portions(SDL_Surface** Surface,char** portions)
{
	srand(time(NULL));
	Uint32 pixel;
	int r=0,g=0,b=0;
	int i=0;
	
	while(portions[i][0] != ':')
	{
		while((r+g+b < 200)||(r+g+b > 255+80*2))
		{
			r = rand()%256;
			g = rand()%256;
			b = rand()%256;
		}
		pixel = SDL_MapRGB((*Surface)->format,r,g,b);
		dessine_chemin(Surface,portions[i],pixel);
		i++;r=0;g=0;b=0;
	}
}

void deplacement_ecran(int mode, ...)
{
	int* depart;int* arrivee;char** portions;
	
	va_list va;
	va_start(va,mode);
	
	if(mode == 0)
	{
		portions = va_arg(va,char**);
	}
	if(mode == 1)
	{
		depart = va_arg(va,int*);
		arrivee = va_arg(va,int*);
	}
	
	va_end(va);
	
	SDL_Init(SDL_INIT_VIDEO);
	
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int resolution_x = info -> current_w;
	int resolution_y = info -> current_h;
	
	SDL_Surface* image = SDL_LoadBMP("plan-metro-paris-2017.bmp");
	SDL_Surface* ecran;
	
	if(mode == 0)dessine_portions(&image,portions);
	
	int z_lvl_max = 1,recouvrement = 0;
	while((((image->w/(z_lvl_max-recouvrement))+(image->w%(z_lvl_max-recouvrement) != 0)) > resolution_x)||(((image->h/(z_lvl_max-recouvrement))+(image->h%(z_lvl_max-recouvrement) != 0)) > resolution_y)) z_lvl_max++;
	
	SDL_Surface* image_z[z_lvl_max];
	
	int i;
	
	for(i=0;i<z_lvl_max;i++)
	{
		image_z[i] = dezoom_vf(image,i+1,0);
	}
	
	ecran = SDL_SetVideoMode(image_z[z_lvl_max - 1]->w,image_z[z_lvl_max - 1]->h,32,SDL_HWSURFACE);
	
	SDL_Rect position,position_copie;position.x=0;position.y=0;
	
	SDL_BlitSurface(image_z[z_lvl_max - 1],NULL,ecran,&position);
		
	SDL_Flip(ecran);
	
	SDL_Event event;
	int continuer = 1,continuer_2 = 1,zoom = z_lvl_max - 1;
	int x_ini,y_ini;
	
	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_MOUSEBUTTONUP:
				switch(event.button.button)
				{
					case SDL_BUTTON_WHEELUP:
						if(zoom > 0)
						{
							zoom--;
							
							position.x = -((event.button.x - position.x)*(zoom+2)/(zoom+1) - event.button.x);
							position.y = -((event.button.y - position.y)*(zoom+2)/(zoom+1) - event.button.y);
							
							position_copie.x = position.x; position_copie.y = position.y;
							SDL_BlitSurface(image_z[zoom],NULL,ecran,&position);
							position.x = position_copie.x; position.y = position_copie.y;
		
							SDL_Flip(ecran);
						}
						break;
					case SDL_BUTTON_WHEELDOWN:
						if(zoom < z_lvl_max -1)
						{	
							zoom++;
							
							position.x = (-1)*((event.button.x - position.x) * (zoom) / (zoom + 1) - event.button.x);
							position.y = (-1)*((event.button.y - position.y) * (zoom) / (zoom + 1) - event.button.y);
							
							if(position.x > 0)position.x = 0;
							if(position.x < -(image_z[zoom]->w - ecran -> w))position.x = -(image_z[zoom]->w - ecran -> w);
							if(position.y > 0)position.y = 0;
							if(position.y < -(image_z[zoom]->h - ecran -> h))position.y = -(image_z[zoom]->h - ecran -> h);
							
							position_copie.x = position.x; position_copie.y = position.y;
							SDL_BlitSurface(image_z[zoom],NULL,ecran,&position);
							position.x = position_copie.x; position.y = position_copie.y;
		
							SDL_Flip(ecran);
						}
						break;
					case SDL_BUTTON_LEFT:
						if(mode == 1)
						{
							//printf("%d %d\n",(event.button.x + position.x) * (zoom + 1),(event.button.y + position.y) * (zoom + 1));
							
							*depart = reconnaissance_gare((event.button.x - position.x) * (zoom + 1),(event.button.y - position.y) * (zoom + 1));
							if(*depart != -1)
							{
								printf("vous partez de la gare : ");
								reperage_gare_2(*depart);
								printf("\n");
								mode = 2;
							}
							
						}
						else if(mode == 2)
						{
							*arrivee = reconnaissance_gare((event.button.x - position.x) * (zoom + 1),(event.button.y - position.y) * (zoom + 1));
							if((*arrivee != -1)&&(!meme_gare(*depart,*arrivee)))
							{
								printf("pour allez a la gare : ");
								reperage_gare_2(*arrivee);
								printf("\n");
								continuer = 0;
							}
							
						}
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					x_ini = event.button.x; y_ini = event.button.y;
					while(continuer_2)
					{
						SDL_WaitEvent(&event);
						switch(event.type)
						{
							case SDL_MOUSEMOTION:
								if((position.x+event.motion.x-x_ini <= 0)&&(position.x+event.motion.x-x_ini > (-1)*(image_z[zoom]->w - ecran ->w))&&(position.y+event.motion.y-y_ini <= 0)&&(position.y+event.motion.y-y_ini > (-1)*(image_z[zoom]->h - ecran ->h)))
								{
									position.x += (event.motion.x - x_ini); position.y += (event.motion.y - y_ini);
									position_copie.x = position.x; position_copie.y = position.y;
																		
									SDL_BlitSurface(image_z[zoom],NULL,ecran,&position);
									
									position.x = position_copie.x; position.y = position_copie.y;
									x_ini = event.motion.x; y_ini = event.motion.y;
																		
									SDL_Flip(ecran);
									
								}
								break;
							case SDL_MOUSEBUTTONUP:
								if(event.button.button == SDL_BUTTON_RIGHT)
								{
									continuer_2 = 0;
								}
								break;
						}
					}
					continuer_2 = 1;
				}
				break;
			case SDL_QUIT:
				continuer = 0;
				break;
		}
	}
	
	
	for(i=0;i<z_lvl_max;i++) SDL_FreeSurface(image_z[i]);
	SDL_FreeSurface(image);
	SDL_FreeSurface(ecran);
	SDL_Quit();
}

