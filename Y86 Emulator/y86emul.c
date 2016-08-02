#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y86emul.h"

char* emem;
int reg[8];
int eip = 0; 
int OF, ZF, SF;
status CPUState = AOK;
int text = 0, line = 2, size = 0;

/*Performs a registry to registry move*/
void rrmovl() {
    eip++;
    bitfield *new = (bitfield *)(emem + eip);
    int lower, higher;
    lower = new->low;
    higher = new->high;
    if ((lower >= 0 && lower <= 7) && (higher >= 0 && higher <= 7)) {
        reg[higher] = reg [lower];
    }
    else {
        fprintf(stderr, "Not valid register.\n");
        exit(0);
    }
    eip++;
    // TODO: WORK ON EC printf("rrmovl reg[%x], reg[%x] , new->low, new->high");
	return;
}

/*Performs an immediate to registry move*/
void irmovl() {
    eip++;
    bitfield *new = (bitfield *)(emem + eip);
    int i;
    i = new->high;
    if ((new->low == 15) && (i >= 0 && i <= 7)) {
        int *numPtr = (int *)(emem + eip + 1);
        reg[i] = *numPtr;
    }
    else {
        fprintf(stderr,"Not valid register.\n");
        exit(0);
    }

    eip += 5;
	return;
}
   
/*Performs a registry to memory move*/
void rmmovl() {
    eip++;
    bitfield *new = (bitfield *)(emem + eip);
    int *num = (int *)(emem + eip + 1);
    int lower, higher;
    int adr, topReg, botReg;
    
    lower = new->low;
    higher = new->high;
    
    if ((lower >= 0 && lower <= 7) && (higher >= 0 && higher <= 7)) {
        topReg = reg[higher];
        botReg = reg[lower];
        adr = topReg + *num;
        int *memArrAddress = (int *)(adr + emem);
        *memArrAddress = botReg;
    }
    else {
        fprintf(stderr,"Not valid register.\n");
        exit(0);
    }
    eip += 5;
	return;
}

/*Performs a memory to registry move*/
void mrmovl() {
    eip++;
    bitfield *new = (bitfield *)(emem + eip);
    int *num = (int *)(emem + eip + 1);
    int lower, higher;
    
    lower = new->low;
    higher = new->high;
    
    int adr, topReg;
    if ((lower >= 0 && lower <= 7) && (higher >= 0 && higher <= 7)) {
        topReg = reg[higher];
        adr = topReg + *num;
        int *memArrAddress = (int *)(adr + emem);
        reg[lower] = *memArrAddress;
    }
    else {
        fprintf(stderr,"Not valid register.\n");
        exit(0);
    }
    eip += 5;
	return;
}

/*Perform an operation between two registers,
 * can be addition, subtraction, multiplication
 * bitwise and or exclusive or */
 void op1() {
     ZF = 0; SF = 0; OF = 0;
     eip++;
     bitfield *new = (bitfield *)(emem + eip);
     int lower, higher;
     higher = new->high;
     lower = new->low;
     int topReg = 0, botReg = 0;
     int count = 0, minus = 0;
     switch (higher) {
         case 0:
             if ((lower >= 0 && lower <= 7) && (higher >= 0 && higher <= 7)) {
                 topReg = reg[higher]; // value of higher register
                 botReg = reg[lower]; // value of lower register
                 count = topReg + botReg;
                 reg[higher] = count;
                 if (count == 0)
                     ZF = 1; // otherwise don't change
                 if ((botReg < 0 && topReg < 0 && count > 0) || (botReg > 0 && topReg > 0 && count < 0))
                     OF = 1; // otherwise don't change
                 if (count < 0)
                     SF = 1; // otherwise don't change
                 
                 eip++;

             }
             else {
                 fprintf(stderr,"Not valid register.\n");
                 exit(0);
                 break;
             }
             break;
         case 1:
             if ((lower >= 0 && lower <= 7) && (higher >= 0 && higher <= 7)) {
                 topReg = reg[higher]; // value of higher register
                 botReg = reg[lower]; // value of lower register
                 minus = topReg - botReg;
                 reg[higher] = minus;
                 if (minus == 0)
                     ZF = 1;
                 if ((botReg < 0 && topReg < 0 && minus > 0) || (botReg > 0 && topReg > 0 && minus < 0))
                     OF = 1;
                 if(minus < 0)
                     SF = 1;
             }
             break;
         case 2:
             //
             break;
         case 3:
             //
             break;
         case 4:
             //
             break;
             
         default:
             fprintf(stderr,"Not valid register.\n");
             exit(0);
             break;
     }
	return;
 }
 
 /*Will perform various code jumps depending 
  * on which jump is declared and what flags
  * are on/off.*/
