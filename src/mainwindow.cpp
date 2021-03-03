#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = new Image();
    picture = new MyGraphicView();
    reference = new Reference();
    info = new Info();
    rgb_window = new RGB_window();
    statusBar()->showMessage("rgb(0; 0; 0)");
    bool succ = connect(picture, SIGNAL(selection()), this, SLOT(selection()));
    Q_ASSERT(succ);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_open_triggered()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Выберите файл для открытия", "/home/user", "*.bmp");

    if (str == nullptr) return;
    button_pressed = 0;
    picture->button_pressed = 0;
    switch (img->loadImage(str.toLocal8Bit().constData())) {
        case ALL_OK:
            info->info.name = QFileInfo(str).baseName();
            info->info.path = QFileInfo(str).path() + "/" + info->info.name + ".bmp";
            switch(img->bih.size){
                case CORE:
                    img->bih.height = 0;
                    img->bih.width = 0;
                    QMessageBox::critical(this, "Ошибка", "Версия CORE bmp изображения не поддерживается.");
                    return;
                case V3:
                    info->info.version = 3;
                    break;
                case V4:
                    info->info.version = 4;
                    break;
                case V5:
                    info->info.version = 5;
                    break;
                default:
                    img->bih.height = 0;
                    img->bih.width = 0;
                    QMessageBox::critical(this, "Ошибка", "Неизвестная версия bmp файла");
                    return;
            }
            info->info.size = img->bfh.size;
            info->info.width = img->bih.width;
            info->info.height = img->bih.height;
            info->info.isReadable = QFileInfo(str).isReadable();
            info->info.isWriteable = QFileInfo(str).isWritable();
            picture->update(img->getPixmap());
            ui->gridLayout->addWidget(picture);
            return;
        case LOAD_ERR:
            img->bih.height = 0;
            img->bih.width = 0;
            QMessageBox::critical(this, "Ошибка", "Возникла ошибка при открытии файла");
            return;
        case BIG_IMG:
            img->bih.height = 0;
            img->bih.width = 0;
            QMessageBox::critical(this, "Ошибка", "Размер изображения не должен превышать 10000х10000 пикселей");
            return;
    }
}

void MainWindow::on_save_triggered()
{
    if (img->bih.width == 0 || img->bih.height == 0) {
        QMessageBox::critical(this, "Ошибка", "Невозможно сохранить пустое изображение");
        return;
    }
    button_pressed = 0;
    picture->button_pressed = 0;

    QString str = QFileDialog::getSaveFileName(nullptr, "Выберите папку для сохранения", "/home/user", "*.bmp");

    if (str == nullptr) return;

    switch (img->saveImage((str + ".bmp").toLocal8Bit().constData())) {
        case ALL_OK:
            return;
        case SAVE_ERR:
            QMessageBox::critical(this, "Ошибка", "Возникла ошибка при сохранении файла");
            return;
    }
}

void MainWindow::selection() {
    switch (button_pressed) {

        case PIPETTE: {
            QPoint coord(picture->coord.x, picture->coord.y);
            if (coord.x() < 0 || coord.y() >= img->bih.height || coord.y() < 0 || coord.x() >= img->bih.width) {
                QMessageBox::critical(this, "Ошибка", "Точка находится вне изображения");
                return;
            }
            int y = img->bih.height - 1 - coord.y();
            int x = coord.x();
            statusBar()->showMessage("rgb(" + QString::number(img->rgb[y][x].red) + "; " + QString::number(img->rgb[y][x].green) + "; " + QString::number(img->rgb[y][x].blue) + ")");
            return;
        }
        case SQUARE: {
            if (picture->coord.x < 0 || picture->coord.y < 0 || picture->coord.x >= img->bih.width  || picture->coord.y >= img->bih.height || picture->c_end.x < 0 || picture->c_end.y < 0 || picture->c_end.x >= img->bih.width  || picture->c_end.y >= img->bih.height){
                QMessageBox::critical(this, "Ошибка", "Выделенная область выходит за пределы рисунка, попробуйте снова");
                button_pressed = 0;
                return;
            }
            int thick = ui->thick->value();
            if (picture->coord.x - thick + 1 < 0 || picture->coord.y - thick + 1 < 0 || picture->coord.x + thick - 1 > img->bih.width || picture->coord.y + thick - 1 > img->bih.height || picture->c_end.x - thick + 1 < 0 || picture->c_end.y - thick + 1 < 0 || picture->c_end.x + thick - 1 > img->bih.width || picture->c_end.y + thick - 1 > img->bih.height) {
                QMessageBox::critical(this, "Ошибка", "Выделенная область выходит за пределы рисунка, попробуйте снова");
                button_pressed = 0;
                return;
            }
            QPoint point1(picture->coord.x, picture->coord.y);
            QPoint point2(picture->c_end.x, picture->c_end.y);
            img->drawSquare(point1.x() > point2.x() ? point2.x() : point1.x(), point1.y() > point2.y() ? point2.y() : point1.y(), abs(point2.x() - point1.x()), thick, color, check, fcolor);
            picture->update(img->getPixmap());
            check = false;
            button_pressed = 0;
            return;
        }
        case CHPARTS: {
            if (picture->coord.x < 0 || picture->coord.y < 0 || picture->coord.x >= img->bih.width  || picture->coord.y >= img->bih.height || picture->c_end.x < 0 || picture->c_end.y < 0 || picture->c_end.x >= img->bih.width  || picture->c_end.y >= img->bih.height){
                QMessageBox::critical(this, "Ошибка", "Выделенная область выходит за пределы рисунка, попробуйте снова");
                button_pressed = 0;
                return;
            }
            QPoint point1(picture->coord.x, picture->coord.y);
            QPoint point2(picture->c_end.x, picture->c_end.y);
            int x1 = point1.x() > point2.x() ? point2.x() : point1.x();
            int y1 = point1.y() > point2.y() ? point2.y() : point1.y();
            int x2 = point1.x() < point2.x() ? point2.x() : point1.x();
            int y2 = point1.y() < point2.y() ? point2.y() : point1.y();
            img->chParts(x1, y1, x2, y2, ui->diagonally->isChecked());
            picture->update(img->getPixmap());
            button_pressed = 0;
            return;
        }
    }
}

