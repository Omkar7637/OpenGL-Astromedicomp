#include<stdio.h> // 'stdio.h' contains declaration of 'printf()'

int main(int argc, char *argv[], char *envp[])
{
    //Function prototype / declaration / signature
    void MyAddition(void);

    //code
    MyAddition(); // function call
    return(0);
}

void MyAddition(void)
{
    // variable declaration : local variables to MyAddition()
    int a, b, sum;

    //code 
    printf("\n\n");
    printf("Enter Integer Value For 'A' : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Integer value For 'B' : ");
    scanf("%d",&b);

    sum = a + b;

    printf("\n\n");
    printf("Sum Of %d and %d = %d\n\n", a, b, sum);
    
}