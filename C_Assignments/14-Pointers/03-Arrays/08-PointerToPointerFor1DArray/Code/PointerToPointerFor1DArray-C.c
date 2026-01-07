#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    void MyAlloc(int **ptr, unsigned int numberOfElements);

    // variable decalarations
    int *piArray = NULL;
    unsigned int num_elements;
    int i;

    // code
    printf("\n\n");
    printf("How Many Elements You Want In Intger Array ?\n\n");
    scanf("%u", &num_elements);

    printf("\n\n");
    MyAlloc(&piArray, num_elements);

    printf("Enter %u Elements To Fill Up Your Intger Array : \n\n", num_elements);
    for(i = 0; i < num_elements; i++)
    {
        scanf("%d", &piArray[i]);
    }
    printf("\n\n");
    printf("The %u Elements Entered By You In The Intger Array : \n\n", num_elements);

    for(i = 0; i < num_elements; i++)
    {
        printf("%u", piArray[i]);
    }

    printf("\n\n");
    if(piArray)
    {
        free(piArray);
        piArray = NULL;
        printf("Memory Alllocated Has Now Been Successfully Freed !!!\n\n");
    }

    return(0);
}

void MyAlloc(int **ptr, unsigned int numberOfElements)
{
    // code
    *ptr = (int *)malloc(numberOfElements * sizeof(int));
    if(*ptr == NULL)
    {
        printf("Could Not Allocate Memory !!! Exitting Now ...\n\n");
        exit(0);
    }
}