void MainWindow::on_rgbcomp_clicked() {

    if (img->bih.width == 0 || img->bih.height == 0) {
        QMessageBox::critical(this, "Ошибка", "Перед использованием функций откройте изображение");
        return;
    }

    button_pressed = 0;
    picture->button_pressed = 0;

    rgb_window->exec();

    if (rgb_window->_close) return;
    rgb_window->_close = 1;

    int comp = rgb_window->comp();
    int value = rgb_window->get_value();

    img->rgb_comp(comp, value);
    picture->update(img->getPixmap());
}

void MainWindow::on_pipette_clicked() {
    if (img->bih.width == 0 || img->bih.height == 0) {
        QMessageBox::critical(this, "Ошибка", "Перед использованием функций откройте изображение");
        return;
    }
    button_pressed = PIPETTE;
    picture->button_pressed = PIPETTE;
}

void MainWindow::on_draw_Square_clicked() {
    if (img->bih.width == 0 || img->bih.height == 0) {
        QMessageBox::critical(this, "Ошибка", "Перед использованием функций откройте изображение");
        return;
    }
    button_pressed = 0;
    picture->button_pressed = 0;
    color = QColorDialog::getColor(Qt::white, this, "Выберите цвет контура");
    if (!color.isValid()) return;
    check = ui->isFill->isChecked();
    if (check) {
        fcolor = QColorDialog::getColor(Qt::white, this, "Выберите цвет заливки");
        if (!fcolor.isValid()) return;
    }
    picture->color = color;
    picture->thick = ui->thick->value();
    button_pressed = SQUARE;
    picture->button_pressed = SQUARE;
}

void MainWindow::on_ch_of_pix_clicked() {

    if (img->bih.width == 0 || img->bih.height == 0) {
        QMessageBox::critical(this, "Ошибка", "Перед использованием функций откройте изображение");
        return;
    }

    button_pressed = 0;
    picture->button_pressed = 0;

    color = QColorDialog::getColor(Qt::white, this, "Выберите цвет пикселя");
    if (!color.isValid()) return;

    img->edit_often_color(color);
    picture->update(img->getPixmap());
}

void MainWindow::on_chParts_clicked() {

    if (img->bih.width == 0 || img->bih.height == 0) {
        QMessageBox::critical(this, "Ошибка", "Перед использованием функций откройте изображение");
        return;
    }

    button_pressed = CHPARTS;
    picture->button_pressed = CHPARTS;
}

void MainWindow::on_reference_triggered() {
    button_pressed = 0;
    picture->button_pressed = 0;
    reference->show();
}

void MainWindow::on_info_triggered() {

    if (img->bih.width == 0 || img->bih.height == 0) {
        QMessageBox::critical(this, "Ошибка", "Чтобы просмотреть информацию об изображении - загрузите его");
        return;
    }

    button_pressed = 0;
    picture->button_pressed = 0;
    info->setInfo();
    info->exec();
}
