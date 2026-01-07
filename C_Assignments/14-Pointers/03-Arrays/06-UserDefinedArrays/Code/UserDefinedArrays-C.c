#include<stdio.h>
#include<stdlib.h>

#define INT_SIZE sizeof(int) 
#define FLOAT_SIZE sizeof(float) 
#define DOUBLE_SIZE sizeof(double) 
#define CHAR_SIZE sizeof(char) 

int main(void)
{
    // varaible declarations
    int *ptr_iArray = NULL;
    unsigned int intArraylength = 0;

    float *ptr_fArray = NULL;
    unsigned int floatArrayLength = 0;

    double *ptr_dArray = 0;
    unsigned int doubleArrayLength = 0;

    char *ptr_cArray = NULL;
    unsigned int charArrayLength = 0;

    int i;

    // code

    // ****** INTGER ARRAY *******

    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The Intger Array : ");
    scanf("%u", &intArraylength);

    ptr_iArray = (int *)malloc(INT_SIZE * intArraylength);
    if(ptr_iArray == NULL)
    {
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY FAILED !!! EXITING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    printf("Enter The %d Intger Elements To Fill Up The Intger Array : \n\n", intArraylength);
    for(i = 0; i < intArraylength; i++)
    {
        scanf("%d", (ptr_iArray + i));
    }


    // ****** FLOAT ARRAY *******

    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The 'FLOAT' Array : ");
    scanf("%u", &floatArrayLength);

    ptr_fArray = (float *)malloc(FLOAT_SIZE * floatArrayLength);
    if(ptr_fArray == NULL)
    {
        printf("MEMORY ALLOCATION FOR FLOATING-POINT ARRAY FAILED !!! EXITING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR FLOATING-POINT ARRAY SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    printf("Enter The %d FLOATING-POINT Elements To Fill Up The FLOATING-POINT Array : \n\n", floatArrayLength);
    for(i = 0; i < floatArrayLength; i++)
    {
        scanf("%f", (ptr_fArray + i));
    }


    // ****** DOUBLE ARRAY *******

    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The 'DOUBLE' Array : ");
    scanf("%u", &doubleArrayLength);

    ptr_dArray = (double *)malloc(DOUBLE_SIZE * doubleArrayLength);
    if(ptr_dArray == NULL)
    {
        printf("MEMORY ALLOCATION FOR DOUBLE ARRAY FAILED !!! EXITING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR DOUBLE ARRAY SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    printf("Enter The %d DOUBLE Elements To Fill Up The DOUBLE Array : \n\n", doubleArrayLength);
    for(i = 0; i < doubleArrayLength; i++)
    {
        scanf("%lf", (ptr_dArray + i));
    }

    // ****** CHAR ARRAY *******

    printf("\n\n");
    printf("Enter The Number Of Elements You Want In The 'CHAR' Array : ");
    scanf("%u", &charArrayLength);

    ptr_cArray = (char *)malloc(CHAR_SIZE * charArrayLength);
    if(ptr_cArray == NULL)
    {
        printf("MEMORY ALLOCATION FOR CHAR ARRAY FAILED !!! EXITING NOW...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("MEMORY ALLOCATION FOR CHAR ARRAY SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    printf("Enter The %d CHAR Elements To Fill Up The CHAR Array : \n\n", charArrayLength);
    for(i = 0; i < charArrayLength; i++)
    {
        scanf("%c", (ptr_cArray + i));
    }

    // ******** DISPLAY OF ARRAYS *********

    // ****** INTGER ARRAY *******
    printf("\n\n");
    printf("The Intger Array Entered By you And Consisting Of %d Elements Is As Follows : \n\n", intArraylength);
    for(i = 0; i < intArraylength; i++)
    {
        printf("%d \t \t At Address : %p\n", *(ptr_iArray + i), (ptr_iArray + i));
    }

    

    // ****** FLOAT ARRAY *******
    printf("\n\n");
    printf("The FLOAT Array Entered By you And Consisting Of %d Elements Is As Follows : \n\n", floatArrayLength);
    for(i = 0; i < floatArrayLength; i++)
    {
        printf("%f \t \t At Address : %p\n", *(ptr_fArray + i), (ptr_fArray + i));
    }


    // ****** Double ARRAY *******
    printf("\n\n");
    printf("The DOUBLE Array Entered By you And Consisting Of %d Elements Is As Follows : \n\n", doubleArrayLength);
    for(i = 0; i < doubleArrayLength; i++)
    {
        printf("%lf \t \t At Address : %p\n", *(ptr_dArray + i), (ptr_dArray + i));
    }


    // ****** CHAR ARRAY *******
    printf("\n\n");
    printf("The CHAR Array Entered By you And Consisting Of %d Elements Is As Follows : \n\n", charArrayLength);
    for(i = 0; i < charArrayLength; i++)
    {
        printf("%c \t \t At Address : %p\n", *(ptr_cArray + i), (ptr_cArray + i));
    }

    // ******** FREEING MEMORY OCCUPIED BY ARRAYS (IN REVERSE ORDER OF ALLOCATION) ********

    if (ptr_cArray)
    {
        free(ptr_cArray);
        ptr_cArray = NULL;

        printf("\n\n");
        printf("MEMORY OCCUPIED BY CHARACTER ARRAY HAS BEEN SUCCESSFULLY FREED !!! \n\n");
    }
    

    if (ptr_dArray)
    {
        free(ptr_dArray);
        ptr_dArray = NULL;

        printf("\n\n");
        printf("MEMORY OCCUPIED BY DOUBLE ARRAY HAS BEEN SUCCESSFULLY FREED !!! \n\n");
    }

    if (ptr_fArray)
    {
        free(ptr_fArray);
        ptr_fArray = NULL;

        printf("\n\n");
        printf("MEMORY OCCUPIED BY FLOATING-POINT ARRAY HAS BEEN SUCCESSFULLY FREED !!! \n\n");
    }
    
    

    if (ptr_iArray)
    {
        free(ptr_iArray);
        ptr_iArray = NULL;

        printf("\n\n");
        printf("MEMORY OCCUPIED BY INTGER ARRAY HAS BEEN SUCCESSFULLY FREED !!! \n\n");
    }

    return(0);
    

}