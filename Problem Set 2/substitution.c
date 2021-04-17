// Implement a program that implements a substitution cipher, per the below.
// $ ./substitution JTREKYAVOGDXPSNCUIZLFBMWHQ
// plaintext:  HELLO
// ciphertext: VKXXN

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int validateKey(string key, string publicKey);

int main(int argc, string argv[])
{
    //arg validation
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }


    // key
    string key = argv[1];
    int keyLength = strlen(key);
    if (keyLength != 26)
    {
        printf("Invalid key, must be 26 unique letters\n");
        return 1;
    }
    string publicKey = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int validation = validateKey(key, publicKey);
    if (validation == 1)
    {
        printf("Not a valid key!,exiting program..\n");
        return 1;
    }

    // plaintext
    string plaintext = get_string("plaintext: ");
    int plaintextLength = (int)strlen(plaintext);

    char ciphertext[100]; // cipher can handle at most 100 chars!

    for (int i = 0; i < plaintextLength; i++)
    {
        for (int j = 0; j < keyLength; j++)
        {
            if ((int)toupper(plaintext[i]) == (int)publicKey[j])
            {
                if (islower(plaintext[i]))
                {
                    ciphertext[i] = tolower(key[j]);
                }
                else
                {
                    ciphertext[i] = toupper(key[j]);
                }
            }
        }

        if ((int)ciphertext[i] == 0)
        {
            ciphertext[i] = plaintext[i];
        }


    }
    // output
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

int validateKey(string key, string publicKey)
{

    for (int i = 0; i < 26; i++)
    {
        int check = 1;
        for (int j = 0; j < 26; j++)
        {
// printf("%c %c",key[j],publicKey[i]);
            if (toupper(key[j]) == toupper(publicKey[i]))
            {
                check = 0;
            }
        }
        if (check == 1)
        {
            return 1;
        }
    }
    return 0;
}