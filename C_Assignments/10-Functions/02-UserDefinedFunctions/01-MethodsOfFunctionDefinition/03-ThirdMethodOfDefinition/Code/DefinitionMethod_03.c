#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    // function prototype
    void MyAddition(int, int);

    // varible decalaration : loacal variables to main()
    int a, b;

    // code

    printf("\n\n");
    printf("Enter Integer Value for 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Integer Value for 'B' : ");
    scanf("%d", &b);

    MyAddition(a, b); // function call

    return (0);
}


// **** USER DEFINE FUNCTION : METHOD OF DEFINATION 3 ****
// **** NO RETURN VALUE, VALID PARAMETERS (int, int)  ****

void MyAddition(int a, int b)
{
    // variable decalartions : local variables to MyAddition()
    int sum;

    //code
    sum = a + b;

    printf("\n\n");
    printf("Sum of %d And %d = %d \n\n", a, b, sum);
}