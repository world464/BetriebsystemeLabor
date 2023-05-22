//
// Created by david on 03.05.23.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "MosaicGenerator.c"

int min(int x, int y){
    if(x > y){
        return y;
    }
    return x;
}

#define BMP_HEADER_SIZE 54

int main(int argc,char* argv[]) {

    //Konsole einlesen
   char* fileName[sizeof (argv[1])];
   *fileName = argv[1];
   int tileSize =  atoi(argv[2]);
   char* filePath = realpath(*fileName, NULL);

   if(tileSize <= 0){
      // printf("Error: Wrong tilesize\n");
       perror("Error: Wrong tilesize\n");
       exit(EXIT_FAILURE);
   }


    // Open the bitmap file
    int fd = open(filePath,  O_RDWR);
    if (fd < 0) {
        printf("Error: Failed to open file\n");
        exit(EXIT_FAILURE);
    }

    unsigned char bmp_header[BMP_HEADER_SIZE];

    // Read the bitmap header
    ssize_t bytes_read = read(fd, bmp_header, BMP_HEADER_SIZE);
    // If there are less bytes read, then the file is not a valid bitmap file
    if (bytes_read != BMP_HEADER_SIZE) {
        printf("Error: Invalid bitmap header\n");
        exit(EXIT_FAILURE);
    }

    // Check the file signature to make sure it's a bitmap file
    if (bmp_header[0] != 'B' || bmp_header[1] != 'M') {
        printf("Error: It's not a bitmap image\n");
        exit(EXIT_FAILURE);
    }

    //Check the Bitmaps for Bit pro Pixle
    if (bmp_header[28] != 24 || bmp_header[29] != 0){
        printf("Error: It's not 24 bit pro Pixle\n");
        exit(EXIT_FAILURE);
    }

    //Check the compression
    if (bmp_header[30] != 0 || bmp_header[31] != 0 || bmp_header[32] != 0 || bmp_header[33] != 0){
        printf("Error: It's not compress\n");
        exit(EXIT_FAILURE);
    }

    //Check the color selection
    if (bmp_header[46] != 0 || bmp_header[47] != 0 || bmp_header[48] != 0 || bmp_header[49] != 0){
        printf("Error: The color \n");
        exit(EXIT_FAILURE);
    }


    // Get the image size, width, height and bit depth
    uint32_t image_size = *(uint32_t*) &bmp_header[2];
    int32_t width = *(int32_t*) &bmp_header[18];
    int32_t height = *(int32_t*) &bmp_header[22];
    uint16_t bit_depth = *(uint16_t*) &bmp_header[28];

    // Print the image size, width, height and bit depth for debugging purposes
    printf( "width: %i, height: %i, size: %i\n", width, height, image_size);

    // Allocate memory for the pixel data
    unsigned char* pixel_data = (unsigned char*) malloc(image_size - BMP_HEADER_SIZE);
    // If the memory allocation fails, exit the program
    if (pixel_data == NULL) {
        printf("Error: Failed to allocate memory for pixel data\n");
        exit(EXIT_FAILURE);
    }

    // Read the pixel data
    bytes_read = read(fd, pixel_data, image_size - BMP_HEADER_SIZE);
    if (bytes_read != image_size - BMP_HEADER_SIZE) {
        printf("Error: Failed to read pixel data\n");
        exit(EXIT_FAILURE);
    }

    // Reset the file pointer to the start of the pixel data
    lseek(fd, BMP_HEADER_SIZE, SEEK_SET);


    // Main part MosaicGenerator
    int bytes_per_pixel = bit_depth / 8;
    int pixel_bytes_per_row = width * bytes_per_pixel;
    int total_bytes_per_row = (pixel_bytes_per_row + 3) & ~3;
    int padding_size = total_bytes_per_row - pixel_bytes_per_row;

    for(int y = 0; y < height; y += tileSize){
        for(int x = 0; x < width; x += tileSize) {


            int tileWidth = min(tileSize , (width - x));
            int tileHeight = min(tileSize, (height - y));

            int offsetWidth = x * bytes_per_pixel;


            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int count = 0;

            int offsetHeight = 0;

            for (int i = 0; i < tileHeight; i++) {
                int offsetHeight = (y + i) * pixel_bytes_per_row + (y + i) * padding_size;
                for (int j = 0, k = offsetHeight + offsetWidth; j < tileWidth; j++, k += bytes_per_pixel) {
                    sumRed += pixel_data[k + 2];
                    sumGreen += pixel_data[k + 1];
                    sumBlue += pixel_data[k];
                    count++;
                }
            }

            int avgRed = sumRed / count;
            int avgGreen = sumGreen / count;
            int avgBlue = sumBlue / count;


            for (int i = 0; i < tileHeight; i++) {
                int offsetHeight = (y + i) * pixel_bytes_per_row + (y + i) * padding_size;
                for (int j = 0, k2 = offsetHeight + offsetWidth; j < tileWidth; j++, k2 += bytes_per_pixel) {
                    pixel_data[k2 + 2] = avgRed;
                    pixel_data[k2 + 1] = avgGreen;
                    pixel_data[k2] = avgBlue;
                }
            }
        }
    }

    close(fd);

    char newFileName[sizeof(argv[1])+sizeof("_mosaic_")+sizeof(tileSize)+sizeof(".bmp")+1];

   int i = 0;
   char firstpartFileName[sizeof(*fileName)];

   while((*fileName)[i] != '.'){
       firstpartFileName[i] = (*fileName)[i];
       i++;
   }
   firstpartFileName[i] = '\0';
    strcat(newFileName, firstpartFileName);
    strcat(newFileName , "_mosaic_");
    strcat(newFileName , argv[2]);
    strcat(newFileName , ".bmp");


    fd = open(newFileName, O_CREAT | O_RDWR, 0666);
    write(fd, bmp_header, BMP_HEADER_SIZE);


    // Overwrite the pixel data of the opened file
    ssize_t bytes_written = write(fd, pixel_data, image_size - BMP_HEADER_SIZE);
    if (bytes_written != image_size - BMP_HEADER_SIZE) {
        printf("Error: Failed to write pixel data\n");
        exit(EXIT_FAILURE);
    }

    // Close the bitmap file
    close(fd);

    // Free memory
    free(pixel_data);

    return 0;



}



