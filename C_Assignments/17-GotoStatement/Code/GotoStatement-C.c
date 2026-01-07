#include<stdio.h>
#include<string.h>

int main(void)
{
    // varaible declarations
    char ch, ch_i;
    unsigned int ascii_ch = 0;

    // code
    printf("\n\n");
    printf("Enter The First Charater Of First Name : ");
    ch = getch();

    ch = toupper(ch);

    for(ch_i = 'A'; ch_i <= 'Z'; ch_i++)
    {
        if(ch == ch_i)
        {
            ascii_ch = (unsigned int)ch;
            goto result_output;
        }
    }

    printf("\n\n");
    printf("Goto statement not executed, so printing \"Hello, World !!!\".\n");

    result_output:

    printf("\n\n");
    if(ascii_ch == 0)
    {
        printf("You must have a strange name! Could not find the character '%c' in the entire English Alphabet!\n", ch);
    }
    else
    {
        printf("Character '%c' found. It Has ASCII Value %u.\n", ch, ascii_ch);
    }

    printf("\n\n");
    return(0);
}