/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
struct TokenizerT_ {

	char *tokStr; //stream of tokens
	char *token; //the current token
	char *type; //the type of token i.e. decimal/octal/hex/float
	
	int impossible[4]; //As we see characters, we can decide impossible types. For example, if we see an 'X' we know that everything other than hex is impossible.
	/*note 
	impossible[0] = if decimal is possible or not
	impossible[1] = if octal is possible or not
	impossible[2] = if hex is possible or not
	impossible[3] = if float is possible or not
	*/
	
	int endOfTokSubscp; //end of token subscript i.e. where to end the old token and start and a new one
	int numDeciPoints; //number of decimal points in a token
	int hasX; //whether the token has X or x for a hex number
	int hasE; //whether the token has E or e for a float
	int hasErr; //whether the token has an error i.e. specifically an error in alphabets like "13RZ8"
	int calledPrint; //if you didn't call print function, example if your token stream was only whitespace, then you should tell the user
	
};

typedef struct TokenizerT_ TokenizerT;

//function to print a token
void printTok(TokenizerT *T)
{	
	(*T).calledPrint = 1;
	int j;
	char *possibleTypes[] = {"decimal","octal","hexadecimal","float"};
	int foundPossible = 0; //to see whether we found a possible type for the token or if all types are impossible 
	int len = strlen((*T).token);
	
	if(!(*T).hasX)
		(*T).impossible[2] = 1; //doesn't have an X, cannot be hex
		
	if(!(*T).hasE && ((*T).numDeciPoints == 0))
		(*T).impossible[3] = 1; //doesn't have an E and a decimal point, cannot be float
		
	for(j=0; j<4; j++)
	{	
		if((*T).impossible[j] == 0)
		{
			(*T).type = possibleTypes[j];
			foundPossible = 1;
			break;
		}
	}
	
	//check for unfinished tokens like "0x"
	if(len!=0)
	{
		char lastChar = (*T).token[len-1];
		int asciiOfLastChar = lastChar;
		if(!(*T).hasX) 
		{
			if(!isdigit(asciiOfLastChar)) 
			{
				foundPossible = 0; //anything that's not a hex must end with a digit. if not, it's an unfinished token
			}
		}
		else
		{
			char hexLetters[] = "abcdefABCDEF"; 
			if(!isdigit(asciiOfLastChar) && strchr(hexLetters,lastChar) == NULL) 
			{
				foundPossible = 0; //doesn't end with a digit or a valid hex letter. it's an unfinished token.
			}
		}
	} 
	
	if(!foundPossible && (*T).token!=NULL)
	{
		printf("unfinished token %s \n",(*T).token); //error message for unfinished tokens
	}
	
	else
		printf("%s  %s \n", (*T).type, (*T).token); //output type and token
}

//function to print an error
void printError(char ch, TokenizerT *T)
{
	(*T).calledPrint = 1;
	if((*T).token!=NULL)
		printTok(T);  //prints the token which was collected before the error
				
	printf( "error [0x%02x]\n", ch ); //print the error as hex value
}

//function to append a character to the end of the token
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

//function to check what types are impossible when the current character is a digit
void Digit(int i, char ch, int impossible[])
{	
	int dig = ch - '0'; //convert char to int
	
	if(i==0) //first character of the token
	{
		if(dig == 0) //starts with a zero, cannot be decimal
		{
			impossible[0] = 1;
		}
		
		else //does not start with a zero, cannot be octal or hexadecimal
		{
			impossible[1] = 1;
			impossible[2] = 1;
		}
	}
	
	else if(dig==8 || dig==9) //cannot be octal
	{
		impossible[1] = 1;
	}
}

//function to check what types are impossible when the current character is a decimal point
void DecimalPoint(int impossible[])
{
	//has a decimal point, it can only be float
	impossible[0] = 1;
	impossible[1] = 1;
	impossible[2] = 1;
}

