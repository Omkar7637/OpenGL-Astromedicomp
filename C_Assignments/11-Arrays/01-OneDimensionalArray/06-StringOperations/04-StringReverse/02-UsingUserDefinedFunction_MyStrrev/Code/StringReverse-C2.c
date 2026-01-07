#include<stdio.h>
#include<string.h> // for strrev()

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototypes
    void MyStrrev(char[], char[]);

    // variable decalrations 
    char chArray_Orignal[MAX_STRING_LENGTH],chArray_Reversed[MAX_STRING_LENGTH]; // A charater Array Is A String 

    // code

    // *** STRING INPUT ***
    printf("\n\n");
    printf("ENter A String : \n\n");
    gets_s(chArray_Orignal, MAX_STRING_LENGTH);

    // *** STRING REVERSE ***
    MyStrrev(chArray_Reversed,chArray_Orignal);

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("The original string Entered by you (i.e : 'chArray_Origanl[]') Is : \n\n");
    printf("%s\n", chArray_Orignal);

    printf("\n\n");
    printf("The Reversed String (i.e : 'chArray_Reverse[]') Is : \n\n");
    printf("%s\n", chArray_Reversed);

    return (0);
}

void MyStrrev(char str_destination[], char str_source[])
{
    // fucntion prototype
    int Mystrlen(char[]);

    // variable decalartions 
    int iStringLength = 0;
    int i, j, len;

    // code
    iStringLength = MyStrlen(str_source);

    // ARRAY INDICES BEGIN FROM 0, HENCE, LAST INDEX WILL ALWAYS BE (LENGTH - 1)

    len = iStringLength - 1;

    // WE NEED TO PUT THE CHARTER WHICH IS AT LAST INDEX OF 'str_source' TO THE FIRST INDEX OF 'str_destination'
    // AND SECOND-LAST CHARACTER OF 'str_source' TO THE SECOND CHARACTER OF 'str_destination' and so on...
    for(i = 0, j =len;i < iStringLength, j >= 0; i++, j--)
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