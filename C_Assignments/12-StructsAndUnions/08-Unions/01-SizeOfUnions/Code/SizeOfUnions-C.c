#include<stdio.h>

#pragma pack(push, 1)

struct MyStruct
{
    int i;
    float f;
    double d;
    char c;
};

union MyUnion
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    // varibel decalarations 
    struct MyStruct s;
    union MyUnion u;

    // code
    printf("\n\n");
    printf("Size Of MyStruct = %zd\n", sizeof(s));
    printf("\n\n");
    printf("Size Of Myunion = %zd\n", sizeof(u));
    return(0);
}