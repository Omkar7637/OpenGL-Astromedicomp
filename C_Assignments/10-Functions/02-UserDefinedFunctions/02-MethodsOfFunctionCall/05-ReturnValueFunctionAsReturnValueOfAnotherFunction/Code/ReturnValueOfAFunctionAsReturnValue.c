#include<stdio.h>
int main(int argc, char *argv[], char *envp[])
{
    //function prototypes or declarations
    int MyAddition(int, int);

    // variable dec;arations 
    int r;
    int num_01, num_02;

    //code
    num_01 = 10;
    num_02 = 20;

    printf("\n\n");
    printf("%d + %d = %d\n", num_01, num_02, MyAddition(num_01, num_02));
    printf("\n\n");

    return (0);
}


// *** Function definition of MyAddition() ***
int MyAddition(int a, int b) // function definition
{
    // function prototype
    int add(int, int);

    // code
    return (add(a, b)); // return value of function add() as return value of function MyAddition()
    
}

int add(int x, int y)
{
    // code
    return(x + y);
}