#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int const ASCI_CODE_START = 65;

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 == score2)
    {
        printf("Tie!\n");
        return 0;
    }
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
        return 0;
    }
    else
    {
        printf("Player 2 wins!\n");
        return 0;
    }

}

int compute_score(string word)
{
    // compare word with points and sum up the points
    int sum = 0;
    int i = 0;
    while ((int) word[i] != 0)
    {

        int j = 0;
        char letter = toupper(word[i]);

        // checks end of array by char beeing 0 when cast to int
        while ((int) POINTS[j] != 0)
        {
            if (j + ASCI_CODE_START == letter)
            {
                sum += POINTS[j];
            }
            j++;
        }

        i++;
    }
    return sum;
}