void jXX(){
	/**** fill your code here ****/



	return;
}

/* Calls the function at the destination*/
void call() {
	/**** fill your code here ****/



	return;
}

/*returns from a function call*/
void ret() {
	/**** fill your code here ****/



	return;
}

/*Pushes onto the top of the stack*/
void pushl() {
	/**** fill your code here ****/



	return;
}

/*pop off the top of the stack*/
void popl() {
	/**** fill your code here ****/



	return;	
}

/*Reads in character input*/
void readB() {
	eip++;
	bitfield* tempByte = (bitfield*)(emem+eip);//get the first byte
	int* num = (int*)(emem + eip + 1);//get the displacement by type casting the pointer to (int*) so that the computer would interpret the next 4 bytes as an integer
	char input;
	OF = 0;
	SF = 0;
	if (scanf("%c",&input) == EOF) { 
		ZF = 1;	//set ZF = 1 when end of file symbol is read in
	}
	else {
		ZF = 0;
		if ((tempByte -> low >= 0 && tempByte -> low <= 7) && (tempByte -> high == 15)) {//if the first half of byte1 is 0~7(which is a valid register), and the second half of byte2 is 'F', it is considered as a valid readB instruction
			emem[reg[tempByte -> low]+ *num] = input;
		}
		else {
			fprintf(stderr,"Read error.\n");
			exit(0);
		}
	}
	eip+=5; //eip should be incremented by 5 to point to the start of next instruction
	return;
}

/*Reads in long*/
void readL() {
	/**** fill in your code here ****/



	return;
}

/*Writes a char to terminal*/
void writeB() {
	eip++; //go to byte1
	bitfield* tempByte = (bitfield*)(emem+eip); //get byte1
	int* num = (int*)(emem + eip + 1); //get byte2-5 and interpret it as integer
	char charCurrByte;
	if ((tempByte -> high == 15) && (tempByte -> low >= 0 && tempByte -> low <= 7)) { //check if byte1 is valid
		charCurrByte = emem[reg[tempByte -> low]+*num]; //set charCurrByte to be the character prepared to be printed out
	}
	else {
		fprintf(stderr,"Write error.\n");
		exit(0);
	}
	printf("%c",charCurrByte);
	eip+=5; //increment eip by 5 to point to the next location
	return;	
}

/*Writes a long to terminal*/
void writeL() {
	/****fill your code here****/



	return;
}

/*Will scan the instrunction string
 * and begin running the operations 
 * as defined. */