//function to check what types are impossible when the current character is an alphabet
void Alphabet(int i, char ch, TokenizerT* T)
{
	char hexLetters[] = "abcdefABCDEF"; //valid alphabets in a hex number 

	if(ch == 'X' || ch == 'x') 
	{
		if(i==1 && (*T).token[0]=='0') //second char is X and the first char was 0
		{
		
			//cannot be decimal,octal or float
			(*T).impossible[0] = 1;
			(*T).impossible[1] = 1;
			(*T).impossible[3] = 1; 
		
			(*T).hasX = 1;
		}
		
		else
		{
			(*T).hasErr = 1; //X is in the wrong place
			return;
		}
	}
	
	else if(ch=='E' || ch=='e')
	{
			
		if(!(*T).hasX) //the token should not have X because then E or e would be part of hex not float
		{
		
			if((*T).hasE) //already has an exponent
			{
				(*T).hasErr = 1;
				return;
			}
			
			int prevChar = (*T).token[strlen((*T).token)-1]; 
			if(!isdigit(prevChar)) //E or e should appear after a digit
			{
				(*T).hasErr = 1;
				return;
			}
			
			//has an E or e cannot be decimal, octal or hex
			(*T).impossible[0] = 1;
			(*T).impossible[1] = 1;
			(*T).impossible[2] = 1; 
			(*T).hasE = 1; 
		}
	}
	
	else if(strchr(hexLetters,ch) == NULL) //ch is not a valid hex letter
	{
		(*T).hasErr = 1;
		return;
	} 
	
	else if(!(*T).hasX) //if the letter is a-f or A-F but not apart of a hex number, it's an error
	{
		(*T).hasErr = 1;
		return;
	}
}

