#include<stdio.h>

int main(void)
{
    // varible declarations
    float f_ok;
    float f_num_ok = 2.7f; // simply change this value only to get differnt outputs...

    //code
    printf("\n\n");

    printf("printing Numbers %f to %f : \n\n", f_num_ok, (f_num_ok * 10.0f));

    f_ok = f_num_ok;

    do
    {
        printf("\t %f \n",f_ok);
        f_ok = f_ok + f_num_ok;
    }while(f_ok <= (f_num_ok * 10.0f));

    printf("\n\n");

    return (0);

}