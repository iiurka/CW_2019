#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QFileDialog>
#include <QColor>
#include <QColorDialog>
#include "image.h"
#include "rgb_window.h"
#include "mygraphicview.h"
#include "reference.h"
#include "info.h"

#define BIG_IMG -2
#define SAVE_ERR -1
#define LOAD_ERR -1
#define ALL_OK 0
#define PIPETTE 1
#define SQUARE 2
#define RGBCMP 3
#define CHPARTS 4
#define CORE 12
#define V3 40
#define V4 108
#define V5 124


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
   void selection();

   void on_open_triggered();

   void on_save_triggered();

   void on_rgbcomp_clicked();

   void on_pipette_clicked();

   void on_draw_Square_clicked();

   void on_ch_of_pix_clicked();

   void on_chParts_clicked();

   void on_reference_triggered();

   void on_info_triggered();

private:
    bool check;
    QColor fcolor;
    QColor color;
    uchar button_pressed;
    Ui::MainWindow *ui;
    RGB_window* rgb_window;
    Reference* reference;
    Info* info;
    MyGraphicView* picture;
    Image *img;
};

#endif // MAINWINDOW_H
