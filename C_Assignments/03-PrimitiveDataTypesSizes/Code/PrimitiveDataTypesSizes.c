#include<stdio.h>

int main(void)
{
    // code
    printf("\n\n");

    printf("size of 'int'                   = %ld bytes\n", sizeof(int));
    printf("size of 'unsigned int'          = %ld bytes\n", sizeof(unsigned int));
    printf("size of 'long'                  = %ld bytes\n", sizeof(long));
    printf("size of 'long long'             = %ld bytes\n", sizeof(long long));

    printf("size of 'float'                 = %ld bytes\n", sizeof(float));
    printf("size of 'double'                = %ld bytes\n", sizeof(double));
    printf("size of 'long double'           = %ld bytes\n", sizeof( long double));

    printf("\n\n");

    return (0);
}
