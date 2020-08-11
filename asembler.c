#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "asembler.h"

/*Struct for the symbol table every struct present a line*/
typedef struct
{
    int address;
    char symbol[MAX_INPUT];
} symbolLine;

/*Struct for the output table every struct present a line*/
typedef struct
{
    int address;
    unsigned int code;
} outputLine;

/*Global variables for the tables*/
symbolLine *symbolP;
outputLine *outputp;

/*Global variables for the length*/
int symbolTableLen, outputTabelLen;

/*Build the symbol table with the correct address*/
void SymbolTable()
{
    extern int line;
    extern char *fileName;
    char entFile[FILENAME_MAX];
    char temp[MAX_INPUT];
    int i;
    unsigned int x;
    int addressCount = FIRST_ADDRESS;
    FILE *fent;
    /*Allocate memory*/
    symbolP = calloc(line, sizeof(symbolLine));
    if (!symbolP)
    {
        printf("Memory allocation failed");
        exit(0);
    }
    /*Handle entry files*/
    strcpy(entFile, fileName);
    strcat(entFile, ".en");
    fent = fopen(entFile, "r");
    if (fent)
    {
        fclose(fent);
        strcat(entFile, "t");
        fent = fopen(entFile, "w+");
    }
    moveToSecondOperand();
    /*We will run all over the input table and take the labels with the correct values*/
    for (i = 1, symbolTableLen = 1; i <= line; i++)
    {
        strcpy(temp, getData(i, LABEL));
        if (strlen(temp))
        { 
            strcpy(symbolP[symbolTableLen].symbol, temp);
            symbolP[symbolTableLen].address = addressCount;

            if (checkEntry(temp))
            {
                x = (unsigned int)addressCount;
                fprintf(fent, "%s\t%07d\n", temp, x);
            }
            symbolTableLen++;
        }

        addressCount += codeLines(i);
    }

    /*address counter should be the length of the output*/
    outputTabelLen = addressCount - FIRST_ADDRESS;
    fclose(fent);
}

