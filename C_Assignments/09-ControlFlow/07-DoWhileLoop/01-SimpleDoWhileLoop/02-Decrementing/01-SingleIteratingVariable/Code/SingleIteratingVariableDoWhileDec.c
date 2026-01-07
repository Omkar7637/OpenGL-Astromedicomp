#include<stdio.h>

int main(void)
{
    // varible declarations
    int i_ok;

    //code
    printf("\n\n");

    printf("Printing Digits 10 to 1 : \n\n");

    i_ok = 10;
    do
    {
        printf("\t %d \n", i_ok);
        i_ok--;
    }while(i_ok >= 1);

    printf("\n\n");
    return(0);
}