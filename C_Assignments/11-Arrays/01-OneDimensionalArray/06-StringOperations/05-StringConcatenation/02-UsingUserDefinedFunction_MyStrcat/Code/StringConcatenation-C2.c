#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{

    // function prototypes
    void Mystrcat(char[], char[]);
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

    Mystrcat(chArray_One,chArray_TWO);

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

void Mystrcat(char str_destination[], char str_source[])
{
    // function prototype
    int MyStrlen(char[]);

    // variable decalartions
    int iStringLength_Source = 0, iStringlength_destination = 0;
    int i, j;

    // code
    iStringLength_Source = MyStrlen(str_source);
    iStringlength_destination = MyStrlen(str_destination);

    for(i = iStringlength_destination, j = 0; j < iStringLength_Source; i++, j++)
    {
        str_destination[i] = str_source[j];
    }

    str_destination[i] = '\0';
}

int MyStrlen(char str[])
{
    // variable declarations
    int j;
    int string_length = 0;
    
    // code
    // *** DETERMINING EXACT LENGTH OF THE STRING, BY DETECTING THE FIRST OCCURENCE OF NULL-TERMINATING CHARACTER (\0) ***

    for (j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if(str[j] == '\0')
            break;
        else    
            string_length++;
    }
    return(string_length);
}

