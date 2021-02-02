#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <cs50.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double avg_colour, r, g, b;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            r = image[row][col].rgbtRed;
            g = image[row][col].rgbtGreen;
            b = image[row][col].rgbtBlue;

            //calculate average
            avg_colour = round(((float)r + g + b) / 3);

            //set all values to average
            image[row][col].rgbtBlue = image[row][col].rgbtGreen =
                                           image[row][col].rgbtRed = (int)avg_colour;
        }
    }
    
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int r1, r2, g1, g2, b1, b2;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width / 2; col++)
        {
            if (width % 2 != 0 && col == (ceil((float)width / 2)))
            {
                break;
            }

            //set temp value
            RGBTRIPLE temp = image[row][col];

            //swap values
            image[row][col] = image[row][width - col - 1];
            image[row][width - col - 1] = temp;

        }

    }

    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempimage[height][width];
    int tempRED, tempBLUE, tempGREEN, division_count;

    //add picture values into temp array
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            tempimage[row][col].rgbtBlue = image[row][col].rgbtBlue;
            tempimage[row][col].rgbtGreen = image[row][col].rgbtGreen;
            tempimage[row][col].rgbtRed = image[row][col].rgbtRed;
        }
    }

    //cycle all items in array
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            //reset colour temp values
            tempRED = tempBLUE = tempGREEN = division_count = 0;

            //Cycle the boxes arround current pixel up to 9
            for (int i = - 1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {

                    //if average pixel falls out of array skip
                    if ((row + i) < 0 || (row + i) > (height - 1) || (col + j) < 0 || (col + j) > (width - 1))
                    {
                        continue;
                    }

                    tempBLUE += tempimage[row + i][col + j].rgbtBlue;
                    tempGREEN += tempimage[row + i][col + j].rgbtGreen;
                    tempRED += tempimage[row + i][col + j].rgbtRed;
                    division_count++;

                }
            }

            image[row][col].rgbtBlue = round(((float)tempBLUE) / division_count);
            image[row][col].rgbtGreen = round(((float)tempGREEN) / division_count);
            image[row][col].rgbtRed = round(((float)tempRED) / division_count);
        }
    }

    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempimage[height][width];

    int RGx, GGx, BGx, RGy, GGy, BGy;

    //add picture values into temp array
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            tempimage[row][col].rgbtBlue = image[row][col].rgbtBlue;
            tempimage[row][col].rgbtGreen = image[row][col].rgbtGreen;
            tempimage[row][col].rgbtRed = image[row][col].rgbtRed;
        }
    }

    int xMatrix [3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int yMatrix [3][3] =
    {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    // CHANGE BACK TO = 0
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            //reset all Gx/y values here
            BGx = GGx = RGx = BGy = GGy = RGy = 0;

            //Cycle the boxes around current pixel up to 9
            //i = row  ||  j = col
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    // if statement to catch if array is out of bounds
                    if ((row + i - 1) < 0 || (row + i - 1) > (height - 1)
                        || (col + j - 1) < 0 || (col + j - 1) > (width - 1))
                    {
                        //if out of bounds skip
                        continue;
                    }

                    //add x values to count
                    BGx += (xMatrix[i][j] * tempimage[row + i - 1][col + j - 1].rgbtBlue);
                    GGx += (xMatrix[i][j] * tempimage[row + i - 1][col + j - 1].rgbtGreen);
                    RGx += (xMatrix[i][j] * tempimage[row + i - 1][col + j - 1].rgbtRed);

                    //add y values to count
                    BGy += (yMatrix[i][j] * tempimage[row + i - 1][col + j - 1].rgbtBlue);
                    GGy += (yMatrix[i][j] * tempimage[row + i - 1][col + j - 1].rgbtGreen);
                    RGy += (yMatrix[i][j] * tempimage[row + i - 1][col + j - 1].rgbtRed);
                }
            }

            float BGColour = sqrt((pow(BGx, 2)) + (pow(BGy, 2)));
            float GGColour = sqrt((pow(GGx, 2)) + (pow(GGy, 2)));
            float RGColour = sqrt((pow(RGx, 2)) + (pow(RGy, 2)));

            if (BGColour > 255)
            {
                BGColour = 255;
            }

            if (GGColour > 255)
            {
                GGColour = 255;
            }

            if (RGColour > 255)
            {
                RGColour = 255;
            }

            image[row][col].rgbtBlue = (int)round(BGColour);
            image[row][col].rgbtGreen = (int)round(GGColour);
            image[row][col].rgbtRed = (int)round(RGColour);

        }
    }

    return;
}
