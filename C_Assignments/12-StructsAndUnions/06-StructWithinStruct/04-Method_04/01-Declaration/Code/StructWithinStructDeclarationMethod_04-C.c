#include<stdio.h>


int main(void)
{

    // variable declarations
    int length, breadth, area;

    struct MyPoint
    {
        int x;
        int y;
    };
    struct Rectangle
    {
        struct MyPoint point_01;
        struct MyPoint point_02;
    }; 

    struct Rectangle rect ;

    //code
    
    printf("\n\n");
    printf("Enter Leftmost X-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_01.x);
    
    printf("\n\n");
    printf("Enter Leftmost Y-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_01.y);
    
    printf("\n\n");
    printf("Enter Leftmost X-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_02.x);
    
    printf("\n\n");
    printf("Enter Leftmost X-Coordinate Of Rectangle : ");
    scanf("%d", &rect.point_02.y);

    length = rect.point_02.y - rect.point_01.y;
    if(length < 0)
        length = length * -1;

    breadth = rect.point_02.x - rect.point_01.y;
    if(breadth < 0)
        breadth = breadth * -1;

    area = length * breadth;

    printf("\n\n");
    printf("Length Of Recatangle = %d\n\n", length);
    printf("Breadth Of Rectangle = %d\n\n", breadth);
    printf("Area Of Rectangle = %d\n\n", area);

    return(0);
}