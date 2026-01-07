#include<stdio.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACTERS_PER_STRING 20

#define ALPHABET_BEGINNING 65 // 'A' 

struct MyDataOne
{
    int iArray[INT_ARRAY_SIZE];
    float fArray[FLOAT_ARRAY_SIZE];
};

struct MyDataTwo
{
    char cArray[CHAR_ARRAY_SIZE];
    char strArray[NUM_STRINGS][MAX_CHARACTERS_PER_STRING];
};

int main(void)
{
    // variable decalartions
    struct MyDataOne data_one;
    struct MyDataTwo data_two;
    int i;

    // code
    data_one.fArray[0] = 0.1f;
    data_one.fArray[1] = 1.2f;
    data_one.fArray[2] = 2.3f;
    data_one.fArray[3] = 3.4f;
    data_one.fArray[4] = 4.5f;

    printf("\n\n");
    printf("Enter %d Integer : \n\n", INT_ARRAY_SIZE);
    for(i = 0; i < INT_ARRAY_SIZE; i++)
    {
        scanf("%d", &data_one.iArray[i]);
    }

    for(i = 0; i < CHAR_ARRAY_SIZE; i++)
    {
        data_two.cArray[i] = (char)(i + ALPHABET_BEGINNING);
    }

    strcpy(data_two.strArray[0], "Welcome !!!");
    strcpy(data_two.strArray[1], "This");
    strcpy(data_two.strArray[2], "Is");
    strcpy(data_two.strArray[3], "ASTROMEDICOMP");
    strcpy(data_two.strArray[4], "Real");
    strcpy(data_two.strArray[5], "Time");
    strcpy(data_two.strArray[6], "Rendering");
    strcpy(data_two.strArray[7], "Batch");
    strcpy(data_two.strArray[8], "Of");
    strcpy(data_two.strArray[9], "2024-2025");

    printf("\n\n");
    printf("Members Of 'struct DataOne' Alongwith Their Assigned Values Are : \n\n");

    printf("\n\n");
    printf("Integer Array (data_one.iArray[]) : \n\n");
    for(i = 0; i < INT_ARRAY_SIZE; i++)
        printf("data_one.iArray[%d] = %d\n", i, data_one.iArray[i]);

    printf("\n\n");
    printf("Floating-point Array (data_one.fArray[]) : \n\n");
    for(i = 0; i < FLOAT_ARRAY_SIZE; i++)
        printf("dat_one.fArray[%d] = %f\n", i, data_one.fArray[i]);

    printf("\n\n");
    printf("Members Of 'struct DataTwo' Alongwith Their Assigneed Values Are : \n\n");

    printf("\n\n");
    printf("Charater Array (data_two.cArray[]) : \n\n");
    for(i = 0; i < CHAR_ARRAY_SIZE; i++)
        printf("data_two.cArray[%d] = %c\n", i, data_two.cArray[i]);

    printf("\n\n");
    printf("String Array (data_two.strArray[]) : \n\n");
    for(i = 0; i < NUM_STRINGS; i++)
        printf("%s ", data_two.strArray[i]);

    printf("\n\n");

    return(0);

}