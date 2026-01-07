#include<stdio.h>

int main(void)
{
    // variable declarations 
    int i_ok;

    //code

    printf("\n\n");

    printf("prrinting Even Numbers from 0 to 100 : \n\n");

    for(i_ok = 0;i_ok <= 100; i_ok++)
    {
        // condition for a number to be even number => division of number by 2 leaves no reminder (reminder = 0)
        // if reminder is not 0 , the number is odd number....

        if(i_ok % 2 != 0)
        {
            continue;
        }
        else
        {
            printf("\t %d \n", i_ok);
        }
    }

    printf("\n\n");
    
    return (0);
}