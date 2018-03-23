#include <stdio.h>

typedef struct pixel_color {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} pixel;

typedef struct image_scale {
    pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} image;


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

/* Compare pixels */
int same_pixel(pixel p1, pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}

/*Get pixel grid */
int pixel_average(pixel pixel_grid){
  return (pixel_grid.r + pixel_grid.g + pixel_grid.b)/3;
}

image gray_scale(image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            int average = pixel_average(image.pixel[i][j]);
            image.pixel[i][j].r = average;
            image.pixel[i][j].g = average;
            image.pixel[i][j].b = average;
        }
    }

    return image;
}

image blur(image image, int T) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            pixel average = {0, 0, 0};

            int minHeight = (image.height - 1 > i + T/2) ? i + T/2 : image.height - 1;
            int minWidth = (image.width - 1 > j + T/2) ? j + T/2 : image.width - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= minHeight; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= minWidth; ++y) {
                    average.r += image.pixel[x][y].r;
                    average.g += image.pixel[x][y].g;
                    average.b += image.pixel[x][y].b;
                }
            }

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

/* Get some image and rotate 90º to the right*/
image right_rotation(image picture) {
    image rotate_image;

    rotate_image.width = picture.height;
    rotate_image.height = picture.width;

    for (unsigned int i = 0, y = 0; i < rotate_image.height; ++i, ++y) {
        for (int j = rotate_image.width - 1, x = 0; j >= 0; --j, ++x) {
            rotate_image.pixel[i][j].r = picture.pixel[x][y].r;
            rotate_image.pixel[i][j].g = picture.pixel[x][y].g;
            rotate_image.pixel[i][j].b = picture.pixel[x][y].b;
        }
    }

    return rotate_image;
}

/* Get some image and invert all pixels*/
image invert_colors(image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            image.pixel[i][j].r = 255 - image.pixel[i][j].r;
            image.pixel[i][j].g = 255 - image.pixel[i][j].g;
            image.pixel[i][j].b = 255 - image.pixel[i][j].b;
        }
    }

    return image;

}

/* Get some image and crop according to the params received*/
image crop_image(image picture, int x, int y, int width, int height) {
    image cropped;

    cropped.width = width;
    cropped.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cropped.pixel[i][j].r = picture.pixel[i + y][j + x].r;
            cropped.pixel[i][j].g = picture.pixel[i + y][j + x].g;
            cropped.pixel[i][j].b = picture.pixel[i + y][j + x].b;
        }
    }

    return cropped;
}


int main() {
    image image;

    /* read type of image*/
    char p3[4];
    scanf("%s", p3);

    /* read width, height and color of image*/
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

    int n_options;
    scanf("%d", &n_options);

    for(int i = 0; i < n_options; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { /* Gray scale*/
                image = gray_scale(image);
                break;
            }
            case 2: { /* Sepia filter*/
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
            case 3: { /* Blur image*/
                int size = 0;
                scanf("%d", &size);
                image = blur(image, size);
                break;
            }
            case 4: { /*Rotate to right*/
                int many_times = 0;
                scanf("%d", &many_times);
                many_times %= 4;
                for (int j = 0; j < many_times; ++j) {
                    image = right_rotation(image);
                }
                break;
            }
            case 5: { /*Mirroring image*/
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = image.width, height = image.height;

                if (horizontal == 1)
                  width = width/2;
                else
                  height = height/2;

                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        int x = i, y = j;

                        if (horizontal == 1) y = image.width - 1 - j;
                        else x = image.height - 1 - i;

                        pixel aux1;
                        aux1.r = image.pixel[i][j].r;
                        aux1.g = image.pixel[i][j].g;
                        aux1.b = image.pixel[i][j].b;

                        image.pixel[i][j].r = image.pixel[x][y].r;
                        image.pixel[i][j].g = image.pixel[x][y].g;
                        image.pixel[i][j].b = image.pixel[x][y].b;

                        image.pixel[x][y].r = aux1.r;
                        image.pixel[x][y].g = aux1.g;
                        image.pixel[x][y].b = aux1.b;
                    }
                }
                break;
            }
            case 6: { /*Invert colors of the image*/
                image = invert_colors(image);
                break;
            }
            case 7: { /*Crop the image*/
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                image = crop_image(image, x, y, width, height);
                break;
            }
        }

    }

    /* print type of image */
    printf("P3\n");
    /* print widthidth height and color of image*/
    printf("%u %u\n255\n", image.width, image.height);

    /* print pixels of image*/
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
