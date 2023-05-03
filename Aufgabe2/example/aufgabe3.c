//
// Created by david on 03.05.23.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BMP_HEADER_SIZE 54

int main(int argc,char* argv[]) {

    //Konsole einlesen
   char* fileName[sizeof (argv[1])];
   *fileName = argv[1];
   int sizeFile =  atoi(argv[2]);
   char* filePath = realpath(*fileName, NULL);

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
    if (bmp_header[30] != 24 || bmp_header[31] != 0 || bmp_header[32] != 24 || bmp_header[33] != 0){
        printf("Error: It's not compress\n");
        exit(EXIT_FAILURE);
    }

    //Check the color selection
    if (bmp_header[46] != 24 || bmp_header[47] != 0 || bmp_header[48] != 24 || bmp_header[49] != 0){
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


}