//function to check the type of the token
void CheckTokenType(TokenizerT* T)
{
	int len = strlen((*T).tokStr);
	char state, ch;	//state is for the switch case, ch is a character in the token  
	int i = 0;
	int ascii; //ascii value of the char ch
	
	for(i=0; i<len; i++)
	{
	
		(*T).endOfTokSubscp = i; //where to end the old token
		ch = (*T).tokStr[i];
		ascii = ch;
		
		if(len==1 && !isdigit(ascii) && ch!=' ')//if the length of token is one, only valid if it's a digit or space
		{
			printError(ch, T);
			return;
		}
		
		if(i==0) //first char
		{
		    //the first char should be a digit or space or backslash (if you're starting with something like \t) 
			if(!isdigit(ascii) && !isspace(ascii) && (ch!='\\'))
			{
				printError(ch, T);
				return;
			}
		}
		
		if(isdigit(ascii))
			state = '0'; //digits
			
		else if(ch == '.')
			state = '1'; //decimal point
		
		else if(isalpha(ascii))
			state = '2'; //alphabet 
			
		else if(ch==' ')
			state = '3'; //blank
			
		else if(ch=='\\') //character is a backslash
		{
			if(i!=len-1) 
			{	
				if((*T).tokStr[i+1] == 't' || (*T).tokStr[i+1] == 'n'|| (*T).tokStr[i+1] == 'v' || (*T).tokStr[i+1] == 'f' || (*T).tokStr[i+1] == 'r' ||(*T).tokStr[i+1] == '0' )
				{
					state = '4'; //whitespace or null character. example: "\t" or "\0"
				}
				
				else //things like "\A" are invalid
				{
					printError(ch, T);
					return;
				}
			}
		}
		
		else if(ch=='+' || ch=='-')
		{
			state = '5'; //character is a sign
		}
		
		else 
		{
			state = '6'; //escape character
		}
		
		switch(state)
		{
			case '0':
			{
				Digit(i, ch, (*T).impossible);
				(*T).token = append((*T).token, ch);
				break;
			}
			
			case '1':
			{
				(*T).numDeciPoints++;
				
				if((*T).hasX) //hex number cannot have decimal point
				{
					printError(ch, T);
					return; 
				}
				
				int prevChar = (*T).tokStr[i-1]; 
				if(!isdigit(prevChar)) //decimal point should be after a digit
				{
					printError(ch, T);
					return; 
				}
				
				if((*T).numDeciPoints > 2) //cannot have more than two decimal points
				{
					printError(ch, T);
					return; 
				}
				
				if((*T).numDeciPoints == 2 && !(*T).hasE) //should not have two decimal points unless there's an exponent example 3.4e1.5
				{
					printError(ch, T);
					return; 
				}
				
				
				DecimalPoint((*T).impossible);
				(*T).token = append((*T).token, ch);
				break;
			}
			
			case '2':
			{
				Alphabet(i, ch, T);
				if((*T).hasErr) //bad alphabet. example: "8M2"
				{
					printError(ch, T);
					return;
				}
				
				(*T).token = append((*T).token, ch);
				break;
			}
			
			case '3':
			{
				if((*T).token!=NULL)
				{
					printTok(T);
				}	
				return;
			}
			
			case '4':
			{
				++i; //exclude the letter which is part of whitespace. example: the t in \t
				++(*T).endOfTokSubscp; 
				
				if((*T).token!=NULL)
				{
					printTok(T);
				}	
				return; 
			}
			
			case '5':
			{
				if((*T).hasX) //hex number cannot have + or - signs
				{
					printError(ch, T);
					return;
				}
				
				if(i!=0)
				{
					int prevChar = (*T).tokStr[i-1];
					if(prevChar!='E' && prevChar!='e') //a sign should be right after an E or e, otherwise it's an error
					{
						printError(ch, T);
						return;
					}
				}
				
				(*T).token = append((*T).token, ch);
				break;
			}
			
			case '6':
			{
				printError(ch, T);
				return; 
			}
			
		}
	
	}
	
	if(i==len && (*T).token!=NULL) //printing the last token
	{
		printTok(T);
	}

}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {

	int len = strlen((*tk).tokStr);
	if((*tk).endOfTokSubscp == (len-1))
		return NULL;

	else
	{
		//clear everything for new token
		(*tk).token = NULL; 
		(*tk).type = NULL;
		(*tk).impossible[0] = 0;
		(*tk).impossible[1] = 0;
		(*tk).impossible[2] = 0;
		(*tk).impossible[3] = 0;
		(*tk).numDeciPoints = 0;
		(*tk).hasX = 0;
		(*tk).hasE = 0;
		(*tk).hasErr = 0;
		
		return (*tk).tokStr + (*tk).endOfTokSubscp+1; //your new stream of tokens should start after the end of the last token 
	}
}

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {

	int i;
	TokenizerT *T = (TokenizerT *)malloc(sizeof(TokenizerT)); 
	for(i=0; i<4; i++) 
	{
		(*T).impossible[i] = 0; //initializing impossible to 0. Later on if a type is impossible, it is changed to 1. See detailed explanation in struct.
	}
	
	(*T).tokStr = ts; //copies the argument 
	
	(*T).calledPrint = 0;//you have not yet called the print function. See detailed explanation in struct.
	
	if(strlen((*T).tokStr) == 0) //argv[1] is empty
	{
		printf("Empty token stream. \n");
		return NULL;
	}
	
	while((*T).tokStr != NULL)
	{
		CheckTokenType(T);
		(*T).tokStr = TKGetNextToken(T);
	}
	
	if((*T).calledPrint == 0) //you never called the print function because the token stream was only whitespace. tell the user.
		printf("Token stream only contains white space or null characters. \n");
		
	return T;
}



/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
	free(tk);
}



/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	
	if(argc == 1)
	{
		printf("Error: No token stream. \n");
		return 0;
	}	
	
	TokenizerT* T;
	T = TKCreate(argv[1]);	
	TKDestroy(T);
	
  return 0;
}
