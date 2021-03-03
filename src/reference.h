#ifndef REFERENCE_H
#define REFERENCE_H

#include <QDialog>

namespace Ui {
class reference;
}

class Reference : public QDialog
{
    Q_OBJECT

public:
    explicit Reference(QWidget *parent = nullptr);
    ~Reference();

private:
    Ui::reference *ui;
};

#endif // REFERENCE_H
