#include<stdio.h>

int main(void)
{
    // varible decalaration
    int i_ok, j_ok, k_ok;

    // code
    printf("\n\n");
    i_ok = 1;
    do
    {
        printf("i = %d\n",i_ok);
        printf("-----------\n\n");

        j_ok = 1;
        do
        {
            printf("\tj = %d\n", j_ok);

            k_ok = 1;
            do
            {
                printf("\t\tk = %d\n",k_ok);
                k_ok++;
            }while(k_ok <= 3);
            printf("\n\n");
            j_ok++;
        }while(j_ok <= 5);
        printf("\n\n");
        i_ok++;
    }while(i_ok <= 10);
    return (0);
}