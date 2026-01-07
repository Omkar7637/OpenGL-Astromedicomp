#include<stdio.h>

int main(void)
{
    //variable declaration
    int a;
    int b;
    int result;

    //code
    printf("\n\n");
    printf("Enter one Integer : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Another Integer : ");
    scanf("%d",&b);

    printf("\n\n");
    printf("If Answer = 0, It Is 'false'.\n");
    printf("If Answer = 1, It IS 'TRUE'.\n\n");

    result = (a < b);
    printf("(a < b) A = %d Is Less Than B = %d                  \t Answer = %d\n",a,b,result);

    result = (a > b);
    printf("(a > b) A = %d Is Greater Than B = %d               \t Answer = %d\n",a,b,result);

    result = (a <= b);
    printf("(a <= b) A = %d Is Greater Than or Equal To B = %d  \t Answer = %d\n",a,b,result);

    result = (a >= b);
    printf("(a >= b) A = %d Is greater Than or equal to b = %d  \t Answer = %d\n",a,b,result);

    result = (a == b);
    printf("(a == b) A = %d Is Equal to B = %d                  \t Answer = %d\n",a,b,result);

    result = (a != b);
    printf("(a != b) A = %d Is Not Equal To B = %d              \t Answer = %d\n",a,b,result);

    return (0);

}