/*Create table with the output data*/
void secondPass()
{
    /*variables*/
    extern int line;
    extern char *fileName;
    char *token;
    int i = 0, j = 0, x = 0, y = 0, len = 0, op1 = 0, op2 = 0, group = 0, opcode = 0, funct = 0, IC, DC, temp = 0;
    char operand1[MAX_INPUT];
    char operand2[MAX_INPUT];
    char command[MAX_INPUT];
    char oFile[FILENAME_MAX];
    char extFile[FILENAME_MAX];
    FILE *fp;
    FILE *fext;

    /*copy the file names and add the correct extensions*/
    strcpy(oFile, fileName);
    strcpy(extFile, fileName);
    strcat(oFile, ".ob");
    strcat(extFile, ".ex");

    /*Open the external file*/
    fext = fopen(extFile, "r");
    if (fext)
    {
        fclose(fext);
        strcat(extFile, "t");
        fext = fopen(extFile, "w+");
    }

    /*Allocate memory for the output*/
    outputp = calloc(outputTabelLen, sizeof(outputLine));
    if (!outputp)
    {
        printf("Memory allocation failed");
        exit(0);
    }
    /*Insert code address to the table*/
    for (i = 0; i < outputTabelLen; i++)
    {
        outputp[i].address = i + FIRST_ADDRESS;
    }
    /*insert the code*/
    for (i = 1; i <= line; i++)
    {
        /*Copy the input in the correct line*/
        strcpy(operand1, getData(i, OPERAND1));
        strcpy(operand2, getData(i, OPERAND2));
        strcpy(command, getData(i, COMMAND));
        if (command[0] != '.')
        {
            /*Check the command and use the right code for it*/
            if (strlen(getData(i, OPERAND2)))
            {
                if (strlen(getData(i, OPERAND1)))
                {
                    group = GROUP1;
                    if (!strcmp(command, "mov"))
                    {
                        opcode = 0;
                        funct = 0;
                    }
                    else if (!strcmp(command, "cmp"))
                    {
                        opcode = 1;
                        funct = 0;
                    }
                    else if (!strcmp(command, "add"))
                    {
                        opcode = 2;
                        funct = 1;
                    }
                    else if (!strcmp(command, "sub"))
                    {
                        opcode = 2;
                        funct = 2;
                    }
                    else if (!strcmp(command, "lea"))
                    {
                        opcode = 4;
                        funct = 0;
                    }
                }
                else
                {
                    group = GROUP2;
                    if (!strcmp(command, "clr"))
                    {
                        opcode = 1;
                        funct = 5;
                    }
                    else if (!strcmp(command, "not"))
                    {
                        opcode = 5;
                        funct = 2;
                    }
                    else if (!strcmp(command, "inc"))
                    {
                        opcode = 5;
                        funct = 3;
                    }
                    else if (!strcmp(command, "dec"))
                    {
                        opcode = 5;
                        funct = 4;
                    }
                    else if (!strcmp(command, "jmp"))
                    {
                        opcode = 9;
                        funct = 1;
                    }
                    else if (!strcmp(command, "bne"))
                    {
                        opcode = 9;
                        funct = 2;
                    }
                    else if (!strcmp(command, "jsr"))
                    {
                        opcode = 9;
                        funct = 3;
                    }
                    else if (!strcmp(command, "red"))
                    {
                        opcode = 12;
                        funct = 0;
                    }
                    else if (!strcmp(command, "prn"))
                    {
                        opcode = 13;
                        funct = 0;
                    }
                }
            }
            else
            {
                group = GROUP3;
                if (!strcmp(command, "rts"))
                {
                    opcode = 14;
                    funct = 0;
                }
                else if (!strcmp(command, "stop"))
                {
                    opcode = 15;
                    funct = 0;
                }
            }
            /*Check status for operands*/
            /*operand1*/
            op1 = IS_EMPTY;
            if (group == GROUP1 && checkNumeric(operand1, i))
                op1 = IS_NUMERIC;
            else if (group == GROUP1 && checkLabel(operand1))
                op1 = IS_LABEL;
            else if (group == GROUP1 && checkRegister(operand1))
                op1 = IS_REGISTER;

            /*operand2*/
            op2 = IS_EMPTY;
            if (group != GROUP3 && checkNumeric(operand2, i))
                op2 = IS_NUMERIC;
            else if (group != GROUP3 && checkLabel(operand2))
                op2 = IS_LABEL;
            else if (group == GROUP2 && checkAmpersand(operand2,command))
                op2 = IS_AMPERSAND;
            else if (group != GROUP3 && checkRegister(operand2))
                op2 = IS_REGISTER;
            /*Get how many code line to the correct line input*/
            len = codeLines(i);

            /*Run inside the output table and insert correct code for every line in the input (for every input line have 1-3 different code line) */
            for (j = 0; j < len; x++, j++)
            {
                /*For the first code line to the input make the correct code by the project rules*/
                if (j == FIRST_LINE)
                {
                    temp = opcode;
                    temp <<= SOURCE_ADDRESS; /* 2 bits for source address */
                    temp += op1;
                    temp <<= SOURCE_OPERAND;  /* 3 bits for source operand */
                    if (op1 == IS_REGISTER)
                        temp += atoi(&operand1[1]);
                    temp <<= DESTINATION_ADDRESS;  /* 2 bits for destiantion address */
                    temp += op2;
                    temp <<= DESTINATION_OPERAND; /* 3 bits for destiantion operand */
                    if (op2 == IS_REGISTER)
                        temp += atoi(&operand2[1]);
                    temp <<= FUNCT;  /* 5 bits for funct */
                    temp += funct;
                    temp <<= ARE;  /* 3 bits for ARE */
                    temp += ACTIVATE_A;
                    outputp[x].code = temp;
                }

                /*Code according to command groups*/

                else if (group == GROUP1 && j > FIRST_LINE)
                {
                    if (j == SECOND_LINE)
                    {
                        if (len == 3 || op2 == IS_REGISTER)
                        {
                            if (op1 == IS_NUMERIC)
                            {
                                temp = atoi(&operand1[1]);
                                if (temp < 0)
                                    isNegative((unsigned int *)&temp);

                                outputp[x].code = (unsigned int)temp;
                                outputp[x].code <<= ARE;  /* 3 bits for ARE */
                                outputp[x].code += ACTIVATE_A;
                            }
                            /*Get the label address from the symbol table*/
                            else if (op1 == IS_LABEL)
                            {

                                outputp[x].code = getSymbolAddress(operand1);
                                outputp[x].code <<= ARE;  /* 3 bits for ARE */
                                if (!checkExtern(operand1))
                                    outputp[x].code += ACTIVATE_R;

                                else
                                {
                                    outputp[x].code += ACTIVATE_E;
                                    temp = outputp[x].address;
                                    fprintf(fext, "%s\t%07d\n", operand1, temp);
                                }
                            }
                        }

                        else
                        {
                        	
                            if (op2 == IS_NUMERIC)
                            {
                                temp = atoi(&operand2[1]);
                                if (temp < 0)
                                    isNegative((unsigned int *)&temp);

                                outputp[x].code = (unsigned int)temp;
                                outputp[x].code <<= ARE;  /* 3 bits for ARE */
                                outputp[x].code += ACTIVATE_A;
                            }
                            /*Get the label address from the symbol table*/
                            else if (op2 == IS_LABEL)
                            {

                                outputp[x].code = getSymbolAddress(operand2);
                                outputp[x].code <<= ARE;  /* 3 bits for ARE */
                                if (!checkExtern(operand2))
                                    outputp[x].code += ACTIVATE_R;

                                else
                                {
                                    outputp[x].code += ACTIVATE_E;
                                    temp = outputp[x].address;
                                    fprintf(fext, "%s\t%07d\n", operand2, temp);
                                }
                            }
                        }
                    }
                    else if (j == THIRD_LINE)
                    {
                        /*Get the number and 3 A,R,E bits*/
                        if (op2 == IS_NUMERIC)
                        {

                            temp = atoi(&operand2[1]);
                            if (temp < 0)
                                isNegative((unsigned int *)&temp);

                            outputp[x].code = (unsigned int)temp;
                            outputp[x].code <<= ARE;  /* 3 bits for ARE */
                            outputp[x].code += ACTIVATE_A;
                        }
                        /*Get the label address from the symbol table*/
                        else if (op2 == IS_LABEL)
                        {
                            outputp[x].code = getSymbolAddress(operand2);
                            outputp[x].code <<= ARE;  /* 3 bits for ARE */
                            if (!checkExtern(operand2))
                                outputp[x].code += ACTIVATE_R;

                            else
                            {
                                outputp[x].code += ACTIVATE_E;
                                temp = outputp[x].address;
                                fprintf(fext, "%s\t%07d\n", operand2, temp);
                            }
                        }
                    }
                }

                else if (group == GROUP2 && j > FIRST_LINE)
                {
                	if (op2 == IS_AMPERSAND)
                	{
                		token = strtok(operand2,"&");
                		temp = getSymbolAddress(token) - outputp[x].address+1;
                		                        if (temp < 0)
                            isNegative((unsigned int *)&temp);

                        outputp[x].code = (unsigned int)temp;
                        outputp[x].code <<= ARE;  /* 3 bits for ARE */
                        outputp[x].code += ACTIVATE_A;
                	}
                	
                    if (op2 == IS_NUMERIC)
                    {
                        temp = atoi(&operand2[1]);
                        if (temp < 0)
                            isNegative((unsigned int *)&temp);

                        outputp[x].code = (unsigned int)temp;
                        outputp[x].code <<= ARE;  /* 3 bits for ARE */
                        outputp[x].code += ACTIVATE_A;
                    }
                    /*Get the label address from the symbol table*/
                    if (op2 == IS_LABEL)
                    {

                        outputp[x].code = getSymbolAddress(operand2);
                        outputp[x].code <<= ARE;  /* 3 bits for ARE */
                        if (!checkExtern(operand2))
                            outputp[x].code += ACTIVATE_R;
                        else
                        {
                            outputp[x].code += ACTIVATE_E;
                            temp = outputp[x].address;
                            fprintf(fext, "%s\t%07d\n", operand2, temp);
                        }
                    }
                }
            }
        }
    }
    /*Finish the output with the data and string*/
    IC = outputTabelLen - x;
    DC = x;
    for (i = 1; i <= line; i++)
    {
        strcpy(command, getData(i, COMMAND));
        if (command[0] == '.')
        {
            strcpy(operand1, getData(i, OPERAND1));
            strcpy(operand2, getData(i, OPERAND2));
            /*For every data or string get how many code line they required, and insert it*/
            len = codeLines(i);
            for (y = 0, j = 0; j < len; j++, x++)
            {
                /*String should get the asci value for the letter*/
                if (!strcmp(command, ".string"))
                {
                    if (j < len - 1)
                        outputp[x].code = operand2[j + 1];
                    else
                        outputp[x].code = 0;
                }
                /*Data should get the number*/
                else if (!strcmp(command, ".data"))
                {
                    if (j == 0)
                    {
                        temp = atoi(operand1);
                        if (temp < 0)
                            negativeWord((unsigned int *)&temp);
                        outputp[x].code = (unsigned int)temp;
                    }
                    else
                    {
                        temp = atoi(&operand2[y]);
                        if (temp < 0)
                            negativeWord((unsigned int *)&temp);
                        outputp[x].code = (unsigned int)temp;
                        do
                        {
                            y++;
                        } while (y < MAX_INPUT && operand2[y] != ',' && operand2[y] != '\0');

                        if (operand2[y] == ',')
                            y++;
                    }
                }
            }
        }
    }

    /*Create the object file*/
    fp = fopen(oFile, "w+");
    fprintf(fp, "\t%i %i", DC, IC);
    fputc('\n', fp);
    for (i = 0; i < outputTabelLen; i++)
        fprintf(fp, "%06d\t%06x\n", outputp[i].address, outputp[i].code);

    fclose(fp);
    fclose(fext);
}

