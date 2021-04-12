#include <cs50.h>
#include <stdio.h>

int get_positive_startSize(void);
int get_positive_endSize(int startSize);

int main(void)
{
    // TODO: Prompt for start size
int startSize = get_positive_startSize();
    // TODO: Prompt for end size
int endSize = get_positive_endSize(startSize);
    // TODO: Calculate number of years until we reach threshold

    int years = 0;
    int threshold = startSize;
    while (threshold < endSize)
    {
        threshold = threshold + (threshold / 3 - threshold / 4);
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i",years);
}

int get_positive_startSize(void)
{
    int n;
    do
    {
        n = get_int("Positive Start Integer: ");
    }
    while(n < 9);
    return n;
}
int get_positive_endSize(int startSize)
{
    int n;
    do
    {
        n = get_int("Positive End Integer: ");
    }
    while(n < 10 || n < startSize);
    return n;
}