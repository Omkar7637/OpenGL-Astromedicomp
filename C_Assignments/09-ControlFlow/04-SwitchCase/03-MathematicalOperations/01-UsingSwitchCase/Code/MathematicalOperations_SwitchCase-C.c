#include<stdio.h> // for printf()
#include<conio.h> //for getch()

int main(void)
{
    // variable declaration
    int a, b;
    int result;

    char option, option_division;

    //code
    printf("\n\n");

    printf("ENter value For 'A' : ");
    scanf("%d",&a);

    printf("Enter value For 'B' : ");
    scanf("%d",&b);

    printf("Enter Option In charater : \n\n");
    printf("'A' or 'a' For ADdition : \n");
    printf("'S' or 's' For Subtraction : \n");
    printf("'M' or 'm' For multiplication : \n");
    printf("'D' or 'd' For Division : \n\n");

    printf("Enter option : ");
    option = getch();

    printf("\n\n");

    switch(option)
    {
        // FALL THROUGH CONSITION FOR 'A' AND 'a'
        case 'A':
        case 'a':
            result = a +b;
            printf("Addition of A = %d And B = %d Gives Result %d !!!\n\n",a,b,result);
            break;

        // FALL THROUGH CONSITION FOR 'S' and 's'
        case 'S':
        case 's':
            if (a >= b)
            {
                result = a - b;
                printf("Subtraction of B = %d From A = %d Gives Result %d !!!\n\n",b,a,result);
            }
            else
            {
                result = b - a;
                printf("subtarction Of A = %d From B = %d Gives Result %d !!!\n\n",a,b,result);
            }
            break;

        //FALL  THROUGH CONSITION FOR 'M' and 'm'
        case 'M':
        case 'm':
            result = a * b;
            printf("multiplication of A = %d And B %d Gives Result %d !!!\n\n", a, b,result);
            break;

        //FALL THROUGH CONSITION FOR 'D' AND 'd'
        case 'D':
        case 'd':
            printf("Enter option In Character : \n\n");
            printf("'Q' or 'q' or '/' For Quotient Upon Division : \n");
            printf("'R' or 'r' '%%' For Reminder Upon Dividion : \n");

            printf("Enter Option : ");
            option_division = getch();

            printf("\n\n");

            switch(option_division)
            {
                //FALL THROUGH CONSITION FOR 'Q' and 'q' and '/' 
                case 'Q':
                case 'q':
                case '/':
                if(a >= b)
                {
                    result = a / b;
                    printf("Division of A = %d By B = %d Gives Quotient = %d !!!\n\n",a,b,result);
                }
                break; // 'break' of case 'Q' or case 'q' or case '/'

                // FALL THROUGH CONSITION FOR 'R' and 'r' and '%'

                case 'R':
                case 'r':
                case '%':
                    if(a >= b)
                    {
                        result = a % b;
                        printf("Division of a = %d By B = %d Gives remonder = %d !!!\n\n",a,b,result);
                    }
                    else
                    {
                        result = b % a;
                        printf("Division of B = %d By A = %d Gives reminder = %d !!!\n\n",b,a,result);
                    }
                    break;
                    // 'break' of case 'R' or case 'r' or case '%'

                default: // 'default' case for switching(option_deivision)
                    printf("Invalid Charater %c Enetred For Division !!! Please Try Again... \n\n", option_division);
                    break;
                    
                }//ending curly brace of switch (option_devision)

        default:
            printf("Invalid charter %c Enetred !!! Please Try AGin....\n\n",option_division);
            break;

    }

    printf("switch case block complete !!!\n");

    return (0);
}