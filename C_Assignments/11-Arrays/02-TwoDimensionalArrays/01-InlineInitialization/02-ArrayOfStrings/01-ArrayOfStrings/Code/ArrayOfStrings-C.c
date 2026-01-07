#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype
    int MyStrlen(char[]);

    // variablde declaratons
    char strArray[10][15] = {"Hello!", "Welocome", "To", "Real", "time", "Rendering", "Batch", "(2024-25)", "Of", "ASTROMEDICOMP."}; //In-Line INITILIAZATION

    int char_size;
    int strArray_size;
    int strArray_num_elements, strArray_num_rows, strArray_num_columns;
    int strActual_num_chars = 0;
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
    
    for(i = 0; i < strArray_num_rows; i++)
    {
        strActual_num_chars = strActual_num_chars + MyStrlen(strArray[i]);
    }

    printf("Actual Number Of Elements (characters) In Two Dimensinal ( 2D ) Character Array (String Array) Is = %d\n\n", strActual_num_chars);

    printf("\n\n");
    printf("String In The 2D Array : \n\n");

    printf("%s ", strArray[0]);
    printf("%s ", strArray[1]);
    printf("%s ", strArray[2]);
    printf("%s ", strArray[3]);
    printf("%s ", strArray[4]);
    printf("%s ", strArray[5]);
    printf("%s ", strArray[6]);
    printf("%s ", strArray[7]);
    printf("%s ", strArray[8]);
    printf("%s \n\n", strArray[9]);

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
