#include<stdio.h>

int main(void)
{
    // variable decalaration
    int num_month;

    //code
    printf("\n\n");

    printf("Enter number of month (1 to 12) : ");
    scanf("%d", &num_month);

    printf("\n\n");

    switch(num_month)
    {
        case 1: // like 'if'
            printf("month number %d is JANUARY !!!\n\n", num_month);
            break;

        case 2: //like 'else if'
            printf("month number %d is FEBRUARY !!!\n\n", num_month);
            break;
        
        case 3: //like 'else if'
            printf("month number %d is MARCH !!!\n\n", num_month);
            break;

        case 4: //like 'else if'
            printf("moNth number %d is APRIL !!!\n\n", num_month);
            break;

        case 5: //like 'else if'
            printf("month number %d is MAY !!!\n\n", num_month);
            break;

        case 6: //like 'else if'
            printf("month number %d is June !!!\n\n", num_month);
            break;

        case 7: //like 'else if'
            printf("month number %d is JULY !!!\n\n", num_month);
            break;

        case 8: //like 'else if'
            printf("month number %d is AUGEST !!!\n\n", num_month);
            break;

        case 9: //like 'else if'
            printf("month number %d is SEPTEMBER !!!\n\n", num_month);
            break;

        case 10: //like 'else if'
            printf("month number %d is OCTOMBER !!!\n\n", num_month);
            break;

        case 11: //like 'else if'
            printf("month number %d is NOVEMBER !!!\n\n", num_month);
            break;

        case 12: //like 'else if'
            printf("month number %d is DECEMBER !!!\n\n", num_month);
            break;

        default: // like ending optional 'else'... just like termenating 'else' is opetianal in if-else ladder,so is the 'default' case opetinal in switch-case
            printf("Inavalid month number %d Entered !!! Please Try again...\n\n",num_month);
            break;         
    }

    printf("Switch case Block Complete !!!\n");

    return (0);
}