/*Returns the symbol address*/
int getSymbolAddress(char symbol[])
{
    int i;
    for (i = 0; i < symbolTableLen; i++)
    {
        if (!strcmp(symbol, symbolP[i].symbol))
            return symbolP[i].address;
    }
    return 0;
}

/*Checks the line on the input and return how many code lines we need to have for output*/
int codeLines(int i)
{
    extern int line;
    int counter = 1;
    int j = 0;
    int len;
    char operand1[MAX_INPUT];
    char operand2[MAX_INPUT];
    char command[MAX_INPUT];
    strcpy(operand1, getData(i, OPERAND1));
    strcpy(operand2, getData(i, OPERAND2));
    strcpy(command, getData(i, COMMAND));
    if (i < 1 || i > line)
        return 0;
    if (!strcmp(command, ".data"))
    {
        len = strlen(operand2);
        for (j = 0; j < len; j++)
        {
            if (isdigit(operand2[j]))
            {
                counter++;
                while (isdigit(operand2[j]) && j < len)
                    j++;
            }
        }
        return counter;
    }
    else if (!strcmp(command, ".string"))
    {
        len = strlen(operand2);
        /*the length minus the quotes*/
        return (counter + len - 2);
    }
    else
    {
        if (strlen(operand1) && !(checkRegister(operand1)))
        {
            counter++;
        }
        if (strlen(operand2) && !(checkRegister(operand2)))
        {
            counter++;
        }
    }
    return counter;
}

