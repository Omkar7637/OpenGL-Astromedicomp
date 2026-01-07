#include<stdio.h>

// *** GLOBAL SCOPE ***

// If not initialized by us, global variables are initialized to their zero values 
// (with respect to their data types i.e: 0 for int, 0.0 for float and double,etc.)by default.
// But still, for good programming discipline, we shall explicitly initialize our gloabl variable with 0.

int global_count = 0;
int main(void)
{
    // funtion prototypes
    void change_count_one(void);
    void change_count_two(void);
    void change_count_three(void);

    // code
    printf("\n");

    printf("main() : Value Of global_count = %d\n", global_count);

    change_count_one();
    change_count_two();
    change_count_three();

    printf("\n");
    return (0);
}

// *** GLOBAL SCOPE ***

void change_count_one(void)
{
    // code
    global_count = 100;
    printf("Chnage_coun_one() : Value of globa_count = %d\n", global_count);
}

// *** GLOABL SCOPE ***

void change_count_two(void)
{
    // code
    global_count = global_count + 1;
    printf("Chnage_count_two() : Value of global_count = %d\n", global_count);
}

// ***GLOABL SCOPE ***

void change_count_three(void)
{
    // code
    global_count = global_count + 10;
    printf("chnage_count_three() : Value of global_count = %d\n", global_count);
}

// *** GLOBAL SCOPE ***