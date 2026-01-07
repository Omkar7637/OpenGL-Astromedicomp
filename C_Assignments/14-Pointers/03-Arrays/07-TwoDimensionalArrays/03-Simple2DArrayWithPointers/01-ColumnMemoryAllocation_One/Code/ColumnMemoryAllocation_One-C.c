#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    // variable declarations
    int *iArray[NUM_ROWS]; // A 2D Array which will have 5 rows and number of columns can be decided llater on ...
    int i, j;

    // code
    printf("\n\n");
    for(i = 0; i < NUM_ROWS; i++)
    {
        iArray[i] = (int *)malloc(NUM_COLUMNS * sizeof(int));
        if(iArray[i] == NULL)
        {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTGER ARRAY !!! EXITING NOW...\n\n", i);
            exit(0);
        }
        else
        {
            printf("MEMEORY ALLOCATION TO ROW %d OF 2D INTGER ARRAY SUCCEEDED !!!\n\n", i);
        }
    }
    // ASSIGNING VALUES TO 2D ARRAY...
    for(i = 0; i < NUM_ROWS; i++)
    {
        for(j = 0; j < NUM_COLUMNS; j++)
        {
            iArray[i][j] = (i + 1) * (j + 1);
        }
    }

    // DISPLAYING 2D ARRAY...
    printf("\n\n");
    printf("DISPLAYING 2D ARRAY : \n\n");
    for(i = 0; i < NUM_ROWS; i++)
    {
        for(j = 0; j < NUM_COLUMNS; j++)
        {
            printf("iArray[%d][%d] = %d\n", i, j, iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");

    // FREEING MEMORY ASSIGNED TO 2D ARRAY (MUST BE DONE IN REVERSE ORDER)
    for(i = (NUM_ROWS - 1); i >= 0; i--)
    {
        free(iArray[i]);
        iArray[i] = NULL;
        printf("MEMORY ALLOCATED TO ROW %d OF 2D INTGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
    }

    return(0);

}