#include <stdio.h>
#include <stdarg.h>

// Function prototype
int CalculateSum(int num, va_list list);
int CalculateSumWrapper(int num, ...);

int main(void)
{
    // variable declarations
    int answer;

    // code
    printf("\n\n");

    answer = CalculateSumWrapper(5, 10, 20, 30, 40, 50);
    printf("Answer = %d\n\n", answer);

    answer = CalculateSumWrapper(10, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
    printf("Answer = %d\n\n", answer);

    answer = CalculateSumWrapper(0);
    printf("Answer = %d\n\n", answer);

    return 0;
}

int CalculateSum(int num, va_list list)
{
    // variable declarations
    int sum_total = 0;
    int n;

    // code
    while (num)
    {
        n = va_arg(list, int);
        sum_total += n;
        num--;
    }

    return sum_total;
}

int CalculateSumWrapper(int num, ...)
{
    va_list list;
    int sum;

    va_start(list, num); 
    sum = CalculateSum(num, list); 
    va_end(list); 

    return sum;
}
