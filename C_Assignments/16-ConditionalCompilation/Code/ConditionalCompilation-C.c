#include<stdio.h>

#define MY_PI_VERSION 9 // change value snd see the output, Also, comment this line and line number 34 and see the output.
#ifdef MY_PI_VERSION
#if MY_PI_VERSION <= 0
    #define MY_PI 3.14
#elif MY_PI_VERSION == 1
    #define MY_PI 3.1415
#elif MY_PI_VERSION == 2
    #define MY_PI 3.141592
#elif MY_PI_VERSION == 3
    #define MY_PI 3.14159265
#elif MY_PI_VERSION == 4
    #define MY_PI 3.1415926535
#elif MY_PI_VERSION == 5
    #define MY_PI 3.141592653589
#else
    #define MY_PI 3.141592653589793
#endif
#endif

#ifndef MY_PI_VERSION
    #define MY_PI 0.0
#endif

int main(void)
{
    // code
    printf("\n\n");
    printf("PI Version Selected = %d\n", MY_PI_VERSION);
    printf("PI Value = %.15lf\n", MY_PI);
    return(0);
}
