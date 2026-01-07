#include<stdio.h>

// *** GLOABL SCOPE ***
int global_count = 0;

int main(void)
{
    // function prototypes
    void change_count(void);
    void change_count_one(void); // function define in File_01.c
    void change_count_two(void); // functiom define in File_02.c

    // code
    printf("\n");

    change_count();
    change_count_one(); // function define in File_01.c
    change_count_two(); // functiom define in File_02.c

    return (0);
}

void change_count(void)
{
    // code
    global_count = global_count + 1;
    printf("Global Count = %d\n", global_count);
}