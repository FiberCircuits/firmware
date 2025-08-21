#include <stdio.h>
#include <string.h>

#define STRIP_NUMBER 5
#define STRIP_LEDS 50

// Character lookup table for a 5x5 font
const int font[40][5] = {
    {0x00, 0x0F, 0x14, 0x0F, 0x00},  // A
    {0x00, 0x1F, 0x15, 0x0A, 0x00},  // B
    {0x00, 0x0E, 0x11, 0x11, 0x00},  // C
    {0x00, 0x1F, 0x11, 0x0E, 0x00},  // D
    {0x00, 0x1F, 0x15, 0x11, 0x00},  // E
    {0x00, 0x1F, 0x14, 0x10, 0x00},  // F
    {0x00, 0x0E, 0x11, 0x17, 0x00},  // G
    {0x00, 0x1F, 0x04, 0x1F, 0x00},  // H
    {0x00, 0x11, 0x1F, 0x11, 0x00},  // I
    {0x00, 0x02, 0x01, 0x1F, 0x00},  // J
    {0x00, 0x1F, 0x04, 0x1B, 0x00},  // K
    {0x00, 0x1F, 0x01, 0x01, 0x00},  // L
    {0x00, 0x1F, 0x08, 0x1F, 0x00},  // M
    {0x00, 0x1F, 0x10, 0x0F, 0x00},  // N
    {0x00, 0x0E, 0x11, 0x0E, 0x00},  // O
    {0x00, 0x1F, 0x14, 0x08, 0x00},  // P
    {0x00, 0x1E, 0x12, 0x1F, 0x00},  // Q
    {0x00, 0x1F, 0x14, 0x0B, 0x00},  // R
    {0x00, 0x09, 0x15, 0x12, 0x00},  // S
    {0x00, 0x10, 0x1F, 0x10, 0x00},  // T
    {0x00, 0x1E, 0x01, 0x1E, 0x00},  // U
    {0x00, 0x1C, 0x03, 0x1C, 0x00},  // V
    {0x00, 0x1F, 0x02, 0x1F, 0x00},  // W
    {0x00, 0x1B, 0x04, 0x1B, 0x00},  // X
    {0x00, 0x18, 0x07, 0x18, 0x00},  // Y
    {0x00, 0x13, 0x15, 0x19, 0x00},  // Z
    {0x00, 0x0E, 0x11, 0x0E, 0x00},  // 0
    {0x00, 0x09, 0x1F, 0x01, 0x00},  // 1
    {0x00, 0x0B, 0x15, 0x09, 0x00},  // 2
    {0x00, 0x11, 0x15, 0x0A, 0x00},  // 3
    {0x00, 0x1C, 0x04, 0x0F, 0x00},  // 4
    {0x00, 0x1D, 0x15, 0x12, 0x00},  // 5
    {0x00, 0x0E, 0x15, 0x02, 0x00},  // 6
    {0x00, 0x10, 0x13, 0x1C, 0x00},  // 7
    {0x00, 0x0A, 0x15, 0x0A, 0x00},  // 8
    {0x00, 0x08, 0x15, 0x0E, 0x00},  // 9
    {0x00, 0x00, 0x00, 0x00, 0x00},  // Space
    {0x00, 0x00, 0x1D, 0x00, 0x00},  // !
    {0x00, 0x00, 0x05, 0x00, 0x00},  // :
    {0x00, 0x00, 0x01, 0x00, 0x00},  // .
};

// Function to generate the 2D array for the given text
void generate_pixels(const char *text, int pixels[STRIP_NUMBER][STRIP_LEDS]) {

    memset(pixels, 0, sizeof(int) * STRIP_NUMBER * STRIP_LEDS);

    int text_length = strlen(text);

    for (int i = 0; i < text_length; ++i) {

        int index = -1;

        char c = text[i];

        if (c >= 'A' && c <= 'Z') {
            index = c - 'A';
        } else if (c >= '0' && c <= '9') {
            index = c - '0' + 26;
        } else if (c == ' ') {
            index = 36;
        } else if (c == '!') {
            index = 37;
        } else if (c == ':') {
            index = 38;
        } else if (c == '.') {
            index = 39;
        }
        if (index != -1) {
            for (int row = 0; row < 5; ++row) {

                for (int col = 0; col < 5; ++col) {

                    if ( (font[index][col]  &  (1 << (4 - row)))  != 0)  {

                        pixels[row][i * 5 + col]  =  1;

                    }
                }
            }
        }
    }
}

// Function to print the 2D array
void print_pixels(int pixels[STRIP_NUMBER][STRIP_LEDS]) {
    for (int i = 0; i < STRIP_NUMBER; ++i) {
        for (int j = 0; j < STRIP_LEDS; ++j) {
            if (pixels[i][j])
                printf("X");
            else
                printf("_");
        }
        printf("\n");
    }
}

// Function to test all characters in the font array
void test_all_characters(int pixels[STRIP_NUMBER][STRIP_LEDS]) {
    char text[11];
    for (int i = 0; i < 40; i += 10) {
        memset(text, 0, sizeof(text));
        for (int j = 0; j < 10 && (i + j) < 40; ++j) {
            if (i + j < 26) {
                text[j] = 'A' + (i + j);
            } else if (i + j < 36) {
                text[j] = '0' + (i + j - 26);
            } else if (i + j == 36) {
                text[j] = ' ';
            } else if (i + j == 37) {
                text[j] = '!';
            } else if (i + j == 38) {
                text[j] = ':';
            } else {
                text[j] = '.';
            }
        }
        generate_pixels(text, pixels);
        print_pixels(pixels);
        printf("\n");
    }
}

int main() {
    int pixels[STRIP_NUMBER][STRIP_LEDS];

    printf("\n");
    test_all_characters(pixels);
    printf("\n");

    generate_pixels(" HELLO...", pixels);
    print_pixels(pixels);
    printf("\n");

    generate_pixels("...WORLD!", pixels);
    print_pixels(pixels);
    printf("\n");

    generate_pixels(" 12:34 PM ", pixels);
    print_pixels(pixels);
    printf("\n");

    char str[11];
    sprintf(str, " %d:%d", 13, 37);
    generate_pixels(str, pixels);
    print_pixels(pixels);
    printf("\n");

    return 0;
}



/*
0	0000
1	0001
2	0010
3	0011
4	0100
5	0101
6	0110
7	0111
8	1000
9	1001
A	1010
B	1011
C	1100
D	1101
E	1110
F	1111
*/
