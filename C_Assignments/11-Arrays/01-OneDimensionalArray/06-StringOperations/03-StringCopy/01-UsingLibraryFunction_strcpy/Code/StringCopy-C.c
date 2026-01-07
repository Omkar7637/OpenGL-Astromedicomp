#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // variable decalartions
    char chArray_Original[MAX_STRING_LENGTH], chArray_Copy[MAX_STRING_LENGTH];// A Charater Array Is A String

    // code

    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(chArray_Original, MAX_STRING_LENGTH);

    // *** STRING COPY ***
    strcpy(chArray_Copy,chArray_Original);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The Orignal String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", chArray_Original);

    printf("\n\n");
    printf("The Copied String (i.e : 'chArray_copy[]') Is : \n\n");
    printf("%s\n", chArray_Copy);

    return(0);
}