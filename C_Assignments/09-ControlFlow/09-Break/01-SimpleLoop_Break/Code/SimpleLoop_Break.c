#include<stdio.h>
#include<conio.h>

int main(void)
{
    // variable decalarations
    int i_ok;
    char ch_ok;

    // code
    printf("\n\n");
    printf("Printing Even Numbers From 1 to 100 For Every User Input. Exitting the Loop When User Enters Character 'Q' or 'q' : \n\n");

    printf("Enter Charater 'Q' or 'q' to exit loop : \n\n");

    for (i_ok = 1; i_ok <= 100; i_ok++)
    {
        printf("\t%d\n", i_ok);
        ch_ok = getchar();
        if( ch_ok == 'Q' || ch_ok == 'q')
        {
            break;
        }
    }

    printf("\n\n");
    printf("EXITING LOOP...");
    printf("\n\n");

    return (0);
}