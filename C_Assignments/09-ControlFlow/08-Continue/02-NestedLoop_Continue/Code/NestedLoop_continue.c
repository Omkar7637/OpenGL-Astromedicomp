#include<stdio.h>

int main(void)
{
    // varibale declarations
    int i_ok, j_ok;

    // code

    printf("\n\n");

    printf("Oter Loop Prints Odd Number Between 1 and 10. \n\n");
    printf("Inner Loop Prints Even Numbers Between 1 and 10 For Every Odd Number Printed by outer Loop. \n\n");

    for(i_ok = 1; i_ok <= 10; i_ok++)
    {
        if(i_ok % 2 != 0)
        {
            printf("i = %d\n",i_ok);
            printf("----------\n");
            for(j_ok = 1;j_ok <= 10; j_ok++)
            {
                if( j_ok % 2 == 0)
                {
                    printf("\t j = %d\n", j_ok);
                }
                else
                {
                    continue;
                }
            }
            printf("\n\n");
        }
        else
        {
            continue;
        }
    }

    printf("\n\n");
    return (0);
}