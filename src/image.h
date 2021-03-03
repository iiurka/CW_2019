#ifndef IMAGE_H
#define IMAGE_H 1

#include <QMessageBox>
#include <QImage>
#include <cstdlib>
#include <algorithm>
#define R 1
#define G 2
#define B 3

class Image{
#pragma pack(push, 1)
typedef struct BITMAPFILEHEADER {
    int16_t type;
    int32_t size;
    int16_t reserved1;
    int16_t reserved2;
    int32_t bfOffBits;
} BITMAPFILEHEADER;


typedef struct BITMAPINFOHEADER {
  int32_t size;
  int32_t width;
  int32_t height;
  int16_t planes;
  int16_t bitCount;
  int32_t compression;
  int32_t sizeImage;
  int32_t xPixPerMeter;
  int32_t yPixPerMeter;
  int32_t clrUsed;
  int32_t clrImportant;
} BITMAPINFOHEADER;


typedef struct RGB {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} RGB;
#pragma pack(pop)



public:
    QImage *image;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGB** rgb;
    QPixmap getPixmap();
    int loadImage(const char*);
    int saveImage(const char*);
    int rgb_comp(int, int);
    int drawSquare(int, int, int, int, QColor, bool, QColor);
    int replace(QColor, QColor);
    int edit_often_color(QColor);
    int chParts(int, int, int, int, bool);
};
#endif // IMAGE_H
