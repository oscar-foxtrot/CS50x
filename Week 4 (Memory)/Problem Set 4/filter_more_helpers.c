#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            uint8_t avrg = round(((double) image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avrg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width / 2; ++j)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

RGBTRIPLE get_avg(int i, int j, int height, int width, RGBTRIPLE image[height][width], int bordercheck);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    // Without edges
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            tmp[i][j] = get_avg(i, j, height, width, image, 0);
        }
    }

    // Borders
    for (int j = 0; j < width; ++j)
    {
        tmp[0][j] = get_avg(0, j, height, width, image, 1);
    }
    for (int i = 0; i < height; ++i)
    {
        tmp[i][0] = get_avg(i, 0, height, width, image, 1);
    }
    for (int j = 0; j < width; ++j)
    {
        tmp[height - 1][j] = get_avg(height - 1, j, height, width, image, 1);
    }
    for (int i = 0; i < height; ++i)
    {
        tmp[i][width - 1] = get_avg(i, width - 1, height, width, image, 1);
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}

RGBTRIPLE get_avg(int i, int j, int height, int width, RGBTRIPLE image[height][width], int bordercheck)
{
    unsigned int avrr = 0, avrg = 0, avrb = 0;
    RGBTRIPLE avg;
    if (bordercheck == 0)
    {
        for (int k = i - 1; k <= i + 1; ++k)
        {
            for (int l = j - 1; l <= j + 1; ++l)
            {
                avrr += image[k][l].rgbtRed;
                avrg += image[k][l].rgbtGreen;
                avrb += image[k][l].rgbtBlue;
            }
        }
        RGBTRIPLE tmp = {(uint8_t)(round((double) avrb / 9)), (uint8_t)round(((double) avrg / 9)), (uint8_t)round(((double) avrr / 9))};
        avg = tmp;
    }
    else
    {
        int cnt = 0;
        for (int k = i - 1; k <= i + 1; ++k)
        {
            for (int l = j - 1; l <= j + 1; ++l)
            {
                if (l < width && l >= 0 && k < height && k >= 0)
                {
                    ++cnt;
                    avrr += image[k][l].rgbtRed;
                    avrg += image[k][l].rgbtGreen;
                    avrb += image[k][l].rgbtBlue;
                }
            }
        }
        RGBTRIPLE tmp = {(uint8_t)round((double) avrb / cnt), (uint8_t)round((double) avrg / cnt), (uint8_t)round((double) avrr / cnt)};
        avg = tmp;
    }
    return avg;
}

RGBTRIPLE get_sob(int i, int j, int height, int width, RGBTRIPLE image[height][width], int bordercheck);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    // Without borders - for the sake of time-effectiveness
    for (int i = 1; i < height - 1; ++i)
    {
        for (int j = 1; j < width - 1; ++j)
        {
            tmp[i][j] = get_sob(i, j, height, width, image, 0);
        }
    }
    // Borders
    for (int j = 0; j < width; ++j)
    {
        tmp[0][j] = get_sob(0, j, height, width, image, 1);
    }
    for (int i = 0; i < height; ++i)
    {
        tmp[i][0] = get_sob(i, 0, height, width, image, 1);
    }
    for (int j = 0; j < width; ++j)
    {
        tmp[height - 1][j] = get_sob(height - 1, j, height, width, image, 1);
    }
    for (int i = 0; i < height; ++i)
    {
        tmp[i][width - 1] = get_sob(i, width - 1, height, width, image, 1);
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;

}

RGBTRIPLE get_sob(int i, int j, int height, int width, RGBTRIPLE image[height][width], int bordercheck)
{
    int avgb[2] = {0, 0}, avgg[2] = {0, 0}, avgr[2] = {0, 0};
    int gxarr[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gyarr[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    if (!bordercheck)
    {
        for (int k = i - 1; k <= i + 1; ++k)
        {
            for (int l = j - 1; l <= j + 1; ++l)
            {
                avgb[0] += image[k][l].rgbtBlue * (int) gxarr[k - (i - 1)][l - (j - 1)];
                avgg[0] += image[k][l].rgbtGreen * (int) gxarr[k - (i - 1)][l - (j - 1)];
                avgr[0] += image[k][l].rgbtRed * (int) gxarr[k - (i - 1)][l - (j - 1)];
                avgb[1] += image[k][l].rgbtBlue * (int) gyarr[k - (i - 1)][l - (j - 1)];
                avgg[1] += image[k][l].rgbtGreen * (int) gyarr[k - (i - 1)][l - (j - 1)];
                avgr[1] += image[k][l].rgbtRed * (int) gyarr[k - (i - 1)][l - (j - 1)];
            }
        }
        int tmpr;
        avgb[0] = (tmpr = (int)(round(sqrt((avgb[0] * avgb[0] + avgb[1] * avgb[1]))))) > 255 ? 255 : tmpr;
        avgg[0] = (tmpr = (int)(round(sqrt((avgg[0] * avgg[0] + avgg[1] * avgg[1]))))) > 255 ? 255 : tmpr;
        avgr[0] = (tmpr = (int)(round(sqrt((avgr[0] * avgr[0] + avgr[1] * avgr[1]))))) > 255 ? 255 : tmpr;
        RGBTRIPLE res = {(uint8_t) avgb[0], (uint8_t) avgg[0], (uint8_t) avgr[0]};
        return res;
    }
    else
    {
        for (int k = i - 1; k <= i + 1; ++k)
        {
            for (int l = j - 1; l <= j + 1; ++l)
            {
                if (k >= 0 && k < height && l < width && l >= 0)
                {
                    avgb[0] += image[k][l].rgbtBlue * gxarr[k - (i - 1)][l - (j - 1)];
                    avgg[0] += image[k][l].rgbtGreen * gxarr[k - (i - 1)][l - (j - 1)];
                    avgr[0] += image[k][l].rgbtRed * gxarr[k - (i - 1)][l - (j - 1)];
                    avgb[1] += image[k][l].rgbtBlue * gyarr[k - (i - 1)][l - (j - 1)];
                    avgg[1] += image[k][l].rgbtGreen * gyarr[k - (i - 1)][l - (j - 1)];
                    avgr[1] += image[k][l].rgbtRed * gyarr[k - (i - 1)][l - (j - 1)];
                }
            }
        }
        int tmpr;
        avgb[0] = (tmpr = (int)(round(sqrt((avgb[0] * avgb[0] + avgb[1] * avgb[1]))))) > 255 ? 255 : tmpr;
        avgg[0] = (tmpr = (int)(round(sqrt((avgg[0] * avgg[0] + avgg[1] * avgg[1]))))) > 255 ? 255 : tmpr;
        avgr[0] = (tmpr = (int)(round(sqrt((avgr[0] * avgr[0] + avgr[1] * avgr[1]))))) > 255 ? 255 : tmpr;
        RGBTRIPLE res = {(uint8_t) avgb[0], (uint8_t) avgg[0], (uint8_t) avgr[0]};
        return res;
    }
}
