#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    currentPolygon = new QPolygon();
    pathExcel = "";
    pathSave = "";
    pathOrtho = "";
    nameOrtho = "";
    excelUse = true;
    saveOk = true;
    load_option();

    infill.setColor(Qt::darkRed);
    infill.setStyle(Qt::Dense6Pattern);
    style.setColor(Qt::red);
    style.setWidth(2);
    police.setPointSize(12);

    QPixmap *logo = new QPixmap("data/logo.png");
    QLabel *labelLogo = new QLabel();
    labelLogo->setPixmap(*logo);
    labelLogo->setMargin(20);
    nomCimetiere = new QLabel(tr("Cimetière : "));
    save = new QPushButton(tr("Sauvegarder"), this);
    //btn = new QPushButton("Spawn", this);
    newItem = new QPushButton(tr("Nouvel Item"), this);
    newItem->setCheckable(true);
    newItem->setChecked(false);
    identifiant = new QLineEdit(tr("Nouvel Identifiant"));
    identifiant->setEnabled(false);
    identifiant->setFixedWidth(200);
    suppress = new QPushButton(tr("Supprimer Item"), this);
    suppress->setCheckable(true);
    suppress->setChecked(false);
    QVBoxLayout *buttonParts = new QVBoxLayout();
    buttonParts->addWidget(labelLogo);
    buttonParts->addWidget(nomCimetiere);
    buttonParts->addWidget(save);
    //buttonParts->addWidget(btn);
    buttonParts->addWidget(newItem);
    buttonParts->addWidget(identifiant);
    buttonParts->addWidget(suppress);

    scene_orthophoto = new QGraphicsScene(this);
    QPixmap ortho(pathOrtho);
    img = scene_orthophoto->addPixmap(ortho);
    qDebug() << img->zValue();
    bloc_ortho = new OrthoBloc(scene_orthophoto);

    QHBoxLayout *allParts = new QHBoxLayout();
    allParts->addWidget(bloc_ortho);
    allParts->addLayout(buttonParts);
    QWidget *central = new QWidget();
    central->setLayout(allParts);
    this->setCentralWidget(central);

    excelSet = new QAction(tr("Excel"));
    excelSet->setCheckable(true);
    openSet = new QAction(tr("OpenOffice/LibreOffice"));
    openSet->setCheckable(true);
    pathSet = new QAction(tr("Chemin du tableur"));
    newOrtho = new QAction(tr("Creer un nouveau fichier Cimetière"));
    loadOrtho = new QAction(tr("Charger un fichier Cimetière"));
    QAction *separat = new QAction();
    separat->setSeparator(true);
    tabSet = new QActionGroup(this);
    tabSet->addAction(pathSet);
    tabSet->addAction(separat);
    tabSet->addAction(excelSet);
    tabSet->addAction(openSet);
    excelSet->setChecked(excelUse);
    openSet->setChecked(!excelUse);

    fichiers = this->menuBar()->addMenu(tr("Fichier"));
    parameters = this->menuBar()->addMenu(tr("Paramètres"));
    fichiers->addAction(newOrtho);
    fichiers->addAction(loadOrtho);
    QMenu *tabMenu = parameters->addMenu(tr("Paramètres du Tableur"));
    tabMenu->addActions(tabSet->actions());

    saved = new QMessageBox();
    saved->setText(tr("Données sauvegardées"));
    saved->setIcon(QMessageBox::Information);
    promptSave = new QMessageBox();
    promptSave->setText(tr("Des données n'ont pas été sauvegardées"));
    promptSave->setInformativeText(tr("Sauvegarder ?"));
    promptSave->setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
    promptSave->setDefaultButton(QMessageBox::Save);
    promptSave->setIcon(QMessageBox::Question);
    failNewOrtho = new QMessageBox();
    failNewOrtho->setText(tr("Echec de la creation du nouveau Cimetière"));
    failNewOrtho->setIcon(QMessageBox::Warning);

    QIcon *icone = new QIcon("logo.png");
    setWindowIcon(*icone);

    connect_obj();
    load_file();
}

