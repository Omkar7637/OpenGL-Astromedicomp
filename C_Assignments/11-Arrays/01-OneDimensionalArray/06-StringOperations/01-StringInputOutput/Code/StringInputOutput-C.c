#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // variable declarations
    char chArray[MAX_STRING_LENGTH];  // A character Array IS A string

    // code
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets(chArray);

    printf("\n\n");
    printf("String Entered By You IS : \n\n");
    printf("%s\n",chArray);

    return(0);
}