#include<stdio.h>
int main(void)
{
    //variable decalaration
    int num;

    //code
    printf("\n\n");

    printf("Enter Value For 'num' : ");
    scanf("%d",&num);

    if(num < 0) // 'if' - 01
    {
        printf("Num = %d Is Less Than 0 (NEGATIVE) !!!\n\n",num);
    }
    else // 'else' - 01
    {
        if((num > 0) && num <= 100) // 'if' -02
        {
            printf("Num = %d Is Between 0 And 100 !!!\n\n", num);
        }
        else // 'else' -02
        {
            if((num > 100) && (num <=200)) // 'if' -03
            {
                printf("Num = %d Is Between 100 And 200!!!\n\n",num);
            }
            else
            {
                if((num > 200) && (num <= 300)) // 'if' -04
                {
                    printf("Num = %d Is Between 200 And 300 !!!\n\n",num);
                }
                else // 'else' -04
                {
                    if((num > 300) && (num <= 400))//  'if' -05
                    {
                        printf("Num = %d Is Between 300 and 400 !!!\n\n",num);
                    }
                    else //'else' -05
                    {
                        if((num > 400) && (num <= 500)) // 'if' -06
                        {
                            printf("Num =%d Is Betwwen 400 And 500 !!!\n\n",num);
                        }
                        else // 'else' -06
                        {
                            printf("Num = %d Is greater Than 500 !!!\n\n",num);
                        }//closinf brace of else -06
                    }//closinf brace of else -05
                }//closinf brace of else -04
            }//closinf brace of else -03
        }//closinf brace of else -02
    }//closinf brace of else -01

    return (0);
}