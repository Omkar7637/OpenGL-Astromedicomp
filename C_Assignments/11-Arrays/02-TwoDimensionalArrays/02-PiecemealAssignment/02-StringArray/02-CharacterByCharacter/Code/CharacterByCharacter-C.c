#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function peototypes
    void MyStrcpy(char[], char[]);

    // variable decalartions
    char strArray[5][10]; 
    int char_size;
    int strArray_size;
    int strArray_num_elements, strArray_num_rows, strArray_num_columns;
    int i;

    // code
    printf("\n\n");

    char_size = sizeof(char);

    strArray_size = sizeof(strArray);
    printf("Size Of Two Dimentional ( 2D ) Charater Array (String Array) Is = %d\n\n", strArray_size);

    strArray_num_rows = strArray_size / sizeof(strArray[0]);
    printf("Number Of Rows (String) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_num_rows);

    strArray_num_columns = sizeof(strArray[0]) / char_size;
    printf("Number Of Columns In Two Dimensinal ( 2D ) Charater Array (String Array) Is = %d \n\n", strArray_num_columns);
    
    strArray_num_elements = strArray_num_columns * strArray_num_rows;
    printf("Maximum Number Of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", strArray_num_elements);
    
    // *** PIECE-MEAL ASSIGNMENT ***
    // ****** ROW 1 / STRING 1 ******
    strArray[0][0] = 'M';
    strArray[0][1] = 'y';
    strArray[0][2] = '\0'; // NULL-TERMINATING CHARACTER 

    // ****** ROW 2 / STRING 2 ******
    strArray[1][0] = 'N';
    strArray[1][1] = 'a';
    strArray[1][2] = 'm';
    strArray[1][3] = 'e';
    strArray[1][4] = '\0';

    // ****** ROW 3 / STRING 3 ******
    strArray[2][0] = 'I';
    strArray[2][1] = 's';
    strArray[2][2] = '\0';

    // ****** ROW 4 / STRING 4 ******
    strArray[3][0] = 'O';
    strArray[3][1] = 'm';
    strArray[3][2] = 'k';
    strArray[3][3] = 'a';
    strArray[3][4] = 'r';
    strArray[3][5] = '\0';

    // ****** ROW 5 / STRING 5 ******
    strArray[4][0] = 'K';
    strArray[4][1] = 'a';
    strArray[4][2] = 's';
    strArray[4][3] = 'h';
    strArray[4][4] = 'i';
    strArray[4][5] = 'd';
    strArray[4][6] = '\0';

    printf("\n\n");
    printf("The String In The 2D Charater Array Are : \n\n");

    for( i = 0; i < strArray_num_rows; i++)
    {
        printf("%s ",strArray[i]);
    }

    printf("\n\n");

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