void runProgram() {
	bitfield* tempByte;
	while (CPUState == AOK) {
		tempByte = (bitfield*)(emem+eip);
		if (tempByte -> low == 0) {					//nope code
			if (tempByte -> high == 0) {
				eip++;
				continue;
			}			
		}
		else if (tempByte -> low == 1) {				//halt code
			if (tempByte -> high == 0) {
				CPUState = HLT;
				continue;
			}
		}
		else if (tempByte -> low == 2)	{			//registry move
			if (tempByte -> high == 0) {
				rrmovl();
				continue;
			}				
		}
		else if (tempByte -> low == 3)	{			//immediate to registry move
			if (tempByte -> high == 0) {
				irmovl();
				continue;
			}
		}
		else if (tempByte -> low == 4)	{			//registry to memory move
			if (tempByte -> high == 0) {
				rmmovl();
				continue;
			}
		}		
		else if (tempByte -> low == 5)	{			//memory to registry move
			if (tempByte -> high == 0) {
				mrmovl();
				continue;
			}
		}
		else if (tempByte -> low == 6)	{			//operation
			if (tempByte -> high >= 0 && tempByte -> high <= 4) {
				op1();
				continue;
			}
		}
		else if (tempByte -> low == 7)	{			//jump
			if (tempByte -> high >= 0 && tempByte -> high <= 6) {
				jXX();
				continue;
			}
		}
		else if (tempByte -> low == 8)	{			//call
			if (tempByte -> high == 0) {
				call();
				continue;
			}
		}
		else if (tempByte -> low == 9)	{			//return
			if (tempByte -> high == 0) {
				ret();
				continue;
			}
		}
		else if (tempByte -> low == 10)	{			//pushl
			if (tempByte -> high == 0) {
				pushl();
				continue;			
			}
		}
		else if (tempByte -> low == 11)	{			//popl
			if (tempByte -> high == 0) {
				popl();
				continue;
			}
		}
		else if (tempByte -> low == 12)	{			//read
			if (tempByte -> high == 0 ) {
				readB();
				continue;
			}
			else if (tempByte -> high == 1) {
				readL();
				continue;
			}
		}
		else if (tempByte -> low == 13)	{			//write
			if (tempByte -> high == 0) {
				writeB();
				continue;
			}
			else if (tempByte -> high == 1) {
				writeL();
				continue;
			}
		}
		fprintf(stderr,"Invalid instruction found.\n");
		exit(0);
	}
	return;
}

/*Takes in the .byte string and input
 * the byte into the memmory location in 
 * array. */ 
void byteToMem(char* memAddr, char* val) {
	int addrStart = strtol(memAddr,NULL,16);
	if (addrStart < 0 || addrStart > size) {
		fprintf(stderr,"OUT OF BOUNDS AT LINE %d.\n",line);
		exit(0);
	}
	int addVal = (int)strtol(val,NULL,16);
	emem[addrStart] = (char)addVal;
	return;
}

/* Takes in the .text string and inputs the 
 * instruction set 2 characters per "byte" of
 * array. Use of bitfield casting allows for
 * entry into a char array. */
void textToMem(char* memAddr, char* val) {
	bitfield* tempByte;
	int addrStart = strtol(memAddr,NULL,16), i = 0;
	if (addrStart < 0 || addrStart > size) {
		fprintf(stderr,"OUT OF BOUNDS AT LINE %d.\n",line);
		exit(0);
	}
	if (strlen(val)%2 != 0) {
		fprintf(stderr,"The length of the instruction set is odd, code missing in instruction set.\n");
		exit(0);
	}
	eip = addrStart;
	for (; i < strlen(val); i+=2, addrStart++) {
		tempByte = (bitfield*) emem+addrStart;
		char* upper = (char*)malloc(1);
		char* lower = (char*)malloc(1);
		strncpy(upper,val+i,1);
		strncpy(lower,val+i+1,1);
		tempByte -> high = strtol(lower,NULL,16);
		tempByte -> low = strtol(upper,NULL,16);		
	}	
	text = 1;	
	return;
}

/*Stores an inputted .long directive 
 * into one array location. */
void longToMem(char* memAddr, char* val) {
	bitfield* tempByte;
	int addrStart = strtol(memAddr,NULL,16);
	if (addrStart < 0 || addrStart > size) {
		fprintf(stderr,"OUT OF BOUNDS AT LINE %d.\n",line);
		exit(0);
	}
	int Value = (int)strtol(val,NULL,10);
	int* num = (int*)(emem + addrStart);
	*num = Value;			
}

/*Stores an inputted .string directive one char
 * into one array location. */
