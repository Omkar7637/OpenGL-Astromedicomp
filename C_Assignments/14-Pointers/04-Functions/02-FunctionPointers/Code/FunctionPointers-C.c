#include<stdio.h>

int main(void)
{
    // function declarations
    int AddIntgers(int, int);
    int SubtractIntgers(int, int);
    float AddFloats(float, float);

    // varible declarations
    typedef int (*AddIntsFnptr)(int, int);
    AddIntsFnptr ptrAddTwoIntegers = NULL;
    AddIntsFnptr ptrFunc = NULL;

    typedef float (*AddFloatFnPtr)(float, float);
    AddFloatFnPtr ptrAddTwoFloats = NULL;

    int iAnswer = 0;
    float fAnswer = 0.0f;

    // code
    ptrAddTwoIntegers = AddIntgers;
    iAnswer = ptrAddTwoIntegers(9, 30);
    printf("\n\n");
    printf("Sum Of Intgers = %d\n\n", iAnswer);

    ptrFunc = SubtractIntgers;
    iAnswer = ptrFunc(9, 30);
    printf("\n\n");
    printf("Subtraction Of Intgers = %d\n\n", iAnswer);

    ptrAddTwoFloats = AddFloats;
    fAnswer = ptrAddTwoFloats(11.45f, 8.2f);
    printf("\n\n");
    printf("Sum Of Floating-Point Numbers = %f\n\n", fAnswer);

    return(0);
}

int AddIntgers(int a, int b)
{
    // variable declarations
    int c;

    // code
    c = a + b;
    return(c);
}

int SubtractIntgers(int a, int b)
{
    // variable declarations
    int c;

    // code
    if(a > b)
    {
        c = a - b;
    }
    else
    {
        c = b - a;
    }

    return(c);
}

float AddFloats(float f_num1, float f_num2)
{
    // varible declarations
    float ans;

    // code
    ans = f_num1 + f_num2;

    return(ans);
}

