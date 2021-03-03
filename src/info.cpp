#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);

}

Info::~Info()
{
    delete ui;
}

void Info::setInfo() {
    ui->_name->setText(info.name);
    ui->_path->setText(info.path);
    ui->_version->setText(QString::number(info.version));
    ui->_size->setText(QString::number(info.size) + " байт");
    ui->_width->setText(QString::number(info.width) + " пикселей");
    ui->_height->setText(QString::number(info.height) + " пикселей");
    ui->_readable->setText(info.isReadable ? "Есть" : "Нет");
    ui->_writable->setText(info.isWriteable ? "Есть" : "Нет");
}
