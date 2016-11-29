#ifndef EBMOVEINRESPONSETOKEYBOARDRELATIVETOSCREEN_H
#define EBMOVEINRESPONSETOKEYBOARDRELATIVETOSCREEN_H

#include <QObject>
#include <QPointer>
#include "Entity.h"

class QTimer;

/// An entity controller (TODO: link to documentation) that
/// moves an Entity relative to the screen in response to keyboard keys.
///
/// Example usage:
/// ECMoveInResponseToKeyboardRelativeToScreen* c = new ECMoveInResponseToKeyboardRelativeToScreen(entity);
///
/// That is all that is needed. Once constructed, the Entity passed into the constructor will move
/// in response to the keyboard. The WASD keys are used to move the entity up, left, down, and right
/// relative to the screen.
/// @author Abdullah Aghazadah
/// @date 11/21/16
class ECMoveInResponseToKeyboardRelativeToScreen: public QObject
{
    Q_OBJECT
public:
    // TODO factor out constructor for behaviors into a base EntityBehavior class
    ECMoveInResponseToKeyboardRelativeToScreen(Entity* entity);
    ~ECMoveInResponseToKeyboardRelativeToScreen();
public slots:
    void moveStep_();
private:
    QPointer<Entity> entity_;   // TODO factor this attribute out into base EntityBehavior class
                                // this is a QPointer because we wanna know when the Entity is deleted
                                // so that we can stop moving it (dangling pointer!)
    double stepSize_;
    double stepFrequency_;

    QTimer* moveTimer_;
};

#endif // EBMOVEINRESPONSETOKEYBOARDRELATIVETOSCREEN_H
