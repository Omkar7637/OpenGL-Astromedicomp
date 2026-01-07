#include<stdio.h> // stdio.h contains declaration of printf()

int main(int argc, char *argv[], char *envp[])
{
    // function prototype
    int MyAddition(void);

    // variable declartion : local variables to main()
    int result;

    //code

    result = MyAddition(); // function call

    printf("\n\n");
    printf("SUN = %d\n\n",result);
    return (0);
}

int MyAddition(void)
{
    // varible decalartion : local variables to MyAddition()
    int a, b, sum;

    //code
    printf("\n\n");
    printf("enter Integer Value For 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' : ");
    scanf("%d", &b);

    sum = a + b;
    return (sum);
}