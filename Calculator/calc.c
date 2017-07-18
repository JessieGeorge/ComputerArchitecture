#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calc.h"

/**
function to check if a number is valid
input: sign, base and number
output: 1 if number is valid, else 0
*/
int checkValidNumber(char sign, char base, char* num)
{
	int len, i, asciiOfChar; 
	char* hexLetters = "abcdefABCDEF";
	
	if(base!='b' && base!='o' && base!='d' && base!='x')
		return 0;
	
	len = strlen(num);
	
	for(i=0; i<len; i++)
	{
	
		asciiOfChar = num[i];
		
		if(base == 'b')
		{
			if(num[i]!='1' && num[i]!='0') //invalid binary
				return 0;
		}
		
		else if(base == 'o')
		{
			if(!isdigit(asciiOfChar))
				return 0;
				
			else if(num[i]=='8' || num[i]=='9') //invalid octal
				return 0;
		}
		
		else if(base == 'd')
		{
			if(!isdigit(asciiOfChar)) //invalid decimal
				return 0;
		}
		
		else
		{
			if(isalpha(asciiOfChar))
			{
				if(strchr(hexLetters, num[i]) == NULL) //invalid hex letter
					return 0;
			}
			
			else if(!isalpha(asciiOfChar) && !isdigit(asciiOfChar)) //invalid character
				return 0;
		}
	}
	
	return 1;
}

/**
function to initialize the struct
input: argc and argv from main
output: the pointer to struct with its variables initialized
*/
Calculate* initCalculate(int argc, char** argv)
{
	Calculate *C = (Calculate *)malloc(sizeof(Calculate));
	
	if(strlen(argv[1]) != 1)
	{
		fprintf(stderr, "ERROR: Invalid operation. \n");
		return NULL;
	}
	
	if(argv[1][0]!='+' && argv[1][0]!='-' && argv[1][0]!='*')
	{
		fprintf(stderr, "ERROR: Invalid operation. \n");
		return NULL;
	}
	(*C).op = argv[1][0];
	
	if(strlen(argv[2]) <2) //minimum length contains at least base and one digit
	{
		fprintf(stderr, "ERROR: Invalid input. \n");
		return NULL;
	}
	if(argv[2][0] == '-')
	{
		if(strlen(argv[2]) <3) //minimum length contains at least sign, base and one digit
		{
			fprintf(stderr, "ERROR: Invalid input. \n");
			return NULL;
		}
		(*C).sign1 = '-';
		(*C).base1 = argv[2][1];
		(*C).num1 = argv[2]+2;
	}
	else
	{
		(*C).sign1 = '+';
		(*C).base1 = argv[2][0];
		(*C).num1 = argv[2]+1;
	}
	
	if(checkValidNumber((*C).sign1, (*C).base1, (*C).num1) == 0)
	{
		fprintf(stderr, "ERROR: Invalid input. \n");
		return NULL;
	}
	
	if(strlen(argv[3]) <2) //minimum length contains at least base and one digit
	{
		fprintf(stderr, "ERROR: Invalid input. \n");
		return NULL;
	}
	if(argv[3][0] == '-')
	{
		if(strlen(argv[2]) <3) //minimum length contains at least sign, base and one digit
		{
			fprintf(stderr, "ERROR: Invalid input. \n");
			return NULL;
		}
		(*C).sign2 = '-';
		(*C).base2 = argv[3][1];
		(*C).num2 = argv[3]+2;
	}
	else
	{
		(*C).sign2 = '+';
		(*C).base2 = argv[3][0];
		(*C).num2 = argv[3]+1;
	}
	if(checkValidNumber((*C).sign2, (*C).base2, (*C).num2) == 0)
	{
		fprintf(stderr, "ERROR: Invalid input. \n");
		return NULL;
	}
	
	
	if(strlen(argv[4]) != 1)
	{
		fprintf(stderr, "ERROR: Invalid output base. \n");
		return NULL;
	}
	if(argv[4][0]!='b' && argv[4][0]!='o' && argv[4][0]!='d' && argv[4][0]!='x')
	{
		fprintf(stderr, "ERROR: Invalid output base. \n"); 
		return NULL;
	}
	(*C).ob = argv[4][0];
	
	return C;
}

/**
function to append a character to a string
input: the string p and the character to append c
output: string with character appended 
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
function to convert string to integer
*/
int StringToInt(char *word)
{
	int l = strlen(word);
	int i, asciiVal;
	int num = 0;
	
	for(i=0; i<l; i++)
	{
		asciiVal = word[i];
		if(asciiVal>=48 && asciiVal<=57) //ascii value of 0-9
		{ 
		    num = num*10 + (asciiVal - '0');
		}
	}
	
	return num;
}

