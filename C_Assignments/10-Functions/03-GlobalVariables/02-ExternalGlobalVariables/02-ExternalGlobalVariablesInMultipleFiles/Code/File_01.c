// *** GLOBAL SCOPE ***

// global_count is a global variable declared in source code file ExternalGlobalVariableInMultipleFiles.c
// To access it in this file, it must first be re-declared as an external variable in the global scope of this file along with the 'exxtern' keyword and its proper data type
// Then, it can be used as any ordinary gloabl variable throughout this file as well

#include<stdio.h>

extern int global_count;

void change_count_one(void)
{
    //code
    global_count = global_count + 1;
    printf("chnage_count_one() : Value of Global_count in File_01 = %d\n",global_count);
}