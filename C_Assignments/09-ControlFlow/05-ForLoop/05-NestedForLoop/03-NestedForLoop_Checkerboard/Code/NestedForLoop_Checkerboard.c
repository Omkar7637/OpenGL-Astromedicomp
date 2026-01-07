#include<stdio.h>

int main(void)
{
    // variable declarations
    int i_ok, j_ok, c_ok;

    //code

    for(i_ok = 0; i_ok < 64; i_ok++)
    {
        for(j_ok = 0; j_ok < 64; j_ok++)
        {
            c_ok = ((i_ok &  0x8) == 0) ^ ((j_ok & 0x8) == 0);

            if(c_ok == 0)
                printf(" ");

            if(c_ok == 1)
                printf("* ");
        }
        printf("\n\n");
    }
    return (0);
}