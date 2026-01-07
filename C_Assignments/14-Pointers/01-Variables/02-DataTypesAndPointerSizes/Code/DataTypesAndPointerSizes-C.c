#include<stdio.h>

struct Employee
{
    char name[100];
    int age;
    float salary;
    char sex;
    char marital_satus;
};

int main(void)
{
    // code
    printf("\n\n");
    printf("SIZE OF DATA AND POINTERS TO THOSE RESPECTIVE DATA TYPES ARE : \n\n");
    printf("Size of (int)               : %d \t \t \t Size of Pointer to int (int*)                        : %d\n\n", sizeof(int), sizeof(int*));
    printf("Size of (float)             : %d \t \t \t Size of Pointer to float (float*)                    : %d\n\n", sizeof(float), sizeof(float*));
    printf("Size of (double)            : %d \t \t \t Size of Pointer to double (double*)                  : %d\n\n", sizeof(double), sizeof(double*));
    printf("Size of (char)              : %d \t \t \t Size of Pointer to char (char*)                      : %d\n\n", sizeof(char), sizeof(char*));
    printf("Size of (struct Employee)   : %d \t \t \t Size of Pointer to struct Employee (struct Employee*): %d\n\n", sizeof(struct Employee), sizeof(struct Employee*));

    return(0);
}