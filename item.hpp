#ifndef ITEM_HPP
#define ITEM_HPP

#include <QGraphicsPolygonItem>
#include <QMouseEvent>

class Item : public QGraphicsPolygonItem
{
    Q_OBJECT

    public:
        Item();
        void mousePressEvent(QMouseEvent *event);
    signals:
        void clicked();
};

#endif // ITEM_HPP
