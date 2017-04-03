#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define t_max 1000

int main(int argc, char** argv)
{
	int a_lib;
	sscanf(argv[3],"%d",&a_lib);
	
	char buf[t_max], c;
	int num1, num2, num3, i;
	
	char n_nom1[strlen(argv[1])+2];
	strcpy(n_nom1,argv[1]);
	n_nom1[strlen(argv[1])+2 - 0] = '\0';
	n_nom1[strlen(argv[1])+2 - 1] = 'm';
	n_nom1[strlen(argv[1])+2 - 2] = '_';
	
	FILE* f1 = fopen(argv[1],"r");
	FILE* f2 = fopen(n_nom1,"w");
	
	
	while(fgets(buf,t_max,f1) != NULL)
	{
		if(buf[0] == 'V')
		{
			sscanf(buf,"%c %d",&c,&num1);
			fprintf(f2,"%c %04d %s",c,(num1>=a_lib)?num1+1:num1,&buf[7]);
		}
		if((buf[0] == 'E')||(buf[0] == 'A'))
		{
			sscanf(buf,"%c %d %d %d",&c,&num1,&num2,&num3);
			fprintf(f2,"%c %d %d %d\n",c,(num1>=a_lib)?num1+1:num1,(num2>=a_lib)?num2+1:num2,num3);
		}
	}
	
	fclose(f1);
	fclose(f2);
	
	
	
	char n_nom2[strlen(argv[2])+2];
	strcpy(n_nom2,argv[2]);
	n_nom2[strlen(argv[2])+2 - 0] = '\0';
	n_nom2[strlen(argv[2])+2 - 1] = 'm';
	n_nom2[strlen(argv[2])+2 - 2] = '_';
	
	f1 = fopen(argv[2],"r");
	f2 = fopen(n_nom2,"w");
	
	while(fgets(buf,t_max,f1) != NULL)
	{
		for(i=0;i<strlen(buf);i += 5)
		{
			if(buf[i] == '\n')fprintf(f2,"%s",&buf[i]);
			else
			{
				sscanf(&buf[i],"%d",&num1);
				fprintf(f2,"%04d ",(num1>=a_lib)?num1+1:num1);
			}
		}
		
		fgets(buf,t_max,f1);
		fprintf(f2,"%s",buf);
	}
	
	fclose(f1);
	fclose(f2);
	
	exit(EXIT_SUCCESS);
}
