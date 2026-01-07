#include<stdio.h>

int main(void)
{
    // varible decalartions
    float f_ok;
    float f_num_ok = 1.7f; // simply chnage thsi value ONLY to get different outputs...

    //code

    printf("\n\n");

    printf("Printing NUmber %f to %f : \n\n", f_num_ok, (f_num_ok * 10.0f));

    for (f_ok = f_num_ok; f_ok <= (f_num_ok * 10.0f);f_ok = f_ok + f_ok)
    {
        printf("\t%f\n",f_ok);
    }

    printf("\n\n");

    return (0);

}