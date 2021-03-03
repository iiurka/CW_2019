#ifndef RGB_WINDOW_H
#define RGB_WINDOW_H
#include <QDialog>

namespace Ui {
class RGB_window;
}

class RGB_window : public QDialog
{
    Q_OBJECT

public:
    explicit RGB_window(QWidget *parent = nullptr);
    ~RGB_window();
    int comp();
    int get_value();
    int _close = 1;

private slots:
    void on_ok_clicked();

private:
    Ui::RGB_window *ui;
};

#endif // RGB_WINDOW_H
