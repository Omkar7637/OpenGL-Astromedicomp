#include<stdio.h>
#include<string.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    // variable decalrations 
    char chArray_Orignal[MAX_STRING_LENGTH]; // A charater Array Is A String 

    // code

    // *** STRING INPUT ***
    printf("\n\n");
    printf("ENter A String : \n\n");
    gets_s(chArray_Orignal, MAX_STRING_LENGTH);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The original string Entered by you (i.e : 'chArray_Origanl[]') Is : \n\n");
    printf("%s\n", chArray_Orignal);

    printf("\n\n");
    printf("The Reversed String (i.e : 'chArray_Reverse[]') Is : \n\n");
    printf("%s\n", strrev(chArray_Orignal));

    return (0);
}