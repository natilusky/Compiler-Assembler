#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asembler.h"

char* fileName;
int main(int argc, char* argv[])
{
    char inputFile[FILENAME_MAX];
    char temp[FILENAME_MAX];
    int flag = TRUE;
    int i;
    FILE *file;
    if(argc==1)
    {
        printf("No files to compile\n");
        return 0;
    }
/*Loop through file names.*/
    for(i=1; i<argc; i++)
    {
        fileName = argv[i];
        strcpy(inputFile,argv[i]);
        strcat(inputFile,".as");
        file = fopen (inputFile, "r");
        if (!file)
        {
            printf("File %s does not exist\n",fileName);
            return 0;
        }          
        
/*Collect the input from the file and make first validation*/
        flag = firstTable(file);
/*Check that all labels, commands and operands are valid*/
        if(!validLabels()||!validAllCommands())
            flag = FALSE;
/*If flag is TRUE, can go on and parser the input*/
        if(flag)
        {
            SymbolTable();
            secondPass();
            freeTables();
        /*Delete temp files*/
            strcpy(temp,fileName);
            strcat(temp,".ex");
            remove(temp);
            strcpy(temp,fileName);
            strcat(temp,".en");
            remove(temp);
            printf("compiled %s.\n",fileName);
        }
/*if Flag is false, print error*/
        else
        {
        /*Print error*/
            printf("Cannot compile %s.\n",fileName);
        /*Delete temp file*/
            strcpy(temp,fileName);
            strcat(temp,".ex");
            remove(temp);
            strcpy(temp,fileName);
            strcat(temp,".en");
            remove(temp);
        }
/*Free input table*/
        freeMatrix();
        fclose(file);
    } /*end for*/
    return 0;
}
