#include<stdio.h>

int main(void)
{
    // variable declarations
    int i_ok, j_ok;

    // code
    printf("\n\n");

    i_ok = 1;

    do
    {
        printf("i = %d\n", i_ok);
        printf("-----------\n\n");

        j_ok = 1;
        do
        {
            printf("\tj = %d\n", j_ok);
            j_ok++;
        }while(j_ok <= 5);
        i_ok++;
        printf("\n\n");
    }while (i_ok <= 10);
    return (0);
}