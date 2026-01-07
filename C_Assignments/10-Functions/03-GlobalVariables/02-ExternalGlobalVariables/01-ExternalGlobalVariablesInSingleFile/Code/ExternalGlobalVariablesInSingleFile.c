#include<stdio.h>

// *** GLOBAL SCOPE ***

int main(void)
{
    //function prototypes
    void change_count(void);

    // varibale declarations
    extern int global_count;

    // code
    printf("\n");
    printf("value of global_count before chnage_count() = %d\n", global_count);
    change_count();
    printf("Value of global_count after chnage_count() = %d\n", global_count);
    return (0);
}

// *** GLOABL SCOPE ***

// global_count is a global varibale.
// since, it is declared before chnage_count(), it can be accssed and used as any ordinary global variable in chnage_count()
// since, it is declared after main(), it must be firest re-declared in main() as an external global variable by means of the 'extern' keyword and the type of the variable.
// Once this is done, ot can be used as an ordinary global variable in main as well.

int global_count = 0;

void change_count(void)
{
    // code
    global_count = 5;
    printf("Value of global_count in change_count() = %d\n", global_count);
}