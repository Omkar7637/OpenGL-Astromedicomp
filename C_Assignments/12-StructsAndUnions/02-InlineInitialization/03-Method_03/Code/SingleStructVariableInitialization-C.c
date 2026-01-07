#include<stdio.h>


int main(void)
{
    // DEFINE STRUCT 
    struct MyData
    {
        int i;
        float f;
        double d;
        char c;
    } data = { 5, 9.1f, 3.78623, 'N' }; 

    // code
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", data.i);
    printf("f = %f\n", data.f);
    printf("d = %lf\n", data.d);
    printf("c = %c\n\n", data.c);

    return(0);
}