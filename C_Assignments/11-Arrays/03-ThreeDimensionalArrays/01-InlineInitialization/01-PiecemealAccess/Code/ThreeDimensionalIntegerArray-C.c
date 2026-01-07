#include<stdio.h>

int main(void)
{
    // variable declarations

    // IN-LINE INITIALIZATION
    int iArray[5][3][2] = { { { 9, 18 }, { 27, 36 }, { 45, 54 } },
                            { { 8, 16 }, { 24, 32 }, { 40, 48 } },
                            { { 7, 14 }, { 35, 42 }, { 35, 42 } },
                            { { 6, 12 }, { 18, 24 }, { 30, 36 } },
                            { { 5, 10 }, { 15, 20 }, { 25, 30 } } };
    int int_size;
    int iArray_size;
    int iArray_num_elements, iArray_width, iArray_height, iArray_depth;

    // code
    printf("\n\n");

    int_size = sizeof(int);

    iArray_size = sizeof(iArray);
    printf("Size Of Three Dimnetional ( 3D ) Intger Array Id = %d\n\n", iArray_size);

    iArray_width = iArray_size / sizeof(iArray[0]);
    printf("Number Of Rows (Width) In Three Dimensional ( 3D ) Intger Array Is = %d\n\n",iArray_width);

    iArray_height = sizeof(iArray[0]) / sizeof(iArray[0][0]);
    printf("Number Of Columns (Height) in Three Dimensional ( 3D ) Intger Array Is = %d\n\n", iArray_height);

    iArray_depth = sizeof(iArray[0][0]) / int_size;
    printf("Depth in Three Dimensional ( 3D ) Intger Array Is = %d\n\n",iArray_depth);

    iArray_num_elements = iArray_width * iArray_height * iArray_depth;
    printf("Number Of Elements In Three Dimensional ( 3D ) Intger Array Is = %d\n\n", iArray_num_elements);

    printf("\n\n");
    printf("Elements In Intger 3D Array : \n\n");

    // *** PIECE-MEAL DISPLAY ***
    // ****** ROW 1 ******
    printf("****** ROW 1 ******\n");
    printf("****** COLUMN 1 ******\n");
    printf("iArray[0][0][0] = %d\n",iArray[0][0][0]);
    printf("iArray[0][0][1] = %d\n",iArray[0][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[0][1][0] = %d\n",iArray[0][1][0]);
    printf("iArray[0][1][1] = %d\n",iArray[0][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[0][2][0] = %d\n",iArray[0][2][0]);
    printf("iArray[0][2][1] = %d\n",iArray[0][2][1]);
    printf("\n");

        // ****** ROW 2 ******
    printf("****** ROW 2 ******\n");
    printf("****** COLUMN 1 ******\n");
    printf("iArray[1][0][0] = %d\n",iArray[1][0][0]);
    printf("iArray[1][0][1] = %d\n",iArray[1][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[1][1][0] = %d\n",iArray[1][1][0]);
    printf("iArray[1][1][1] = %d\n",iArray[1][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[1][2][0] = %d\n",iArray[1][2][0]);
    printf("iArray[1][2][1] = %d\n",iArray[1][2][1]);
    printf("\n");

        // ****** ROW 3 ******
    printf("****** ROW 3 ******\n");
    printf("****** COLUMN 1 ******\n");
    printf("iArray[2][0][0] = %d\n",iArray[2][0][0]);
    printf("iArray[2][0][1] = %d\n",iArray[2][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[2][1][0] = %d\n",iArray[2][1][0]);
    printf("iArray[2][1][1] = %d\n",iArray[2][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[2][2][0] = %d\n",iArray[2][2][0]);
    printf("iArray[2][2][1] = %d\n",iArray[2][2][1]);
    printf("\n");

        // ****** ROW 4 ******
    printf("****** ROW 4 ******\n");
    printf("****** COLUMN 1 ******\n");
    printf("iArray[3][0][0] = %d\n",iArray[3][0][0]);
    printf("iArray[3][0][1] = %d\n",iArray[3][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[3][1][0] = %d\n",iArray[3][1][0]);
    printf("iArray[3][1][1] = %d\n",iArray[3][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[3][2][0] = %d\n",iArray[3][2][0]);
    printf("iArray[3][2][1] = %d\n",iArray[3][2][1]);
    printf("\n");

        // ****** ROW 5 ******
    printf("****** ROW 5 ******\n");
    printf("****** COLUMN 1 ******\n");
    printf("iArray[4][0][0] = %d\n",iArray[4][0][0]);
    printf("iArray[4][0][1] = %d\n",iArray[4][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[4][1][0] = %d\n",iArray[4][1][0]);
    printf("iArray[4][1][1] = %d\n",iArray[4][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[4][2][0] = %d\n",iArray[4][2][0]);
    printf("iArray[4][2][1] = %d\n",iArray[4][2][1]);
    printf("\n");

    return(0);

}