void MainWindow::connect_obj(){
    QObject::connect(bloc_ortho, SIGNAL(clicked(int, int)), this, SLOT(click(int, int)));
    //QObject::connect(btn, SIGNAL(clicked(bool)), this, SLOT(btn_act()));
    QObject::connect(newItem, SIGNAL(toggled(bool)), this, SLOT(item_create()));
    QObject::connect(save, SIGNAL(clicked(bool)), this, SLOT(save_file()));
    QObject::connect(suppress, SIGNAL(clicked(bool)), this, SLOT(btn_suppress()));
    QObject::connect(pathSet, SIGNAL(triggered(bool)), this, SLOT(slotpathSet()));
    QObject::connect(newOrtho, SIGNAL(triggered(bool)), this, SLOT(slot_newOrtho()));
    QObject::connect(loadOrtho, SIGNAL(triggered(bool)), this, SLOT(slot_loadOrtho()));
    QObject::connect(excelSet, SIGNAL(triggered(bool)), this, SLOT(slotexcelSet()));
    QObject::connect(openSet, SIGNAL(triggered(bool)), this, SLOT(slotopenSet()));
    QObject::connect(identifiant, SIGNAL(returnPressed()), newItem, SLOT(click()));
}

void MainWindow::load_file(){
    QFile *fichier = new QFile((pathSave + "/" + nameOrtho + "/"+ nameOrtho +".csav").toLocal8Bit());
    qDebug() << (pathSave + "/" + nameOrtho + "/"+ nameOrtho +".csav").toLocal8Bit();
    nomCimetiere->setText(tr("Cimetière : ") + nameOrtho);
    fichier->open(QIODevice::ReadOnly);
    QString firstLine = fichier->readLine();
    while(!fichier->atEnd()){
        QString ligne = fichier->readLine();
        QStringList noms = ligne.split(QRegExp("\"*\""));
        QStringList infos = noms[2].split(" ");
        qDebug() << noms;
        qDebug() << infos;
        currentPolygon->clear();
        for(int i = 1; i+1 < infos.length(); i += 2){
            *currentPolygon << QPoint(infos[i].toInt(), infos[i+1].toInt());
        }
        Sepulture *newSepulture = new Sepulture(*currentPolygon, this);
        items.append(newSepulture);
        newSepulture->setIdentifiant(noms[1]);
        newSepulture->setPen(style);
        newSepulture->setBrush(infill);
        newSepulture->setZValue(1);
        scene_orthophoto->addItem(newSepulture);
        QGraphicsTextItem *id = scene_orthophoto->addText(noms[1]);
        identifiants.append(id);
        id->setFont(police);
        id->setDefaultTextColor(QColor(Qt::darkYellow));
        id->setPos(currentPolygon->at(0));
        id->setZValue(1);
    }
    fichier->close();
}

void MainWindow::save_file(){
    if(!QDir((pathSave + "/" + nameOrtho).toLocal8Bit()).exists())
        QDir().mkdir((pathSave + "/" + nameOrtho).toLocal8Bit());

    QFileInfo check(pathSave + "/" + nameOrtho + "/"+ nameOrtho +".csav");
    if(check.exists() && check.isFile())
        remove((pathSave + "/" + nameOrtho + "/"+ nameOrtho +".csav").toLocal8Bit());

    QFile *fichier = new QFile((pathSave + "/" + nameOrtho + "/"+ nameOrtho +".csav").toLocal8Bit());
    fichier->open(QIODevice::WriteOnly);
    fichier->write(("\"" + nameOrtho + "\" \"" + pathSave + "\" \"" + pathOrtho + "\"\n").toLocal8Bit());
    for(int i = 0; i < items.length(); i++){
        QString ligne = "\"" + items[i]->getID() + "\"";
        for(int j = 0; j < items[i]->polygon().length(); j++){
            QString x;
            x.setNum(items[i]->polygon()[j].x());
            QString y;
            y.setNum(items[i]->polygon()[j].y());
            ligne += " " + x + " " + y;
        }
        ligne += "\n";
        fichier->write(ligne.toLocal8Bit());
    }
    fichier->close();
    saveOk = true;
    saved->exec();
}

void MainWindow::btn_act(){
    system("start \"C:\\Program Files (x86)\\Microsoft Office\\root\\Office16\\EXCEL.EXE\" \"C:\\Users\\Biscuit\\Desktop\\blop.xlsx\"");
}

QString MainWindow::getPath(){
    QString path;
    path =  "\"" + pathExcel + "\" \"" + pathSave + "/" + nameOrtho + "/";
    return path;
}

