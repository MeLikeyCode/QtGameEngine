#ifndef MYQOBJECT_H
#define MYQOBJECT_H

#include <QObject>

class MyQObject : public QObject
{
    Q_OBJECT
public:
    explicit MyQObject(QObject *parent = nullptr);

signals:

public slots:
    void myKeyPressedCallback(int key);
    void myKeyReleasedCallback(int key);
};

#endif // MYQOBJECT_H
