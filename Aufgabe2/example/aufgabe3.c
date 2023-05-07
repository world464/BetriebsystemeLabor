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

int min(int x, int y){
    if(x > y){//Gibt immer das groessere zurueck
        return y;
    } else{
        return x;
    }
}

int gerneratePicture(char* argv[]) {

    //Konsole einlesen
    /*char fileName[1000];
   int i1;
    for(i1 = 0; i1 < *(argv[1]); i1++){//funktioniert nicht!!!
        fileName[i1] = argv[1][i1];
        printf("%c, %c\n", fileName[i1], argv[1][i1]);
    }
    fileName[i1] = '\0';
*/
    int tileSize =  atoi(argv[2]);
//Der Pfad ist:          /home/user/Documents/BetriebsystemeLabor/Aufgabe2/example/example.bmp

    //printf("%s \n %s", argv[1], argv[2]);
    //printf("%s \t %s", fileName, argv[1]);
    //char* filePath = fileName;
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
    int32_t width = *(int32_t*) &bmp_header[18] ;
    int32_t height = *(int32_t*) &bmp_header[22];
    uint16_t bit_depth = *(uint16_t*) &bmp_header[28];

    // Print the image size, width, height and bit depth for debugging purposes
    printf( "width: %i, height: %i, size: %i\n", width, height, image_size);
    printf( "bitdepth: %i\n", bit_depth);

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


    // Pixel data is upside-down, so the first pixel in the data is the pixel on the bottom left.
    // So, to get the value of a pixel with given coordinates x and y, this circumstance has to be considered.
    // However, this only applies when width and height are positive.

    int bytes_per_pixel = bit_depth / 8; // Careful: this only works for bit depths which are a multiples of 8
    int pixel_bytes_per_row = width * bytes_per_pixel;

    // Calculate the amount of necessary bytes for each row so that the next row starts at a 4-byte boundary.
    // This is done by adding 3 to the amount of bytes in a row and then masking the last two bits.
    // A more exhaustive calculation would be something like ((width * bit_depth + 31) / 32) * 4;
    int total_bytes_per_row = (pixel_bytes_per_row + 3) & ~3;

    // The padding size is the number of bytes which are added to the end of each row, so that the next row starts at a 4-byte boundary.
    // E.g. if the image width is 15 pixels, the row size is 48 bytes (15 pixels * 3 bytes per pixel + 3 bytes padding).
    // Calculate the padding size by subtracting the amount of pixel bytes in a row from the amount of total bytes in a row.
    int padding_size = total_bytes_per_row - pixel_bytes_per_row;

    // Only bitmaps with a bit depth of 24 bit and positive width and height are considered.
    // That means e.g. assuming the bottom left is (0|0), (0|14) should therefore be the top left pixel of the resulting image,
    // so it is the first pixel of the last row which starts at k = height-1 * width * 3 + (height-1) * padding_size.

    // At position k the byte corresponds to the blue value of the pixel. The next byte (+1) is green and +2 is red.
    // This is because the pixel values are stored in reverse order (BGR).

    /*// Change every odd row to black
    for (int i = 0; i < height; i++) {//odd = uneven
        int offset = i * pixel_bytes_per_row + i * padding_size;
        for (int j = 0, k = offset; j < width; j++, k += bytes_per_pixel) {
            if(i % 2 == 1){
            pixel_data[k] = pixel_data[k + 1] = pixel_data[k + 2] = 0;
            } else
            {
                pixel_data[k] = pixel_data[k + 1] = pixel_data[k + 2] = 0xff;
            }
        }
    }*/

   /* //Reset Picture
    for (int i = 0; i < height; i++) {
        int offset = i * pixel_bytes_per_row + i * padding_size;
        for (int j = 0, k = offset; j < width; j++, k += bytes_per_pixel) {
            pixel_data[k] = pixel_data[k + 1] = pixel_data[k + 2] = 0xff;
        }
    }*/

    // Change every odd row to black
    for (int i = 1; i < height; i+=2) {//odd = uneven
        int offset = i * pixel_bytes_per_row + i * padding_size;
        for (int j = 0, k = offset; j < width; j++, k += bytes_per_pixel) {
            pixel_data[k] = pixel_data[k + 1] = pixel_data[k + 2] = 0;
        }
    }


   // Print pixel data
    for (int i = 0; i < height; i++) {
        int offset = i * pixel_bytes_per_row + i * padding_size;
        for (int j = 0, k = offset; j < width; j++, k += bytes_per_pixel) {
            printf("%02x%02x%02x ", pixel_data[k + 2], pixel_data[k + 1], pixel_data[k]);
        }
        offset += pixel_bytes_per_row;
        for (int j = 0; j < padding_size; j++) {
            printf("%02x ", pixel_data[offset + j]);
        }
        printf("\n");
    }


    //TODO: picture does not work up to now
    //TODO: Title has to be changed

    //unsigned char* newPixels = generateMosaic(kleinesImage, tileSize);

   for(int y = 0; y < height; y += tileSize){
        int offsetHeightY = y * pixel_bytes_per_row + y * padding_size;
        for(int32_t x = 0; x < width; x += tileSize) {
            int32_t difWidth = (width - x);
            int32_t difHeight = (height - y);
            int32_t tileWidth = min(tileSize, difWidth);
            int32_t tileHeight = min(tileSize, difHeight);//min war falsch!!!

            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int count = 0;

            //Von einer Kachel wird der Mittelwert berechnet
            for (int i = 0; i < tileHeight; i++) {
                int offsetHeight = (y + i) * pixel_bytes_per_row + (y + i) * padding_size;
                for (int j = 0, k = offsetHeight + x; j < tileWidth; j++, k += bytes_per_pixel) {
                    sumRed += pixel_data[k + 2];//Plus vergessen!!!!!!
                    sumGreen += pixel_data[k + 1];
                    sumBlue += pixel_data[k];
                    count++;
                }
            }

            int avgRed = sumRed / count;
            int avgGreen = sumGreen / count;
            int avgBlue = sumBlue / count;
            //printf("Red %d, Green %d, Blue %d",avgRed, avgGreen, avgBlue);

           //Eine Kachel mit dem Mittelwert ueberschrieben
            for (int i = 0; i < tileHeight; i++) {
                int offsetHeight = (y + i) * pixel_bytes_per_row + (y + i) * padding_size;
                for (int j = 0, k2 = offsetHeight + x; j < tileWidth; j++, k2 += bytes_per_pixel) {
                    pixel_data[k2 + 2] = avgRed;
                    pixel_data[k2 + 1] = avgGreen;
                    pixel_data[k2] = avgBlue;
                }
            }
        }
    }


    // Print pixel data
    for (int i = 0; i < height; i++) {
        int offset = i * pixel_bytes_per_row + i * padding_size;
        for (int j = 0, k = offset; j < width; j++, k += bytes_per_pixel) {
            printf("%02x%02x%02x ", pixel_data[k + 2], pixel_data[k + 1], pixel_data[k]);
        }
        offset += pixel_bytes_per_row;
        for (int j = 0; j < padding_size; j++) {
            printf("%02x ", pixel_data[offset + j]);
        }
        printf("\n");
    }


    // Reset the file pointer to the start of the pixel data
    lseek(fd, BMP_HEADER_SIZE, SEEK_SET);


    //Hier passiert nichts!!!
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

