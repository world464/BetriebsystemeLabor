#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BMP_HEADER_SIZE 54

int main() {

    // Open the bitmap file
    int fd = open("example.bmp", O_RDWR);
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

    // Change every odd row to black
    for (int i = 1; i < height; i+=2) {
        int offset = i * pixel_bytes_per_row + i * padding_size;
        for (int j = 0, k = offset; j < width; j++, k += bytes_per_pixel) {
            pixel_data[k] = pixel_data[k + 1] = pixel_data[k + 2] = 0;
        }
    }

    // Reset the file pointer to the start of the pixel data
    lseek(fd, BMP_HEADER_SIZE, SEEK_SET);

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
