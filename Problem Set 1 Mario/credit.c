#include <cs50.h>
#include <stdio.h>

int get_digit(long num);
int multiply_digit(int digit);

int main(void)
{
    // input
    long cardNumber = get_long("Number: ");

    // logic card validation formula
    long cardLengthCounter = 1;
    int checkSum = 0;
    int digit = 0;
    int secondDigit;

   while(cardNumber)
{
    // get second last digit
    secondDigit = digit;

    // get last digit
    digit = get_digit(cardNumber);


    // multiply every other digit
     if((cardLengthCounter % 2) == 0)
    {
        int multiplied_digit = multiply_digit(digit);

        while(multiplied_digit)
        {
            checkSum += get_digit(multiplied_digit);
            multiplied_digit/=10;
        }
    }
    else
    {
        checkSum += digit;
    }

    // truncate away last digit
    cardNumber /= 10;
    cardLengthCounter++;
}

// branching logic on formula result
int check = get_digit(checkSum);

if(get_digit(checkSum) != 0)
{
    printf("%s\n","INVALID");
    return 0;
}

    if((cardLengthCounter-1 == 16 || cardLengthCounter-1 == 13) && digit == 4)
    {
        printf("%s\n","VISA");
        return 0;
    }
    if((secondDigit == 4 || secondDigit == 7) && digit == 3 && cardLengthCounter-1 == 15)
    {
           printf("%s\n","AMEX");
           return 0;
    }
     if((secondDigit >= 1 && secondDigit <= 5) && digit == 5 && cardLengthCounter-1 == 16)
    {
           printf("%s\n","MASTERCARD");
           return 0;
    }
    printf("%s\n","INVALID");
    return 0;

}

// DRY functions
int get_digit(long num)
{
    return num % 10;
}

int multiply_digit(int digit)
{
    return (digit * 2);
}