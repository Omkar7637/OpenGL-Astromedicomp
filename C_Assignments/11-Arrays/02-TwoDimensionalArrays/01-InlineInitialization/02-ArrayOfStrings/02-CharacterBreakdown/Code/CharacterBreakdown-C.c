#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototypes
    int MyStrlen(char[]);

    // variable decalartions
    char strArray[10][15] = {"Hello!", "Welocome", "To", "Real", "time", "Rendering", "Batch", "(2024-25)", "Of", "ASTROMEDICOMP."}; //In-Line INITILIAZATION
    int iStrLengths[10];
    int strArray_size;
    int strArray_num_rows;
    int i, j;

    //code
    strArray_size = sizeof(strArray);
    strArray_num_rows = strArray_size / sizeof(strArray[0]);

    // Storing in lengths of all the strings...
    for(i = 0; i < strArray_num_rows; i++)
    {
        iStrLengths[i] = MyStrlen(strArray[i]);
    }

    printf("\n\n");
    printf("The Entire String Array : \n\n");

    for(i = 0;i < strArray_num_rows; i++)
    {
        printf("%s ", strArray[i]);
    }

    printf("\n\n");
    printf("String In The 2D Array : \n\n");

    for(i = 0; i < strArray_num_rows; i++)
    {
        printf("String Number %d => %s\n\n", (i + 1), strArray[i]);
        for(j = 0; j < iStrLengths[i]; j++)
        {
            printf("Character %d = %c\n", (j + 1), strArray[i][j]);
        }
        printf("\n\n");
    }

    return(0);
}


int MyStrlen(char str[])
{
    // variable declarations
    int j;
    int string_length = 0;
    
    // code
    // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER (\0) ***

    for (j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if(str[j] == '\0')
            break;
        else    
            string_length++;
    }
    return(string_length);
}