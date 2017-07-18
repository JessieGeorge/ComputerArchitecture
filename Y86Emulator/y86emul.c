#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "y86emul.h"

int hexToDecimal(char* num)
{
	int power, len, i, asciiVal, intVal, answer;
	
	power = 1;
	answer = 0;
	len = strlen(num);
	
	for(i=len-1; i>=0; i--)
	{
		asciiVal = num[i];
		if(isdigit(asciiVal))
			intVal = asciiVal - '0';
		else //alphabet for hex
		{
			if(asciiVal>=97) //it's a lowercase letter, because ascii value of 'a' is 97
				intVal = asciiVal - 87; //example intVal of 'a' = 97 - 87 = 10
			else //it's an uppercase letter
				intVal = asciiVal - 55; //example 'A' has ascii value 65 so it's intVal = 65 - 55 = 10 
		}
		
		answer += intVal * power;
		power *= 16;
	}
	
	return answer;
}

char* copyString(char* str)
{
	if(str==NULL)
		return NULL;
	
	else
	{
		char* copy = malloc(strlen(str) + 1);
		strcpy(copy, str);
		//copy[strlen(str)] = '\0'; //necessary??
		return copy;
	}
}

char* getSubstring(char* output, char* input, size_t startPos, size_t len) {
	strncpy(output, input + startPos, len);
	output[len] = '\0';
	return output;
}


