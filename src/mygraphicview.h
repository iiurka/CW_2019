#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QImage>
#include <QObject>
#include <QMouseEvent>

#define PIPETTE 1
#define SQUARE 2
#define RGBCMP 3
#define CHPARTS 4


class MyGraphicView : public QGraphicsView
{
    Q_OBJECT


public:
    explicit MyGraphicView(QWidget* parent = nullptr);
    ~MyGraphicView();
    int button_pressed = 0;
    int thick;
    QColor color;
    struct Coordinate {
        int x;
        int y;
    }coord, c_end;
    void update(QPixmap pixmap);

signals:
    void selection();

private slots:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:
    QGraphicsItemGroup* group;
    QPixmap pixmap;
    QGraphicsScene* scene;
    bool mouse_press = false;

private:
    void deleteItemsFromGroup(QGraphicsItemGroup*);

};
#endif //MYGRAPHICVIEW_H
