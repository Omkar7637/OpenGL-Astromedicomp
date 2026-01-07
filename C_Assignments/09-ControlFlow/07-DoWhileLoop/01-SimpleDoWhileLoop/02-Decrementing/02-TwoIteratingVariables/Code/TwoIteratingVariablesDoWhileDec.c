#include<stdio.h>
int main(void)
{
    // variable declarations
    int i_ok, j_ok;

    // code
    printf("\n\n");

    printf("Printing Digits 10 to 1 and 100 to 10 : \n\n");

    i_ok = 10;
    j_ok = 100;
    do
    {
        printf("\t %d \t %d \n",i_ok, j_ok);
        i_ok--;
        j_ok = j_ok - 10;
    }while(i_ok >= 1, j_ok >= 10);
    
    printf("\n\n");

    return(0);
}