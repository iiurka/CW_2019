#include "rgb_window.h"
#include "ui_rgb_window.h"

RGB_window::RGB_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RGB_window)
{
    ui->setupUi(this);
}

RGB_window::~RGB_window() {
    delete ui;
}

void RGB_window::on_ok_clicked()
{
    this->close();
    _close = 0;
}

int RGB_window::comp() {
    if (ui->comp_r->isChecked()) {
        return 1; /* red */
    }
    if (ui->comp_g->isChecked()) {
        return 2; /* green */
    }

    return 3; /* blue */
}

int RGB_window::get_value() {
    if (ui->min->isChecked())
        return ui->min->text().toInt(); /* return zero */
    return ui->max->text().toInt(); /* return 255 */
}

