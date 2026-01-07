#include<stdio.h>
int main(void)
{
    // variable decalarations
    int i_num_ok, num_ok, i_ok;

    // code
    printf("\n\n");

    printf("Enter An Integer Value From Which Iteration Must Begin : ");
    scanf("%d", &i_num_ok);

    printf("How many Digits Do you Want To print From %d Onwards ? : ",i_num_ok);
    scanf("%d", &num_ok);

    printf("Printing Digits %d to %d : \n\n", i_num_ok, (i_num_ok + num_ok));
    i_ok = i_num_ok;

    while(i_ok <= (i_num_ok + num_ok))
    {
        printf("\t%d\n",i_ok);
        i_ok++;
    }
    printf("\n\n");

    return (0);
}