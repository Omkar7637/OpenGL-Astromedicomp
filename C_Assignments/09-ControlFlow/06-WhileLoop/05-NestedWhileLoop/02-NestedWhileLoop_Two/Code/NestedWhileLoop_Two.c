#include<stdio.h>

int main(void)
{
    // variable declarations
    int i_ok, j_ok, k_ok;

    //code 
    printf("\n\n");
    i_ok = 1;
    while(i_ok <= 10)
    {
        printf("i = %d\n", i_ok);
        printf("-----------\n\n");

        j_ok = 1;
        while(j_ok <= 5)
        {
            printf("\t j = %d \n ",j_ok);
            printf("-----------\n\n");

            k_ok = 1;
            while(k_ok <= 3)
            {
                printf("\t\tk =%d\n",k_ok);
                k_ok++;
            }
            printf("\n\n");
            j_ok++;
        }
        printf("\n\n");
        i_ok++;
    }
    return (0);
}