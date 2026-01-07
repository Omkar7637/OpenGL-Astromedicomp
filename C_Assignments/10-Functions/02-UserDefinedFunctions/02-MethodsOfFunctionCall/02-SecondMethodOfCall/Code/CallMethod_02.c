#include<stdio.h> // 'stdio.h' contains declaration of 'printf()'

// ***** USER DEFINE FUNCTIONS : METHOD OF CALLING FUNCTION 2 ******
// ***** CALLING ONLY TWO FUNCTIONS DIRECTLY IN main(), REST OF THE FUNCTIONS TRACE THEIR CALL INDERECTLY TO main() *****

int main(int argc, char *argv[], char *envp[])
{
    // function prototypes
    void display_information(void);
    void Function_Country(void);

    //code
    display_information(); // function call
    Function_Country(); // Function call

    return (0);
}

// ******* User-Defined Functions' Definitions... ***
void display_information(void) // function defination
{
    // function prototypes
    void Function_my(void);
    void Function_name(void);
    void Function_is(void);
    void Function_FirstName(void);
    void Function_MiddleName(void);
    void Function_Surname(void);
    void Function_OfAMC(void);

    // code

    // *** FUNCTION CALLS ***
    Function_my();
    Function_name();
    Function_is();
    Function_FirstName();
    Function_MiddleName();
    Function_Surname();
    Function_OfAMC();
}

void Function_my(void) // function defination
{
    // code
    printf("\n\n");
    printf("My");
}

void Function_name(void) // function defination
{
    // code
    printf("\n\n");
    printf("Name");
}

void Function_is(void) // function defination
{
    // code
    printf("\n\n");
    printf("Is");

}

void Function_FirstName(void) // function defination
{
    // code
    printf("\n\n");
    printf("Omkar");

}

void Function_MiddleName(void) // function defination
{
    // code
    printf("\n\n");
    printf("Ankush");
}

void Function_Surname(void) // function defination
{
    // code
    printf("\n\n");
    printf("Kashid");
}

void Function_OfAMC(void) // function defination
{
    //code
    printf("\n\n");
    printf("Of ASTROMEDICOMP");
}

void Function_Country(void) // function defination
{
    //code
    printf("\n\n");
    printf("I live in India.");
    printf("\n\n");
}