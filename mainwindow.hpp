#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <stdlib.h>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QScrollBar>
#include <QLineEdit>
#include <QMenuBar>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QLabel>
#include "orthobloc.hpp"
#include "sepulture.hpp"

class OrthoBloc;
class Sepulture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        bool suppressing();
        void deleteSepulture(Sepulture* target);
        QString getPath();
        QString get_savepath();

    private slots:
        void click(int x, int y);
        void btn_act();
        void item_create();
        void btn_suppress();
        void save_file();
        void slotdirSet();
        void slotpathSet();
        void slotexcelSet();
        void slotopenSet();
        void slotorthoSet();
        void slot_newOrtho();
        void slot_loadOrtho();

    private:
        void connect_obj();
        void load_file();
        void load_option();
        void save_option();

        QGraphicsScene *scene_orthophoto;
        QLabel *nomCimetiere;
        QPushButton *btn;
        QPushButton *newItem;
        QPushButton *save;
        QPushButton *suppress;
        OrthoBloc *bloc_ortho;
        QGraphicsPixmapItem *img;
        QLineEdit *identifiant;
        QPen style;
        QBrush infill;
        QFont police;
        QMenu *parameters;
        QMenu *fichiers;
        QActionGroup *tabSet;
        QAction *pathSet;
        QAction *excelSet;
        QAction *openSet;
        QAction *newOrtho;
        QAction *loadOrtho;
        QMessageBox *saved;
        QMessageBox *promptSave;
        QMessageBox *failNewOrtho;

        QPolygon *currentPolygon;
        QList<Sepulture*> items;
        QList<QGraphicsTextItem*> identifiants;
        bool excelUse;
        QString pathExcel;
        QString pathSave;
        QString pathOrtho;
        QString nameOrtho;
        bool saveOk;
};

#endif // MAINWINDOW_HPP
