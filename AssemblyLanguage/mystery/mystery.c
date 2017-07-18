#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mystery.h"

int add(int x, int y)
{
	return x+y;
}

/*calculate the nth Fibonacci number*/
int dothething(int n)
{
	int fib = 0;
	if(n==0)
		fib = 0;
	else if(n==1 || n==2)
		fib = 1;
	else
	{
		if(num[n-2]!=0 && num[n-1]!=0)
		{
			/*optimization: use Fibonacci numbers which 
			were calculated and stored in the array num 
			instead of making recursive calls.*/
			fib = add(num[n-2], num[n-1]); 
		}
		
		else 
			fib = add(dothething(n-2),dothething(n-1));
	}
	
	num[n] = fib; //store nth Fibonacci number in the nth subscript of num
	
	return fib;
}

int main(int argc, char **argv) {
	
	int n;
	n = atoi(argv[1]);

	int size = sizeof(num);
	while((size/4)<=199)
	{
		size++;
	}
		
	printf("Value:   %d\n", dothething(n));

	return 0;
}