//
// Created by david on 03.05.23.
//

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "MosaicGenerator.c"

#define BMP_HEADER_SIZE 54

int gerneratePicture(char* argv[]) {

    //Konsole einlesen
    char fileName[1000];
    int i1;
    for(i1 = 0; i1 < *(argv[1]); i1++){//funktioniert nicht!!!
        fileName[i1] = argv[1][i1];
        printf("%c, %c\n", fileName[i1], argv[1][i1]);
    }
    fileName[i1] = '\0';

    int pkachel =  atoi(argv[2]);
//Der Pfad ist:          /home/user/Documents/BetriebsystemeLabor/Aufgabe2/example/example.bmp

    //printf("%s \n %s", argv[1], argv[2]);
    //printf("%s \t %s", fileName, argv[1]);
    char* filePath = fileName;
    // Open the bitmap file
    int fd = open(argv[1],  O_RDWR);// argv[1] geht
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

    Image kleinesImage;
    kleinesImage.height = height;
    kleinesImage.width = width;
    kleinesImage.pixel = pixel_data;
    unsigned char* newPixels = generateMosaic(kleinesImage, pkachel);



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

