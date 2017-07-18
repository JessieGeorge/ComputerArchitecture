#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "format.h"

/**
function to initialize the struct 
input: argc and argv from main
output: struct with input and type initialized
*/
Format* initFormat(int argc, char** argv)
{	 
	Format* F = (Format*)malloc(sizeof(Format));
	
	if(strlen(argv[1])!=32)
	{
		fprintf(stderr, "ERROR: Invalid length for input bit sequence. \n");
		return NULL;
	}
	
	(*F).input = argv[1];
	int i;
	for(i=0; i<32; i++)
	{
		if((*F).input[i]!='0'&&(*F).input[i]!='1')
		{
			fprintf(stderr, "ERROR: Invalid input bit sequence. \n");
			return NULL;
		}
	}
	
	(*F).type = argv[2];
	
	if(strcmp((*F).type,"int")!=0 && strcmp((*F).type,"float")!=0)
	{
		fprintf(stderr, "ERROR: Invalid type. Must be int or float. \n");
		return NULL;
	}
	
	return F;
}

/**
function to convert integer in binary form to decimal 
input: binary integer in string form
output: decimal integer
*/
int Integer_BinToDecimal(char* num)
{
	int power, len, i, dig, answer;
	
	power = 1;
	answer = 0;
		
	len = strlen(num);
	
	for(i=len-1; i>=0; i--)
	{
		dig = num[i] - '0';
		answer += dig * power;
		power *= 2;
	}
	return answer;
}

/**
function to append a character to a string
input: the string p and the character to append c
output: new string with character appended
*/
char* append(char* p, char c)
{
	int len;
	if(p == 0) //empty pointer
	{
		p = (char*)realloc(p,2);
		p[0] = c;
		p[1] = '\0';
	}
	
	else
	{
		len = strlen(p);
		p = (char*)realloc(p, len+2);
		p[len] = c;
		p[len+1] = '\0';
	}
	
	return p;
}

/**
function to compute mantissa
input: 23 bit mantissa in string form
output: mantissa in float form
*/
float getMantissa(char* num)
{
	int len, i;
	float power, dig;
	float answer;
	
	power = 2;
	answer = 0;
		
	len = strlen(num);
	
	for(i=0; i<len; i++)
	{
		dig = num[i] - '0';
		
		answer += dig/power;
		//printf("dig=%f power=%f quotient=%f answer=%f\n", dig, power, dig/power, answer); //remove
		power *= 2;
	}
	
	return answer;
}

/**
function to check for infinity or NaN
input: sign, exponent and mantissa of the float
output: 1 if it is inf or NaN, else 0
*/
int checkINForNAN(int sign, char* exp, char* mant)
{
	if(strcmp(exp,"11111111")==0)
	{
		if(strcmp(mant,"00000000000000000000000")==0)
		{
			if(sign == 1)
				printf("ninf \n");
			else
				printf("pinf \n");
		}
		
		else
		{
			if(sign == 1)
				printf("-NaN \n");
			else
				printf("+NaN \n");
		}
		
		return 1;
	}
	
	return 0;
}

/**
function to convert float in binary form to decimal
calculates sign, exponent and mantissa
formula used: answer = -1^sign*(1 + mantissa)*2^e 
*/
void Float_BinToDecimal(Format* F)
{
	int sign;
	char* num = (*F).input;
	char* exp = (char*)malloc(8*sizeof(char)); //exponent in string form
	int exponent,i;
	char* mant = (char*)malloc(23*sizeof(char)); //mantissa in string form
	float mantissa, answer, powerOfTwo;
	exponent = 0;
	
	if(num[0]=='1') //negative number
		sign = 1;
	else
		sign = 0;
	
	exp = strncpy(exp, num+1, 8);
	mant = strcpy(mant, num+9);
	
	if(checkINForNAN(sign, exp, mant) == 1) //check for infinity or NaN
		return;
	exponent = Integer_BinToDecimal(exp) - 127;
	
	powerOfTwo = 1;
	for(i=0; i<abs(exponent); i++)
	{
		powerOfTwo *= 2;
	}
	
	mantissa = getMantissa(mant);
	
	answer = 1+mantissa;
	if(sign == 1)
		answer = -1 * answer; 
	
	if(exponent<0)
		answer = answer/powerOfTwo;
	else
		answer = answer * powerOfTwo;
		
	if(strcmp(num,"00000000000000000000000000000000") == 0)
		printf("0.0e0 \n");
	else if(strcmp(num,"10000000000000000000000000000000") == 0)
		printf("-0.0e0 \n");
		
	else 
		printf("%e \n",answer);
}


/**
control function to convert input to decimal
if type is int --> call Integer_BinToDecimal
if type is float --> call Float_BinToDecimal 
*/
void convertToDecimal(Format* F)
{
	
	if(strcmp((*F).type,"int")==0)
	{
		printf("%d \n", Integer_BinToDecimal((*F).input)); 
	}
	
	else
	{
		Float_BinToDecimal(F);	
	}
}

/**
function to free the memory we used
*/
void Destroy(Format* F)
{
	free(F);
}

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		fprintf(stderr, "ERROR: Invalid number of arguments. \n");
		return 0;
	}
	
	Format* F;
	F = initFormat(argc, argv);
	
	if(F == NULL)
		return 0;
		
	convertToDecimal(F);
	
	Destroy(F);
	return 0;
}