#include<stdio.h>
int main(void)
{
    // variable declaration
    int num;

    //code
    printf("\n\n");

    printf("Enter Value for 'num' : ");
    scanf("%d",&num);

    if(num < 0)
    {
        printf("Num = %d Is Less Than 0 (Negative).\n\n",num);
    }

    if((num > 0) && (num <= 100))
    {
        printf("Num = %d Is Betwwen 0 and 100.\n\n",num);
    }

    if((num > 100) && (num <= 200))
    {
        printf("Num = %d Is Between 100 And 200.\n\n",num);
    }

    if((num > 200) && (num <= 300))
    {
        printf("Num = %d Is Between 200 and 300.\n\n",num);
    }

    if((num > 300) && (num <= 400))
    {
        printf("Num = %d Is Between 300 and 400.\n\n",num);
    }
    
    if((num > 400) && (num <= 500))
    {
        printf("Num = %d Is Between 400 and 500.\n\n",num);
    }

    if(num > 500)
    {
        printf("Num = %d Is greater Than 500.\n\n",num);
    }

    return(0);

}