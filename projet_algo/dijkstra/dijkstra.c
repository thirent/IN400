#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nbr_station 384
#define t_max 100

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
				strcpy(nom1,&buf[1+1+4+1]);
			}
			if(num == n2)
			{
				strcpy(nom2,&buf[1+1+4+1]);
			}
		}
	}
	fclose(metro);
	
	if(strcmp(nom1,nom2) == 0)return 1;
	else return 0;
}

int t_marque(int* marque){
	
	int i, res=0;
	for(i=0;i<nbr_station;i++) res+= marque[i];
	
	if (res==nbr_station) return 1;
	else return 0;
}

void maj_distance(int* pere, unsigned int* distance, int sommet, int** matrice){
	
	int i;
	
	for(i=0;i<nbr_station;i++){	
		if(matrice[sommet][i]!=0){
			if(distance[i]>(distance[sommet]+matrice[sommet][i])){
				distance[i]=distance[sommet]+matrice[sommet][i];
				pere[i]=sommet;
			}
		}
	}	
}

int minimum(unsigned int* distance, int* marque){
	
	int min = 2147483647; 
	int i, res = 0;
	
	for(i=0;i<nbr_station;i++){
		if((marque[i]==0)&&(distance[i]<min)){
			min = distance[i];
			res = i;
		}
	}
	marque[res]=1;
	return res;
}

char* dijkstra(int** matrice, int depart, int arrivee,int* temps)
{	
	unsigned int distance[nbr_station];
	int pere[nbr_station];
	int marque[nbr_station];
	
	int i,j,tmp,tmp2;
	
	for(i=0;i<nbr_station;i++){
		distance[i]=4294967295;
		pere[i]=0;
		marque[i]=0;
	}
		
	distance[depart]=0;
	pere[depart]=-1;
		
	maj_distance(pere,distance,depart,matrice);
		
	while(!t_marque(marque)){
		maj_distance(pere,distance,minimum(distance,marque),matrice);
	}
	
	tmp = arrivee;
	tmp2 = arrivee;
	i = arrivee - 1;
	while((i>=0)&&(meme_gare(i,arrivee)))
	{
		if(distance[arrivee]>distance[i]) tmp = i;
		i--;
	}
	i = arrivee + 1;
	while((i<nbr_station)&&(meme_gare(i,arrivee)))
	{
		if(distance[arrivee]>distance[i]) tmp2 = i;
		i++;
	}
	arrivee = (distance[tmp]<distance[tmp2])?tmp:tmp2; 
	
	int chemin_tmp[nbr_station];
	
	chemin_tmp[0]=arrivee;
	tmp=arrivee;
	i=1;
	
	while(pere[tmp]!=-1){
		tmp=pere[tmp];
		chemin_tmp[i]=tmp;
		i++;
	}
	
	int chemin_tmp2[i];
		
	char* chemin = malloc((i*5+1)*sizeof(char));
	
	tmp=i-1;
	
	while(tmp>=0){
		chemin_tmp2[i-tmp-1]=chemin_tmp[tmp];
		tmp--;
	}
	
	for(tmp=0;tmp<i;tmp++)
	{
		sprintf(&chemin[tmp*5],"%04d ",chemin_tmp2[tmp]);
	}
	
	chemin[tmp*5] = '\0';
	
	//printf("%s\n",chemin);
	
	*temps = distance[arrivee];
	
	return chemin;
}

char* pcc(int depart, int arrivee,int* temps)
{
	int temps_tmp,tmp,i;
	char* chemin;
	char* chemin_tmp;
	
	int** matrice = creation_graphe();
	
	chemin = dijkstra(matrice,depart,arrivee,temps);
	
	i = depart - 1;
	while((i>=0)&&(meme_gare(i,depart)))
	{
		chemin_tmp = dijkstra(matrice,i,arrivee,&temps_tmp);
		if(temps_tmp < (*temps))
		{
			//printf("-%d:%d\n",i,temps_tmp);
			free(chemin);
			chemin = chemin_tmp;
			*(temps)=temps_tmp;
		}
		i--;
	}
	
	i = depart + 1;
	while((i<nbr_station)&&(meme_gare(i,depart)))
	{
		chemin_tmp = dijkstra(matrice,i,arrivee,&temps_tmp);
		if(temps_tmp < (*temps))
		{
			//printf("+%d:%d\n",i,temps_tmp);
			free(chemin);
			chemin = chemin_tmp;
			*(temps)=temps_tmp;
		}
		i++;
	}
		
	return chemin;
}
