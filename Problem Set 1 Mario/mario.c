#include <cs50.h>
#include <stdio.h>

int get_positive_int(void);

int main(void)
{
    // user input
    int height = get_positive_int();

    for(int i=height;i > 0;i--)
    {
       for(int j=0;j < i-1;j++)
       {
           printf(" ");

       }
       for(int k=0;k < (height+1-i);k++)
       {
           printf("#");
       }
        printf("  ");
        for(int m=0;m<height-i+1;m++)
        {
            printf("#");
        }
            printf("\n");
    }
}
int get_positive_int(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while(n < 1 || n > 8);
    return n;
}