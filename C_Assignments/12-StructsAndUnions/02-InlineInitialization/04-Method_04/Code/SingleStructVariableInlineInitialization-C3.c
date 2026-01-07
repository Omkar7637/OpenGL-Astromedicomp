#include<stdio.h>

// DEFINE STRUCT 
struct MyData
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    struct MyData data_one = { 35, 3.9f, 1.23765, 'A'}; 
    struct MyData data_two = { 'P', 6.2f, 12.199523, 68}; 
    struct MyData data_three = { 26, 'G'}; 
    struct MyData data_four = { 79 }; 
    // code
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data_one ' ARE : \n\n");
    printf("i = %d\n", data_one.i);
    printf("f = %f\n", data_one.f);
    printf("d = %lf\n", data_one.d);
    printf("c = %c\n\n", data_one.c);
    
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data_two ' ARE : \n\n");
    printf("i = %d\n", data_two.i);
    printf("f = %f\n", data_two.f);
    printf("d = %lf\n", data_two.d);
    printf("c = %c\n\n", data_two.c);
    
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data_three ' ARE : \n\n");
    printf("i = %d\n", data_three.i);
    printf("f = %f\n", data_three.f);
    printf("d = %lf\n", data_three.d);
    printf("c = %c\n\n", data_three.c);
    
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data_four ' ARE : \n\n");
    printf("i = %d\n", data_four.i);
    printf("f = %f\n", data_four.f);
    printf("d = %lf\n", data_four.d);
    printf("c = %c\n\n", data_four.c);

    return(0);
}