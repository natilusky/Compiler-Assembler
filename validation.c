#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "asembler.h"

/*Check command and its operands in line i*/
int validCommand (char com[], int i)
{
    int flag = FALSE;
    char *token;
    /*Commands mov, add and sub
      Must have two operands
      Operand1 must be either Label, Register or legal numeric value
    */
    if ( !strcmp("mov", com) || !strcmp("add", com) || !strcmp("sub", com) )
    {
        /*Do we have two operands?*/
        if (strlen(getData(i, OPERAND1)) != 0 && strlen(getData(i, OPERAND2)) != 0)
        {
            /*Is Operand1 legal?*/
            if(checkRegister(getData(i, OPERAND1)) || checkLabel(getData(i, OPERAND1)) || checkNumeric(getData(i, OPERAND1), i))
            {
                /*Is Operand2 legal?*/
                if (checkRegister(getData(i, OPERAND2)) || checkLabel(getData(i, OPERAND2)))
                {
                    flag = TRUE;
                }
                else
                {
                    flag = FALSE;
                }

            }
            else
            {
                flag = FALSE;
            }
        }

        else
        {
            flag = FALSE;
        }
    }


    /*Command cmp
      Must have two operands
      Operands 1 and 2 must be either Label, Register or legal numeric value
    */
    if ( !strcmp("cmp", com))
    {
        /*Do we have two operands?*/
        if (strlen(getData(i, OPERAND1)) != 0 && strlen(getData(i, OPERAND2)) != 0)
        {
            /*Is Operand1 legal?*/
            if(checkRegister(getData(i, OPERAND1)) || checkLabel(getData(i, OPERAND1)) || checkNumeric(getData(i, OPERAND1), i))
            {
                /*Is Operand2 legal?*/
                if (checkRegister(getData(i, OPERAND1)) || checkLabel(getData(i, OPERAND1)) || checkNumeric(getData(i, OPERAND1), i))
                {
                    flag = TRUE;
                }
                else
                {
                    flag = FALSE;
                }

            }
            else
            {
                flag = FALSE;
            }
        }

        else
        {
            flag = FALSE;
        }
    }

    /*Command lea
      Must have two operands
      Operand 1 must be a label
      Operand 2 must be either register or label
    */
    if (  !strcmp("lea", com) )
    {
        /*Do we have two operands?*/
        if (strlen(getData(i, OPERAND1)) != 0 && strlen(getData(i, OPERAND2)) != 0)
        {
            /*Is Operand1 legal?*/
            if(checkLabel(getData(i, OPERAND1)))
            {
                /*Is Operand2 legal?*/
                if (checkRegister(getData(i, OPERAND1)) || checkLabel(getData(i, OPERAND1)))
                {
                    flag = TRUE;
                }
                else
                {
                    flag = FALSE;
                }

            }
            else
            {
                flag = FALSE;
            }
        }

        else
        {
            flag = FALSE;
        }
    }
    /*Command not
      Must have one operand
      Operands 1 must be  register
    */
    if (  !strcmp("not", com) )
    {
        /*Do we have one operands?*/
        if (strlen(getData(i, OPERAND1)) != 0 && strlen(getData(i, OPERAND2)) == 0)
        {
            /*Is Operand1 legal?*/
            if(checkRegister(getData(i, OPERAND1)))
            {
                flag = TRUE;
            }
            else
            {
                flag = FALSE;
            }
        }

        else
        {
            flag = FALSE;
        }
    }
    /*Commands clr, inc. dec, jmp, bne, red
      Must have one operands
      Operands 1 must be either Register or Label
    */
    if(  !strcmp("clr", com) ||   !strcmp("inc", com) || !strcmp("dec", com) ||
         !strcmp("jmp", com) ||  !strcmp("bne", com) ||  !strcmp("red", com) )
    {
        /*Do we have one operands?*/
        if (strlen(getData(i, OPERAND1)) != 0 && strlen(getData(i, OPERAND2)) == 0)
        {
            /*Is Operand1 legal?*/
            if(checkRegister(getData(i, OPERAND1)) || checkLabel(getData(i, OPERAND1)))
            {	
                flag = TRUE;
            }
            else
            {    
                        if ((!strcmp("jmp", com) || !strcmp("bne", com)) && getData(i, OPERAND1)[0]=='&')
                   {   
                   token = strtok(getData(i, OPERAND1), "&");
                   if (checkLabel(token))
                   	flag = TRUE;
                   }        	    	
		else
                flag = FALSE;
            }
        }

        else
        {
            flag = FALSE;
        }
    }

    /*Command prn
      Must have one operand
      Operands 1 must be either register, label or numeric
    */
    if (  !strcmp("prn", com) )
    {
        /*Do we have one operands?*/
        if (strlen(getData(i, OPERAND1)) != 0 && strlen(getData(i, OPERAND2)) == 0)
        {
            /*Is Operand1 legal?*/
            if(checkRegister(getData(i, OPERAND1)) || checkLabel(getData(i, OPERAND1)) || checkNumeric(getData(i, OPERAND1), i))
            {
                flag = TRUE;
            }
            else
            {
                flag = FALSE;
            }
        }

        else
        {
            flag = FALSE;
        }
    }

    /*Command jsr
      Must have one operand
      Operands 1 must be label
    */
    if (  !strcmp("jsr", com))
    {
        /*Do we have one operands?*/
        if (strlen(getData(i, OPERAND1)) != 0 && strlen(getData(i, OPERAND2)) == 0)
        {
            /*Is Operand1 legal?*/
            if(checkLabel(getData(i, OPERAND1)))
            {
                flag = TRUE;
            }
            else
            {
                flag = FALSE;
            }
        }

        else
        {
            flag = FALSE;
        }
    }
    /*
      Commands rts and stop
      Must have no operands
    */
    if (    !strcmp("rts", com) ||   !strcmp("stop", com) )
    {
        if (strlen(getData(i, OPERAND1)) == 0 && strlen(getData(i, OPERAND2)) == 0)
            flag = TRUE;
        else
        {
            flag = FALSE;
        }
    }

    /*.data tests*/
    if (!strcmp(".data", com))
    {
        if(checkData(i))
            flag = TRUE;
        else
        {
            flag = FALSE;
        }
    }

    /*.string tests*/
    if (!strcmp(".string", com))
    {
        if(checkString(i))
            flag = TRUE;
        else
        {
            flag = FALSE;
        }
    }

    if (flag==FALSE)
        printf("Found wrong command or illegal operands in line %i\n", getInputLine(i));
    return flag;
}

