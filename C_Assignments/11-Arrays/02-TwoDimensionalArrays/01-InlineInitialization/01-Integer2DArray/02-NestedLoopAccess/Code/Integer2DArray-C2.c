#include<stdio.h>

int main(void)
{
    // variable decalrations
    int iArray[5][3] = {{1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}, {5, 10, 15}};// IN-LINE INITIALIZATION
    int int_size;
    int iArray_size;
    int iArray_num_elements, iArray_num_rows, iArray_num_columns;
    int i, j;

    // code

    printf("\n\n");

    int_size = sizeof(int);

    iArray_size = sizeof(iArray);
    printf("Size Of Two Dimensinal ( 2D ) Integer Array Is = %d \n\n",iArray_size);

    iArray_num_rows = iArray_size / sizeof(iArray[0]);
    printf("Number of Rows In two Dimensional ( 2D ) Integer Array Is = %d \n\n",iArray_num_rows);

    iArray_num_columns = sizeof(iArray[0]) / int_size;
    printf("Number Of Columns In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_columns);

    iArray_num_elements = iArray_num_rows * iArray_num_columns;
    printf("Number Of Elements In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", iArray_num_elements);

    printf("\n\n");

    printf("Elements In The 2D Array : \n\n");

    for(i = 0; i < iArray_num_rows; i++)
    {
        printf("****** ROW %d ******\n",(i + 1));
        for(j = 0; j < iArray_num_columns; j++)
        {
            printf("iArray[%d][%d] = %d\n", i, j, iArray[i][j]);
        }
        printf("\n\n");
    }

    return(0);
}