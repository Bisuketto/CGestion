#include "mainwindow.hpp"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("data/splash.png"));
    splash->show();

    QFile *fichier = new QFile("parameters.ini");
    fichier->open(QIODevice::ReadOnly);
    QString ligne = fichier->readLine();
    QStringList infos = ligne.split(QRegExp("\"*\""));
    QTranslator translator;
    translator.load("translations/cgestion_" + infos[1]);
    a.installTranslator(&translator);

    MainWindow w;

    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(2500, &w, SLOT(show()));

    return a.exec();
}
