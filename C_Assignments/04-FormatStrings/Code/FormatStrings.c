#include<stdio.h>

int main(void)
{
    // code
    printf("\n\n");
    printf("********************************************************************************************");
    printf("\n\n");

    printf("Hello World !!!\n\n");

    int a =13;
    printf("Integer decimal value of 'a' = %d\n", a);
    printf("Integer Octal value of 'a' = %o\n",a);
    printf("Integer Hexadecimal value of 'a' (Hexadecimal Letters in lower case) = %x\n",a);
    printf("Integer Hexadecimal value of 'a' (Hexadecimal letters in lower case) = %X\n\n",a);

    char ch = 'A';
    printf("Character ch = %c\n", ch);
    char str[] = "AstroMediComp's Real Time Rendering Batch";
    printf("String str = %s\n\n",str);

    long num = 30121005L;
    printf("Long Integer = %ld\n\n",num);

    unsigned int b = 7;
    printf("Unsigned Integer 'b' = %u\n\n",b);

    float f_num = 3012.1995f;
    printf("Floating Point Number with just %%f 'f_num' = %f\n", f_num);
    printf("Floating point Number with %%4.2f 'f_num' = %4.2f\n", f_num);
    printf("Floating ponit Number with %%6.5f 'f_num' = %6.5f\n\n", f_num);

    double d_pi = 3.14159265358979323846;
    printf("Double precision floating ponit number without exponential = %g\n",d_pi);
    printf("Double precision floating point number with exponential (lower case) = %e\n",d_pi);
    printf("Double precision floating point with expnential (upper case) = %E\n\n",d_pi);
    printf("Double Hexadeciaml value of 'd_pi' (Hexadecimal letters in lower case) = %a\n",d_pi);
    printf("Double Hexadeciaml value of 'd_pi' (Hexadecimal letters in Upper case) = %A\n\n",d_pi);
    
    printf("********************************************************************************************");
    printf("\n\n");

    return (0);
    
}