QString MainWindow::get_savepath(){
    return pathSave + "/" +nameOrtho;
}

void MainWindow::item_create(){
    if(newItem->isChecked()){
        suppress->setEnabled(false);
        newItem->setText(tr("Terminer l'item"));
        identifiant->setEnabled(true);
        identifiant->clear();
        currentPolygon->clear();
    }
    else{
        saveOk = false;
        suppress->setEnabled(true);
        newItem->setText(tr("Nouvel Item"));
        identifiant->setEnabled(false);
        if(currentPolygon->length() > 0){
            Sepulture *newSepulture = new Sepulture(*currentPolygon, this);
            items.append(newSepulture);
            newSepulture->setIdentifiant((identifiant->text() != "")? identifiant->text() : "unknown");
            newSepulture->setPen(style);
            newSepulture->setBrush(infill);
            newSepulture->setZValue(1);
            scene_orthophoto->addItem(newSepulture);
            QGraphicsTextItem *id = scene_orthophoto->addText((identifiant->text() != "")? identifiant->text() : "unknown");
            identifiants.append(id);
            id->setFont(police);
            id->setDefaultTextColor(QColor(Qt::darkYellow));
            id->setPos(currentPolygon->at(0));
            id->setZValue(1);
        }
    }
}

void MainWindow::btn_suppress(){
    if(suppress->isChecked())
        newItem->setEnabled(false);
    else
        newItem->setEnabled(true);
}

bool MainWindow::suppressing(){
    return suppress->isChecked();
}

void MainWindow::deleteSepulture(Sepulture *target){
    saveOk = false;
    int index = items.indexOf(target);
    delete target;
    items.removeAt(index);
    delete identifiants[index];
    identifiants.removeAt(index);
}

void MainWindow::click(int x, int y){
    qDebug() << "Click caught by orthobloc at" << x << y;
    qDebug() << "Vertical ScrollBar at"<< bloc_ortho->verticalScrollBar()->value();
    qDebug() << "Horizontal ScrollBar at"<< bloc_ortho->horizontalScrollBar()->value();
    qDebug() << "Position on picture" << x + bloc_ortho->horizontalScrollBar()->value() << y + bloc_ortho->verticalScrollBar()->value();
    *currentPolygon << QPoint(x + bloc_ortho->horizontalScrollBar()->value(), y + bloc_ortho->verticalScrollBar()->value());
}

void MainWindow::slotdirSet(){
    pathSave = QFileDialog::getExistingDirectory(this, tr("Selection du dossier de sauvegarde"));
    save_option();
}

void MainWindow::slotpathSet(){
    pathExcel = QFileDialog::getOpenFileName(this, tr("Selection du fichier executable"), "C:/", tr("Executable Excel (*.exe)"));
    save_option();
}

void MainWindow::slotexcelSet(){
    if(excelSet->isChecked()){
        excelUse = true;
        save_option();
    }
}

void MainWindow::slotopenSet(){
    if(openSet->isChecked()){
        excelUse = false;
        save_option();
    }
}

void MainWindow::slotorthoSet(){
    nameOrtho = QInputDialog::getText(this, tr("Nom du cimetière"), tr("Entrez le nom du cimetière"));
    pathOrtho = QFileDialog::getOpenFileName(this, tr("Selection de l'orthophoto"), "C:/", tr("Orthophoto (*.png; *jpg; *.tif)"));

    delete img;
    QPixmap ortho(pathOrtho);
    img = scene_orthophoto->addPixmap(ortho);
    for(int i = 0; i < items.length(); i++){
        delete items.at(i);
        delete identifiants.at(i);
    }
    items.clear();
    identifiants.clear();
    load_file();
    save_option();
}

