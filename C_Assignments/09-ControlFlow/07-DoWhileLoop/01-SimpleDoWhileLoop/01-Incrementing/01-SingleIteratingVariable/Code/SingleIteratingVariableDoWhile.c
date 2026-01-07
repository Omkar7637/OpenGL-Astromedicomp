#include<stdio.h>
int main(void)
{
    // variable declarations
    int i_ok;

    //code
    printf("\n\n");

    printf("Printing Digits 1 to 10 : \n\n");

    i_ok =1;
    do
    {
        printf("\t%d\n",i_ok);
        i_ok++;
    }while(i_ok <= 10);

    printf("\n\n");

    return (0);
}