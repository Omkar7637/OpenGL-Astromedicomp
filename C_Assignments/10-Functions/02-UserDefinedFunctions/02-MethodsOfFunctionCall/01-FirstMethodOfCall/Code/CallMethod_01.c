#include<stdio.h> // 'stdio.h' contains decalaration of 'printf()'

// ****** USER DEFINE fUNCTIONS : METHOD OF CALLING FUNCTION 1 ******
// ****** CALLING ALL FUNCTION IN main() DIRECT;Y ******

// Entry-point function => main() => Valid return type (int) and 3 parameters (int argc, char *argv[], char *envp[])

int main(int argc, char *argv[], char *envp[])
{
    // function prototypes OR declarations 
    void MyAddition(void);
    int MySubtraction(void);
    void MyMultiplication(int, int);
    int MyDivision(int, int);

    // varible decalaration
    int result_subtraction;
    int a_multiplication, b_multiplication;
    int a_division, b_division, result_division;

    // code

    // *** ADDITION ***
    MyAddition(); // Function call

    // *** SUBTRACTION ***
    result_subtraction = MySubtraction(); // function call
    printf("\n\n");
    printf("Subtraction Yields Result = %d\n", result_subtraction);

    // *** MULTIPLICATION ***
    printf("\n\n");
    printf("ENter Integer Value For 'A' For Multiplication : ");
    scanf("%d",&a_multiplication);

    printf("\n\n");
    printf("ENter Integer value for 'B' For Multiplication : ");
    scanf("%d", &b_multiplication);

    MyMultiplication(a_multiplication, b_multiplication); // function call    

    // *** DIVISION ***

    printf("\n\n");
    printf("ENter Intger Value for 'A' for division : ");
    scanf("%d",&a_division);

    printf("\n\n");
    printf("ENter Intger Value for 'B' for division : ");
    scanf("%d",&b_division);

    result_division = MyDivision(a_division, b_division); // function call

    printf("\n\n");

    printf("Division of %d and %d Gives = %d (Quotient)\n", a_division, b_division, result_division);

    printf("\n\n");
    return(0);
}

// *** Function defination of MyAddition() *******

void MyAddition(void)// function defination
{
    // varible decalartions : local variables to MyAddition()
    int a, b, sum;

    // code
    printf("\n\n");
    printf("Enter Intger Value for 'A' Addition : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Intger Value for 'B' addition : ");
    scanf("%d", &b);

    sum = a + b;

    printf("\n\n");
    printf("Sum of %d And %d = %d \n\n", a, b, sum);
}

// *** Function Definataion of MySubtarction() ******

int MySubtraction(void) // function defination
{
    // variable declaration :  local variables to MySubtarction()
    int a, b, subtraction;

    // code
    printf("\n\n");
    printf("Enter Intger Value fro 'A' For Subtraction : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Intger Value for 'B' for Subtarction : ");
    scanf("%d", &b);

    subtraction = a - b;
    return(subtraction);
}


// *** Function definition of MyMultiplication() *****
void MyMultiplication(int a, int b) // function defination
{
    // varible declaration : local variables to MyMultiplication()
    int multiplication;

    //code
    multiplication = a * b;
    printf("\n\n");
    printf("Multiplication of %d ANd %d = %d \n\n", a, b, multiplication);
}


// *** Function Defination of Mydivision() ****
int MyDivision(int a, int b)// function definition
{
    // variable declaration : loacl variables to MyDivision()
    int division_quotient;

    //code
    if(a > b)
    {
        division_quotient = a / b;
    }
    else
    {
        division_quotient = b / a;
    }

    return(division_quotient);
}