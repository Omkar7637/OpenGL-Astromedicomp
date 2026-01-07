#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // variable declarations
    int iArray_2D[NUM_ROWS][NUM_COLUMNS]; // TOTALL NUMBER OF ELEMENTS = NUM_ROWS * NUM_COLUMNS
    int iArray_1D[NUM_ROWS*NUM_COLUMNS];
    int i, j;
    int num;

    // code
    printf("Enter Elements Of Your Choice To Fill Up The Intger 2D Array : \n\n");
    for( i = 0; i < NUM_ROWS; i++)
    {
        printf("FOR ROW NUMBER %d : \n", (i + 1));
        for(j = 0;j < NUM_COLUMNS; j++)
        {
            printf("Enter Elements Number %d : \n",(j + 1));
            scanf("%d", &num);
            iArray_2D[i][j] = num;
        }
        printf("\n\n");
    }

    // *** DISPLAY OF 2D ARRAY ***
    printf("\n\n");
    printf("Two-Dimentional ( 2D ) Array Of Integers : \n\n");
    for(i = 0; i < NUM_ROWS; i++)
    {
        printf("****** ROW %d ******\n",(i + 1));
        for(j = 0; j < NUM_COLUMNS; j++)
        {
            printf("iArray_2D[%d][%d] = %d\n", i, j, iArray_2D[i][j]);
        }
        printf("\n\n");
    }

    // *** CONVERTING 2d INTEGER ARRAY TO 1D INTEGER ARRAY ***
    for(i = 0; i < NUM_ROWS; i++)
    {
        for(j = 0;j < NUM_COLUMNS; j++)
        {
            iArray_1D[( i * NUM_COLUMNS) + j] = iArray_2D[i][j];
        }
    }

    // *** PRINTING 1D ARRAY ***
    printf("\n\n");
    printf("One-Dimensional ( 1D ) Array Of Integers : \n\n");
    for(i = 0; i < (NUM_ROWS * NUM_COLUMNS); i++)
    {
        printf("iArray_1D[%d] = %d\n", i, iArray_1D[i]);
    }

    printf("\n\n");

    return(0);
}