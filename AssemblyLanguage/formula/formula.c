#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include "formula.h"
#include "nCr.h"

typedef struct timeval timeval;

//convert string to integer
int StringToInt(char* num)
{
	int len, i, dig, result;
	len = strlen(num);
	result = 0;
	
	for(i=0; i<len; i++)
	{
		dig = num[i];
		if(isdigit(dig)==0)
		{
			fprintf(stderr, "ERROR: Input must be a positive integer. \n");
			return -1;
		}
		result = (result*10) + (dig - '0');
	}
	
	return result;
}

void printFormula(int n)
{
	if(nCr(n,1) == 0)
	{
		fprintf(stderr, "ERROR: n=13 is the overflow point. \n");
		return;
	}
	
	timeval* start = (timeval*)malloc(sizeof(timeval));
	timeval* stop = (timeval*)malloc(sizeof(timeval));
	
	gettimeofday(start, 0);
	if(n==0)
	{
		printf("%s \n", "1");
	}
	
	else
	{
		printf("%s", "1 + ");
		int i;
		for(i=1; i<=n; i++)
		{
			printf("%d%s%d", nCr(n,i),"*x^",i);
		
			if(i!=n)
				printf("%s"," + ");
		}
	
		printf("\n");
	}
	
	gettimeofday(stop, 0);
	printf("Time Required = %lu microsecond \n", (*stop).tv_usec - (*start).tv_usec);
}

void Destroy(Formula* F)
{
	free(F);
}

int main(int argc, char** argv)
{
	if(argc!=2)
	{
		fprintf(stderr, "ERROR: Incorrect number of arguments. \n");
		return 0;
	}
	
	if(strcmp(argv[1], "-h") == 0)
	{
		printf("%s \n", "Usage: formula <positive integer>");
		return 0;
	}
	
	if(argv[1][0]=='-')
	{
		fprintf(stderr, "ERROR: Input must be a positive integer. \n");
		return 0;
	}
	
	Formula* F = (Formula*)malloc(sizeof(Formula));
	(*F).n = StringToInt(argv[1]);
	if((*F).n == -1)
	{
		Destroy(F);
		return 0;
	}
	
	printFormula((*F).n);
	Destroy(F);
	
	return 0;
}