#include "image.h"

using namespace std;

int Image::loadImage(const char *file) {
    FILE* f = fopen(file, "rb");
    if (!f)
        return -1;
    fread(&bfh, sizeof(bfh), 1, f);
    fread(&bih, sizeof(bih), 1, f);
    if (bih.height > 10000 || bih.width > 10000)
        return -2;
    size_t padding = 0;
    if ((bih.width*3) % 4)
        padding = 4 - (bih.width*3) % 4;
    rgb = new RGB* [bih.height];
    for (int i = 0; i < bih.height; i++){
        rgb[i] = new RGB[bih.width + 1];
    }
    fseek(f, long(bfh.bfOffBits), SEEK_SET);
    for (int i = 0; i <  bih.height; i++) {
        int j;
        for (j = 0;  j < bih.width; j++){
            fread(&rgb[i][j], sizeof(RGB), 1, f);
        }
        if (padding)
            fread(&rgb[i][j], padding, 1, f);
    }
    fclose(f);
    return 0;
}

int Image::saveImage(const char* file) {
    FILE* f = fopen(file, "wb");
    if (!f)
        return -1;
    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);
    size_t padding = 0;
    if ((bih.width*3) % 4)
        padding = 4 - (bih.width*3) % 4;
    fseek(f, long(bfh.bfOffBits), SEEK_SET);
    for (int i = 0; i < bih.height; i++) {
        int j;
        for (j = 0; j < bih.width; j++){
            fwrite(&rgb[i][j], sizeof(RGB), 1, f);
        }
        if (padding)
            fwrite(&rgb[i][j], padding, 1, f);
    }
    fclose(f);
    return 0;
}

QPixmap Image::getPixmap() {
    image = new QImage(bih.width, bih.height, QImage::Format_RGB16);
    QColor pixel;
    for (int i = bih.height - 1; i >= 0; i--) {
        for (int j = 0; j < bih.width; j++) {
            pixel.setRed(rgb[i][j].red);
            pixel.setGreen(rgb[i][j].green);
            pixel.setBlue(rgb[i][j].blue);
            image->setPixel(j, bih.height - i - 1, pixel.rgb());
        }
    }
    return QPixmap::fromImage(*image);
}

int Image::rgb_comp(int comp, int value) {

    if (comp == R) {
        for (int i = 0; i <  bih.height; i++)
            for (int j = 0;  j < bih.width; j++)
                rgb[i][j].red = uchar(value);
    } else if (comp == G) {
        for (int i = 0; i <  bih.height; i++)
            for (int j = 0;  j < bih.width; j++)
                rgb[i][j].green = uchar(value);
    } else if (comp == B) {
        for (int i = 0; i <  bih.height; i++)
            for (int j = 0;  j < bih.width; j++)
                rgb[i][j].blue = uchar(value);
    }

    return 0;
}

int Image::drawSquare(int x, int y, int len, int thick, QColor color, bool fill, QColor fcolor) {

    for (int i = x - thick + 1; i <= x + len + thick - 1; i++)
        for (int j = -thick + 1; j <= 0; j++) {
            rgb[bih.height - 1 - y - j][i].red = uchar(color.red());
            rgb[bih.height - 1 - y - j][i].green = uchar(color.green());
            rgb[bih.height - 1 - y - j][i].blue = uchar(color.blue());
                                                                        /* horizontal lines */
            rgb[bih.height - 1 - y - len + j][i].red = uchar(color.red());
            rgb[bih.height - 1 - y - len + j][i].green = uchar(color.green());
            rgb[bih.height - 1 - y - len + j][i].blue = uchar(color.blue());
        }

    for (int j = bih.height - 1 - y + thick - 1; j >= bih.height - 1 - y - len - thick + 1; j--)
        for (int i = -thick + 1; i <= 0; i++) {
            rgb[j][x + i].red = uchar(color.red());
            rgb[j][x + i].green = uchar(color.green());
            rgb[j][x + i].blue = uchar(color.blue());
                                                                        /* vertical lines */
            rgb[j][x + len - i].red = uchar(color.red());
            rgb[j][x + len - i].green = uchar(color.green());
            rgb[j][x + len - i].blue = uchar(color.blue());
        }

    /* fill */

    if (fill) {
        for (int j = bih.height - y - 2; j >= bih.height - y - len; j--)
            for (int i = x + 1; i < x + len; i++) {
                rgb[j][i].red = uchar(fcolor.red());
                rgb[j][i].green = uchar(fcolor.green());
                rgb[j][i].blue = uchar(fcolor.blue());
            }
    }
    return 0;
}

typedef struct my_Dict {
    QColor color;
    long amount;
}dict;

void add(dict* d, QColor color) {
    d->amount = 1;
    d->color = color;
}