/*Run all over the the matrix with the input and use validCommands to get validation */
int validAllCommands ()
{
    extern int line;
    int i=1;
    int flag = TRUE;
    while (i<=line)
    {
        if(!validCommand(getData(i, COMMAND), i))
            flag=FALSE;
        i++;
    }
    return flag;
}

/*Run all over the input and check for one label*/
int validLabels ()
{
    char temp1 [MAX_LABEL];
    char temp2 [MAX_LABEL];
    extern int line;
    int i=1;
    int j=1;
    int flag = TRUE;
    while (i<=line)
    {
        strcpy(temp1,getData(i,LABEL));
        while (j<=line)
        {
            strcpy(temp2,getData(j,LABEL));
            if (j!=i && strlen(temp1) != 0 && strlen(temp2) != 0)
            {
                if(strcmp(temp1,temp2)==0)
                {
                    flag = FALSE;
                    printf("Found duplicate label in line %i and line %i\n",getInputLine(i), getInputLine(j));
                }


            }

            j++;
        }
        i++;
    }
    return flag;
}

/*Test for existing and legal label*/
int checkLabel(char operand[])
{
    extern int line;
    int i = 1;
    while (i<=line)
    {

        /*Check in the label column*/
        if (strcmp(getData(i, LABEL),operand) == 0)
        {

                return TRUE;


        }

        i++;
    }
    return checkExtern(operand);
}

int checkExtern(char param[])
{
    FILE *fp;
    char c [MAX_INPUT];
    char fileExt[FILENAME_MAX];
    extern char* fileName;
    strcpy(fileExt,fileName);
    strcat(fileExt,".ex");
    fp = fopen(fileExt,"r");
    if(!fp)
        return FALSE;
    fseek(fp,SEEK_SET,0);
    while((fscanf(fp,"%s",c))==1)
    {
        if(!strcmp(param,c))
        {
            fclose(fp);
            return TRUE;
        }

    }
    fclose(fp);
    return FALSE;
}
int checkEntry(char param[])
{
    FILE *fp;
    char c [MAX_INPUT];
    char fileEnt[FILENAME_MAX];
    extern char* fileName;
    strcpy(fileEnt,fileName);
    strcat(fileEnt,".en");
    fp = fopen(fileEnt,"r");
    if(!fp)
        return FALSE;
    fseek(fp,SEEK_SET,0);
    while((fscanf(fp,"%s",c))==1)
    {
        if(!strcmp(param,c))
        {
            fclose(fp);
            return TRUE;
        }

    }
    fclose(fp);
    return FALSE;
}

