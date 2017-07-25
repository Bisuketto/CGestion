#include "item.hpp"

Item::Item()
{

}

void Item::mousePressEvent(QMouseEvent *event){
    emit clicked();
}
