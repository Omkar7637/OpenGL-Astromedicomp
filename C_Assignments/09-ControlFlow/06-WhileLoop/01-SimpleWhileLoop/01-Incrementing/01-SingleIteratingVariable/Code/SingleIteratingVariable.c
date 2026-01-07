#include<stdio.h>

int main(void)
{
    // varible declarations
    int i_ok;

    //code
    printf("\n\n");

    printf("Printing Digits 1 to 10 : \n\n");

    i_ok = 1;

    while(i_ok <= 10)
    {
        printf("\t%d\n",i_ok);
        i_ok++;
    }
    printf("\n\n");

    return (0);
}