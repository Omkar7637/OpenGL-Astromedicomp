#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // variable decalarations
    char chArray[MAX_STRING_LENGTH]; // A Charater Array Is String
    int iStringLength = 0;

    //code

    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A string : \n\n");
    gets_s(chArray, MAX_STRING_LENGTH);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You IS : \n\n");
    printf("%s\n",chArray);

    // *** STRING LENGTH ***
    printf("\n\n");
    iStringLength = strlen(chArray);
    printf("Length Of String Is = %d Characters !!!\n\n",iStringLength);

    return (0);
}