#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

#define AOK "Status = AOK. Everything is fine. \n"
#define HLT "Halt instruction has been encountered. \n"
#define ADR "\nInvalid address. \n"
#define INS "\nInvalid instruction. \n"

struct emul {
	
	unsigned int size;
	unsigned int registers[8];
	int progCounter;
	
	int OF;
	int ZF;
	int SF;
	
	//int destination; //not needed?
};
typedef struct emul emul;