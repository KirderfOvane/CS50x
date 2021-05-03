#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    typedef uint8_t BYTE;

    // Ensure only one arg
    if (argc != 2)
    {
        printf("Only one argument allowed. Usage: .recover infile\n");
        return 1;
    }

    // Open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    // Open output file
    FILE *outptr = fopen(("000.jpg"), "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s. \n", "000.jpg");
        return 1;
    }


    BYTE buffer[512]; // change to 512 bytes instead. first byte must be jpeg start or else its not jpeg data.
    int fileCount = 1;
    int counter = 0;
    int foundjpeg = 0;
    int foundEndOfJpeg = 0;
    BYTE firstThreeCheck[3] = { 0xff, 0xd8, 0xff };
    BYTE finalCheck[16] = { 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef };

    while (fread(&buffer, sizeof(BYTE), 512, inptr))
    {

        if (buffer[0] == 0xff)
        {
            if (buffer[1] == firstThreeCheck[1])
            {
                if (buffer[2] == firstThreeCheck[2])
                {
                    if (foundjpeg == 1)
                    {

                        // Start a new file
                        fclose(outptr);
                        char filename[8];
                        sprintf(filename, "%03i.jpg", fileCount++);

                        // Open output file
                        outptr = fopen((filename), "w");
                        if (outptr == NULL)
                        {
                            fclose(inptr);
                            printf("Could not create %s. \n", "000.jpg");
                            return 1;
                        }
                    }
                    counter++;
                    foundjpeg = 1;
                }
            }



        }
        if (foundjpeg == 1)
        {
            fwrite(&buffer, sizeof(BYTE), 512, outptr);
        }
    }
    fclose(inptr);
    fclose(outptr);
    return 0;

}