void MainWindow::slot_newOrtho(){
    QString newNameOrtho = QInputDialog::getText(this, tr("Nom du cimetière"), tr("Entrez le nom du cimetière"));
    if(newNameOrtho != ""){
        QString newOrtho = QFileDialog::getOpenFileName(this, tr("Selection de l'orthophoto"), "C:/", tr("Orthophoto (*.png; *.jpg; *.tif)"));
        if(newOrtho != ""){
            QFileInfo check(newOrtho);
            QString newPathSave = QFileDialog::getExistingDirectory(this, tr("Selection du répertoire de travail"), pathSave);
            if(newPathSave != ""){
                nameOrtho =newNameOrtho;
                pathSave = newPathSave;

                pathOrtho = pathSave + "/" + nameOrtho + "/" + nameOrtho + "." + check.completeSuffix();
                qDebug() << pathOrtho;

                if(!QDir((pathSave + "/" + nameOrtho).toLocal8Bit()).exists())
                    QDir().mkdir((pathSave + "/" + nameOrtho).toLocal8Bit());

                if(check.exists() && check.isFile()){
                    QFile::copy(newOrtho, pathOrtho);
                }

                delete img;
                QPixmap ortho(pathOrtho);
                img = scene_orthophoto->addPixmap(ortho);
                for(int i = 0; i < items.length(); i++){
                    delete items.at(i);
                    delete identifiants.at(i);
                }
                items.clear();
                identifiants.clear();
                save_file();
                save_option();
            }
            else
                failNewOrtho->exec();
        }
        else
            failNewOrtho->exec();
    }
    else
        failNewOrtho->exec();
}

void MainWindow::slot_loadOrtho(){
    QString newOrtho = QFileDialog::getOpenFileName(this, tr("Selection du fichier de sauvegarde"), "C:/", tr("Sauvegarde CGestion (*.csav)"));
    if(newOrtho != ""){
        QFile *fichier = new QFile(newOrtho);
        fichier->open(QIODevice::ReadOnly);
        QString firstLine = fichier->readLine();
        fichier->close();
        QStringList firstInfos = firstLine.split(QRegExp("\"*\""));
        qDebug() << "first infos : " << firstInfos;
        nameOrtho = firstInfos[1];
        pathSave = firstInfos[3];
        pathOrtho = firstInfos[5];

        delete img;
        QPixmap ortho(pathOrtho);
        img = scene_orthophoto->addPixmap(ortho);
        for(int i = 0; i < items.length(); i++){
            delete items.at(i);
            delete identifiants.at(i);
        }
        items.clear();
        identifiants.clear();

        load_file();
        save_option();
    }
}

void MainWindow::load_option(){
    QFile *fichier = new QFile("parameters.ini");
    if(fichier->open(QIODevice::ReadOnly)){
        while(!fichier->atEnd()){
            QString ligne = fichier->readLine();
            QStringList infos= ligne.split(" ");
            QStringList path = ligne.split(QRegExp("\"*\""));
            qDebug() << path;
            if(infos[0] == "langue"){
            }
            else if(infos[0] == "tableur"){
                excelUse = (infos[2] == "excel\n")? true : false;
            }
            else if(infos[0] == "tableur_path"){
                pathExcel = path[1];
            }
            else if(infos[0] == "dossier_save"){
                pathSave = path[1];
            }
            else if(infos[0] == "fichier_ortho"){
                pathOrtho = path[1];
            }
            else if(infos[0] == "name_ortho"){
                nameOrtho = path[1];
            }
        }
        fichier->close();
    }
}

void MainWindow::save_option(){
    QFileInfo check("parameters.ini");
    if(check.exists() && check.isFile()){
        remove("parameters.ini");
    }
    QFile *fichier = new QFile("parameters.ini");
    fichier->open(QIODevice::WriteOnly);
    QString ligne;
    ligne = "langue = \"\"\n";
    fichier->write(ligne.toLocal8Bit());
    ligne = "tableur = ";
    ligne.append((excelUse)? "excel\n" : "open\n");
    fichier->write(ligne.toLocal8Bit());
    ligne = "tableur_path = \"" + pathExcel + "\"\n";
    fichier->write(ligne.toLocal8Bit());
    ligne = "dossier_save = \"" + pathSave + "\"\n";
    fichier->write(ligne.toLocal8Bit());
    ligne = "fichier_ortho = \"" + pathOrtho + "\"\n";
    fichier->write(ligne.toLocal8Bit());
    ligne = "name_ortho = \"" + nameOrtho + "\"\n";
    fichier->write(ligne.toLocal8Bit());
    fichier->close();
}

MainWindow::~MainWindow(){
    if(!saveOk){
        int r = promptSave->exec();
        if(r == QMessageBox::Save){
            this->save_file();
        }
    }
}
