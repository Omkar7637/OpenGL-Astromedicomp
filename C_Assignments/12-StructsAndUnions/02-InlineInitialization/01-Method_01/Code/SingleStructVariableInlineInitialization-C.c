#include<stdio.h>

// DEFINE STRUCT 
struct MyData
{
    int i;
    float f;
    double d;
    char c;
} data = { 30, 4.5f, 11.451995, 'A'}; 

int main(void)
{
    // code
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n", data.i);
    printf("f = %f\n", data.f);
    printf("d = %lf\n", data.d);
    printf("c = %c\n\n", data.c);

    return(0);
}