/**
function to convert decimal number to binary
*/
char* DeciToBin(char* num)
{
	int n, i, len;
	char* revAnswer = NULL; //we get the answer in reverse
	char* answer = NULL;
	
	n = StringToInt(num);
	
	while(n!=1)
	{
		if((n%2) == 0)
			revAnswer = append(revAnswer,'0');
		else
			revAnswer = append(revAnswer,'1');
			
		n = n/2;
	}
	
	revAnswer = append(revAnswer,'1');
	
	len = strlen(revAnswer);
	for(i=len-1; i>=0; i--)
	{
		answer = append(answer, revAnswer[i]);
	}
	
	return answer;
}

/**
function to get binary number 
input: character (1-9, A-F, a-f)
output: equivalent 4 bit binary number

*/
char* getBinNumber(char num)
{
	switch(num)
	{
		case '0':
			return "0000";
		case '1':
			return "0001";
		case '2':
			return "0010";
		case '3':
			return "0011";
		case '4':
			return "0100";
		case '5':
			return "0101";
		case '6':
			return "0110";
		case '7':
			return "0111";
		case '8':
			return "1000";
		case '9':
			return "1001";
		case 'A':
			return "1010";
		case 'a':
			return "1010";
		case 'B':
			return "1011";
		case 'b':
			return "1011";
		case 'C':
			return "1100";
		case 'c':
			return "1100";
		case 'D':
			return "1101";
		case 'd':
			return "1101";
		case 'E':
			return "1110";
		case 'e':
			return "1110";
		case 'F':
			return "1111";
		default:
			return "1111";
	}
}

/**
function to convert octal number to binary
*/
char* OctalToBin(char* num)
{
	int len, i;
	len = strlen(num);
	char* binNum;
	char* answer = NULL;
	
	for(i=0; i<len; i++)
	{
		binNum = getBinNumber(num[i])+1; //because you want 3 bits, not 4
		answer = append(answer,binNum[0]);
		answer = append(answer,binNum[1]);
		answer = append(answer,binNum[2]);
	}
	
	return answer;
}

/**
function to convert hex number to binary
*/
char* HexToBin(char* num)
{
	int len, i;
	len = strlen(num);
	char* binNum;
	char* answer = NULL;
	
	for(i=0; i<len; i++)
	{
		binNum = getBinNumber(num[i]);
		answer = append(answer,binNum[0]);
		answer = append(answer,binNum[1]);
		answer = append(answer,binNum[2]);
		answer = append(answer,binNum[3]);
	}
	
	return answer;
}

/**
function to convert an integer to string
*/
char* IntToString(int num)
{
	int n = num;
	char* str;
	int size = 0;
	
	if(n == 0)
		size = 1;
	
	while(n>0)
	{
		size++;
		n = n/10;
	}
	
	n = num;
	str = (char*)calloc(size, size+1);
	while(size>0)
	{
		size--;
		str[size] = (n%10) + '0';
		n = n/10; 
	}
	str[strlen(str)] = '\0';
	return str;
}

/**
function to convert binary number to decimal
*/
char* BinToDeci(char* num)
{
	int len,i,pow,dig,sum; //pow is used to calculate powers of two
	
	len = strlen(num);
	pow = 1; 
	sum = 0;
	
	for(i = len-1; i>=0; i--)
	{ 
		dig = num[i] - '0'; //extract each digit of the number
		sum += dig*pow;
		pow *=2;
	}

	return IntToString(sum);
}

/**
function to convert binary number to octal
*/
char* BinToOctal(char* num)
{
	int len, i, leftOver, parcel; //split the number into parcels of 3 bits each
	int dig1, dig2, dig3;
	char parcelChar;
	char* revAnswer = NULL;
	char* answer = NULL;
	
	len = strlen(num);
	leftOver = len%3; //number of digits left over after parcelling
	
	for(i=len-1; i-2>=leftOver; i=i-3)
	{
		dig1 = num[i] - '0';
		dig2 = num[i-1] - '0';
		dig3 = num[i-2] - '0';
		parcel = dig1 + dig2*2 + dig3*4; //multiply each number of the parcel by increasing powers of 2
		parcelChar = parcel + '0';
		revAnswer = append(revAnswer,parcelChar);
	}
	
	if(i==0) //one digit left over
		revAnswer = append(revAnswer,num[0]);
	
	else if(i==1)//two digits left over
	{
		dig1 = num[1] - '0';
		dig2 = num[0] - '0';
		parcel = dig1 + dig2*2;
		parcelChar = parcel + '0';
		revAnswer = append(revAnswer, parcelChar);
	}
	
	len = strlen(revAnswer);
	for(i=len-1; i>=0; i--)
	{
		answer = append(answer,revAnswer[i]);
	}
	
	return answer;
}

