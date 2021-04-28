#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>



// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixelArray[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixelArray[i][j] = image[i][j];

            BYTE red = pixelArray[i][j].rgbtRed;
            BYTE green = pixelArray[i][j].rgbtGreen;
            BYTE blue = pixelArray[i][j].rgbtBlue;
            BYTE average = (BYTE)(round((red + green + blue) / 3.0));

            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;

        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixelArray[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixelArray[i][j] = image[i][j];

            BYTE originalRed = pixelArray[i][j].rgbtRed;
            BYTE originalGreen = pixelArray[i][j].rgbtGreen;
            BYTE originalBlue = pixelArray[i][j].rgbtBlue;

            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = (BYTE)sepiaRed;
            image[i][j].rgbtBlue = (BYTE)sepiaBlue;
            image[i][j].rgbtGreen = (BYTE)sepiaGreen;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixelArray[height][width];

    for (int i = 0; i < height; i++)
    {
        int w = width - 1;
        for (int j = 0; j < width; j++)
        {
            pixelArray[i][j] = image[i][w];
            w--;
        }
        for (int j = 0; j < width; j++)
        {
            image[i][j] = pixelArray[i][j];
        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixelArray[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // how many pixels is involved in calculating average.
            int averageDivider = 9;

            // if on right edge
            if (j == width - 1)
            {
                averageDivider = averageDivider - 3;
                // if on top
                if (i == 0)
                {
                    averageDivider = averageDivider - 2;
                    RGBTRIPLE blurArray[4] = { image[i][j], image[i][j - 1], image[i + 1][j - 1], image[i + 1][j] };
                    float red = 0;
                    float green = 0;
                    float blue = 0;
                    for (int k = 0; k < averageDivider; k++)
                    {

                        red += blurArray[k].rgbtRed;
                        green +=  blurArray[k].rgbtGreen;
                        blue +=  blurArray[k].rgbtBlue;
                    }
                    red = round(red / (float)averageDivider);
                    green = round(green / (float)averageDivider);
                    blue = round(blue / (float)averageDivider);

                    pixelArray[i][j].rgbtRed = (BYTE)red;
                    pixelArray[i][j].rgbtGreen = (BYTE)green;
                    pixelArray[i][j].rgbtBlue = (BYTE)blue;
                    continue;
                }
                // if on bottom
                if (i == height - 1)
                {
                    averageDivider = averageDivider - 2;
                    RGBTRIPLE blurArray[4] = { image[i][j], image[i - 1][j - 1], image[i - 1][j], image[i][j - 1] };
                    float red = 0;
                    float green = 0;
                    float blue = 0;
                    for (int k = 0; k < averageDivider; k++)
                    {

                        red += blurArray[k].rgbtRed;
                        green +=  blurArray[k].rgbtGreen;
                        blue +=  blurArray[k].rgbtBlue;
                    }
                    red = round(red / (float)averageDivider);
                    green = round(green / (float)averageDivider);
                    blue = round(blue / (float)averageDivider);

                    pixelArray[i][j].rgbtRed = (BYTE)red;
                    pixelArray[i][j].rgbtGreen = (BYTE)green;
                    pixelArray[i][j].rgbtBlue = (BYTE)blue;
                    continue;
                }
                // if not in corner
                if (i != height - 1 && i != 0)
                {
                    RGBTRIPLE blurArray[6] = { image[i][j], image[i + 1][j], image[i + 1][j - 1], image[i][j - 1], image[i - 1][j - 1], image[i - 1][j] };
                    float red = 0;
                    float green = 0;
                    float blue = 0;
                    for (int k = 0; k < averageDivider; k++)
                    {

                        red += blurArray[k].rgbtRed;
                        green +=  blurArray[k].rgbtGreen;
                        blue +=  blurArray[k].rgbtBlue;
                    }
                    red = round(red / (float)averageDivider);
                    green = round(green / (float)averageDivider);
                    blue = round(blue / (float)averageDivider);

                    pixelArray[i][j].rgbtRed = (BYTE)red;
                    pixelArray[i][j].rgbtGreen = (BYTE)green;
                    pixelArray[i][j].rgbtBlue = (BYTE)blue;
                    continue;
                }
            }

            // if on left edge
            if (j == 0)
            {
                averageDivider = averageDivider - 3;


                // if on top left corner
                if (i == 0)
                {
                    averageDivider = averageDivider - 2;
                    RGBTRIPLE blurArray[4] = { image[i][j], image[i][j + 1], image[i + 1][j + 1], image[i + 1][j] };
                    float red = 0;
                    float green = 0;
                    float blue = 0;
                    for (int k = 0; k < averageDivider; k++)
                    {

                        red += blurArray[k].rgbtRed;
                        green +=  blurArray[k].rgbtGreen;
                        blue +=  blurArray[k].rgbtBlue;
                    }
                    red = round(red / (float)averageDivider);
                    green = round(green / (float)averageDivider);
                    blue = round(blue / (float)averageDivider);

                    pixelArray[i][j].rgbtRed = (BYTE)red;
                    pixelArray[i][j].rgbtGreen = (BYTE)green;
                    pixelArray[i][j].rgbtBlue = (BYTE)blue;
                    continue;
                }

                // if on bottom left corner
                if (i == height - 1)
                {

                    averageDivider = averageDivider - 2;
                    RGBTRIPLE blurArray[4] = { image[i][j], image[i - 1][j], image[i - 1][j + 1], image[i][j + 1] };
                    float red = 0;
                    float green = 0;
                    float blue = 0;
                    for (int k = 0; k < averageDivider; k++)
                    {

                        red += blurArray[k].rgbtRed;
                        green +=  blurArray[k].rgbtGreen;
                        blue +=  blurArray[k].rgbtBlue;
                    }
                    red = round(red / (float)averageDivider);
                    green = round(green / (float)averageDivider);
                    blue = round(blue / (float)averageDivider);

                    pixelArray[i][j].rgbtRed = (BYTE)red;
                    pixelArray[i][j].rgbtGreen = (BYTE)green;
                    pixelArray[i][j].rgbtBlue = (BYTE)blue;
                    continue;
                }

                // if not in corner
                if (i != height - 1 && i != 0)
                {
                    RGBTRIPLE blurArray[6] = { image[i][j], image[i - 1][j], image[i - 1][j + 1], image[i][j + 1], image[i + 1][j + 1], image[i + 1][j] };
                    float red = 0;
                    float green = 0;
                    float blue = 0;
                    for (int k = 0; k < averageDivider; k++)
                    {
                        red += blurArray[k].rgbtRed;
                        green +=  blurArray[k].rgbtGreen;
                        blue +=  blurArray[k].rgbtBlue;
                    }
                    red = round(red / (float)averageDivider);
                    green = round(green / (float)averageDivider);
                    blue = round(blue / (float)averageDivider);

                    pixelArray[i][j].rgbtRed = (BYTE)red;
                    pixelArray[i][j].rgbtGreen = (BYTE)green;
                    pixelArray[i][j].rgbtBlue = (BYTE)blue;
                    continue;
                }

            }

            // if on top row but not in corner
            if (i == 0)
            {
                averageDivider = averageDivider - 3;
                RGBTRIPLE blurArray[6] = { image[i][j], image[i][j + 1], image[i + 1][j + 1], image[i + 1][j], image[i + 1][j - 1], image[i][j - 1] };
                float red = 0;
                float green = 0;
                float blue = 0;
                for (int k = 0; k < averageDivider; k++)
                {
                    red += blurArray[k].rgbtRed;
                    green +=  blurArray[k].rgbtGreen;
                    blue +=  blurArray[k].rgbtBlue;
                }
                red = round(red / (float)averageDivider);
                green = round(green / (float)averageDivider);
                blue = round(blue / (float)averageDivider);

                pixelArray[i][j].rgbtRed = (BYTE)red;
                pixelArray[i][j].rgbtGreen = (BYTE)green;
                pixelArray[i][j].rgbtBlue = (BYTE)blue;
                continue;
            }

            // if on bottom row but not in corner
            if (i == height - 1)
            {
                averageDivider = averageDivider - 3;
                RGBTRIPLE blurArray[6] = { image[i][j], image[i][j - 1], image[i - 1][j - 1], image[i - 1][j], image[i - 1][j + 1], image[i][j + 1] };
                float red = 0;
                float green = 0;
                float blue = 0;
                for (int k = 0; k < averageDivider; k++)
                {
                    red += blurArray[k].rgbtRed;
                    green +=  blurArray[k].rgbtGreen;
                    blue +=  blurArray[k].rgbtBlue;
                }
                red = round(red / (float)averageDivider);
                green = round(green / (float)averageDivider);
                blue = round(blue / (float)averageDivider);

                pixelArray[i][j].rgbtRed = (BYTE)red;
                pixelArray[i][j].rgbtGreen = (BYTE)green;
                pixelArray[i][j].rgbtBlue = (BYTE)blue;
                continue;
            }


            // if all adjacent pixels
            if (averageDivider == 9)
            {
                RGBTRIPLE blurArray[9] = { image[i][j], image[i][j - 1], image[i + 1][j - 1], image[i + 1][j], image[i + 1][j + 1], image[i][j + 1], image[i - 1][j + 1], image[i - 1][j], image[i - 1][j - 1] };
                float red = 0;
                float green = 0;
                float blue = 0;
                for (int k = 0; k < averageDivider; k++)
                {
                    red += blurArray[k].rgbtRed;
                    green +=  blurArray[k].rgbtGreen;
                    blue +=  blurArray[k].rgbtBlue;
                }
                red = round(red / (float)averageDivider);
                green = round(green / (float)averageDivider);
                blue = round(blue / (float)averageDivider);

                pixelArray[i][j].rgbtRed = (BYTE)red;
                pixelArray[i][j].rgbtGreen = (BYTE)green;
                pixelArray[i][j].rgbtBlue = (BYTE)blue;
                continue;
            }



        }
    }
    // assign image-pointer to blur-result in pixelArray;
    for (int p = 0; p < height; p++)
    {
        for (int c = 0; c < width; c++)
        {

            image[p][c] = pixelArray[p][c];
        }
    }

    return;
}

