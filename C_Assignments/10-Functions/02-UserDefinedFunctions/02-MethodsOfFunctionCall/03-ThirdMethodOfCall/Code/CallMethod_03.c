#include<stdio.h> // 'stdio.h' contains declaration of 'printf()'

// ******* USER DEFINED FUNCTION : METHOD OF CALLING FUNCTION 3 *******
// ******* CALLING ONLY ONE FUNCTION DIRECTLY IN main(), REST OF THE FUNTIONS TRAVE THEIR CALL INDIRECTLY TO main()*****

int main(int argc, char *argv[], char *envp[])
{
    // function prototypes
    void Function_Country();

    //code
    Function_Country();
    return (0);
}


void Function_Country(void) // function defination
{
    // Function declarations
    void Function_OfAMC(void);

    //code
    Function_OfAMC();
    printf("\n\n");
    printf("I live In INDIA.");
    printf("\n\n");
}

void Function_OfAMC(void) // function defination
{
    // function prototype
    void Function_Surname(void);

    //code
    Function_Surname();

    printf("\n\n");
    printf("Of ASTROMEDICOMP");
}

void Function_Surname(void) // function Defination
{
    // function prototypes
    void Function_MiddleName(void);

    //code
    Function_MiddleName();

    printf("\n\n");
    printf("Kashid");
}

void Function_MiddleName(void) // function defination
{
    // function declarations
    void Function_FirstName(void);

    //code
    Function_FirstName();

    printf("\n\n");
    printf("Ankush");
}


void Function_FirstName(void) // Function Defination
{
    // function declarations
    void Function_Is(void);

    // code
    Function_Is();

    printf("\n\n");
    printf("Omkar");
}


void Function_Is(void) // Function defination
{
    // Function declaration
    void Function_Name(void);

    //code
    Function_Name();

    printf("\n\n");
    printf("Is");
}


void Function_Name(void) // function defination
{
    // function declarations
    void Function_My(void);

    //code
    Function_My();
    printf("\n\n");
    printf("Name");
}

void Function_My(void) // function defination 
{
    // code
    printf("\n\n");

    printf("My");
}