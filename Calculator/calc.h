struct Calculate
{
	char op; //operation to perform
	
	char sign1; //sign of first number
	char sign2; //sign of second number
	
	char base1; //base of first number
	char base2; //base of second number
	
	char* num1; //first number
	char* num2; //second number
	
	char* result; //the result after performing operation
	
	char ob; //output base
	
	char outputSign; //sign of output number
};
typedef struct Calculate Calculate;