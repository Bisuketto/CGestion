#include "sepulture.hpp"

Sepulture::Sepulture() : QGraphicsPolygonItem()
{
}

Sepulture::Sepulture(const QPolygonF &polygon, QGraphicsItem *parent) : QGraphicsPolygonItem(polygon, parent){
    identifiant = "unknown";
}

Sepulture::Sepulture(const QPolygonF &polygon, MainWindow *_program, QGraphicsItem *parent) : QGraphicsPolygonItem(polygon, parent){
    identifiant = "unknown";
    program = _program;
}

Sepulture::~Sepulture(){

}

void Sepulture::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(program->suppressing()){
        program->deleteSepulture(this);
    }
    else{
        QFileInfo check(program->get_savepath() + "/" + identifiant + ".xlsx");
        if(!(check.exists() && check.isFile()))
            QFile::copy("example.xlsx", program->get_savepath() + "/" + identifiant + ".xlsx");
        QString instruction;
        instruction = "start "+ program->getPath() + identifiant + ".xlsx\"";
        system(instruction.toLocal8Bit());
    }
}

void Sepulture::setIdentifiant(QString _id){
    identifiant = _id;
}

QString Sepulture::getID(){
    return identifiant;
}
