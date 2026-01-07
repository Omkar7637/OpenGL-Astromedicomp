#include<stdio.h>

int main(void)
{
    // variable declartions
    float f_ok;
    float f_num_ok = 1.7f; // simply change this Only to get different outputs...

    //code
    printf("\n\n");

    printf("Printing Numbers %f to %f : \n\n", f_num_ok, (f_num_ok * 10.0f));

    f_ok = f_num_ok;

    while(f_ok <= (f_num_ok * 10.0f))
    {
        printf("\t %f \n",f_ok);
        f_ok = f_ok + f_num_ok;
    }

    printf("\n\n");
    return (0);
}