void decode(emul * E, unsigned char * bytes) 
{
	//printf("E %d \n", bytes[(*E).progCounter]); //remove
	int nextReg;
	int a, b;
	int D; //the 32 bit displacement
	//int val;
	int returnAddress = -1;
	int diff =0;
	unsigned int mask;
	int dig;
	//(*E).destination=0; //keep??
	
	//bytes[100] = 04; //remove
	
	//printf("size %d \n",(*E).size); //remove
	
	/*
	//remove
	int k;
	for(k=0; k<=50; k++)
	{
		printf("bytes[%d] = %d \n", k, bytes[k]); 
	}*/
	
	//DON'T FORGET TO INCREMENT PROGCOUNTER IN EVERY CASE!!!!
	//printf("Made a change! \n"); //remove
	//int i=1; //remove
	while(1) //change it!!
	{
		//printf("progCounter = %d bytes[pC] = %x \n", (*E).progCounter, bytes[(*E).progCounter]); //remove
		switch(bytes[(*E).progCounter]) //checks the OPCODE
		{
			case 0x00: //nop
				//printf("NOP\n"); //remove
				(*E).progCounter++;
				break;
				
			case 0x10: //halt FIX IT!!
				//printf("\n%s \n", HLT); //remove
				printf("\n%s \n", AOK);
				free(E);
				return;
				
				break;
				
			case 0x20: //rrmovl
				//printf("entered here");//remove
				a =  bytes[(*E).progCounter+1]/16;
				b =  bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				//printf("bytes[pC] = %d \n", bytes[(*E).progCounter+1]);//remove
				//printf("a %d \n", a); //remove
				//printf("b %d \n", b); //remove
				(*E).registers[b] = (*E).registers[a];
				//printf("registers[b] %d \n",(*E).registers[b]); //remove
				(*E).progCounter = (*E).progCounter + 2;
				//i++; // remove
				//printf("RRMOVL registers[%d]=%d registers[%d]=%d \n", a, (*E).registers[a], b, (*E).registers[b]);//remove
				break;
				
			case 0x30: //irmovl
				nextReg = bytes[(*E).progCounter+1]%16;
				if(nextReg>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				//printf("bytes[pC] = %d \n", bytes[(*E).progCounter+1]);//remove
				//printf("nextReg %d \n", nextReg); //remove
				//(*E).registers[nextReg] = bytes[(*E).progCounter + 5]*10000 + bytes[(*E).progCounter + 4]*1000 + bytes[(*E).progCounter + 3]*100 + bytes[(*E).progCounter + 2]; //because it's little endian 
				(*E).registers[nextReg] = (bytes[(*E).progCounter + 5] << 24) + (bytes[(*E).progCounter + 4] << 16) + (bytes[(*E).progCounter + 3] << 8) + bytes[(*E).progCounter + 2]; //because it's little endian 
				//(*E).registers[nextReg] = (int *)(bytes[(*E).progCounter + 2]); 
				//(*E).registers[nextReg] = val;
				//printf("val %d \n",val); //remove
				//printf("reg %d \n",(*E).registers[nextReg]); //remove
				(*E).progCounter = (*E).progCounter + 6;
				//i++; //remove
				//printf("i = %d", i); //remove
				//printf("IRMOVL registers[%d]=%d \n", nextReg, (*E).registers[nextReg]);//remove
				break;
				
			case 0x40: //rmmovl
				//printf("entered 40\n"); //remove
				a = bytes[(*E).progCounter+1]/16;
				b = bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				D = (bytes[(*E).progCounter + 5] << 24) + (bytes[(*E).progCounter + 4] << 16) + (bytes[(*E).progCounter + 3] << 8) + bytes[(*E).progCounter + 2];
				//printf("a=%d b=%d D = %x", a, b, D); //remove
				bytes[(*E).registers[b]+D+0] = ((*E).registers[a]& 0x000000ff);
				bytes[(*E).registers[b]+D+1] = ((*E).registers[a]& 0x0000ff00) >> 8;
				bytes[(*E).registers[b]+D+2] = ((*E).registers[a]& 0x00ff0000) >> 16;
				bytes[(*E).registers[b]+D+3] = ((*E).registers[a]& 0xff000000) >> 24;
				//printf("reg[b]=%x reg[a]=%x", (*E).registers[b], (*E).registers[a]); //remove
				(*E).progCounter = (*E).progCounter + 6;
				//printf("RMMOVL bytes[b...] = %x %x %x %x \n", bytes[(*E).registers[b]+D+0], bytes[(*E).registers[b]+D+1], bytes[(*E).registers[b]+D+2], bytes[(*E).registers[b]+D+3]);//remove
				break;
				
			case 0x50: //mrmovl
				a = bytes[(*E).progCounter+1]/16;
				b = bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				D = (bytes[(*E).progCounter + 5] << 24) + (bytes[(*E).progCounter + 4] << 16) + (bytes[(*E).progCounter + 3] << 8) + bytes[(*E).progCounter + 2];
				//(*E).registers[b] = bytes[(*E).registers[a]+D]+(bytes[(*E).registers[a]+D+1]<<8)+(bytes[(*E).registers[a]+D+2]<<16)+(bytes[(*E).registers[a]+D+3]<<24);
				(*E).registers[a] = bytes[(*E).registers[b]+D]+(bytes[(*E).registers[b]+D+1]<<8)+(bytes[(*E).registers[b]+D+2]<<16)+(bytes[(*E).registers[b]+D+3]<<24);
				//DECIDE WHICH ONE OF THE ABOVE TWO TO KEEP
				(*E).progCounter = (*E).progCounter + 6;
				//printf("MRMOVL registers[%d]=%d \n", b, (*E).registers[b]);//remove
				break;
		
			case 0x60: //addl
				//printf("Entered 60 \n"); //remove
				a = bytes[(*E).progCounter+1]/16;
				b = bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				(*E).registers[b] += (*E).registers[a];
				//printf("INSIDE ADD \n"); //remove
				//printf("a = %d and b = %d \n", a, b); //remove
				//printf("registers[a]=%d registers[b]=%d\n",(*E).registers[a], (*E).registers[b]); //remove
				(*E).progCounter = (*E).progCounter + 2;
				
				int val = (*E).registers[b];
				(*E).ZF = 0;
				
				if(val==0)
					(*E).ZF = 1;
				/*
				diff = (*E).registers[b] - (*E).registers[a];
				if(diff==0)
					(*E).ZF = 1;
				else
				{
					(*E).ZF = 0;
				}
				
				if(diff<0)
					(*E).SF = 1;
				else
				{
					(*E).SF = 0;
				}
				//SET OVERFLOW FLAG!!
				*/
				//printf("ADD reg[%d]=%d and reg[%d]=%d ZF=%d SF=%d \n", a, (*E).registers[a], b, (*E).registers[b], (*E).ZF, (*E).SF);//remove
				break;
				
			case 0x61: //subtract
				a = bytes[(*E).progCounter+1]/16;
				b = bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				(*E).registers[b] -= (*E).registers[a];
				(*E).progCounter = (*E).progCounter + 2;
				
				diff = (*E).registers[b] - (*E).registers[a];
				(*E).ZF = 0;
				if(diff==0)
					(*E).ZF = 1;
				/*
				if(diff==0)
					(*E).ZF = 1;
				else
				{
					(*E).ZF = 0;
				}
				
				if(diff<0)
					(*E).SF = 1;
				else
				{
					(*E).SF = 0;
				}
				//SET OVERFLOW FLAG!!
				*/
				//printf("SUB reg[%d]=%d and reg[%d]=%d ZF=%d SF=%d \n", a, (*E).registers[a], b, (*E).registers[b], (*E).ZF, (*E).SF);//remove
				break;
				
			case 0x62: //andl
				a = bytes[(*E).progCounter+1]/16;
				b = bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				//(*E).registers[b] &= (*E).registers[a];
				(*E).progCounter = (*E).progCounter + 2;
				
				(*E).ZF = 0;
				val = (*E).registers[b] & (*E).registers[a];
				if(val==0)
					(*E).ZF = 1;
				/*
				diff = (*E).registers[b] - (*E).registers[a];
				if(diff==0)
					(*E).ZF = 1;
				else
				{
					(*E).ZF = 0;
				}
				
				if(diff<0)
					(*E).SF = 1;
				else
				{
					(*E).SF = 0;
				}
				//SET OVERFLOW FLAG!!
				*/
				//printf("ANDL reg[%d]=%d and reg[%d]=%d ZF=%d SF=%d \n", a, (*E).registers[a], b, (*E).registers[b], (*E).ZF, (*E).SF);//remove
				break;
			
			case 0x64:
				a = bytes[(*E).progCounter+1]/16;
				b = bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				(*E).registers[b] *= (*E).registers[a];
				//(*E).registers[b] &= (*E).registers[a];
				(*E).progCounter = (*E).progCounter + 2;
				
				(*E).ZF = 0;
				val = (*E).registers[b] * (*E).registers[a];
				if(val==0)
					(*E).ZF = 1;
				break;
				
			case 0x65: //cmpl //CHECK THIS AGAIN! FIX THIS!! 
				//printf("Entered CMPL \n"); //remove
				a = bytes[(*E).progCounter+1]/16;
				b = bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				//printf("INSIDE CMPL \n"); //remove
				//printf("a = %d and b = %d \n", a, b); //remove
				//printf("registers[a] = %d and registers[b] = %d \n", (*E).registers[a], (*E).registers[b]); //remove
				//int isEqual = (*E).registers[2] == (*E).registers[0];
				unsigned int diff = (*E).registers[b] - (*E).registers[a];
				if(diff == 0)
					(*E).ZF = 1;
				else 
					(*E).ZF = 0;
				
				//printf("ZF = %d \n", (*E).ZF); //remove
				//NOTE - you have to set SF and OF too!!!!!
				//printf("CMPL registers[%d] = %d and registers[%d] = %d Zf=%d\n", a,(*E).registers[a], b,(*E).registers[b], (*E).ZF); //remove 
				(*E).progCounter = (*E).progCounter + 2;
				//i++;
				
				break;
				
			case 0x70: //jmp
				//printf("Entered 70 \n");//remove
				//(*E).destination = bytes[(*E).progCounter + 4]*10000 + bytes[(*E).progCounter + 3]*1000 + bytes[(*E).progCounter + 2]*100 + bytes[(*E).progCounter + 1];
				//printf("destination %x \n" , (*E).destination); //remove
				//(*E).progCounter = (*E).destination;
				(*E).progCounter = (bytes[(*E).progCounter + 4] << 24) + (bytes[(*E).progCounter + 3] << 16) + (bytes[(*E).progCounter + 2] << 8) + bytes[(*E).progCounter + 1];
				//i++;
				//printf("JMP\n"); //remove
				break;
				
			case 0x73: //je  //FIX IT!!!
				if((*E).ZF == 1)
				{
					//printf("JE\n");//remove
					//(*E).destination = bytes[(*E).progCounter + 4]*10000 + bytes[(*E).progCounter + 3]*1000 + bytes[(*E).progCounter + 2]*100 + bytes[(*E).progCounter + 1];
					//(*E).progCounter = (*E).destination;
					(*E).progCounter = (bytes[(*E).progCounter + 4] << 24) + (bytes[(*E).progCounter + 3] << 16) + (bytes[(*E).progCounter + 2] << 8) + bytes[(*E).progCounter + 1];
					//printf("progCounter %x \n" , (*E).progCounter); //remove
				}
				else
					(*E).progCounter = (*E).progCounter + 5;
				break;
				
			case 0x74: //jne 
				if((*E).ZF == 0)
				{
					//printf("JNE\n ");//remove
					(*E).progCounter = (bytes[(*E).progCounter + 4] << 24) + (bytes[(*E).progCounter + 3] << 16) + (bytes[(*E).progCounter + 2] << 8) + bytes[(*E).progCounter + 1];
					//printf("pC= %x  b[pC] = %x \n",(*E).progCounter, bytes[(*E).progCounter]);//remove
				}
				else
					(*E).progCounter = (*E).progCounter + 5;
				break;
			
				
			case 0x80: //call
				returnAddress = (bytes[(*E).progCounter + 4] << 24) + (bytes[(*E).progCounter + 3] << 16) + (bytes[(*E).progCounter + 2] << 8) + bytes[(*E).progCounter + 1];
				//printf("%x \n",(*E).progCounter);//remove
				(*E).registers[ESP] -=4;
				dig = (*E).progCounter + 5;
				bytes[(*E).registers[ESP] + 0] = (dig & 0x000000ff);
				bytes[(*E).registers[ESP] + 1] = (dig & 0x0000ff00) >> 8;
				bytes[(*E).registers[ESP] + 2] = (dig & 0x00ff0000) >> 16;
				bytes[(*E).registers[ESP] + 3] = (dig & 0xff000000) >> 24;
				(*E).progCounter = returnAddress;
				
				//printf("CALL retAdr=%d bytes[esp...] %x %x %x %x\n", returnAddress, bytes[(*E).registers[ESP] + 0], bytes[(*E).registers[ESP] + 1], bytes[(*E).registers[ESP] + 2], bytes[(*E).registers[ESP] + 3]); //remove
				break;
				
			case 0x90:
				(*E).progCounter = (bytes[(*E).registers[ESP] + 3] << 24) + (bytes[(*E).registers[ESP] + 2] << 16) + (bytes[(*E).registers[ESP] + 1] << 8) + bytes[(*E).registers[ESP] + 0];
				//printf("RETURN pC=%d bytes[pc] = %x\n",(*E).progCounter, bytes[(*E).progCounter]); //remove
				(*E).registers[ESP] +=4;
				break;
				
			case 0xa0: //pushl
				a = bytes[(*E).progCounter+1]/16;
				if(a>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				(*E).registers[ESP] -=4;
				bytes[(*E).registers[ESP]+0] = (*E).registers[a];
				bytes[(*E).registers[ESP]+1] = ((*E).registers[a]& 0x0000ff00) >> 8;
				bytes[(*E).registers[ESP]+2] = ((*E).registers[a]& 0x00ff0000) >> 16;
				bytes[(*E).registers[ESP]+3] = ((*E).registers[a]& 0xff000000) >> 24;
				//printf("PUSHL a=%d reg[esp]=%d bytes[stuff in esp]=%d \n", a, (*E).registers[ESP],(*E).registers[a]); //remove
				(*E).progCounter = (*E).progCounter + 2;
				break;
			
			case 0xb0: //popl
				a = bytes[(*E).progCounter+1]/16;
				if(a>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				(*E).registers[a] = (bytes[(*E).registers[ESP] + 0])
				                    + (bytes[(*E).registers[ESP] + 1] << 8)
				                    + (bytes[(*E).registers[ESP] + 2] << 16)
				                    + (bytes[(*E).registers[ESP] + 3] << 24);
				(*E).registers[ESP] +=4;
				//printf("POPL a=%d reg[esp]=%d bytes[stuff in esp]=%d \n", a, (*E).registers[ESP],(*E).registers[a]); //remove
				(*E).progCounter = (*E).progCounter + 2;
				break;
				
			case 0xc1: //readl
				a = bytes[(*E).progCounter+1]/16;
				if(a>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				D = (bytes[(*E).progCounter + 5] << 24) + (bytes[(*E).progCounter + 4] << 16) + (bytes[(*E).progCounter + 3] << 8) + bytes[(*E).progCounter + 2];
				//int dig = 0;
				
				if(EOF == scanf("%i",&dig))
				{
					(*E).ZF = 1;
					printf("\n%s \n", AOK);
					free(E); 
					return;
				}
				else if(dig<1 || dig>=13)
				{
					printf("Input out of bounds. Integer should be between 1 and 12 \n");
					free(E); 
					return;
				}
				else
				{
					(*E).ZF = 0;
				}
				
				//printf("%d", dig); //remove
				bytes[(*E).registers[a] + D + 0] =  dig & 0x000000ff;
				bytes[(*E).registers[a] + D + 1] = (dig & 0x0000ff00) >> 8;
				bytes[(*E).registers[a] + D + 2] = (dig & 0x00ff0000) >> 16;
				bytes[(*E).registers[a] + D + 3] = (dig & 0xff000000) >> 24;
				(*E).progCounter = (*E).progCounter + 6;
				//printf("READL dig=%i ZF=%d\n", dig, (*E).ZF); //remove
				break;
				
			case 0xd1: //writel
				a = bytes[(*E).progCounter+1]/16;
				if(a>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				D = (bytes[(*E).progCounter + 5] << 24) + (bytes[(*E).progCounter + 4] << 16) + (bytes[(*E).progCounter + 3] << 8) + bytes[(*E).progCounter + 2];
				val = (bytes[(*E).registers[a]+D+3] <<24) + (bytes[(*E).registers[a]+D+2] <<16) + (bytes[(*E).registers[a]+D+1] <<8) + bytes[(*E).registers[a]+D];
				printf("%i",val);
				//printf("WRITEL %i\n",val); //remove
				(*E).progCounter = (*E).progCounter + 6;
				break;
				
			case 0xd0: //writeb
				//printf("Entered d0 \n");//remove
				a = bytes[(*E).progCounter+1]/16;
				if(a>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				//printf("a=%d \n", a);//remove
				//D = bytes[(*E).progCounter + 5]*10000 + bytes[(*E).progCounter + 4]*1000 + bytes[(*E).progCounter + 3]*100 + bytes[(*E).progCounter + 2];
				D = (bytes[(*E).progCounter + 5] << 24) + (bytes[(*E).progCounter + 4] << 16) + (bytes[(*E).progCounter + 3] << 8) + bytes[(*E).progCounter + 2];
				//printf("D %d \n",D); //remove
				int ch = bytes[(*E).registers[a] + D];
				printf("%c",ch); //UNCOMMENT THIS LINE BEFORE YOU SUBMIT!!!!!!!!!
				(*E).progCounter = (*E).progCounter + 6;
				//printf("WRITEB\n"); //remove
				//i++;
				break;
				
			case 0xe0: //movsbl       
				//printf("Entered e0 \n");//remove
				a =  bytes[(*E).progCounter+1]/16;
				b =  bytes[(*E).progCounter+1]%16;
				if(a>7 || b>7)
				{
					fprintf(stderr, ADR);
					return;
				}
				//D = bytes[(*E).progCounter + 5]*10000 + bytes[(*E).progCounter + 4]*1000 + bytes[(*E).progCounter + 3]*100 + bytes[(*E).progCounter + 2]; 
				//D = bytes[(*E).progCounter + 5]*1000000 + bytes[(*E).progCounter + 4]*10000 + bytes[(*E).progCounter + 3]*100 + bytes[(*E).progCounter + 2];
				D = (bytes[(*E).progCounter + 5] << 24) + (bytes[(*E).progCounter + 4] << 16) + (bytes[(*E).progCounter + 3] << 8) + bytes[(*E).progCounter + 2];
				//int X = (bytes[(*E).progCounter + 5]) + (bytes[(*E).progCounter + 4]) + (bytes[(*E).progCounter + 3]) + bytes[(*E).progCounter + 2]; //remove
				//12 34 56 78  =  78 12 34 56 = 78 56 12 34 = 78 56 34 12
				
				/*
				if((*E).progCounter == 26) //remove
				{
					printf("b = %d and registers[b] = %d \n", b,(*E).registers[b]); //remove
					printf("D = %d \n",D); //remove
					//printf("X = %d \n",X); //remove
				}*/
				
				val = bytes[(*E).registers[b]+D];
				//int ValX = bytes[(*E).registers[b]+D]; //remove
				
				/*
				if((*E).progCounter == 26) //remove
				{
					printf("val = %d \n", val); //remove
					//printf("ValX = %d \n", ValX); //remove
				}*/
				
				mask = 255; 
				(*E).registers[a] = mask & val;
				//(*E).registers[a] = bytes[(mask | (*E).registers[b]) + D]; //remove
				//printf("b = %d and registers[b] = %d \n", b,(*E).registers[b]); //remove
				
				//printf("val %x ",val); //remove
				//printf("registers[%d] %x \n", a, (*E).registers[a]); //remove
				(*E).progCounter = (*E).progCounter + 6;
				//i++; //remove
				
				//printf("MOVSBL registers[%d]=%d \n", a, (*E).registers[a]); //remove 
				break;
		
			default:
				//printf("%x \n",bytes[(*E).progCounter]); //remove
				//printf("DEFAULT CASE\n"); //remove
				fprintf(stderr, INS);
				return;
		}
	}
	
	/*
	//remove
	int j;
	for(j=0; j<8; j++)
	{
		printf("registers[%d] = %d \n", j, (*E).registers[j]); //remove
	}*/
}


void fillArray(char * f) {
	
	/*
	printf("\nfile details\n");
	printf("\n%s\n",f);
	int flen=strlen(f);
	printf("\n%d\n",flen);
	*/
	
	emul * E = (emul*)malloc(sizeof(emul));
	(*E).progCounter = -1;
	//printf("line 228 progCounter = %d \n", (*E).progCounter); //remove
	(*E).size = 0;
	
	char * file = copyString(f);
	char* delim = "\t\r\n\v\f";
	char * tok = strtok(file, delim);
	char * dir; //directive
	char * adr; //address
	char * arg; //argument
	 
	int i; 
	int j; 

	int long_arg;

	char * miniInstruc;
	unsigned char * bytes; //bytes of memory
	
	int sawSize = 0;
	int sawText = 0;
	
	if(strcmp(tok,".size")==0)
	{
		tok = strtok(NULL, delim);
		(*E).size = hexToDecimal(tok);
		sawSize = 1;
	}
	tok = strtok(NULL, delim); //get the next dir
	
	bytes = malloc(((*E).size + 1) * sizeof(unsigned char));

	for (i = 0; i < (*E).size; i++) {
		bytes[i] = 0;
	}

	for (i = 0; i < 9; i++) {
		(*E).registers[i] = 0;
	}

	i = 0;

	while (tok!=NULL) {
		if(strcmp(tok,".size")==0 && sawSize==1)
		{
			printf("WARNING: More than one .size directive \n");
			tok = strtok(NULL, delim); //skip
		}
		
		else if (strcmp(tok, ".string") == 0) 
		{		
			dir = copyString(tok); 
			tok = strtok(NULL, delim); 

			adr = copyString(tok); 
			tok = strtok(NULL, delim); 

			arg = copyString(tok); 
			tok = strtok(NULL, delim); 
				i = hexToDecimal(adr);
				j = 1;

				while (j <= strlen(arg)) {
					bytes[i] = (unsigned char) arg[j];

					i++;
					j++;
				}
				
			free(adr); 
			free(arg);
			free(dir);
			adr = 0; 
		} 
		
		else if (strcmp(tok, ".long") == 0) //fix it! see email!
		{ 
			dir = copyString(tok); 
			tok = strtok(NULL, delim); 
			
			adr = copyString(tok); 
			tok = strtok(NULL, delim); 

			arg = copyString(tok); 
			tok = strtok(NULL, delim); 
				long_arg = atoi(arg);
				j = hexToDecimal(adr);
				*(unsigned int*)(bytes+j) = long_arg; 
				free(adr); 
	
			free(arg);
			free(dir);
			adr = 0; 
		}
		
		else if (strcmp(tok, ".bss") == 0) 
		{ 
			dir = copyString(tok); 
			tok = strtok(NULL, delim); 

			adr = copyString(tok); 
			tok = strtok(NULL, delim); 

			arg = copyString(tok); 
			tok = strtok(NULL, delim); 
				i = atoi(arg); 
				j = hexToDecimal(adr); 

				for (; i > 0; i--) {
					bytes[j] = (unsigned char)0; 
					j++;
				}
			
			free(adr); 
			free(arg);
			free(dir);
			adr = 0; 
		} 
		
		

		else if (strcmp(tok, ".byte") == 0) 
		{ 
			dir = copyString(tok); 
			tok = strtok(NULL, delim); 

			adr = copyString(tok); 
			tok = strtok(NULL, delim); 

			arg = copyString(tok); 
			tok = strtok(NULL, delim); 
			bytes[hexToDecimal(adr)] = (unsigned char)hexToDecimal(arg);
			free(adr); 
			free(arg);
			free(dir);
			adr = 0; 
		}
		
		else if ( strcmp(tok, ".text") == 0) 
		{
			dir = copyString(tok); 
			tok = strtok(NULL, delim); 

			adr = copyString(tok); 
			tok = strtok(NULL, delim); 

			arg = copyString(tok); 
			tok = strtok(NULL, delim); 
				if(sawText == 1)
				{
					printf("WARNING: More than one .text directive \n");
					//tok = strtok(NULL, delim); //skip
					//tok = strtok(NULL, delim); //skip
				}
				else
				{
					i = hexToDecimal(adr);
					j = 0;
					
					(*E).progCounter = i;
					//printf("line 384 progCounter = %d \n", (*E).progCounter); //remove
				
					/*
					if ((*E).progCounter == -1) {
						(*E).progCounter = i;
					} else if ((*E).progCounter != -1) {
						printf("progCounter = %d \n", (*E).progCounter); //remove
						fprintf(stderr, INS);
						return;
					}*/

					while (j < strlen(arg)) {
						miniInstruc = getSubstring(miniInstruc, arg, j, 2);
						bytes[i] = (unsigned char) hexToDecimal(miniInstruc);

						i++;
						j = j + 2;
					}
				}
			
			free(adr); 
			free(arg);
			free(dir);
			adr = 0; 
		} 
		
			
		else 
		{
			//printf("tok = %s", tok); //remove
			fprintf(stderr, "Input file contains bad directive. \n");
			return;
		}
	}
	
	 
	free(file);
	free(tok);

	/*
	//remove
	int k;
	for(k=0; k<=(*E).size; k++)
	{
		printf("bytes[%d] = %d \n", k, bytes[k]); 
	}
	*/
	
	//printf("REACHED HERE"); //remove
	decode(E, bytes); 
}

char* inputFile(const char* fileName)
{
	FILE* fp = fopen(fileName,"r");
	
	if(fp == NULL)
	{
		fprintf(stderr, "ERROR: File not found. \n");
		return NULL;
	}
	
	else
	{
		long numBytes;
		char* buffer = 0;
		
		fseek(fp, 0, SEEK_END);
		numBytes = ftell(fp);
		buffer = malloc(numBytes);
		
		if(buffer == NULL)
		{
			fprintf(stderr, "ERROR: Empty file. \n");
			return NULL;
		}
		
		else
		{
			fseek(fp, 0, SEEK_SET);
			fread(buffer, 1, numBytes, fp);
			fclose(fp);
			return buffer;
		}
	}

}

int main(int argc, const char * argv[]) {
	if(argc!=2)
		fprintf(stderr, "ERROR: Invalid input \n"); 
	
	else if(strcmp(argv[1],"-h")==0)
		printf("%s \n", "Usage: y86emul <y86 input file>");
	
	else
	{
		char* file = inputFile(argv[1]);
		if(file)
		{
			fillArray(file);
		}
	}
	
	return 0;
}