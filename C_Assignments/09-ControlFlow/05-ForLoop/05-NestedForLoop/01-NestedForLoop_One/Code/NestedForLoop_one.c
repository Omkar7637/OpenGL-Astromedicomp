#include<stdio.h>

int main()
{
    // variable declarations
    int i_ok, j_ok;

    // code
    printf("\n\n");
    for(i_ok = 1;i_ok <= 10;i_ok++)
    {
        printf("i_ok = %d\n", i_ok);
        printf("----------\n\n");

        for(j_ok = 1;j_ok <= 5; j_ok++)
        {
            printf("\tj = %d\n", j_ok);
        }
        printf("\n\n");
    }

    return (0);
}