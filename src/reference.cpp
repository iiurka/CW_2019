#include "reference.h"
#include "ui_reference.h"

Reference::Reference(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reference)
{
    ui->setupUi(this);
}

Reference::~Reference()
{
    delete ui;
}
