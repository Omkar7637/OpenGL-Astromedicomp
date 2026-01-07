#include<stdio.h>

int main(void)
{
    // variable declarations
    int a;
    int b;
    int result;

    //code
    printf("\n\n");
    printf("Enter A number :");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Another Number :");
    scanf("%d",&b);

    printf("\n\n");

    //*** The following are the 5 arithmetic operators +,-,*,/ and % ***
    //*** Also , the resultants of the arithmetic operations in all the below
    // five cases have been assigned to the variable 'result' using the assignment operator(=)***

    result = a + b;
    printf("addition of A = %d And B = %d gives %d.\n",a,b,result);

    result = a - b;
    printf("Subtraction of A = %d And B = %d gives %d.\n",a,b,result);

    result = a / b;
    printf("Multiplication of A = %d And B = %d Gives Quotient %d.\n",a,b,result);

    result = a % b;
    printf("Division of A = %d And B = %d Gives Reminder %d.\n",a,b,result);

    printf("\n\n");
    return (0);
}
