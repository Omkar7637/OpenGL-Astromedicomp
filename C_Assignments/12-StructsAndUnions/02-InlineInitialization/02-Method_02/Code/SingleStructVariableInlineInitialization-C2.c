#include<stdio.h>

// DEFINE STRUCT 
struct MyData
{
    int i;
    float f;
    double d;
    char c;
};
struct MyData data = { 9, 8.2f, 9.61998, 'P'}; 

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