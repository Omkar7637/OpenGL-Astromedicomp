#include<stdio.h>

int main(void)
{
    //variable declaration
    int a;
    int b;
    int c;
    int result;

    //code
    printf("\n\n");
    printf("Enter First Integer : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter Second Integer : ");
    scanf("%d",&b);

    printf("\n\n");
    printf("Enter Third Integer : ");
    scanf("%d",&c);

    printf("\n\n");
    printf("If Answer = 0, It is 'FALSE'.\n");
    printf("If Anwer = 1, It is 'TRUE'.\n\n");

    result = (a <= b) && (b != c);
    printf("LOGICAL AND (&&) : Anser is TRUE (1) If And Onaly if both conditions are true. the anser is false (0), If any one or both condition are false.\n\n");
    printf("A = %d Is Less Than or Equal To B = %d AND B = %d Is Not Equal to C = %d            \t Answer = %d\n\n",a,b,b,c,result);

    result = (b >= a) || (a == c);
    printf("LOGICAL OR (||) : Answer is False (0) if And only if Both Conditions Are False. The Anser Is TRUE (1), if any one or both condition are tru.\n\n");
    printf("Either b = %d Is greater Than or Equal to A = Gives %d OR A = %d Is Equal To C = %d \t Answer = %d\n\n",b,a,a,c,result);

    result = !a;
    printf("A = %d And Using logical NOT (!) operator on A Gives Result = %d\n\n",a,result);

    result = !b;
    printf("B = %d And using logical Not (i) Operator on B Gives Result = %d\n\n",b,result);

    result = !c;
    printf("C = %d And using logical Not (!) Operator on C Gives Result = %d\n\n",c,result);
}