#include <stdio.h>

typedef struct pixelColor {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct imageScale {
    Pixel pixel[512][512]; // pode ser uma matriz de pixel de 512 1 largura por 512 altura, 3 é o numero de cores
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

int minPixel(Pixel pixelGrid){
  return (pixelGrid.r + pixelGrid.g + pixelGrid.b)/3;
}

Image grayScale(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            int average = minPixel(image.pixel[i][j]);
            image.pixel[i][j].r = average;
            image.pixel[i][j].g = average;
            image.pixel[i][j].b = average;
        }
    }

    return image;
}

Image blur(Image image, int T) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            Pixel average = {0, 0, 0};

            int minHeight = (image.height - 1 > i + T/2) ? i + T/2 : image.height - 1;
            int minWidth = (image.width - 1 > j + T/2) ? j + T/2 : image.width - 1; // essa linha faz o mesmo q a de cima
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= minHeight; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= minWidth; ++y) {
                    average.r += image.pixel[x][y].r;
                    average.g += image.pixel[x][y].g;
                    average.b += image.pixel[x][y].b;
                }
            }

            // printf("%u", average.r)
            average.r /= T * T;
            average.g /= T * T;
            average.b /= T * T;

            image.pixel[i][j].r = average.r;
            image.pixel[i][j].g = average.g;
            image.pixel[i][j].b = average.b;
        }
    }

    return image;
}

Image rightRotation(Image image) {
    Image rotateImage;

    rotateImage.width = image.height;
    rotateImage.height = image.width;

    for (unsigned int i = 0, y = 0; i < rotateImage.height; ++i, ++y) {
        for (int j = rotateImage.width - 1, x = 0; j >= 0; --j, ++x) {
            rotateImage.pixel[i][j].r = image.pixel[x][y].r;
            rotateImage.pixel[i][j].g = image.pixel[x][y].g;
            rotateImage.pixel[i][j].b = image.pixel[x][y].b;
        }
    }

    return rotateImage;
}

Image invertColors(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            image.pixel[i][j].r = 255 - image.pixel[i][j].r;
            image.pixel[i][j].g = 255 - image.pixel[i][j].g;
            image.pixel[i][j].b = 255 - image.pixel[i][j].b;
        }
    }

    return image;

}

Image cropImage(Image image, int x, int y, int width, int height) {
    Image cropped;

    cropped.width = width;
    cropped.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cropped.pixel[i][j].r = image.pixel[i + y][j + x].r;
            cropped.pixel[i][j].g = image.pixel[i + y][j + x].g;
            cropped.pixel[i][j].b = image.pixel[i + y][j + x].b;
        }
    }

    return cropped;
}


int main() {
    Image image;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read widthidth height and color of image
    int max_color;
    scanf("%u %u %d", &image.width, &image.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            scanf("%hu %hu %hu", &image.pixel[i][j].r,
                                 &image.pixel[i][j].g,
                                 &image.pixel[i][j].b);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                image = grayScale(image);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < image.height; ++x) {
                    for (unsigned int j = 0; j < image.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = image.pixel[x][j].r;
                        pixel[1] = image.pixel[x][j].g;
                        pixel[2] = image.pixel[x][j].b;

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int minor_r = (255 >  p) ? p : 255;
                        image.pixel[x][j].r = minor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        minor_r = (255 >  p) ? p : 255;
                        image.pixel[x][j].g = minor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        minor_r = (255 >  p) ? p : 255;
                        image.pixel[x][j].b = minor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                image = blur(image, size);
                break;
            }
            case 4: { // Rotacao
                int manyTimes = 0;
                scanf("%d", &manyTimes);
                manyTimes %= 4;
                for (int j = 0; j < manyTimes; ++j) {
                    image = rightRotation(image);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = image.width, height = image.height;

                if (horizontal == 1) width /= 2;
                else height /= 2;

                for (int i2 = 0; i2 < height; ++i2) {
                    for (int j = 0; j < width; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = image.width - 1 - j;
                        else x = image.height - 1 - i2;

                        Pixel aux1;
                        aux1.r = image.pixel[i2][j].r;
                        aux1.g = image.pixel[i2][j].g;
                        aux1.b = image.pixel[i2][j].b;

                        image.pixel[i2][j].r = image.pixel[x][y].r;
                        image.pixel[i2][j].g = image.pixel[x][y].g;
                        image.pixel[i2][j].b = image.pixel[x][y].b;

                        image.pixel[x][y].r = aux1.r;
                        image.pixel[x][y].g = aux1.g;
                        image.pixel[x][y].b = aux1.b;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                image = invertColors(image);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                image = cropImage(image, x, y, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print widthidth height and color of image
    printf("%u %u\n255\n", image.width, image.height);

    // print pixels of image
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j].r,
                                   image.pixel[i][j].g,
                                   image.pixel[i][j].b);

        }
        printf("\n");
    }
    return 0;
}
