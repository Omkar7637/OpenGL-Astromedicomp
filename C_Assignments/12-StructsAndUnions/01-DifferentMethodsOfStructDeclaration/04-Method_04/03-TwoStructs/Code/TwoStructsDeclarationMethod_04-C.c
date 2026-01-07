#include<stdio.h>

/// DEFINE STRUCT 
struct MyPoint
{
    int x;
    int y;
};

struct MyPointProperties
{
    int quadrant;
    char axis_locations[10];
};

int main(void)
{
    // variable decalarations 
    struct MyPoint point;
    struct MyPointProperties point_properties;
    //code
    printf("\n\n");
    printf("Enter X-Coordinate For A Point : ");
    scanf("%d", &point.x);
    printf("Enter Y-Coordinate For A Point : ");
    scanf("%d", &point.y);

    printf("\n\n");
    printf("Point Co-ordinates (x, y) Are : (%d, %d) !!!\n\n", point.x, point.y);

    if(point.x == 0 && point.y ==0)
    {
        printf("The Point Is the Origin (%d, %d) !!!\n", point.x, point.y);
    }
    else
    {
        if(point.x == 0)
        {
            if(point.y < 0)
                strcpy(point_properties.axis_locations, "Negative Y");

            if(point.y > 0)
                strcpy(point_properties.axis_locations, "Positive Y");

            point_properties.quadrant = 0;
            printf("The Point Lies On The %s Axis !!!\n\n", point_properties.axis_locations);
        }
        else if(point.y == 0)
        {
            if(point.x < 0)
                strcpy(point_properties.axis_locations, "Negative X");

            if(point.x > 0)
                strcpy(point_properties.axis_locations, "Positive X");

            point_properties.quadrant = 0;
            printf("The Point Lies On The %s Axis !!!\n\n", point_properties.axis_locations);
        }
        else
        {
            point_properties.axis_locations[0] = '\0';

            if(point.x > 0 && point.y > 0)
                point_properties.quadrant = 1;

            else if (point.x < 0 && point.y > 0)
                point_properties.quadrant = 2;

            else if(point.x < 0 && point.y < 0)
                point_properties.quadrant = 3;

            else
                point_properties.quadrant = 4;

            printf("The Point lies In Quadrant Number %d !!!\n\n", point_properties.quadrant);
        }
    }
    return(0);
}