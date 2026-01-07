#include<stdio.h>

int main(void)
{
    // function decalrations
    void Mathematicaloperations(int, int, int*, int*, int*, int*, int*);

    // variable declarations
    int a;
    int b;
    int answer_sum;
    int answer_difference;
    int answer_product;
    int answer_quotient;
    int answer_remainder;

    // code
    printf("\n\n");
    printf("Enter Value Of 'A' : ");
    scanf("%d", &a);

    printf("\n\n");
    printf("Enter Value Of 'B' : ");
    scanf("%d", &b);

    Mathematicaloperations(a, b, &answer_sum, &answer_difference, &answer_product, &answer_quotient, &answer_remainder);

    printf("\n\n");
    printf("****** RESULT ****** : \n\n");
    printf("Sum = %d\n\n", answer_sum);
    printf("Difference = %d\n\n", answer_difference);
    printf("Product = %d\n\n", answer_product);
    printf("Quotient = %d\n\n", answer_quotient);
    printf("Reminder = %d\n\n", answer_remainder);

    return(0);

}

void Mathematicaloperations(int x, int y, int *sum, int *difference, int *product, int *quotitent, int *reminder)
{
    //code
    *sum = x + y;
    *difference = x - y;
    *product = x * y;
    *quotitent = x / y;
    *reminder = x % y;
}