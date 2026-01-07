#include<stdio.h> // for printf()
#include<conio.h> // for getch()

int main(void)
{
    // variable decalartions
    int a, b;
    int result;

    char option, option_division;

    //code
    printf("\n\n");

    printf("Enter value fro 'A' : ");
    scanf("%d", &a);

    printf("Enter Value For 'B' : ");
    scanf("%d",&b);

    printf("Enetr option In Character : \n\n");
    printf("'A' or 'a' For Addition : \n");
    printf("'S' or 's' For Subtraction : \n");
    printf("'M' or 'm' For Multiplication : \n");
    printf("'D' or 'd' For Division : \n\n");

    printf("Enter option : ");
    option = getch();

    printf("\n\n");

    if(option == 'A' || option == 'a')
    {
        result = a + b;
        printf("Addition of A = %d And B = %d Gives result %d !!!\n\n",a,b,result);
    }
    else if (option == 'S' || option == 's')
    {
        if(a >= b)
        {
            result = a - b;
            printf("Subtraction of B =%d From A = %d Gives Result %d !!!\n\n",b,a,result);
        }
        else 
        {
            result = b - a;
            printf("Subtraction of A = %d From B = %d Gives Result %d !!!\n\n",a,b,result); 
        }
    }
    else if (option == 'M' || option == 'm')
    {
        result = a * b;
        printf("Multiplication of A = %d And B = %d Gives Result %d !!!\n\n",a,b,result);
    }
    else if (option == 'D' || option == 'd')
    {
        printf("ENetr Option In charater : \n\n");
        printf("'Q' or 'q' or '/' For Quotient Upon Division : \n");
        printf("'R' or 'r' or '%%' For Reminder Upon Division : \n");

        printf("Enter Option : ");
        option_division = getch();

        printf("\n\n");

        if(option_division == 'Q' || option_division == 'q' || option_division == '/')
        {
            if(a < b)
            {
                result = a / b;
                printf("Division of A = %d By B = %d Gives Quotient = %d !!!\n\n",a,b,result);
            }
            else
            {
                result = b /a;
                printf("Division of B = %d By A =%d Gives Quotient = %d !!!\n\n",b,a,result);
            }
        }
        else if(option_division == 'R' || option_division == 'r' || option_division == '%')
        { 
            if(a >= b)
            {
                result = a % b;
                printf("Division of A = %d By B = %d Gives Reminder = %d !!!\n\n",a,b,result);
            }
            else
            {
                result = b % a;
                printf("Division of B = %d By A = %d Gives Reminder = %d !!!\n\n",b,a,result);
            }
        }
        else
        {
            printf("Invalid Charater %c Enetred For Division !!! Please Try Agin...\n\n",option_division);
        }
    }
    else
    {
            printf("Invalid Charater %c Enetred For Division !!! Please Try Agin...\n\n",option);
    }

    printf(" IF - Else If - Else Ladder Complete !!!\n");


    return (0);
}