/*The func give us the binary value in integer from 21 bits negative number*/
void isNegative(unsigned int *num)
{
    unsigned int temp;
    int i, j;
    int bits[TWENTEE_ONE_BITS];
    double x;
    temp = *num;
    *num = 0;
    j = 1;
    for (i = 0; i < TWENTEE_ONE_BITS; i++)
    {
        bits[i] = (temp & j);
        temp >>= 1;
    }

    for (x = 0, i = 0; i < TWENTEE_ONE_BITS; i++)
    {
        x += bits[i] * (pow(2, i));
    }

    *num = (unsigned int)x;
}
/*The func give us the binary value in integer from 24 bits negative number*/
void negativeWord(unsigned int *num)
{
    unsigned int temp;
    int i, j;
    int bits[FULL_BITS - 1];
    double x;
    temp = *num;
    *num = 0;
    j = 1;
    for (i = 0; i < FULL_BITS; i++)
    {
        bits[i] = (temp & j);
        temp >>= 1;
    }

    for (x = 0, i = 0; i < FULL_BITS; i++)
    {
        x += bits[i] * (pow(2, i));
    }

    *num = (unsigned int)x;
}

/*All the commend in group 2 should be just with operand destination */
void moveToSecondOperand()
{
    extern int line;
    int i;
    char temp1[MAX_INPUT];
    int len;
    for (i = 1; i <= line; i++)
    {
        strcpy(temp1, getData(i, OPERAND2));
        if (strcmp(getData(i, COMMAND), ".data"))
        {
            (len = strlen(temp1));
            if (!len)
            {
                setData(i, OPERAND2, getData(i, OPERAND1));
                setData(i, OPERAND1, temp1);
            }
            else
            {
                if (!strlen(temp1))
                {
                    setData(i, OPERAND2, getData(i, OPERAND1));
                    setData(i, OPERAND1, temp1);
                }
            }
        }
    }
}

/*Free the memory tables*/
void freeTables()
{
    free(symbolP);
    free(outputp);
}
