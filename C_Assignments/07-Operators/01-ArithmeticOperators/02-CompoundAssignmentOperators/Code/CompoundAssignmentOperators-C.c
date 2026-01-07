#include<stdio.h>

int main(void)
{
    //variable declarations

    int a;
    int b;
    int x;

    //code
    printf("\n\n");
    printf("Enter A Number : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Another Number : ");
    scanf("%d",&b);

    printf("\n\n");

    //since, In all the followng 5 case, the operand on the left 'a' is
    //getting repeated immeiately on the right (e.g : a = a + b or a = a-b),
    //we are using compund assignment operators +=, -=,*=,/= and %=

    //since, 'a' will be be assigned the value of (a +b)at the experssion (a += b),
    //we must save the original value of 'a' to another variable (x)

    x = a;
    a +=b; // a = a +b;
    printf("Addition of A = %d And B = %d Gives %d.\n", x,b,a);

    //Value of 'a' Altered in the in the above expression is used here...
    //since, 'a' will be assigned the value of 'a' to another varible (x)
    x = a;
    a -= b; // a = a - b
    printf("Subtraction of A = %d And B = %d Gives %d.\n",x,b,a);

    //value of 'a' altered in the above expression is used here....
    //since, 'a' will be assigned the value of (a * b) at the expression (a *= b),
    // we must save original value of 'a' to another variable (x)

    x = a;
    a *=b;// a = a * b
    printf("Multplication of A = %d And B = %d gives %d.\n",x,b,a);

    //value of 'a' altered in the above expression is used here...
    //since, we must save the original value of 'a' to another variable (x)

    x = a;
    a %= b; // a = a % b

    printf("Division of A = %d And B = %d gives Remainder %d.\n",x,b,a);

    printf("\n\n");

    return(0);


}