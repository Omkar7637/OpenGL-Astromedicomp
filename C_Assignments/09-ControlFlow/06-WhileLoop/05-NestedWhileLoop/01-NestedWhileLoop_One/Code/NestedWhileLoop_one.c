#include<stdio.h>
int main(void)
{
    // variable declarations
    int i_ok, j_ok;

    // code
    printf("\n\n");

    i_ok = 1;

    while(i_ok <= 10)
    {
        printf("i = %d\n",i_ok);
        printf("-----------\n\n");

        j_ok = 1;

        while(j_ok <= 5)
        {
            printf("\tj = %d\n", j_ok);
            j_ok++;
        }
        i_ok++;
        printf("\n\n");
    }
    return (0);

}