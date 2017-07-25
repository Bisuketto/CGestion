#include "orthobloc.hpp"

OrthoBloc::OrthoBloc(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{

}

void OrthoBloc::mousePressEvent(QMouseEvent *event){
    emit clicked(event->x(), event->y());
}

