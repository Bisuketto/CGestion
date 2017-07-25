#ifndef ORTHOBLOC_HPP
#define ORTHOBLOC_HPP

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
#include <typeinfo>
#include "sepulture.hpp"

class Sepulture;
class MainWindow;

class OrthoBloc : public QGraphicsView
{
    Q_OBJECT
    public:
        OrthoBloc(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
        void mousePressEvent(QMouseEvent *event);
    signals :
        void clicked(int x, int y);
};

#endif // ORTHOBLOC_HPP
