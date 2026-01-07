#include<stdio.h>

int main(void)
{
    // variable decalaration
    int age;

    //code
    printf("\n\n");
    printf("Enter Age : ");
    scanf("%d", &age);
    printf("\n\n");
    if(age >= 18)
    {
        printf("Entering if-block...\n\n");
        printf("You Are Eligible For Voting !!!\n\n");
    }
    else
    {
        printf("Entering Else-block...\n\n");
        printf("You Are NOT Eligible For Voting !!!\n\n");
    }
    printf("Bye!!!\n\n");

    return (0);
}