/**
function to get hexadecimal letter
input: integer between 10-15
output: equivalent hex letter A-F
*/
char getHexLetter(int num)
{
	switch(num)
	{
		case 10:
			return 'A';
				
		case 11:
			return 'B';
					
		case 12:
			return 'C';
					
		case 13:
			return 'D';
					
		case 14:
			return 'E';
					
		default:
			return'F';
	}
}

/**
function to convert binary number to hexadecimal
*/
char* BinToHex(char* num)
{
	int len, i, leftOver, parcel; //split number into parcels of 4 bits each
	int dig1, dig2, dig3, dig4;
	char parcelChar;
	char* revAnswer = NULL;
	char* answer = NULL;
	
	len = strlen(num);
	leftOver = len%4; //number of digits left over after parcelling
	
	for(i=len-1; i-3>=leftOver; i=i-4)
	{
		dig1 = num[i] - '0';
		dig2 = num[i-1] - '0';
		dig3 = num[i-2] - '0';
		dig4 = num[i-3] - '0';
		parcel = dig1 + dig2*2 + dig3*4 + dig4*8; //multiply each number of the parcel by increasing powers of 2
		
		if(parcel>9) //double digit
			parcelChar = getHexLetter(parcel);
		else 
			parcelChar = parcel + '0';
			
		revAnswer = append(revAnswer,parcelChar);
	}
	
	if(i==0) //one digit left over
		revAnswer = append(revAnswer,num[0]);
	
	else if(i==1)//two digits left over
	{
		dig1 = num[1] - '0';
		dig2 = num[0] - '0';
		parcel = dig1 + dig2*2;
		parcelChar = parcel + '0';
		revAnswer = append(revAnswer, parcelChar);
	}
	
	else if(i==2)//three digits left over
	{	
		dig1 = num[2] - '0';
		dig2 = num[1] - '0';
		dig3 = num[0] - '0';
		parcel = dig1 + dig2*2 + dig3*4;
		parcelChar = parcel + '0';
		revAnswer = append(revAnswer, parcelChar);
	}
	
	len = strlen(revAnswer);
	for(i=len-1; i>=0; i--)
	{
		answer = append(answer,revAnswer[i]);
	}
	
	return answer;
}

/**
function to convert input into binary
input: number to convert and its base
output: equivalent binary string
*/
char* ConvertInputToBinary(char* num, char base)
{
	if(base == 'o')
		return OctalToBin(num);
		
	else if(base == 'x')
		return HexToBin(num);
		
	else
		return DeciToBin(num);
}

/**
function to convert binary number to output base
input: binary number, base into which we must convert it
output: string in the output base
*/
char* ConvertBinaryToOutput(char* num, char base)
{
	if(base == 'o')
		return BinToOctal(num);
		
	else if(base == 'x')
		return BinToHex(num);
		
	else
		return BinToDeci(num);
}

/**
function to insert zeros at the beginning of a string
(because it's easier to add and subtract strings of the same length)
input: n = number of zeros to insert. num = number which we must insert zeros into.
output: string with zeros in front of a number.
*/
char* insertZeros(int n, char* num)
{
	int i, len;
	char* temp = num;
	char* zeros = NULL;
	
	len = strlen(num);
	
	while(n!=0)
	{
		zeros = append(zeros,'0');
		n = n - 1;
	}
		
	num = zeros;
		
	for(i=0; i<len; i++)
	{
		num = append(num,temp[i]);
	}
	
	return num;
}

