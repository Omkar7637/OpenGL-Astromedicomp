#include<stdio.h>

// *** GLOBAL SCOPE ***
int main(void)
{
    // *** LOCAL SCOPE OF main() begins ***

    // variable declarations
    // 'a' is a local variable. It is local to main() only.
    int a = 5;

    // function prototypes
    void change_count(void);

    // code
    printf("\n");
    printf("A = %d\n\n",a);

    // local_count is initializaed to 0.
    // local_count = local_count + 1 = 0 + 1 = 1
    change_count();

    change_count();

    change_count();

    return (0);
}

void change_count(void)
{
    static int local_count = 0;
    local_count = local_count + 1;
    printf("Local Count = %d \n", local_count);
}