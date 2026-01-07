#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(void)
{
    // variable declarations
    int *iArray[NUM_ROWS];
    int i, j;

    // code
    printf("\n\n");
    printf("****** MEMORY ALLOCATION TO 2DINTGER ARRAY ******\n\n");
    for(i = 0; i < NUM_ROWS; i++)
    {
        iArray[i] = (int *)malloc((NUM_COLUMNS - i) * sizeof(int));
        if(iArray[i] == NULL)
        {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTGER ARRAY !!! EXITING NOW... \n\n", i);
            exit(0);
        }
        else
        {
            printf("MEMORY ALLOCATIN TO ROW %d OF 2D INTGER ARRAY SUCCEEDED !!!\n\n", i);
        }
    }

    for(i = 0; i < NUM_ROWS; i++)
    {
        for(j = 0; j < (NUM_COLUMNS - i); j++)
        {
            iArray[i][j] = (i * 1) + (j * 1);
        }
    }

    for(i = 0; i < NUM_ROWS; i++)
    {
        for(j = 0; j < (NUM_COLUMNS - i); j++)
        {
            printf("iArray[%d][%d] = %d \t At Address : %p\n", i, j, iArray[i][j], &iArray[i][j]);
        }
        printf("\n");
    }

    for(i = (NUM_ROWS - 1); i >=0; i--)
    {
        if(iArray[i])
        {
            free(iArray[i]);
            iArray[i] = NULL;
            printf("MEMORY ALLOCATED TO ROW %d HAS BEEN SUCCESSFULLY FREED !!!\n\n", i);
        }
    }

    return(0);
}