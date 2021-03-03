#ifndef INFO_H
#define INFO_H

#include <QDialog>
#include "image.h"

namespace Ui {
class Info;
}

class Info : public QDialog
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = nullptr);
    struct Information {
        QString name;
        QString path;
        int version;
        long size;
        int width;
        int height;
        bool isReadable;
        bool isWriteable;
    }info;
    void setInfo();
    ~Info();

private:
    Ui::Info *ui;
};

#endif // INFO_H
