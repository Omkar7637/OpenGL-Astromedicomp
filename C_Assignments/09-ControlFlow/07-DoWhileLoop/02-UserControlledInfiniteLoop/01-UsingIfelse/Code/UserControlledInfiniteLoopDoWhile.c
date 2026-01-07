#include<stdio.h>
int main(void)
{
    // varible declarations
    char option_ok, ch_ok = '\0';

    //code

    printf("\n\n");
    printf("Once The Infinite Loop Begins, enter 'Q' or 'q' to quite The Infinte For Loop : \n\n");
    printf("Enter 'Y' or 'y' To Initiate User Controlled Infinite Loop : ");
    printf("\n\n");
    option_ok = getchar();
    if( option_ok == 'Y' || option_ok == 'y')
    {
        do
        {
            printf("In Loop...\n");
            ch_ok = getchar(); // control flow waits for charater input....
            if(ch_ok == 'Q' || ch_ok == 'q' )
            {
                break; // UserControlled Exitting From Infinite Loop 
            }
        }while (1);

        printf("\n\n");
        printf("EXITTING USER CONTROLLED INFINITE LOOP...");
        printf("\n\n");
    }
    else
    {
        printf("You Must press 'Y' or ;y; to Initiate the user Controlled Infinite Loop....Plese Try Agin ...\n\n");
        return(0);
    }
}