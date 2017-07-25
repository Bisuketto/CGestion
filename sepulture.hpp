#ifndef SEPULTURE_HPP
#define SEPULTURE_HPP

#include <QGraphicsPolygonItem>
#include <QMouseEvent>
#include <QString>
#include "mainwindow.hpp"

class MainWindow;
class Orthobloc;

class Sepulture : public QGraphicsPolygonItem
{
    public:
        Sepulture();
        Sepulture(const QPolygonF &polygon, QGraphicsItem *parent = Q_NULLPTR);
        Sepulture(const QPolygonF &polygon, MainWindow *_program, QGraphicsItem *parent = Q_NULLPTR);
        ~Sepulture();
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void setIdentifiant(QString _id);
        QString getID();

    private:
        QString identifiant;
        MainWindow *program;
};

#endif // SEPULTURE_HPP
