#include<stdio.h>

int main(void)
{
    // variable declarations
    int i_ok, j_ok, k_ok;

    //code

    printf("\n\n");

    for(i_ok = 1; i_ok <= 10; i_ok++)
    {
        printf("i = %d\n", i_ok);
        printf("---------\n\n");
        
        for(j_ok = 1; j_ok <= 5; j_ok++)
        {
            printf("\tj = %d\n",j_ok);
            printf("\t---------\n\n");

            for(k_ok = 1; k_ok <= 3; k_ok++)
            {
                printf("\t\tk = %d\n",k_ok);
            }
            printf("\n\n");
        }
        printf("\n\n");
    }
    printf("\n\n");


    return (0);
}