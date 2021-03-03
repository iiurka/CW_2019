#include "mygraphicview.h"
#include "mainwindow.h"

MyGraphicView::MyGraphicView(QWidget *parent) : QGraphicsView(parent) {
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    group = new QGraphicsItemGroup();
}

void MyGraphicView::update(QPixmap pixmap) {
    scene = new QGraphicsScene();
    scene->addPixmap(pixmap);
    this->setScene(scene);
}

void MyGraphicView::mousePressEvent(QMouseEvent* event) {
    QPoint pos = mapToScene(event->pos()).toPoint();
    coord.x = pos.x();
    coord.y = pos.y();
    if (button_pressed != PIPETTE)
        mouse_press = true;
}

void MyGraphicView::mouseReleaseEvent(QMouseEvent* event) {
    if (mouse_press && button_pressed != SQUARE) {
        QPoint pos = mapToScene(event->pos()).toPoint();
        c_end.x = pos.x();
        c_end.y = pos.y();
        if (c_end.x < 0)
            c_end.x = 0;
        if (c_end.y < 0)
            c_end.y = 0;
    }
    mouse_press = false;
    this->deleteItemsFromGroup(group);

    if (button_pressed != PIPETTE) button_pressed = 0;

    emit selection();
}

void MyGraphicView::mouseMoveEvent(QMouseEvent *event) {
    if (mouse_press && button_pressed == SQUARE) {
        QPoint pos = mapToScene(event->pos()).toPoint();
        int x = pos.x();
        int y = pos.y();
        this->deleteItemsFromGroup(group);
        group = new QGraphicsItemGroup();
        QPen penColor(color);
        if (x < 0)
            x = 0;
        y = ((y >= coord.y) ? coord.y + abs(x - coord.x) : coord.y - abs(x - coord.x));
        if (y < 0) {
            y = 0;
        }
        if (y == 0) {
            x = (x >= coord.x) ? coord.x + coord.y : coord.x - coord.y;
        }
        group->addToGroup(scene->addLine(coord.x, coord.y, x, coord.y, penColor));
        group->addToGroup(scene->addLine(x, coord.y, x, y, penColor));
        group->addToGroup(scene->addLine(x, y, coord.x, y, penColor));
        group->addToGroup(scene->addLine(coord.x, y, coord.x, coord.y, penColor));
        scene->addItem(group);
        c_end.x = x;
        c_end.y = y;
    } else if (mouse_press && button_pressed == CHPARTS) {
        QPoint pos = mapToScene(event->pos()).toPoint();
        int x = pos.x();
        int y = pos.y();
        this->deleteItemsFromGroup(group);
        group = new QGraphicsItemGroup();
        QPen penBlack(Qt::black);
        if (x<0)
            x=0;
        if (y<0)
            y=0;
        group->addToGroup(scene->addLine(coord.x, coord.y, x, coord.y, penBlack));
        group->addToGroup(scene->addLine(x, coord.y, x, y, penBlack));
        group->addToGroup(scene->addLine(x, y, coord.x, y, penBlack));
        group->addToGroup(scene->addLine(coord.x, y, coord.x, coord.y, penBlack));
        scene->addItem(group);
    }
}

MyGraphicView::~MyGraphicView() {
    delete group;
    delete scene;
}

void MyGraphicView::deleteItemsFromGroup(QGraphicsItemGroup *group) {
    foreach(QGraphicsItem *item, scene->items())
       if(item->group() == group)
          delete item;
}
