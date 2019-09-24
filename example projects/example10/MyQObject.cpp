#include "MyQObject.h"

#include <QDebug>

MyQObject::MyQObject(QObject *parent) : QObject(parent)
{

}

void MyQObject::myKeyPressedCallback(int key)
{
    // This callback function is executed whenever a key is pressed

    qDebug() << "I know you pressed a key!";

    // This is how you can check what key was pressed. For example, let's see if we are checking whether escape was pressed
    if (key == Qt::Key::Key_Escape){
        qDebug() << "...and you pressed escape!";
    }
}

void MyQObject::myKeyReleasedCallback(int key)
{
    qDebug() << "I know you released a key!";
}
