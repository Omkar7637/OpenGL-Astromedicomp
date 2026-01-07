#include<stdio.h>

int main(void)
{
    // varibble declartion
    int num;

    //code
    printf("\n\n");
    printf("ENter value for 'num' : ");
    scanf("%d",&num);

    // IF -ELSE - IF Ladder begind from here...
    if(num < 0)
        printf("Num = %d is less than 0 (negative) !!! \n\n",num);

    else if ((num > 0) && (num <=100))
        printf("num = %d is between 0 and 100 !!!\n\n",num);

    else if ((num > 100) && (num <= 200))
        printf("Num = %d is betwwen 100 and 200 !!!\n\n",num);

    else if((num > 200) && (num <= 300))
        printf("Num = %d is betwwen 200 and 300 !!!\n\n",num);
    
    else if((num > 300) && (num <= 400))
        printf("Num = %d is betwwen 300 and 400 !!!\n\n",num);

    else if((num > 400) && (num <= 500))
        printf("Num = %d is betwwen 400 and 500 !!!\n\n",num);

    else if(num > 500)
        printf("Num = %d is greater than 500 !!!\n\n",num);

    //*** np terminating 'else' in this ladder !!! ***

    return (0);

}