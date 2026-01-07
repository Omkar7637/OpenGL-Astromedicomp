#include<stdio.h> // for printf()
#include<conio.h> // for getch()

// ASCII Values for 'A' to 'Z' => 65 to 90
#define CHAR_ALPHABET_UPPER_CASE_BEGINNING 65
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90

// ASCII Values For 'a' to 'z' => 97 to 122
#define CHAR_ALPHABET_LOWER_CASE_BEGINNING 97
#define CHAR_ALPHABET_LOWER_CASE_ENDING 122

//ASCII Values for '0' to '9' => 48 to 57
#define CHAR_DIGIT_BEGINNING 48
#define CHAR_DIGIT_ENDING 57

int main(void)
{
    // varibale decalartion 
    char ch;
    int ch_value;

    //code
    printf("\n\n");

    printf("Enetr Character : ");
    ch = getch();

    printf("\n\n");

    if((ch =='A' || ch == 'a') || (ch == 'E' || ch =='e') || (ch == 'I' || ch == 'i') || (ch == 'O' || ch =='o') || (ch == 'U' || ch == 'u'))
    {
        printf("Charater \'%c\' Enetred by you, is A vowel Charater from the English Alphabet !!!\n\n", ch);
    }
    else
    {
        ch_value = (int)ch;
        // If the character Has ASCII value Between 65 AND 90 OR Between 97 ANd 122,It is Still A letter of the Alphabet, But It is A 'CONSONANT', AND NOT a 'VOWL'....

        if((ch_value >= CHAR_ALPHABET_UPPER_CASE_BEGINNING && ch_value <= CHAR_ALPHABET_UPPER_CASE_ENDING) || (ch_value >= CHAR_ALPHABET_LOWER_CASE_BEGINNING && ch_value<= CHAR_ALPHABET_LOWER_CASE_ENDING))
        {
            printf("charater \'%c\' ENtered By you, is a CONSTANT CHARTER from The ENglish Alphabet !!!\n\n",ch);
        }

        else if(ch_value >= CHAR_DIGIT_BEGINNING && ch_value <= CHAR_DIGIT_ENDING)
        {
            printf("Charater \'%c\' Entered By you, Is a DIGIT CHARACTER !!!\n\n",ch);
        }
        else{
            printf("Charater \'%c\' Entered by you, Is A SPECIAL CHARATER !!!\n\n",ch);
        }
    }
    return (0);
}
