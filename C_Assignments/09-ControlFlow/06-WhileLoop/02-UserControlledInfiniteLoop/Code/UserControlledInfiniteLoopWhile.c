#include<stdio.h>

int main(void)
{
    // variable declarations
    char option_ok, ch_ok = '\0';

    // code

    printf("\n\n");

    printf("Once The Infinite Loop Begins, ENter 'Q' or'q' To quite the infinite For Loop : \n\n");
    printf("Enter 'Y' or 'y' To Initiate User Controlled Infinite Loop : ");
    printf("\n\n");
    option_ok = getchar();
    if(option_ok == 'Y' || option_ok == 'y')
    {
        while(1)
        {
            printf("In Loop...\n");
            ch_ok = getchar();
            if(ch_ok == 'Q' || ch_ok == 'q')
            {
                break; // User Contolled Exitting From Infinte Loop 
            }
        }

        printf("\n\n");
        printf("EXITING USER CONTROLLED INFINITE LOOP...");
        printf("\n\n");
    }
    else
    {
        printf("You Must press 'Y' or 'y' To Initiate The User Controlled Infinite Loop....Please Try Agin...\n\n");
    }

    return (0);
}