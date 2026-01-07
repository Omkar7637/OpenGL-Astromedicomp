#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    // function prototype 
    int MyStrlen(char[]);
    void MyStrcpy(char[], char[]);

    // variable declarations
    char chArray_Original[MAX_STRING_LENGTH], chArray_VowelsReplaced[MAX_STRING_LENGTH];
    int iStringLength;
    int i;

    // code

    // *** STRING INPUT ***
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(chArray_Original, MAX_STRING_LENGTH);

    // *** STRING OUTPUT ***
    MyStrcpy(chArray_VowelsReplaced,chArray_Original);
    iStringLength = MyStrlen(chArray_VowelsReplaced);

    for( i = 0; i < iStringLength; i++)
    {
        switch(chArray_VowelsReplaced[i])
        {
            case'A':
            case'a':
            case'E':
            case'e':
            case'I':
            case'i':
            case'O':
            case'o':
            case'U':
            case'u':
                chArray_VowelsReplaced[i] = '*';
                break;
            default: 
                break;
        }
    }

    // *** STRING OUTPUT ***
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n",chArray_Original);

    printf("\n\n");
    printf("String After Replacaement Of Vowels By * Is : \n\n");
    printf("%s\n", chArray_VowelsReplaced);

    return(0);

}

























void MyStrcpy(char str_destination[], char str_source[])
{
    // function prototype
    int MyStrlen(char[]);

    // variablde decalrations
    int iStringLength = 0;
    int j;

    // code
    iStringLength = MyStrlen(str_source);
    for(j = 0; j < iStringLength; j++)
    {
        str_destination[j] = str_source[j];
    }

    str_destination[j] = '\0';
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