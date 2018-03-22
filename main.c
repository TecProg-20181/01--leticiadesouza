#include <stdio.h>

typedef struct pixelColor {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct imageScale {
    unsigned short int pixel[512][512][3]; // pode ser uma matriz de pixel de 512 1 largura por 512 altura, 3 é o numero de cores
    unsigned int width;
    unsigned int height;
} Image;


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int samePixel(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}


Image grayScale(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int mean = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            mean /= 3;
            img.pixel[i][j][0] = mean;
            img.pixel[i][j][1] = mean;
            img.pixel[i][j][2] = mean;
        }
    }

    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel mean = {0, 0, 0};

            int lessHeight = (height - 1 > i + T/2) ? i + T/2 : height - 1;
            int minWidth = (width - 1 > j + T/2) ? j + T/2 : width - 1; // essa linha faz o mesmo q a de cima
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= lessHeight; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= minWidth; ++y) {
                    mean.r += pixel[x][y][0];
                    mean.g += pixel[x][y][1];
                    mean.b += pixel[x][y][2];
                }
            }

            // printf("%u", mean.r)
            mean.r /= T * T;
            mean.g /= T * T;
            mean.b /= T * T;

            pixel[i][j][0] = mean.r;
            pixel[i][j][1] = mean.g;
            pixel[i][j][2] = mean.b;
        }
    }
}

Image rightRotation(Image img) {
    Image rotateImage;

    rotateImage.width = img.height;
    rotateImage.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotateImage.height; ++i, ++y) {
        for (int j = rotateImage.width - 1, x = 0; j >= 0; --j, ++x) {
            rotateImage.pixel[i][j][0] = img.pixel[x][y][0];
            rotateImage.pixel[i][j][1] = img.pixel[x][y][1];
            rotateImage.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotateImage;
}

void invertColors(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image cropImage(Image img, int x, int y, int width, int height) {
    Image cropped;

    cropped.width = width;
    cropped.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cropped.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cropped.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cropped.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cropped;
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read widthidth height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = grayScale(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < img.height; ++x) {
                    for (unsigned int j = 0; j < img.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = img.pixel[x][j][0];
                        pixel[1] = img.pixel[x][j][1];
                        pixel[2] = img.pixel[x][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int minor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][0] = minor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        minor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][1] = minor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        minor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][2] = minor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                blur(img.height, img.pixel, size, img.width);
                break;
            }
            case 4: { // Rotacao
                int manyTimes = 0;
                scanf("%d", &manyTimes);
                manyTimes %= 4;
                for (int j = 0; j < manyTimes; ++j) {
                    img = rightRotation(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = img.width, height = img.height;

                if (horizontal == 1) width /= 2;
                else height /= 2;

                for (int i2 = 0; i2 < height; ++i2) {
                    for (int j = 0; j < width; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height - 1 - i2;

                        Pixel aux1;
                        aux1.r = img.pixel[i2][j][0];
                        aux1.g = img.pixel[i2][j][1];
                        aux1.b = img.pixel[i2][j][2];

                        img.pixel[i2][j][0] = img.pixel[x][y][0];
                        img.pixel[i2][j][1] = img.pixel[x][y][1];
                        img.pixel[i2][j][2] = img.pixel[x][y][2];

                        img.pixel[x][y][0] = aux1.r;
                        img.pixel[x][y][1] = aux1.g;
                        img.pixel[x][y][2] = aux1.b;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                invertColors(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = cropImage(img, x, y, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print widthidth height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}
