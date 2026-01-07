#include <stdio.h>

int main(void)
{
    // variable declarations 
    int num;
    int *ptr = NULL;
    int *copy_ptr = NULL;

    // code
    num = 5;
    ptr = &num;

    printf("****** BEFORE copy_ptr = ptr ******\n\n");
    printf("num     =%d\n", num);
    printf("&num    =%p\n", &num);
    printf("*(&num) =%d\n", *(&num));
    printf("ptr     =%p\n", ptr);
    printf("*ptr    =%d\n", *ptr);

    copy_ptr = ptr; // copy_ptr = ptr = &num

    printf("\n\n");
    printf("****** AFTER copy_ptr = ptr ******\n\n");
    printf("num     =%d\n", num);
    printf("&num    =%p\n", &num);
    printf("*(&num) =%d\n", *(&num));
    printf("ptr     =%p\n", ptr);
    printf("*ptr    =%d\n", *ptr);
    printf("copy_ptr = %p\n", copy_ptr);
    printf("*copy_ptr = %d\n", *copy_ptr);

    return(0);


}