#include<stdio.h>

// DEFINE STRUCT
struct MyData
{
    int i;
    float f;
    double d;
};

struct MyData data;
int main(void)
{
    // variable decalarations
    int i_size;
    int f_size;
    int d_size;
    int struct_MyData_size;

    //code
    //Assigninig Data Values To The Data Members Of 'struct MyData'
    data.i = 30;
    data.f = 11.45f;
    data.d = 1.2995;

    //Displaying Values Of the Data Members Of 'Struct MyData'
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n",data.i);
    printf("f = %f\n",data.f);
    printf("d = %lf\n",data.d);

    //Calculating Size (In Bytes) Of The Data Members Of 'struct MyData'
    i_size = sizeof(data.i);
    f_size = sizeof(data.f);
    d_size = sizeof(data.d);

    //Displaying Size (In Bytes) Of The Data Members Of 'struct MyData'
    printf("\n\n");
    printf("SIZE (In Bytes) OF DATA MEMBERS OF 'Struct MyData' ARE : \n\n");
    printf("Size of 'i' = %d bytes\n", i_size);
    printf("Size of 'f' = %d bytes\n", f_size);
    printf("Size of 'd' = %d bytes\n", d_size);

    //Calculating Size (In Bytes) Of the entire 'struct MyData'
    struct_MyData_size = sizeof(struct MyData); // can also give struct name -> sizeof(MyData)

    //Displaying Size (In Bytes) Of the entire 'struct MyData'
    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes\n\n", struct_MyData_size);

    return(0);
    

}