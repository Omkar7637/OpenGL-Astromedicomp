#include<stdio.h>

int main(void)
{
    // variable declarations 
    char option_ok, ch_ok = '\0';

    // code
    printf("\n\n");
    printf("Once The infinite loop begins, Enter 'Q' or 'q' To Quite the infinite for loop : \n\n");

    do
    {
        do
        {
            printf("\n");
            printf("In Loop...\n");
            ch_ok = getchar(); // control flow waits for charater input...
        }while(ch_ok != 'Q' && ch_ok != 'q');
        printf("\n\n");
        printf("EXTTING USER CONTROLLED INFNITE LOOP...");

        printf("\n\n");
        printf("DO YOU WANT TO BEGIN USER CONTROLLED INFINITE LOOP AGIN?....(Y/y - Yes, any Other Key - No) : ");
        option_ok = getchar();
    }while(option_ok == 'Y' || option_ok == 'y');

    return (0);
       

}