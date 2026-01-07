#include<stdio.h>

#define MY_PI 3.1415926535897932

#define AMC_STRING "AstroMediComp RTR"

// IF first Constant is not assigned A value, It is Assumed To be 0 i.e:
//'SUNDAY' will be 0
// and the rst of the constants are assigned consecutive intger values from 0 
// onwards i.e: 'MONDAY' will be 1, 'TUESDAY' will be 2, and so on......

// Un-named enums

enum
{
    sunday,monday,tuesday,wednesday,thursday,friday,saturday,
};

enum
{
    january = 1,february,march,april,may,june,july,august,september,october,november,december
};

enum
{
    one,two,three,four,five = 5,six,seven,eight,nine,ten
};

enum boolean
{
    TRUE = 1,
    FALSE = 0
};

int main(void)
{
    // local constant declarations 
    const double epsilon = 0.000001;

    //code
    printf("\n\n");
    printf("local constant Epsilon = %lf\n\n", epsilon);

    printf("Sunday is day number = %d\n",sunday);
    printf("monday is day number = %d\n",monday);
    printf("Tuesday is day number = %d\n",tuesday);
    printf("wednesday is day number = %d\n",wednesday);
    printf("Thursday is day number = %d\n",thursday);
    printf("Friday is day number = %d\n",friday);
    printf("Saturday is day number = %d\n\n",saturday);

    printf("One is Enum Number = %d\n",one);
    printf("Two is Enum Number = %d\n",two);
    printf("Three is Enum Number = %d\n",three);
    printf("Four is Enum Number = %d\n",four);
    printf("Five is Enum Number = %d\n",five);
    printf("six is Enum Number = %d\n",six);
    printf("Seven is Enum Number = %d\n",seven);
    printf("Eight is Enum Number = %d\n",eight);
    printf("Nine is Enum Number = %d\n",nine);
    printf("Ten is Enum Number = %d\n\n",ten);

    printf("January is month Number = %d\n", january);
    printf("February is month Number = %d\n", february);
    printf("March is month Number = %d\n", march);
    printf("April is month Number = %d\n", april);
    printf("May is month Number = %d\n", may);
    printf("Jun is month Number = %d\n", june);
    printf("July is month Number = %d\n", july);
    printf("August is month Number = %d\n", august);
    printf("September is month Number = %d\n", september);
    printf("October is month Number = %d\n", october);
    printf("November is month Number = %d\n", november);
    printf("December is month Number = %d\n\n", december);

    printf("value of TRUE is = %d\n",TRUE);
    printf("value of FALSE is = %d\n\n", FALSE);

    printf("MY_PI Macro Value = %.10lf\n\n",MY_PI);
    printf("Area of circle of radius 2 units = %f\n\n", (MY_PI * 2.0f * 2.0f));
    // pi * r * r = area of circle of radius 'r'

    printf("\n\n");

    printf(AMC_STRING);
    printf("\n\n");


}