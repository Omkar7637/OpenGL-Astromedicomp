#include<stdio.h>

int main(void)
{
    // varible declarations
    char option_ok, ch_ok = '\0';

    //code
    printf("\n\n");

    printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quite The Infinite For Loop : \n\n");
    printf("Enter 'Y' or 'y' To initiate User Controlled Infinite Loop : ");
    printf("\n\n");

    option_ok = getchar();

    if(option_ok == 'Y' || option_ok == 'y')
    {
        for(;;) // Infinte Loop
        {
            printf("IN Loop...\n");
            ch_ok = getchar();
            if(ch_ok == 'Q' || ch_ok == 'q')
            {
                break; // User Controlled Exiting From Infinite Loop
            }
        }
    }

    printf("\n\n");
    printf("EXITING USER CONTROLLED INFINITE LOOP...");
    printf("\n\n");

    return (0);
}