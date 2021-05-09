// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *tmp = table[hash(word)];

    while(tmp != NULL)
    {
        if(strcasecmp( word, tmp->word ) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }

    return false;
}



// Hashes word to a number
unsigned int hash(const char *word)
{
    return ( (int)toupper(word[0]) - 65);
}

int numOfWordsInDictionary = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary
    FILE* dictPtr = fopen(dictionary, "r");
    if(dictPtr == NULL)
    {
        printf("Could not find a dictionary");
        return false;
    }

    // Create a initial node
    node *n = NULL;
    n = malloc(sizeof(node));
    if(n == NULL)
    {
        fclose(dictPtr);
        return false;
    }

   // for every word in the dictionary
    while ((fscanf(dictPtr, "%s", n->word)) != EOF)
    {
            if(numOfWordsInDictionary == 0)
            {
              n->next = NULL;
              table[hash(n->word)] = n;
            }
            else
            {
              n->next = table[hash(n->word)];
              table[hash(n->word)] = n;
            }


            n = malloc(sizeof(node));
            if(n == NULL)
            {

                fclose(dictPtr);
                return false;
            }
            numOfWordsInDictionary++;

    }
    fclose(dictPtr);
    free(n);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
   if(numOfWordsInDictionary)
   {
       return numOfWordsInDictionary;
   }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    // Free list, by using a while loop and a temporary variable to point
    // to the next node before freeing the current one
    for(int t = 0; t < 26; t++)
    {
        while (table[t] != NULL)
        {
            // We point to the next node first
            node *tmp = table[t]->next;
            // Then, we can free the first node
            free(table[t]);
            // Now we can set the list to point to the next node
            table[t] = tmp;
            // If list is null, when there are no nodes left, our while loop will stop
        }
    }

    return true;
}