/*check if is a register*/
int checkRegister(char oper[])
{
    if (!strcmp(oper, "r0") || !strcmp(oper, "r1") ||
            !strcmp(oper, "r2") || !strcmp(oper, "r3") ||
            !strcmp(oper, "r4") || !strcmp(oper, "r5") ||
            !strcmp(oper, "r6") || !strcmp(oper, "r7") )
        return TRUE;
    return FALSE;
}

/*check if is ampersand*/
int checkAmpersand(char oper[],char cmd[])
{
char *token;
if (oper[0] == '&')
	{
	token = strtok(oper,"&");
		if (checkLabel(token))
			if (!strcmp(cmd, "jmp") || !strcmp(cmd, "bne") || !strcmp(cmd, "jsr"))
			        	return TRUE;
	}
    return FALSE;
}

/*check if Operand is a numeric value*/
int checkNumeric(char oper[], int i)
{
    int flag = FALSE;
    int length = strlen(oper);
    int value;
    int j;
    /*NUMBER*/
    if (oper[0] == '#')
    {
        if (length<2)
        {
            printf("Wrong operand %s in line %i\n", oper, getInputLine(i));
        }
        else if (length==2)
        {
            if (isdigit(oper[1]))
                flag = TRUE;
            else
                printf("Wrong operand %s in line %i, # must be followed by a number \n", oper, getInputLine(i));
        }

        else if (length>2)
        {
            if (oper[1]=='-'|| oper[1]=='+' || isdigit(oper[1]))
            {
                flag=TRUE;
                for(j=2; j<length; j++)
                {
                    if (!(isdigit(oper[j])))
                    {
                        flag = FALSE;
                        printf("Wrong value \"%c\" in line %i \n", oper[j], getInputLine(i));
                    }

                }
            }
            else
            {
                printf("Wrong operand %s in line %i, # must be followed by a number \n", oper, getInputLine(i));
            }

            /*Check that the numeric value is in range*/
            value = strtol(oper+1, NULL, 10);

            if (value > MAX_INT || value < MIN_INT)
            {
                printf("Numeric value of operand %s in line %i is out of bounds \n", oper, getInputLine(i));
                flag = FALSE;
            }
        }
    }/*end number*/
    return flag;
}

/*Check if .Data has legal values*/
int checkData(int i)
{
    char str [MAX_LABEL];
    int state = COMMA;
    int len;
    int bigflag =TRUE;
    int flag;
    int j = 0;
    int num;

    strcpy(str, getData(i, OPERAND1));

    len = strlen(str);
    if(len>0)
    {
        while(j<len)
        {
            if(isdigit(str[j]) || str[j]=='+' || str[j] == '-')
            {
                num = strtol(str+j, NULL, 10);
                if (num<MIN_DATA_INT || num>MAX_DATA_INT)
                {
                    printf("Number %d out of bound in line %i\n", num, getInputLine(i));
                    bigflag = FALSE;
                }

            }
            else
            {
                bigflag = FALSE;
                printf("ilegal value %c in line %i col %i\n", str[j], getInputLine(i), j);
            }
            j++;
        }
    }

    j=0;
    state=COMMA;
    strcpy(str, getData(i, OPERAND2));
    len = strlen(str);
    if(len>0)
    {
        while(j<len)
        {
            flag = FALSE;
            if(state==COMMA && flag==FALSE)
            {
                if(isdigit(str[j]) || str[j]=='+' || str[j] == '-')
                {
                    state=NUMBER;
                    flag = TRUE;
                    num = strtol(str+j, NULL, 10);
                    if (num<MIN_DATA_INT || num>MAX_DATA_INT)
                    {
                        printf("Number %d out of bound in line %i\n", num, getInputLine(i));
                        bigflag = FALSE;
                    }

                }

                if(str[j]!=' ' && state==COMMA && flag==FALSE)
                {
                    printf("ilegal value %c in line %i\n", str[j], getInputLine(i));
                    bigflag = FALSE;
                }
            }

            if(state==NUMBER && flag==FALSE)
            {
                if(!isdigit(str[j]))
                {
                    if(str[j]==',')
                    {
                        state=COMMA;
                        flag =TRUE;
                    }

                    if(str[j]!=' ' &&state==NUMBER && flag==FALSE)
                    {
                        printf("ilegal character \"%c\" in line %i\n", str[j], getInputLine(i));
                        bigflag = FALSE;
                    }

                }
            }
            j++;
        }
    }
    return bigflag;
}

/*Check if .String has legal values*/
int checkString(int i)
{
    char str [MAX_LABEL];
    int flag=FALSE;
    int len;

    strcpy(str, getData(i, OPERAND1));
    len = strlen(str);

    if(str[0]=='"' && str[len-1]=='"')
        flag = TRUE;

    return flag;
}


