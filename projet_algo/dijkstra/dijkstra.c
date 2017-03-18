#include <stdio.h>

#define nbr_station 376

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

int* dijkstra(int** matrice, int depart, int arrivee){
		
	unsigned int distance[nbr_station];
	int pere[nbr_station];
	int marque[nbr_station];
	
	int i, tmp;
	
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
		
	int chemin_tmp[nbr_station];
	
	chemin_tmp[0]=arrivee;
	tmp=arrivee;
	i=1;
	
	while(pere[tmp]!=-1){
		tmp=pere[tmp];
		chemin_tmp[i]=tmp;
		i++;
	}
	
	tmp=i-1;
	
	int* chemin= malloc(i*sizeof(int));
	
	while(tmp>=0){
		printf("%d\n",chemin_tmp[tmp]);
		chemin[i-tmp]=chemin_tmp[tmp];
		tmp--;
	}
	
	return chemin;
}



int main(int argc, char **argv)
{
	
	return 0;
}