/**
function to add two numbers of the same base
input: number 1 and number 2 and their base
output: sum of the two numbers in the same base
*/
char* add(char* num1, char* num2, char base)
{
	int len1, len2, i, j;
	int length, lenDiff;
	int asciiVal1, asciiVal2, dig1, dig2, carries, sumVal;
	int b;

	char* result = NULL;
	
	if(base == 'b')
		b = 2;
	else if(base == 'o')
		b = 8;
	else if(base == 'x')
		b = 16;
	else
		b = 10;
	
	
	len1 = strlen(num1);
	len2 = strlen(num2);
	
	lenDiff = len1 - len2;
	
	
	if(lenDiff > 0) //num1 is longer than num2
	{
		length = len1;
		num2 = insertZeros(lenDiff, num2);
	}
	
	else if(lenDiff < 0) //num1 is shorter than num2
	{
		length = len2;
		num1 = insertZeros(-1*lenDiff, num1); 
	}
	
	else //num1 and num2 are the same length
	{
		length = len1;
	}
	char sumChar[length+1]; 
	carries = 0;
	j = 0;
	
	for(i=length-1; i>=0; i--)
	{
		asciiVal1 = num1[i];
		asciiVal2 = num2[i];
		
		if(isalpha(asciiVal1))
		{
			if(asciiVal1>=97) //lowercase letter
				dig1 = asciiVal1 - 87; //example for 'a', dig1 = 97 - 87 = 10
			else //uppercase letter
				dig1 = asciiVal1 - 55; //example for 'A', dig1 = 65 - 55 = 10
		
		}
		else 
		{
			dig1 = asciiVal1 - '0';
		}
		
		if(isalpha(asciiVal2))
		{
			if(asciiVal2>=97) //lowercase letter
				dig2 = asciiVal2 - 87; //example for 'a', dig2 = 97 - 87 = 10
			else //uppercase letter
				dig2 = asciiVal2 - 55; //example for 'A', dig2 = 65 - 55 = 10
		}
		else
		{
			dig2 = asciiVal2 - '0';
		}
		
		sumVal = (dig1+dig2+carries) % b;
		
		if(sumVal > 9) //double digit value
		{
			sumChar[j] = getHexLetter(sumVal);
		}
		else
		{
			sumChar[j] = sumVal + '0';
		}
		
		carries = (dig1+dig2+carries)/ b;
		j = j+1;
	}
	
	if(carries!=0)
	{
		if(carries > 9) //double digit value
		{
			sumChar[j] = getHexLetter(carries);
		}
		else
			sumChar[j] = carries+'0';
	}
	else
	{
		j = j-1; //because you didn't use the last subscript of sumChar
	}
		
	for(i = j; i>=0; i--)
	{
		result = append(result, sumChar[i]);
	}
	return result;
}

/**
function to subtract two numbers of the same base
input: number 1 and number 2 in binary form and the struct C (in order to access the outputSign and base)
output: the difference of the two numbers in binary form
*/
char* subtract(char* number1, char* number2, Calculate* C)
{
	int len1, len2, length, i, j;
	len1 = strlen(number1);
	len2 = strlen(number2);
	char* tempnum1  = number1;
	char* tempnum2  = number2;
	
	char* result = NULL; 
	char* big = NULL; //the bigger number
	char* small = NULL; //the smaller number
	
	if(len1>len2)
	{
		length = len1;
		tempnum2 = insertZeros(len1-len2, tempnum2);
	}
	
	else if(len1<len2)
	{
		length = len2;
		tempnum1 = insertZeros(len2-len1, tempnum1);
	}
	
	else
		length = len1;
		
	char n1[length];
	char n2[length];
	char ans[length];
	
	if(strcmp(tempnum1,tempnum2)>=0) //then tempnum1>=tempnum2
	{
		big = tempnum1;
		small = tempnum2;
		(*C).outputSign = (*C).sign1;
	}
	else
	{
		big = tempnum2;
		small = tempnum1;
		(*C).outputSign = (*C).sign2;
	}
	
	for(i=0; i<length; i++)
	{
		
		n1[i] = big[i];
		n2[i] = small[i];
		ans[i] = '0';
	}
	
	for(i=length-1; i>=0; i--)
	{
		if(n1[i]=='0' && n2[i]=='1')
		{
			ans[i] = '1';
			
			if(n1[i-1]=='1') //you just borrowed the 1
				n1[i-1] = '0';
			
			else //go further back to borrow the 1
			{
				j = i-2;
				while(j>=0 && n1[j]!='1')
				{
					j--;
				}
				
				n1[j] = '0';
				j = j+1;
				while(j!=i)
				{
					n1[j] = '1';
					j++;
				}
			}
		}
		else
		{
			ans[i] = n1[i] - n2[i] + '0';
		}
	}
	
	for(i=0; i<length; i++)
	{
		result = append(result,ans[i]);
	}
	
	return result;
}

/**
function to convert any base to Decimal
input: base and number to convert
output: equivalent decimal in integer form
NOTE - This function is a helper function to multiply
*/
int baseToDecimal(char base, char* num)
{
	int power, b, len, i, asciiVal, intVal, answer;
	
	power = 1;
	answer = 0;
	
	if(base == 'b')
		b = 2;
	else if(base == 'o')
		b = 8;
	else if(base == 'x')
		b = 16;
	else
		b = 10;
		
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
		power *= b;
	}
	
	return answer;
}

