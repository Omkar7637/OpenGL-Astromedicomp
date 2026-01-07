#include<stdio.h>

int main(void)
{
    // varible declarations 
    int i_ok, j_ok;

    // code

    printf("\n\n");
    for( i_ok = 1; i_ok<= 20; i_ok++)
    {
        for(j_ok = 1; j_ok <= 20; j_ok++)
        {
            if(j_ok > i_ok)
            {
                break;
            }
            else
            {
                printf("* ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
    return (0);
}