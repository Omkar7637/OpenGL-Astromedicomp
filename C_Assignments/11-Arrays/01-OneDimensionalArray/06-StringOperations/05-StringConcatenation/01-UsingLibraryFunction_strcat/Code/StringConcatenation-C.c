#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // variable declarations
    char chArray_One[MAX_STRING_LENGTH], chArray_TWO[MAX_STRING_LENGTH];
    // A Charater Array Is A String

    //code

    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter first String : \n\n");
    gets_s(chArray_One, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("ENter Second String : \n\n");
    gets_s(chArray_TWO, MAX_STRING_LENGTH);

    // *** STRING CONCAT ***
    printf("\n\n");
    printf("******* BEFORE CONCATENATION *******");
    printf("\n\n");
    printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is : \n\n");
    printf("%s\n", chArray_One);

    printf("\n\n");
    printf("The Original Secound String Entered by You (i.e : 'chArray_Two[]') Is : \n\n");
    printf("%s\n",chArray_TWO);

    strcat(chArray_One,chArray_TWO);

    printf("\n\n");
    printf("******* AFTER CONCATENATION *******");
    printf("\n\n");
    printf("'chArray_One[]' Is : \n\n");
    printf("%s\n",chArray_One);

    printf("\n\n");
    printf("'chArray_Two[]' Is : \n\n");
    printf("%s\n", chArray_TWO);

    return(0);
}