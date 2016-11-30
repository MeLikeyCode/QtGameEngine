#ifndef ECMOVEINRESPONSETOKEYBOARDRELATIVETOSELF_H
#define ECMOVEINRESPONSETOKEYBOARDRELATIVETOSELF_H

#include <QObject>
#include <QPointer>
#include "Entity.h"

/// An entity controller (TODO: link to doc) that moves an Entity relative to
/// his own facing angle in response to keyboard keys.
///
/// Example usage:
/// ECMoveInResponseToKeyboardRelativeToSelf* c = new ECMoveInResponseToKeyboardRelativeToSelf(entity);
///
/// That is all needed. Once constructed, te Entity passed into the constructor will move
/// in response to the keyboard. WASD keys are used to control the Entity.
class ECMoveInResponseToKeyboardRelativeToSelf: public QObject
{
    Q_OBJECT
public:
    ECMoveInResponseToKeyboardRelativeToSelf(Entity* entity);
public slots:
    void moveStep_();
private:
    QPointer<Entity> entity_;
    double stepSize_;
    double stepFrequency_;

    QTimer* moveTimer_;
};

#endif // ECMOVEINRESPONSETOKEYBOARDRELATIVETOSELF_H