long in(dict* d, QColor color, long size) {
    for (long i = 0; i < size; i++)
        if (d[i].color == color)
            return i;
    return -1;
}

QColor find_max(dict* d, long size) {

    long max = d->amount;
    long max_i = 0;

    for (int i = 1; i < size; i++)
        if (d[i].amount > max) {
            max = d[i].amount;
            max_i = i;
        }

    return d[max_i].color;
}

int Image::replace(QColor rgb1, QColor rgb2) {

    for (int i = 0; i <  bih.height; i++)
        for (int j = 0;  j < bih.width; j++)
            if (rgb[i][j].red == rgb1.red() && rgb[i][j].blue == rgb1.blue() && rgb[i][j].green == rgb1.green()) {
                rgb[i][j].red = uchar(rgb2.red());
                rgb[i][j].blue = uchar(rgb2.blue());
                rgb[i][j].green = uchar(rgb2.green());
            }

    return 0;
}

int Image::edit_often_color(QColor color) {

    long size = 1000000;
    long pos = 1;
    QColor temp(rgb[0][0].red, rgb[0][0].green, rgb[0][0].blue);
    dict* d = new dict[size];

    add(d, temp);

    for (int i = 0; i <  bih.height; i++)
        for (int j = 1;  j < bih.width; j++) {

            temp.setRgb(rgb[i][j].red, rgb[i][j].green, rgb[i][j].blue);
            long check = in(d, temp, pos);

            if (pos == size) {
                size *= 2;
                dict* tmp = d;
                d = new dict[size];
                memcpy(d, tmp, ulong(pos) * sizeof(dict));
                delete [] tmp;
            }

            if (check == -1) {
                add(d + pos++, temp);
            } else {
                d[check].amount++;
            }
        }

    QColor of_pix = find_max(d, pos);

    Image::replace(of_pix, color);

    delete [] d;

    return 0;
}

int Image::chParts(int x1, int y1, int x2, int y2, bool isDiagonally) {

    if (isDiagonally) {
        for (int i = y1; i < y1 + (y2 - y1) / 2; i++)
            for (int j = x1; j < x1 + (x2 - x1) / 2 ; j++) {
                swap(rgb[bih.height - i - 1][j].red, rgb[bih.height - i - 1 - (y2 - y1) / 2][j + (x2 - x1) / 2].red);
                swap(rgb[bih.height - i - 1][j].green, rgb[bih.height - i - 1 - (y2 - y1) / 2][j + (x2 - x1) / 2].green);
                swap(rgb[bih.height - i - 1][j].blue, rgb[bih.height - i - 1 - (y2 - y1) / 2][j + (x2 - x1) / 2].blue);

                swap(rgb[bih.height - i - 1 - (y2 - y1) / 2][j].red, rgb[bih.height - i - 1][j + (x2 - x1) / 2].red);
                swap(rgb[bih.height - i - 1 - (y2 - y1) / 2][j].green, rgb[bih.height - i - 1][j + (x2 - x1) / 2].green);
                swap(rgb[bih.height - i - 1 - (y2 - y1) / 2][j].blue, rgb[bih.height - i - 1][j + (x2 - x1) / 2].blue);
            }

    } else {
        for (int i = y1; i < y1 + (y2 - y1) / 2; i++)
            for (int j = x1; j < x1 + (x2 - x1) / 2 ; j++) {
                swap(rgb[bih.height - i - 1][j].red, rgb[bih.height - i - 1][j + (x2 - x1) / 2].red);
                swap(rgb[bih.height - i - 1][j].green, rgb[bih.height - i - 1][j + (x2 - x1) / 2].green);
                swap(rgb[bih.height - i - 1][j].blue, rgb[bih.height - i - 1][j + (x2 - x1) / 2].blue);

                swap(rgb[bih.height - i - 1][j].red, rgb[bih.height - i - 1 - (y2 - y1) / 2][j].red);
                swap(rgb[bih.height - i - 1][j].green, rgb[bih.height - i - 1 - (y2 - y1) / 2][j].green);
                swap(rgb[bih.height - i - 1][j].blue, rgb[bih.height - i - 1 - (y2 - y1) / 2][j].blue);

                swap(rgb[bih.height - i - 1 - (y2 - y1) / 2][j].red, rgb[bih.height - i - 1 - (y2 - y1) / 2][j + (x2 - x1) / 2].red);
                swap(rgb[bih.height - i - 1 - (y2 - y1) / 2][j].green, rgb[bih.height - i - 1 - (y2 - y1) / 2][j + (x2 - x1) / 2].green);
                swap(rgb[bih.height - i - 1 - (y2 - y1) / 2][j].blue, rgb[bih.height - i - 1 - (y2 - y1) / 2][j + (x2 - x1) / 2].blue);
            }
    }

    return 0;
}
