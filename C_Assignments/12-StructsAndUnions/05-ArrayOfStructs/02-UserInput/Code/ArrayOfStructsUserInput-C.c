#include<stdio.h>
#include<ctype.h>

#define NUM_EMPLOYEES 5 

#define NAME_LENGTH 100
#define MARTIAL_STATUS 10

struct Employee
{
    char name[NAME_LENGTH];
    int age;
    char sex;
    float salary;
    char marital_status;
};

int main(void)
{
    // function prototype 
    void MyGetString(char[], int);

    // variable decalarations 
    struct Employee EmployeeRecord[NUM_EMPLOYEES];
    int i;

    // code

    // user input 
    for(i = 0; i < NUM_EMPLOYEES; i++)
    {
        printf("\n\n\n\n");
        printf("****** DATA ENTRY FOR EMPLOYEE NUMBER %d ******\n", (i + 1));

        printf("\n\n");
        printf("Enter Employee Name : ");
        MyGetString(EmployeeRecord[i].name, NAME_LENGTH);

        printf("\n\n\n");
        printf("Enter Employee's Age (in Years) : ");
        scanf("%d", &EmployeeRecord[i].age);

        printf("\n\n");
        printf("Enter Emplyee's Sex (M/m For Male, F/f For Female) : ");
        EmployeeRecord[i].sex = getch();
        printf("%c", EmployeeRecord[i].sex);
        EmployeeRecord[i].sex = toupper(EmployeeRecord[i].sex);

        printf("\n\n\n");
        printf("Enter Employee's Salary (in Indian Rupees) : ");
        scanf("%f", &EmployeeRecord[i].salary);

        printf("\n\n");
        printf("Is The Emloyee Married? (Y/y For yes, N/n For no) : ");
        EmployeeRecord[i].marital_status = getch();
        printf("%c", EmployeeRecord[i].marital_status);
        EmployeeRecord[i].marital_status = toupper(EmployeeRecord[i].marital_status);
    }

    // Display
    printf("\n\n\n\n");
    printf("****** DISPLAY EMPLOYEE RECORDS ******\n\n");
    for(i = 0; i <NUM_EMPLOYEES; i++)
    {
        printf("****** EMPLOYEE NUMBER %d ******\n\n", (i + 1));
        printf("Name            : %s\n", EmployeeRecord[i].name);
        printf("Age             : %d Years\n", EmployeeRecord[i].age);

        if(EmployeeRecord[i].sex == 'M')
            printf("Sex             : Male\n");
        else if(EmployeeRecord[i].sex == 'F')
            printf("Sex             : Female\n");
        else
            printf("Sex             : Invalid Data Entered\n");

        printf("Salary         : Rs. %f\n", EmployeeRecord[i].salary);

        if(EmployeeRecord[i].marital_status == 'Y')
            printf("Marital Status : Married\n");
        else if(EmployeeRecord[i].marital_status == 'N')
            printf("Marital Status : Unmarried\n");
        else
            printf("Marital Status : Invalid Data Entered\n");
            
        printf("\n\n");

    }
    
    return(0);            
}

void MyGetString(char str[], int str_size)
{
    // variable decalarations 
    int i;
    char ch = '\0';

    // code
    i = 0;
    do
    {
        ch = getch();
        str[i] =ch;
        printf("%c", str[i]);
        i++;
    }while ((ch != '\r') && (i < str_size));

    if( i == str_size)
        str[i - 1] = '\0';
    else
        str[i] = '\0';
}
