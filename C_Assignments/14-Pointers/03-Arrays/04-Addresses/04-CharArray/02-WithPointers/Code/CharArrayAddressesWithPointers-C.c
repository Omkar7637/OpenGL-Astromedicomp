#include<stdio.h>

int main(void)
{
    // variable declarations
    char cArray[10];
    char *ptr_cArray = NULL;
    int i;

    // code
    for (i = 0; i < 10; i++)
    {
        cArray[i] = (char)(i + 65);
    }

    ptr_cArray = cArray;
    printf("\n\n");
    printf("Elements Of The Character Array : \n\n");
    for(i = 0; i < 10; i++)
    {
        printf("cArray[%d] = %c\n", i, *(ptr_cArray + i));
    }

    printf("\n\n");
    printf("Elements Of The Character Array : \n\n");
    for(i = 0; i < 10; i++)
    {
        printf("cArray[%d] = %c \t \t Address = %p\n", i, *(ptr_cArray + i), (ptr_cArray + i));
    }

    printf("\n\n");

    return(0);
}