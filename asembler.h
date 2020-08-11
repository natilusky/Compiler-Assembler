/*Include guard*/
#ifndef ASEMBLER_H_
#define ASEMBLER_H_
#endif

/*Bits of word*/
#define TWENTEE_ONE_BITS 21
#define FULL_BITS 24
#define SOURCE_ADDRESS 2
#define SOURCE_OPERAND 3
#define DESTINATION_ADDRESS 2
#define DESTINATION_OPERAND 3
#define FUNCT 5
#define ARE 3

/*should present status*/
#define LABEL 1
#define COMMAND 2
#define OPERAND1 3
#define OPERAND2 4

/*Limits input*/
#define MAX_INPUT 80 
#define MAX_LABEL 30
#define FIRST_ADDRESS 100

/*Command groups*/
#define GROUP1 2
#define GROUP2 1
#define GROUP3 0

/*For every code line have between 1-3 different code lines */
#define FIRST_LINE 0
#define SECOND_LINE 1
#define THIRD_LINE 2

/*For activate A,R,E */
#define ACTIVATE_A 4
#define ACTIVATE_R 2
#define ACTIVATE_E 1

/*Operands binary values*/
#define IS_EMPTY 0
#define IS_NUMERIC 0
#define IS_LABEL 1
#define IS_AMPERSAND 2
#define IS_REGISTER 3

#define COMMA 0
#define NUMBER 1

/*For Boolean variable*/
#define TRUE 1
#define FALSE 0

/*Limits values*/
#define MIN_DATA_INT -8388608
#define MAX_DATA_INT 8388607
#define MIN_INT -1048576  
#define MAX_INT 1048575   




/*\\\\\\\\\\\\\\\Function prototype\\\\\\\\\\\\\*/

/*Read line by line in the input and sort the data into a struct*/
int firstTable(FILE *file);
/*Count data lines in the input file*/
int numberOfLine(FILE *file);
/*Print a line from the matrix after the input was sorted*/
void printLine(int i);
/*Print all the lines after the input was sorted*/
void printMatrix();
/*Get a specific value in line index, parameter should be between 0-4, see constants for values*/
char* getData(int index, int param);
/*Free matrix memory*/
void freeMatrix();
/*Get the line number in the input file [not ignoring ext/ent lines like the matrix does]*/
int getInputLine(int i);
/*Change a specific value in the matrix*/
void setData (int index, int param,char data []);
/*Validates a single line's command and operands*/
int validCommand (char com[], int i);
/*Checks all commands and operands*/
int validAllCommands ();
/*Checks for duplicate labels*/
int validLabels ();
/*Checks if the operand is a legal label*/
int checkLabel(char operand[]);
/*Checks that the numeric value is valid, must begin with # and follows by a signed number*/
int checkNumeric(char operand[], int i);
/*check if is ampersand*/
int checkAmpersand(char oper[],char cmd[]);
/*Checks the the value is a legal register*/
int checkRegister(char oper[]);
/*Checks the legality of .data numeric values*/
int checkData(int i);
/*Checks that a string is legal, can't be longer than 31 chars*/
int checkString(int i);
/*Is the data in the external file?*/
int checkExtern(char param[]);
/*Is the data in the entry file?*/
int checkEntry(char param[]);
/*Build the symbol table*/
void SymbolTable();
/*Free table memory*/
void freeTables();
/*Build the Output data table into a struct*/
void secondPass();
/*Move all the single operand command from the source column to the destination column for easy parsing*/
void moveToSecondOperand();
/*Check if the nubmer is Negative on 24 bits*/
void negativeWord (unsigned int *num);
/*Checks negative in decimal*/
void isNegative (unsigned int *num);
/*Get the symbol address*/
int getSymbolAddress(char symbol[]);
/*Get the code lines*/
int codeLines(int i);