/**
function to control addition:
if the bases of all numbers are same, just add, no need to call conversion functions
if the bases of numbers are different, convert the numbers to binary and then add
finally convert the answer to output base
*/
void AdditionControl(Calculate* C)
{
	char* n1 = NULL;
	char* n2 = NULL;
	if(((*C).base1 == (*C).base2) && ((*C).base1 == (*C).ob)) 
	{
		(*C).result = add((*C).num1, (*C).num2, (*C).ob);
		return;
	}
	
	else
	{
		if((*C).base1!='b')
			n1 = ConvertInputToBinary((*C).num1, (*C).base1);
		else
			n1 = (*C).num1;
			
		if((*C).base2!='b')
			n2 = ConvertInputToBinary((*C).num2, (*C).base2);
		else
			n2 = (*C).num2;
			
		(*C).result = add(n1, n2, 'b');
		
		if((*C).ob!='b')
		{
			(*C).result = ConvertBinaryToOutput((*C).result, (*C).ob);
		}
	}
}

/**
function to control subtraction:
convert the numbers to binary and then subtract
finally convert the answer to output base
*/
void SubtractionControl(Calculate* C)
{
	char* n1 = NULL;
	char* n2 = NULL;
	if((*C).base1!='b')
		n1 = ConvertInputToBinary((*C).num1, (*C).base1);
	else
		n1 = (*C).num1;
		
	if((*C).base2!='b')
		n2 = ConvertInputToBinary((*C).num2, (*C).base2);
	else
		n2 = (*C).num2;

	(*C).result = subtract(n1, n2, C);
		
	if((*C).ob!='b')
	{
		(*C).result = ConvertBinaryToOutput((*C).result, (*C).ob);
	}
}

/**
function to multiply two numbers of any base.
The logic I used: take two numbers, convert to decimal, multiply, convert the product from decimal to binary (later it will be converted to output base)
input: the two numbers to multiply and each of their bases
output: the product in binary string form
*/

char* multiply(char* num1, char* num2, char base1, char base2)
{
	int n1 = baseToDecimal(base1, num1);
	int n2 = baseToDecimal(base2, num2);
	int mul = n1*n2;
	
	char* str = IntToString(mul);
	str = DeciToBin(str);
	
	return str;
}


/**function to control multiplication:
it decides sign of answer based on signs of numbers
it calls multiply function and receives the answer in binary form
it converts the binary answer into the output base
*/
void MultiplicationControl (Calculate* C)
{
	if((*C).sign1 == (*C).sign2)
		(*C).outputSign = (*C).sign1;
	else
		(*C).outputSign = '-';
	
	(*C).result = multiply((*C).num1, (*C).num2, (*C).base1, (*C).base2);
	
	if((*C).ob!='b')
	{
		(*C).result = ConvertBinaryToOutput((*C).result, (*C).ob);
	}
}

/**
function to perform operations:
adding two numbers with the same sign --> call AdditionControl
adding two numbers with different signs --> call SubtractionControl
subtracting two numbers with the same sign --> call SubtractionControl
subtracting two numbers with different signs --> call AdditionControl 
multiplying two numbers --> call MultiplicationControl
*/
void performOperation(Calculate* C)
{
	if((*C).op == '+')
	{
		if((*C).sign1 == (*C).sign2)
		{
			(*C).outputSign = (*C).sign1;
			AdditionControl(C);
		}
		else
		{
			SubtractionControl(C);
		}
	}
	
	else if((*C).op == '-')
	{
		if((*C).sign1 == (*C).sign2)
		{
			SubtractionControl(C);
		}
		else
		{
			(*C).outputSign = (*C).sign1;
			AdditionControl(C);
		}
	}
	
	else if((*C).op == '*')
	{
		MultiplicationControl(C);
	}
}

/**
function to free the memory we used
*/
void Destroy( Calculate* C ) {
	free(C);
}

int main(int argc, char** argv)
{	
	if(argc!=5)
	{
		fprintf(stderr, "ERROR: Invalid input. \n");
		return 0;
	}
	
	Calculate* C;
	C = initCalculate(argc, argv);
	if(C == NULL)
		return 0;
	
	performOperation(C); 
	
	if((*C).outputSign == '-')
		printf("%c",(*C).outputSign);
		
	printf("%c%s \n",(*C).ob,(*C).result); //print output
	
	Destroy(C);

	return 0;
}
