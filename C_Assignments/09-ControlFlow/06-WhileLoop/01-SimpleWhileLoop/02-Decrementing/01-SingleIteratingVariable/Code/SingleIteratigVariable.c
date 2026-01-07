#include<stdio.h>

int main(void)
{
    // variable declarations
    int i_ok;

    //code
    printf("\n\n");

    printf("Printing Digits 10 to 1 : \n\n");

    i_ok = 10;
    while(i_ok >= 1)
    {
        printf("\t%d\n",i_ok);
        i_ok--;
    }

    printf("\n\n");
    return (0);
}