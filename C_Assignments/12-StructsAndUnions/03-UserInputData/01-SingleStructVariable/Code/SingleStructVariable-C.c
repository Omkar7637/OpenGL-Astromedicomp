#include<stdio.h>
#include<conio.h>

struct MyData
{
    int i;
    float f;
    double d;
    char ch;
};

int main(void)
{
    // variable decalarations
    struct MyData data;

    //code
    printf("\n\n");

    printf("Enter Intger Value For Data Member 'i' Of 'struct MyData' : \n");
    scanf("%d", &data.i);
    printf("Enter Floating-point Value For Data Member 'f' Of 'struct MyData' : \n");
    scanf("%f", &data.f);
    printf("Enter 'Double' Value For Data Member 'd' Of 'struct MyData' : \n");
    scanf("%lf", &data.d);
    printf("Enter Character Value For Data Member 'c' Of 'struct MyData' : \n");
    data.ch = getch();

    // Display Values Of Data Members Of 'Struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' Are : \n\n");
    printf("i = %d\n", data.i);
    printf("f = %f\n", data.f);
    printf("i = %lf\n", data.d);
    printf("i = %c\n", data.ch);

    return(0);

}