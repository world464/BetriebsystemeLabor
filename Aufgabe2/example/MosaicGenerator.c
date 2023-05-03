#include "stdio.h"

typedef struct {
    int red;
    int green;
    int blue;

}pixel;

typedef struct {
    int width;
    int height;
    char* pixel;

}Image;


void generateMosaic(Image image, int tileSize) {

    int width = image.width;
    int height = image.height;
    int tileWidth = 0;
    int tileHeight = 0;
    char pixelatedImage [height*width*3];

    pixel Image[height][width];
    char counter = 0;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            Image[i][j].red = image.pixel[counter];
            Image[i][j].green = image.pixel[counter+1];
            Image[i][j].blue = image.pixel[counter+2];
            counter +=3;
        }
    }

    for(int y = 0; y< height;y+= tileSize) {
        for(int x = 0; x < width; x+= tileSize) {
            if(tileSize < (width-x))
                tileWidth = tileSize;
            else
                tileWidth = (width-x);

            if(tileSize < (height-y))
                tileHeight = tileSize;
            else
                tileHeight = (height-y);

            int sumRed = 0, sumGreen = 0, sumBlue = 0, count = 0;

            for(int i = 0; i < tileHeight; i++) {
                for (int j = 0; j < tileWidth; j++) {
                    sumRed += Image[i+y][j+x].red;
                    sumGreen += Image[i+y][j+x].green;
                    sumBlue += Image[i+y][j+x].blue;
                    count++;
                }
            }

            int avgRed = sumRed /count,avgGreen = sumGreen / count, avgBlue = sumBlue / count;

            for(int i = 0; i < tileHeight; i++) {
                for (int j = 0; j < tileWidth; j++) {
                    pixelatedImage[(y*3*width + i*width)+(x*3 + j*3)] = avgRed;
                    pixelatedImage[(y*3*width + i*width)+(x*3 + j*3)+1] = avgGreen;
                    pixelatedImage[(y*3*width + i*width)+(x*3 + j*3)+2] = avgBlue;
                }
            }
        }
    }
}