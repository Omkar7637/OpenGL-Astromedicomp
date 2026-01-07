#include<stdio.h> // 'stdio.h' contains decalartion of 'printf()'

// Entery point Function => main() => valid return type (int) and 3 parameters (int argc, char *argv[], char *envp[])

int main(int argc, char *argv[], char *envp[])
{
    // function prototype
    int MyAddition(int, int);

    //variable declaration : local variable to main()
    int a, b, result;

    // code
    printf("\n\n");
    printf("Enter Integer Value for 'A' : ");
    scanf("%d", & a);

    printf("\n\n");
    printf("Enter Integer Value For 'B' : ");
    scanf("%d", &b);
    result = MyAddition(a, b); // Function call

    printf("\n\n");

    printf("Sum of %d And %d = %d\n\n", a, b, result);

    return (0);
}

// ****** USER DEFINE FUNCTION : METHOD OF DEFINITION 4 ******
// ****** VALID (int) RETURN VALUE, VALID PARAMETERS (int, int) ******

int MyAddition(int a, int b) // function definition
{
    // variable declaration : local variable to MyAddition()
    int sum;

    //code
    sum = a + b;
    return(sum);
}