#include<stdio.h>

int main()
{
    // variable declarations
    int i_num_ok, num_ok, i_ok;

    //code

    printf("\n\n");

    printf("Enter An Integer Value From Which Iteration Must Begin : ");
    scanf("%d",&i_num_ok);

    printf("How many Digits Do You Want To Print From %d Onwards ? : ",i_num_ok);
    scanf("%d", &num_ok);

    printf("printing Digits %d to %d : \n\n", i_num_ok,(i_num_ok + num_ok));

    for( i_ok = i_num_ok; i_ok <= (i_num_ok + num_ok); i_ok++)
    {
        printf("\t%d\n",i_ok);
    }

    printf("\n\n");

    return (0);
}