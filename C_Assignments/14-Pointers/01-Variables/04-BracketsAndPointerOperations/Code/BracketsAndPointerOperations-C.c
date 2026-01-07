#include<stdio.h>

int main(void)
{
    // variable decalarations

    int num;
    int *ptr = NULL;
    int ans;

    // code
    num = 5;
    ptr =&num;

    printf("\n\n");
    printf("num     =%d\n", num);
    printf("&num    =%p\n", &num);
    printf("*(&num) =%d\n", *(&num));
    printf("ptr     =%p\n", ptr);
    printf("*ptr    =%d\n", *ptr);

    printf("\n\n");

    printf("Answer Of (ptr + 10)    = %p\n", (ptr + 10));
    printf("Answer Of *(ptr + 10)    = %d\n", *(ptr + 10));
    printf("Answer Of (*ptr + 10)    = %d\n\n", (*ptr + 10));

    ++*ptr;

    printf("Answer Of ++*ptr    :   %d\n", *ptr);

    *ptr++;

    printf("Answer Of *ptr++    :   %d\n", *ptr);

    ptr = &num;

    (*ptr)++;

    printf("Answer Of (*ptr)++    :   %d\n\n", *ptr);

    return(0);
    

}