void stringToMem(char* memAddr) {
	char* val = strtok(NULL,delim);
	if (val == NULL) {
		fprintf(stderr,"No string given after .string directive.\n");
		exit(0);
	}	
	int addrStart = strtol(memAddr,NULL,16);	
	if (*val == '"') {
		val++;
	}
	else {
		fprintf(stderr,"String must be contained within double quotes.\n");
		exit(0);
	}	
	while (val != NULL) {
		while (*val != '\0') {
			emem[addrStart] = *val;
			addrStart++;
			val++;
		}
		emem[addrStart] = ' ';
		addrStart++;
		val = strtok(NULL,delim);
	}
	addrStart-=2;
	if (emem[addrStart] != '"') {
		fprintf(stderr,"String must be contained within double quotes.\n");
		exit(0);
	}
	else {
		emem[addrStart] = '\0';
	}	
	return;
}

/*Starts reading in file and storing based on string inputed*/
void storeToMemory(char* temp) {
	if (temp == NULL ) {
		return;
	}
	char* stringTemp = temp;
	char* typeIn = strtok(temp,delim);
	char* memAddr = strtok(NULL,delim);	
	if (typeIn != NULL && memAddr == NULL) {
		fprintf(stderr,"Error at line: %d\n",line);
		exit(0);
	}	
	if (typeIn == NULL && memAddr == NULL) {
		return;
	}	
	if (strcmp(typeIn, ".string")==0) {//handle the .string line
		stringToMem(memAddr);
		line++;
		return;
	}
	char* val = strtok(NULL,delim);
	if (val == NULL) {
		fprintf(stderr,"Error at line: %d\n",line);
		exit(0);
	}	
	if (strcmp(typeIn,".text")==0) {//handle the .text line
		textToMem(memAddr,val);
	}
	else if (strcmp(typeIn,".byte")==0) {//handle the .byte line
		byteToMem(memAddr,val);
	}
	else if(strcmp(typeIn, ".long")==0) {//handle the .long line
		longToMem(memAddr,val);
	}
	else {
		fprintf(stderr,"Unknown directive encountered. Line: %d\n",line);
		exit(0);
	}	
	line++;
	return;
}

/* Set array size for memory */
void setMemory(char* temp) {
	//check the first line's content
	//it should begin with .size with a valid number
	if (strcmp(temp,"\n") == 0) {
		fprintf(stderr,"The first line should contain the memmory size.\n");
		exit(0);
	}	
	char* token;
	token = strtok(temp,delim);	
	if (strcmp(token,".size") != 0) {
		fprintf(stderr,"The first line should contain the memmory size.\n");
		exit(0);
	}
	token = strtok(NULL,delim);
	if (token == NULL) {
		fprintf(stderr,"No size given after size directive.\n");
		exit(0);
	}
	if (sscanf(token,"%x",&size) <= 0) {
		fprintf(stderr,"A number is needed after size directive.\n");
		exit(0);
	}
	emem = (char*)malloc(sizeof(char)*size);
	return;
}

/* Main takes in a file path and runs the code
 * contained within the file. User can enter in 
 * "-h" flag for help. */
int main(int argc, char** argv) {
	if (argc < 2 || argc > 3) {
		fprintf(stderr,"Arguments given are not in valid form\n");
		return -1;
	}
	
	if (strcmp(argv[1], "-h") == 0) {
		printf("Enter in \"y86emul\" followed by a space and then the filename\n");
		return 0;
	}
		
	FILE* fp;
	char strTemp[5000];	
	fp = fopen(argv[1], "r");	
	if (fp == NULL) {
		printf("The filename could not be found\n");
		exit(1);
	}		
	if (fgets(strTemp,5000,fp)==NULL) {
		printf("File is empty.");
		exit(1);
	}
	setMemory(strTemp);//initialize the emulated memory to have the required size	
	while (fgets(strTemp,5000,fp) != NULL) {
		storeToMemory(strTemp);
	}//read in the file content and fill in the emulated memory
	if (text == 0) {
		fprintf(stderr,".text directive not found.\n");
		exit(0);
	}
	runProgram();
	printf("\n");
	return 0;	
}
