#include<stdio.h> // 'stdio.h' contains declaration of 'printf()'
#include<stdlib.h> // 'stdlin.h' contains declaration of 'exit()'

int main(int argc, char *argv[], char *envp[])
{
    // variable decalarations
    int i;

    // code
    if(argc != 4) // Program name + first name + middle name + surname = 4 command line argumnets are required
    {
        printf("\n\n");
        printf("Invalid Usage !!! Exitting Now ... \n\n");
        printf("usage : CommandLineArgumentsApplication <first name> <middle name> <surname> \n\n");
        exit(0);
    }

    printf("\n\n");
    printf("your Full Name Is : ");
    for(i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }

    printf("\n\n");

    return(0);
}
