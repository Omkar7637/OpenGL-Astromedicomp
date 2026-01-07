#include<stdio.h>

int main(void)
{
    //code

    printf("\n\n");
 
    printf("Going on to next Line .... using \\n Escape sequence\n\n");
    printf("Demonstrating \t Horizontal \t Tab \t using \t \\t Escape sequence !!!\n\n");
    printf("\"This Is A Double Quoted Output\" Done using \\\" \\\" Escap Seqence \n\n");
    printf("\'This Is A Single Quoted Output\' Done using \\\' \\\' Escap Seqence \n\n");
    printf("BACKSPACE Turned To BACKSPACE \b Using Escape Sequence \\b\n\n");

    printf("\r Demonstrating Carriage Return using \\r Escape sequence\n");
    printf("Demonstarting \r carriage return using \\r Escape sequence\n");
    printf("Demonstarting Carriage \r Return Using \\r Escape Sequenence\n\n");

    printf("Demonstarting \x41 Using \\xhh Escape Sequence \n");
    //0x41 is the Hexadecimal code for letter 'A'. 'xhh' Is the place-holder for 'x'
    //Followed by 2 digits(hh), altogether forming a Hexadeciaml Number.
    printf("Demonstarting \102 Using \\ooo Escape sequence \n\n");
    // 102 is the octal code for letter 'B'. 'ooo' is the place-holder fir 3 digit
    // formating an octal number.
    return(0);
}